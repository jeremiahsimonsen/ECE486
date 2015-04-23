/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 5 DSP for a doppler radar detector
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Apr 10, 2015
 * 
 * This file contains the real-time implementation of the digital signal
 * processing for a doppler radar detector.
 *
 * A 5.8 GHz signal is transmitted from one antenna. The signal reflects off of
 * objects. Moving objects cause a change in frequency known as Doppler shift,
 * roughly proportional to their velocity. The reflected signal is received by
 * a second antenna and split into two signals 90 degrees out of phase. Each
 * signal is mixed with the transmitted signal to find the difference frequency, which corresponds to the Doppler shift. 
 * The mixed signals are then low-passed filtered and sampled by the STM32F407 development board.
 * 
 * The two ADC inputs are sampled at 24 ksps. The signals are lowpass filtered
 * to limit the bandwidth to 2 kHz (equivalent to about 50 m/s). The sampled
 * signals are decimated to reduce the necessary processing time. An FFT is
 * performed to examine the spectrum of the complex input signal. The highest
 * positive and negative velocities are detected by taking the highest positive
 * and negative frequencies with FFT component above a certain threshold. These
 * velocities are then written to the serial port, so they can be read.
 * 
 */

// Define the decimation rate...
#define MY_FS 24000.0
// #define D1 4
// #define D2 4
#define MY_NSAMP 200
// #define BLOCKS 16
#define FFT_N 512
#define FORWARD_FFT 0
#define INVERSE_FFT 1

// #define POS_THRESHOLD 10.0
// #define NEG_THRESHOLD 10.0


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
#include <math.h>
#include "arm_math.h"
// #include "arm_common_tables.h"

// #include "ece486_fir.h"			// FIR filter routines
#include "ece486_biquad.h"			// IIR filter routines

#include "lowpass1_coef.h"

#include "numtostr.h"
#include "ece486_mixer.h"

int main(void)
{
	static int block_count = 0;
	static int total_blocks = 0;

	// Set the ADC/DAC Block size; note the code below assumes MY_NSAMP is a
	// multiple of D1
	setblocksize( MY_NSAMP );

	// Set up the required sample rate and ADC/DAC input/output
	initialize(FS_24K, STEREO_IN, STEREO_OUT);

	// Index variables
	int i, start;

	// ADC inputs
	float *input1 = (float *)malloc(sizeof(float)*MY_NSAMP);
	float *input2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// DAC outputs
 	float *output1 = (float *)malloc(sizeof(float)*MY_NSAMP);
 	float *output2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// Decimated real and imaginary signals; outputs of lowpass filter
 	// float32_t *buffer_re = (float32_t *)malloc(sizeof(float32_t)*MY_NSAMP/D1);
 	// float32_t *buffer_im = (float32_t *)malloc(sizeof(float32_t)*MY_NSAMP/D1);

 	// Input (and output) to the complex FFT; interleaved of above and zero pad
 	float32_t *fft_in = (float32_t *)malloc(sizeof(float32_t)*FFT_N*2);

 	// Magnitude of the FFT
 	float32_t *mag = (float32_t *)malloc(sizeof(float32_t)*FFT_N);

 	// Complex FFT structure initializations	
 	arm_cfft_radix2_instance_f32 fft;
	arm_status status = arm_cfft_radix2_init_f32(&fft, FFT_N, FORWARD_FFT, 1);
	if (status != ARM_MATH_SUCCESS)
		flagerror(MEMORY_ALLOCATION_ERROR);
 	
 	// Error check memory allocation
 	if (input1==NULL || input2==NULL || output1==NULL || output2==NULL || fft_in==NULL || mag==NULL) {
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}
  
 	// Filter initializations
	// BIQUAD_T *low1_re = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, MY_NSAMP);
	// BIQUAD_T *low1_im = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, MY_NSAMP);


	// Infinite Loop to process the data stream "MY_NSAMP" samples at a time
	while(1){
		
		// Collect a block of samples from the ADC; waits until the input
		// buffer is filled
		getblockstereo(input1,input2);
    
    	/*
    	 * Stage 1:  Complete processing at the incoming sample frequency fs.
    	 *           (Array size MY_NSAMP samples)
    	 */
		DIGITAL_IO_SET();

    	// Lowpass filter
		// calc_biquad(low1_re,input1,input1);
		// calc_biquad(low1_im,input2,input2);

    	// Decimate by D1
    	// for (i=0; i<MY_NSAMP/D1; i++) {
    	// 	buffer_re[i] = input1[i*D1];
    	// 	buffer_im[i] = input2[i*D1];
    	// }

    	// We are going to wait for several blocks of input samples, so keep
    	// track of the index to start inserting new samples at
		// start = block_count*2*MY_NSAMP/D1;
    	// Interleave real and complex arrays for arm function format purposes
    	for (i=0; i<MY_NSAMP; i++) {
    		fft_in[2*i] = input1[i];
    		fft_in[2*i+1] = input2[i];
    	}
    	// block_count++;

    	// if (block_count == D2) {
    		// block_count = 0;

	    	// Zero the rest of the fft_in array
	    	for (i=2*MY_NSAMP; i<2*FFT_N; i++) {
	    		fft_in[i] = 0.0;
	    	}
			
			// Windowing here may be helpful to discern between frequencies if the velocities are close together

	    	// Calculate complex fft
	    	arm_cfft_radix2_f32(&fft, fft_in);

	    	// Calculate complex magnitude
	    	arm_cmplx_mag_f32(fft_in, mag, FFT_N);
	    
	    	/* 
	    	 * Write output values to the DAC....  NOTE: Be sure to set the output
	    	 * array to values for every INPUT sample (not just samples at the decimated 
	    	* rates!
	    	*/
	    // 	for (i=0; i<MY_NSAMP/D1; i++) {
	    // 	  	// Every stage-3 output should be written to D1 output samples!
	    // 	  	for (j=0; j<D1; j++) {
					// output1[i*D1+j] = buffer[i];
	    // 		}
	    // 	}
	    
    	// }

	    DIGITAL_IO_RESET();

    	// Pass output buffers to the DAC
    	putblockstereo(output1,output2);
      
  }
}