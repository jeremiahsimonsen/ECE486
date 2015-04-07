/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 4 Real-time implementation of an FSK detector
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 22, 2015
 * 
 * This file contains the real-time implementation of an FSK detector for a 
 * Bell 202 half-duplex modem. A 1200 Hz tone denotes a binary 1 and a 2200 Hz
 * tone denotes a binary 0. Binary data is transmitted at 1200 bits/second. The
 * detector in this lab would be combined with a UART to form the core of the 
 * Bell 202 Modem.
 *
 * An input on pin AIN1 is sampled at 48 ksps. The signal is lowpass filtered
 * to extract the data signal, which is bandwidth limited to 3.4 kHz. Audio 
 * tones above 3.4 kHz are rejected. The signal is passed through a second
 * to reject any impulses at DC caused by the signal not be centered at 1.5 V.
 * The sample sequence is decimated, effectively reducing the sample rate by a
 * factor of 5, to 9.6 ksps. The signal is mixed with a complex exponential 
 * (frequency of 1700 Hz - halfway between "0" and "1"), shifting the spectrum
 * down by 1.7/9.6 cycles/sample. The signal is lowpass filtered again to
 * reject duplicate bands. The frequency of the signal is then estimated and
 * a voltage proportional to the distance from the center frequency is output
 * to pin AOUT1
 * 
 */

// Define the decimation rate...
#define D1 5
#define MY_NSAMP 100


#include "stm32f4xx_hal.h"

#ifdef STM32F407xx
#include "stm32f4_discovery.h"
#endif /* STM32F407xx */

#ifdef STM32F429xx
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#endif /* STM32F429xx */

#include "ece486.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "arm_math.h"

// #include "ece486_fir.h"			// FIR filter routines
#include "ece486_biquad.h"			// IIR filter routines

#include "lowpass1_coef.h"

int main(void)
{
	int nsamp;
	/*
	 * Optional: set the ADC/DAC Block Size to anything down to one sample
	 * (Omitting this call uses the default block size of 100 samples per data
	 * block)
	 * The code below assumes that MY_NSAMP is a multiple of D1... otherwise, 
	 * you'll have to modify the decimation code to avoid discontinuities
	 * between blocks of input data.
	 */
	setblocksize( MY_NSAMP );

	/* 
	 * Set up the required 50 ksps sample rate... Use the ADC to measure 
	 * a function generator or a microphone input
	 */
	initialize(FS_48K, MONO_IN, STEREO_OUT);       // Set up: ADC input, DAC output

	nsamp = getblocksize(); 	// get number of samples

	// Other variables
	int i;
	int j;
	float *input, *output1, *output2;
	input = (float *)malloc(sizeof(float)*nsamp);
 	output1 = (float *)malloc(sizeof(float)*nsamp);
 	output2 = (float *)malloc(sizeof(float)*nsamp);
 	float *buffer = (float *)malloc(sizeof(float)*nsamp/D1);
 	float *buffer2 = (float *)malloc(sizeof(float)*nsamp/D1);
 	float *w_re = (float *)malloc(sizeof(float)*nsamp/D1);
 	float *w_im = (float *)malloc(sizeof(float)*nsamp/D1);
 	float *df = (float *)malloc(sizeof(float)*nsamp/D1);	
 	
 	// Error check memory allocation
 	if (input==NULL || output1==NULL || output2==NULL || buffer==NULL || buffer2==NULL || w_re==NULL || w_im==NULL || df==NULL) {
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}
  
 	// Filter initializations
	BIQUAD_T *f1 = init_biquad(filter1_num_stages, filter1_g, filter1_a_coef, filter1_b_coef, nsamp);
	BIQUAD_T *f2_re = init_biquad(filter2_num_stages, filter2_g, filter2_a_coef, filter2_b_coef, nsamp/D1);
	BIQUAD_T *f2_im = init_biquad(filter2_num_stages, filter2_g, filter2_a_coef, filter2_b_coef, nsamp/D1);
	BIQUAD_T *f3 = init_biquad(filter3_num_stages, filter3_g, filter3_a_coef, filter3_b_coef,nsamp/D1);

	// DC blocker initialization
	DCBLOCK_T *dcblocker;
	dcblocker = init_dcblock(nsamp/D1);

	// Initialize the mixer
	MIXER_T *cosine_mix = init_mixer(mixer_coef, n_mixer, nsamp/D1);
	MIXER_T *sine_mix = init_mixer(mixer_coef, n_mixer, nsamp/D1);
	sine_mix->m_index += 24;		// phase shift
  
	/*
	 * Infinite Loop to process the data stream "MY_NSAMP" samples at a time
	 */
	while(1){
		/*
		 * Ask for a block of ADC samples to be put into the working buffer
		 *   getblock() will wait until the input buffer is filled...  On return
		 *   we work on the new data buffer.
		 */
		getblock(input);	// Wait here until the input buffer is filled... 
    
    	/*
    	 * Stage 1:  Complete processing at the incoming sample frequency fs.
    	 *           (Array size MY_NSAMP samples)
    	 */
		DIGITAL_IO_SET();

    	// Block DC
		calc_biquad(f1,input,input);

    	// Decimate by D1
    	for (i=0; i<MY_NSAMP/D1; i++) 
    		buffer[i] = input[i*D1];
    
    	// Reject DC
    	calc_dcblock(dcblocker, buffer, buffer2);

    	/*
    	 * Stage 2:  Complete processing at the intermediate sample rate fs/D1.
    	 *           (Array size MY_NSAMP/D1 samples)
    	 */

    	// Mix signals
	   	calc_mixer(cosine_mix,buffer2,w_re);
	   	calc_mixer(sine_mix,buffer2,w_im);

		// Lowpass filter
    	calc_biquad(f2_re,w_re,w_re);
    	calc_biquad(f2_im,w_im,w_im);

    	// Frequency Estimation
    	delta_f(df,w_re,w_im,nsamp/D1);

    	// Clean up frequency estimation
    	calc_biquad(f3, df, df);
    
    	/* 
    	 * Write output values to the DAC....  NOTE: Be sure to set the output
    	 * array to values for every INPUT sample (not just samples at the decimated 
    	* rates!
    	*/
    	for (i=0; i<nsamp/D1; i++) {
    	  	// Every stage-3 output should be written to D1 output samples!
    	  	for (j=0; j<D1; j++) {
				output1[i*D1+j] = df[i];
    		}
    	}
    
    	DIGITAL_IO_RESET();

    	/*
    	 * pass the (length MY_NSAMP) calculated buffers back for DAC output
    	 */
    	putblockstereo(output1, output2);
      
  }
}
