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
#define MY_FS FS_48K
#define D1 10
#define D2 10
#define MY_NSAMP 620
// #define BLOCKS 40
#define FFT_N 1024
#define FORWARD_FFT 0
#define INVERSE_FFT 1

#define POS_THRESHOLD 0.0
#define NEG_THRESHOLD 0.0


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
// #include <stdio.h>
#include <math.h>
#include "arm_math.h"
// #include "arm_common_tables.h"

// #include "ece486_fir.h"			// FIR filter routines
#include "ece486_biquad.h"			// IIR filter routines

// #include "lowpass1_coef.h"

#include "numtostr.h"
void send_report(float x1, float x2, float x3, float x4);

int main(void)
{
	int nsamp;
	static int block_count = 0;
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
	initialize(MY_FS, STEREO_IN, MONO_OUT);       // Set up: ADC input, DAC output

	

	// nsamp = getblocksize(); 	// get number of samples
	nsamp = MY_NSAMP;

	// Index variables
	int i, start, end;

	// ADC inputs
	float *input1 = (float *)malloc(sizeof(float)*nsamp);
	float *input2 = (float *)malloc(sizeof(float)*nsamp);

 	// DAC outputs
 	// float *output1 = (float *)malloc(sizeof(float)*nsamp);
 	// float *output2 = (float *)malloc(sizeof(float)*nsamp);

 	// Decimated real and imaginary signals; outputs of lowpass filter
 	// float32_t *buffer_re = (float32_t *)malloc(sizeof(float32_t)*nsamp/D1);
 	// float32_t *buffer_im = (float32_t *)malloc(sizeof(float32_t)*nsamp/D1);

 	// Input (and output) to the complex FFT; interleaved of above and zero pad
 	// float32_t *fft_in = (float32_t *)malloc(sizeof(float32_t)*FFT_N*2);
 	// float32_t *fft_in = (float32_t *)malloc(sizeof(float32_t)*2*nsamp/D1);

 	// Magnitude of the FFT
 	// float32_t *mag = (float32_t *)malloc(sizeof(float32_t)*FFT_N);
 	// float32_t *mag = (float32_t *)malloc(sizeof(float32_t)*nsamp/D1);


 	// Maxima
 	// int one_ms;
 	// float max_pos_val = 0.0;
 	// float max_neg_val = 0.0;
 	// float max_pos_ind = 0;
 	// float max_neg_ind = 0;
 	// float32_t max_pos_freq = 0.0;
 	// float32_t max_neg_freq = 0.0;

 	// Velocities
 	// float pos_vel, neg_vel;

 	// Complex FFT structure initializations	
 // 	arm_cfft_radix2_instance_f32 fft;
	// arm_status status = arm_cfft_radix2_init_f32(&fft, FFT_N, FORWARD_FFT, 1);
	// if (status != ARM_MATH_SUCCESS)
	// 	flagerror(MEMORY_ALLOCATION_ERROR);
 	
 	// Error check memory allocation
 	if (input1==NULL || input2==NULL){// || buffer_re==NULL || buffer_im==NULL || fft_in==NULL || mag==NULL) {
 	// || buffer2==NULL || w_re==NULL || w_im==NULL || df==NULL
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}
  
 	// Filter initializations
	// BIQUAD_T *low1_re = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, nsamp);
	// BIQUAD_T *low1_im = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, nsamp);

	// DC blocker initialization
	// DCBLOCK_T *dcblocker;
	// dcblocker = init_dcblock(nsamp/D1);

	char buf[20] = "Running\n\r";
	char buf2[23];
	UART_putstr(buf);

	/*
	 * Infinite Loop to process the data stream "MY_NSAMP" samples at a time
	 */
	while(1){
		inttostr(block_count,buf2);
		UART_putstr(buf2);

		/*
		 * Ask for a block of ADC samples to be put into the working buffer
		 *   getblock() will wait until the input buffer is filled...  On return
		 *   we work on the new data buffer.
		 */
		getblockstereo(input1,input2);	// Wait here until the input buffer is filled... 
    
    	/*
    	 * Stage 1:  Complete processing at the incoming sample frequency fs.
    	 *           (Array size MY_NSAMP samples)
    	 */
		// DIGITAL_IO_SET();

  //   	// Lowpass filter
		// calc_biquad(low1_re,input1,input1);
		// calc_biquad(low1_im,input2,input2);

  //   	// Decimate by D1
  //   	for (i=0; i<nsamp/D1; i++) {
  //   		buffer_re[i] = input1[i*D1];
  //   		buffer_im[i] = input2[i*D1];
  //   	}

    	
		// start = block_count*2*nsamp/D1;
		// end = start + 2*nsamp/D1;
    	// Interleave real and complex
    	// for (i=start; i<nsamp/D1; i++) {
    	// 	fft_in[start+2*i] = buffer_re[i];
    	// 	fft_in[start+2*i+1] = buffer_im[i];
    	// }
    	block_count++;

    	if (block_count == D2) {
    		block_count = 0;
    	
	    	UART_putstr(buf);

	    	// Zero the rest of the fft_in array
	    	// for (i=2*nsamp; i<2*FFT_N; i++) {
	    	// 	fft_in[i] = 0.0;
	    	// }

	    	// Calculate complex fft
	    	// arm_cfft_radix2_f32(&fft, fft_in);

	    	// DIGITAL_IO_RESET();

	    	// Calculate complex magnitude
	    	// for (i=0; i<FFT_N; i++) {
	    	// 	mag[i] = sqrt(fft_in[2*i]*fft_in[2*i] + fft_in[2*i+1]*fft_in[2*i+1]);
	    	// }
	    	// arm_cmplx_mag_f32(fft_in, mag, FFT_N);
	    	// arm_cmplx_mag_f32(fft_in, mag, nsamp/D1);

	    	// README Find maximum of positive (first half array) and negative (2nd half) frequencies
	    	// arm_max_f32(&mag[0], FFT_N/2, &max_pos_val, &max_pos_ind);
	    	// arm_max_f32(&mag[FFT_N/2+1], FFT_N/2, &max_neg_val, &max_neg_ind);
	    	// arm_max_f32(mag, nsamp/D1/2, max_pos_val, max_pos_ind);
	    	// arm_max_f32(&mag[nsamp/D1/2+1], nsamp/D1/2, max_neg_val, max_neg_ind);

	    	// README Find highest positive frequency above a threshold in first half
	    	// of the magnitude array
	    	// one_ms = (int) 38.667/MY_FS/D1*FFT_N;

	    	// for (i=one_ms; i<FFT_N/2; i++) {
	    	// 	if (mag[i] > POS_THRESHOLD) {
	    	// 		max_pos_val = mag[i];
	    	// 		max_pos_ind = i;
	    	// 	}
	    	// 	if (mag[FFT_N-1-i] > NEG_THRESHOLD) {
	    	// 		max_neg_val = mag[FFT_N-1-i];
	    	// 		max_neg_ind = FFT_N-1-i;
	    	// 	}
	    	// }

	    	// Un-normalize frequency
	    	// max_pos_freq = (max_pos_ind/1024.0) * 48000.0 / 10.0;
	    	// max_neg_freq = ( (max_neg_ind - 1024.0)/1024.0 ) * 48000.0 / 10.0;

	    	// README Calculate velocity
			// pos_vel = max_pos_freq * 3e8 / (2*5.8e9)
			// pos_vel = max_pos_freq * 0.025862068966;
			// neg_vel = max_neg_freq * 0.025862068966;


	    	// README Print to serial
	    	// block_count++;
	    	// if (block_count == BLOCKS) {
	    		// send_report(max_pos_ind, max_pos_freq, max_neg_ind, max_neg_freq);
	    		// send_report(pos_vel, fft_in[0], neg_vel, fft_in[1]);
	    		// block_count = 0;
	    	// }
	    
	    	/* 
	    	 * Write output values to the DAC....  NOTE: Be sure to set the output
	    	 * array to values for every INPUT sample (not just samples at the decimated 
	    	* rates!
	    	*/
	    // 	for (i=0; i<nsamp/D1; i++) {
	    // 	  	// Every stage-3 output should be written to D1 output samples!
	    // 	  	for (j=0; j<D1; j++) {
					// output1[i*D1+j] = buffer[i];
	    // 		}
	    // 	}
	    
	    	// DIGITAL_IO_RESET();
    }

    	/*
    	 * pass the (length MY_NSAMP) calculated buffers back for DAC output
    	 */
    	// putblockstereo(output1, output2);
      
  }
}

void send_report(float x1, float x2, float x3, float x4) {
  // Send a comma-separated list of four numbers to the serial port,
  // terminated by a newline.
  static char outstr[100], *endstr;
  
  endstr = outstr;
  floattostr(x1, outstr, 2);
  while (*endstr != '\0') endstr++;
  *endstr = ',';
  endstr++;

  floattostr(x2, endstr , 2);
  while (*endstr != '\0') endstr++;
  *endstr = ',';
  endstr++;
  
  //strcat(outstr, ", ");
  floattostr(x3, endstr , 2);
  while (*endstr != '\0') endstr++;
  *endstr = ',';
  endstr++;
  
  //strcat(outstr, ", ");
  floattostr(x4, endstr , 2);
  while (*endstr != '\0') endstr++;
  endstr[0] = '\n';
  endstr[1] = '\0';
  
 //strcat(outstr,"\n");
  UART_putstr(outstr);
  return;
}