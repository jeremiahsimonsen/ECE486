/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 6 DSP for a stepped frequency radar detector
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Apr 24, 2015
 * 
 * This file contains the real-time implementation of the digital signal
 * processing for a stepped frequency radar detector.
 *
 * The transmit frequency is approximately 5.8 GHz, but may be adjusted by changing 
 * the VCO control voltage (about 75 MHz/V over the zero to five volt range). 
 * We use the DAC output to create voltages between one and three volts, so that our measured radar returns will 
 * be collected over a 150 MHz band. The key to the stepped frequency radar is that the VCO is used to increment 
 * the transmitter frequency to provide multiple measurement taken at equally spaced frequencies.The transmitted signal reflects off of
 * objects. Moving objects cause a change in frequency known as Doppler shift,
 * roughly proportional to their velocity. The reflected signal is received by
 * a second antenna and split into two signals 90 degrees out of phase. 
 * By estimating the discrete-time frequency of the sequence of measurements, the distance to the target may be obtained. 
 * Of course, the received signal normally consists of many reflections off of multiple targets, and each target provides 
 * its own contribution to the sequence x(n). An FFT is used to examine the received sequence x(n) and determine what frequencies 
 * (distances) are present, and what the signal strength is for the reflection from each distance. \
 * 
 * 
 */

#define MY_FS 12000.0
#define MY_NSAMP 256
#define FFT_N 512
#define FORWARD_FFT 0
#define INVERSE_FFT 1


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

// #include "ece486_fir.h"			// FIR filter routines
#include "ece486_biquad.h"			// IIR filter routines

#include "lowpass1_coef.h"

#include "numtostr.h"
#include "ece486_mixer.h"

