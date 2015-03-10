/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 3 Real-time implementation of a low pass
 * FIR filter using the STM-provided functions
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 10, 2015
 * 
 * This file contains the real-time implementation of a 20-coefficient, low-
 * pass FIR filter using the optimized DSP library functions provided by STM.
 * The filter will have a 3 dB cut-off frequency of 5 kHz when Fs = 48 kHz; 
 * i.e., f = .104167
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

 	// Impulse response/FIR coefficient array
 	float32_t h[20] = {0.000168, 
                  -0.000883, 
                  -0.004735, 
                  -0.010728, 
                  -0.013639, 
                  -0.004205, 
                  0.025992, 
                  0.077462, 
                  0.138625, 
                  0.188861, 
                  0.208333, 
                  0.188861, 
                  0.138625, 
                  0.077462, 
                  0.025992, 
                  -0.004205, 
                  -0.013639, 
                  -0.010728, 
                  -0.004735, 
                  -0.000883};

    // state buffer used by arm routine of size NUM_TAPS + BLOCKSIZE -1
	float32_t *state = (float32_t *) malloc(sizeof(float)*(20+nsamp-1));
	// arm FIR filter struct initialization
	arm_fir_instance_f32 f1;
	arm_fir_init_f32(&f1,20,&h[0],state,nsamp);

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
    	arm_fir_f32(&f1, input, output1, nsamp);
    	
    	DIGITAL_IO_RESET();	// (falling edge....  done processing data )
    	
    	// pass the processed working buffer back for DAC output
    	putblockstereo(output1, output2);
	}
}
