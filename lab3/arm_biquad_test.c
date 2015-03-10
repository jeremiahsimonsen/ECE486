/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 3 Real-time implementation of an IIR filter
 * using the STM-provided functions
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 10, 2015
 * 
 * This file contains the real-time implementation of a 4th-order IIR filter
 * using the optimized DSP library functions provided by STM.
 * The filter will reject 7.2 kHz and 12 kHz when Fs = 48 kHz; i.e., it rejects
 * f = 0.15 and f = 0.25
 *
 * An input is taken on pin AIN1, the FIR filtered output is sent to pin
 * AOUT1, and the input waveform is copied to AOUT2 for reference
 * 
 */

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

#include "ece486_fir.h"
#include "ece486_biquad.h"

int main(void)
{
	int nsamp, i;
 	float32_t *input, *output1, *output2;
 	initialize(FS_48K, MONO_IN, STEREO_OUT); 	// Set up the DAC/ADC interface
 	
 	// Allocate Required Memory
 	nsamp = getblocksize();
	
 	input = (float32_t *)malloc(sizeof(float)*nsamp);
 	output1 = (float32_t *)malloc(sizeof(float)*nsamp);
 	output2 = (float32_t *)malloc(sizeof(float)*nsamp);
  
 	if (input==NULL || output1==NULL || output2==NULL) {
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}

 	// IIR filter coefficient array
 	// {b10, b11, b12, a11, a12, b20, b21, b22, a21, a22}
 	float32_t coef[10] = {2.2044, 0.0, 2.2044, -0.6088, 0.9702,
 							2.9658, -3.4865, 2.9658, -0.3500, -0.4250};

 	// state buffer used by arm routine of size 2*NUM_SECTIONS
 	float32_t *state = (float32_t *)malloc(sizeof(float32_t)*(2*2));
 	// arm biquad structure initialization
 	arm_biquad_cascade_df2T_instance_f32 f1;
 	arm_biquad_cascade_df2T_init_f32(&f1,2,&coef[0],state);

	// Infinite Loop to process the data stream, "nsamp" samples at a time	
	while(1){
    	/*
    	 * Ask for a block of ADC samples to be put into the working buffer
    	 *   getblock() will wait until the input buffer is filled...  On return
    	 *   we work on 	the new data buffer.
    	 */
    	getblock(input);	// Wait here until the input buffer is filled... Then process	
    	
    	// signal processing code to calculate the required output buffers
    	// copy input to output2 for reference
    	for(i=0;i<nsamp;i++) {
    		output2[i] = input[i];
    	}
    	DIGITAL_IO_SET(); 	// Use a scope on PC4 to measure execution time
    	// Call the arm provided FIR filter routine
    	arm_biquad_cascade_df2T_f32(&f1, input, output1, nsamp);
    	
    	DIGITAL_IO_RESET();	// (falling edge....  done processing data )
    	
    	// pass the processed working buffer back for DAC output
    	putblockstereo(output1, output2);
	}
}