int main(void) 
{

	// Set the ADC/DAC Block size; note the code below assumes MY_NSAMP is a
	// multiple of D1
	setblocksize( MY_NSAMP );

	// Set up the required sample rate and ADC/DAC input/output
	initialize(FS_12K, STEREO_IN, STEREO_OUT);

	// Index variables
	int i;//, start;

	// ADC inputs
	float *input1 = (float *)malloc(sizeof(float)*MY_NSAMP);
	float *input2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// DAC outputs
 	float *output2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// Input (and output) to the complex FFT; interleaved of above and zero pad
 	float32_t *fft_in = (float32_t *)malloc(sizeof(float32_t)*FFT_N*2);

 	// Magnitude of the FFT
 	float32_t *mag = (float32_t *)malloc(sizeof(float32_t)*FFT_N);

 	// Maxima
 	float32_t max_fft = 1.0;
 	uint32_t max_ind = 0;

 	// Complex FFT structure initializations	
 	arm_cfft_radix2_instance_f32 fft;
	arm_status status = arm_cfft_radix2_init_f32(&fft, FFT_N, FORWARD_FFT, 1);
	if (status != ARM_MATH_SUCCESS)
		flagerror(MEMORY_ALLOCATION_ERROR);
 	
 	// Error check memory allocation
 	if (input1==NULL || input2==NULL || output2==NULL || fft_in==NULL || mag==NULL) {
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}
  
	float ramp[256] = {-0.3333,
		   -0.3281,
		   -0.3229,
		   -0.3176,
		   -0.3124,
		   -0.3072,
		   -0.3020,
		   -0.2967,
		   -0.2915,
		   -0.2863,
		   -0.2810,
		   -0.2758,
		   -0.2706,
		   -0.2654,
		   -0.2601,
		   -0.2549,
		   -0.2497,
		   -0.2444,
		   -0.2392,
		   -0.2340,
		   -0.2288,
		   -0.2235,
		   -0.2183,
		   -0.2131,
		   -0.2078,
		   -0.2026,
		   -0.1974,
		   -0.1922,
		   -0.1869,
		   -0.1817,
		   -0.1765,
		   -0.1712,
		   -0.1660,
		   -0.1608,
		   -0.1556,
		   -0.1503,
		   -0.1451,
		   -0.1399,
		   -0.1346,
		   -0.1294,
		   -0.1242,
		   -0.1190,
		   -0.1137,
		   -0.1085,
		   -0.1033,
		   -0.0980,
		   -0.0928,
		   -0.0876,
		   -0.0824,
		   -0.0771,
		   -0.0719,
		   -0.0667,
		   -0.0614,
		   -0.0562,
		   -0.0510,
		   -0.0458,
		   -0.0405,
		   -0.0353,
		   -0.0301,
		   -0.0248,
		   -0.0196,
		   -0.0144,
		   -0.0092,
		   -0.0039,
		    0.0013,
		    0.0065,
		    0.0118,
		    0.0170,
		    0.0222,
		    0.0275,
		    0.0327,
		    0.0379,
		    0.0431,
		    0.0484,
		    0.0536,
		    0.0588,
		    0.0641,
		    0.0693,
		    0.0745,
		    0.0797,
		    0.0850,
		    0.0902,
		    0.0954,
		    0.1007,
		    0.1059,
		    0.1111,
		    0.1163,
		    0.1216,
		    0.1268,
		    0.1320,
		    0.1373,
		    0.1425,
		    0.1477,
		    0.1529,
		    0.1582,
		    0.1634,
		    0.1686,
		    0.1739,
		    0.1791,
		    0.1843,
		    0.1895,
		    0.1948,
		    0.2000,
		    0.2052,
		    0.2105,
		    0.2157,
		    0.2209,
		    0.2261,
		    0.2314,
		    0.2366,
		    0.2418,
		    0.2471,
		    0.2523,
		    0.2575,
		    0.2627,
		    0.2680,
		    0.2732,
		    0.2784,
		    0.2837,
		    0.2889,
		    0.2941,
		    0.2993,
		    0.3046,
		    0.3098,
		    0.3150,
		    0.3203,
		    0.3255,
		    0.3307,
		    0.3359,
		    0.3412,
		    0.3464,
		    0.3516,
		    0.3569,
		    0.3621,
		    0.3673,
		    0.3725,
		    0.3778,
		    0.3830,
		    0.3882,
		    0.3935,
		    0.3987,
		    0.4039,
		    0.4092,
		    0.4144,
		    0.4196,
		    0.4248,
		    0.4301,
		    0.4353,
		    0.4405,
		    0.4458,
		    0.4510,
		    0.4562,
		    0.4614,
		    0.4667,
		    0.4719,
		    0.4771,
		    0.4824,
		    0.4876,
		    0.4928,
		    0.4980,
		    0.5033,
		    0.5085,
		    0.5137,
		    0.5190,
		    0.5242,
		    0.5294,
		    0.5346,
		    0.5399,
		    0.5451,
		    0.5503,
		    0.5556,
		    0.5608,
		    0.5660,
		    0.5712,
		    0.5765,
		    0.5817,
		    0.5869,
		    0.5922,
		    0.5974,
		    0.6026,
		    0.6078,
		    0.6131,
		    0.6183,
		    0.6235,
		    0.6288,
		    0.6340,
		    0.6392,
		    0.6444,
		    0.6497,
		    0.6549,
		    0.6601,
		    0.6654,
		    0.6706,
		    0.6758,
		    0.6810,
		    0.6863,
		    0.6915,
		    0.6967,
		    0.7020,
		    0.7072,
		    0.7124,
		    0.7176,
		    0.7229,
		    0.7281,
		    0.7333,
		    0.7386,
		    0.7438,
		    0.7490,
		    0.7542,
		    0.7595,
		    0.7647,
		    0.7699,
		    0.7752,
		    0.7804,
		    0.7856,
		    0.7908,
		    0.7961,
		    0.8013,
		    0.8065,
		    0.8118,
		    0.8170,
		    0.8222,
		    0.8275,
		    0.8327,
		    0.8379,
		    0.8431,
		    0.8484,
		    0.8536,
		    0.8588,
		    0.8641,
		    0.8693,
		    0.8745,
		    0.8797,
		    0.8850,
		    0.8902,
		    0.8954,
		    0.9007,
		    0.9059,
		    0.9111,
		    0.9163,
		    0.9216,
		    0.9268,
		    0.9320,
		    0.9373,
		    0.9425,
		    0.9477,
		    0.9529,
		    0.9582,
		    0.9634,
		    0.9686,
		    0.9739,
		    0.9791,
		    0.9843,
		    0.9895,
		    0.9948,
		    1.0000};

		    // Windowing
	float kaiser[256] = {0.0023,
		    0.0030,
		    0.0037,
		    0.0044,
		    0.0053,
		    0.0062,
		    0.0073,
		    0.0084,
		    0.0096,
		    0.0110,
		    0.0124,
		    0.0140,
		    0.0157,
		    0.0175,
		    0.0195,
		    0.0216,
		    0.0238,
		    0.0262,
		    0.0288,
		    0.0315,
		    0.0344,
		    0.0374,
		    0.0406,
		    0.0440,
		    0.0476,
		    0.0514,
		    0.0553,
		    0.0595,
		    0.0638,
		    0.0684,
		    0.0732,
		    0.0782,
		    0.0834,
		    0.0888,
		    0.0945,
		    0.1004,
		    0.1065,
		    0.1129,
		    0.1194,
		    0.1263,
		    0.1333,
		    0.1406,
		    0.1482,
		    0.1559,
		    0.1639,
		    0.1722,
		    0.1807,
		    0.1894,
		    0.1984,
		    0.2076,
		    0.2171,
		    0.2268,
		    0.2367,
		    0.2468,
		    0.2572,
		    0.2678,
		    0.2786,
		    0.2896,
		    0.3008,
		    0.3122,
		    0.3238,
		    0.3356,
		    0.3476,
		    0.3597,
		    0.3721,
		    0.3845,
		    0.3972,
		    0.4099,
		    0.4229,
		    0.4359,
		    0.4490,
		    0.4623,
		    0.4756,
		    0.4890,
		    0.5025,
		    0.5161,
		    0.5297,
		    0.5434,
		    0.5570,
		    0.5707,
		    0.5844,
		    0.5981,
		    0.6118,
		    0.6254,
		    0.6390,
		    0.6525,
		    0.6659,
		    0.6793,
		    0.6925,
		    0.7056,
		    0.7186,
		    0.7315,
		    0.7442,
		    0.7567,
		    0.7691,
		    0.7813,
		    0.7932,
		    0.8049,
		    0.8164,
		    0.8277,
		    0.8387,
		    0.8494,
		    0.8598,
		    0.8699,
		    0.8798,
		    0.8893,
		    0.8984,
		    0.9073,
		    0.9158,
		    0.9239,
		    0.9317,
		    0.9390,
		    0.9460,
		    0.9526,
		    0.9588,
		    0.9646,
		    0.9700,
		    0.9749,
		    0.9794,
		    0.9835,
		    0.9871,
		    0.9903,
		    0.9931,
		    0.9953,
		    0.9972,
		    0.9986,
		    0.9995,
		    0.9999,
		    0.9999,
		    0.9995,
		    0.9986,
		    0.9972,
		    0.9953,
		    0.9931,
		    0.9903,
		    0.9871,
		    0.9835,
		    0.9794,
		    0.9749,
		    0.9700,
		    0.9646,
		    0.9588,
		    0.9526,
		    0.9460,
		    0.9390,
		    0.9317,
		    0.9239,
		    0.9158,
		    0.9073,
		    0.8984,
		    0.8893,
		    0.8798,
		    0.8699,
		    0.8598,
		    0.8494,
		    0.8387,
		    0.8277,
		    0.8164,
		    0.8049,
		    0.7932,
		    0.7813,
		    0.7691,
		    0.7567,
		    0.7442,
		    0.7315,
		    0.7186,
		    0.7056,
		    0.6925,
		    0.6793,
		    0.6659,
		    0.6525,
		    0.6390,
		    0.6254,
		    0.6118,
		    0.5981,
		    0.5844,
		    0.5707,
		    0.5570,
		    0.5434,
		    0.5297,
		    0.5161,
		    0.5025,
		    0.4890,
		    0.4756,
		    0.4623,
		    0.4490,
		    0.4359,
		    0.4229,
		    0.4099,
		    0.3972,
		    0.3845,
		    0.3721,
		    0.3597,
		    0.3476,
		    0.3356,
		    0.3238,
		    0.3122,
		    0.3008,
		    0.2896,
		    0.2786,
		    0.2678,
		    0.2572,
		    0.2468,
		    0.2367,
		    0.2268,
		    0.2171,
		    0.2076,
		    0.1984,
		    0.1894,
		    0.1807,
		    0.1722,
		    0.1639,
		    0.1559,
		    0.1482,
		    0.1406,
		    0.1333,
		    0.1263,
		    0.1194,
		    0.1129,
		    0.1065,
		    0.1004,
		    0.0945,
		    0.0888,
		    0.0834,
		    0.0782,
		    0.0732,
		    0.0684,
		    0.0638,
		    0.0595,
		    0.0553,
		    0.0514,
		    0.0476,
		    0.0440,
		    0.0406,
		    0.0374,
		    0.0344,
		    0.0315,
		    0.0288,
		    0.0262,
		    0.0238,
		    0.0216,
		    0.0195,
		    0.0175,
		    0.0157,
		    0.0140,
		    0.0124,
		    0.0110,
		    0.0096,
		    0.0084,
		    0.0073,
		    0.0062,
		    0.0053,
		    0.0044,
		    0.0037,
		    0.0030,
		    0.0023};

    MIXER_T *window_mix = init_mixer(kaiser,MY_NSAMP,MY_NSAMP);

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

    	// Windowing here may be helpful to discern between frequencies if the velocities are close together
	    calc_mixer(window_mix, input1, input1);	// OK to use same mixer b/c
	    calc_mixer(window_mix, input2, input2);	// input1 & kaiser same size

    	// Interleave real and complex arrays for arm function format purposes
    	for (i=0; i<MY_NSAMP; i++) {
    		fft_in[2*i] = input1[i];
    		fft_in[2*i+1] = input2[i];
    	}

    	// Zero the rest of the fft_in array
    	for (i=2*MY_NSAMP; i<2*FFT_N; i++) {
    		fft_in[i] = 0.0;
    	}

    	// Calculate complex fft
    	arm_cfft_radix2_f32(&fft, fft_in);

    	// Calculate complex magnitude
    	arm_cmplx_mag_f32(fft_in, mag, FFT_N);
    
	    arm_max_f32(&mag[20],FFT_N/2-20,&max_fft,&max_ind); // calculate max value

	    for (i=0; i<MY_NSAMP; i++) {
	    	output2[i] = (mag[i]/(max_fft/2.0)) - 1.0;
	    }

	    DIGITAL_IO_RESET();

    	// Pass output buffers to the DAC
    	putblockstereo(&ramp[0],output2);
      
  }
}