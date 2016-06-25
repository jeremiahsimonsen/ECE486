/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 3 Real-time implementation of a IIR filter 
 * using the developed routines
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 10, 2015
 * 
 * This file contains the real-time implementation of a 4th-order, IIR filter
 * using the developed subroutines. 
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

#include "ece486_biquad.h"

int main(void)
{
	// int nsamp, i;
 // 	float *input, *output1, *output2;
 	initialize(FS_48K, MONO_IN, STEREO_OUT); 	// Set up the DAC/ADC interface
 	while(1){
    UART_putstr("Hello");
}
 	// Allocate Required Memory
 	// nsamp = getblocksize();
	
 	// input = (float *)malloc(sizeof(float)*nsamp);
 	// output1 = (float *)malloc(sizeof(float)*nsamp);
 	// output2 = (float *)malloc(sizeof(float)*nsamp);
  
 	// if (input==NULL || output1==NULL || output2==NULL) {
 	// 	flagerror(MEMORY_ALLOCATION_ERROR);
 	// 	while(1);
 	// }

 	// Filter coefficients
 // 	float a[2][3] = {{1.0, -0.6088, 0.9702},
 //           {1.0, -0.3500, -0.4250}};

 //  	float b[2][3] = {{2.2044, 0.0, 2.2044},
 //           {2.9658, -3.4865, 2.9658}};

 //    // Biquad structure initialization
	// BIQUAD_T *f1;
	// f1 = init_biquad(2, 0.01718740, a, b, nsamp);

	// // Infinite Loop to process the data stream, "nsamp" samples at a time	
	// while(1){
 //    	/*
 //    	 * Ask for a block of ADC samples to be put into the working buffer
 //    	 *   getblock() will wait until the input buffer is filled...  On return
 //    	 *   we work on 	the new data buffer.
 //    	 */
 //    	getblock(input);	// Wait here until the input buffer is filled... Then process	
    	
 //    	// signal processing code to calculate the required output buffers
 //    	// copy input to output2 for reference
 //    	for(i=0;i<nsamp;i++) {
 //    		output2[i] = input[i];
 //    	}
 //    	DIGITAL_IO_SET(); 	// Use a scope on PC4 to measure execution time
 //    	// Call the biquad filter routine
 //    	calc_biquad(f1,input,output1);
    	
 //    	DIGITAL_IO_RESET();	// (falling edge....  done processing data )
    	
 //    	// pass the processed working buffer back for DAC output
 //    	putblockstereo(output1, output2);
	// }
}
