/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 6 DSP for a doppler radar detector
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Apr 24, 2015
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
#define D1 4
#define D2 4
#define MY_NSAMP 776
#define BLOCKS 16
#define FFT_N 1024
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

	// Set the ADC/DAC Block size; note the code below assumes MY_NSAMP is a
	// multiple of D1
	setblocksize( MY_NSAMP );

	// Set up the required sample rate and ADC/DAC input/output
	initialize(FS_24K, STEREO_IN, MONO_OUT);

	// Index variables
	int i, start;

	// Which velocities to send to the DAC
	int positives = 1;

	// ADC inputs
	float *input1 = (float *)malloc(sizeof(float)*MY_NSAMP);
	float *input2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// DAC outputs
 	float *output1 = (float *)malloc(sizeof(float)*MY_NSAMP);
 	// float *output2 = (float *)malloc(sizeof(float)*MY_NSAMP);

 	// Decimated real and imaginary signals; outputs of lowpass filter
 	float32_t *buffer_re = (float32_t *)malloc(sizeof(float32_t)*MY_NSAMP/D1);
 	float32_t *buffer_im = (float32_t *)malloc(sizeof(float32_t)*MY_NSAMP/D1);

 	// Input (and output) to the complex FFT; interleaved of above and zero pad
 	float32_t *fft_in = (float32_t *)malloc(sizeof(float32_t)*FFT_N*2);

 	// Magnitude of the FFT
 	float32_t *mag = (float32_t *)malloc(sizeof(float32_t)*FFT_N);

 	// Maxima
 	// int one_ms;
 	// float max_pos_val = 0.0;
 	// float max_neg_val = 0.0;
 	// float max_pos_ind = 0;
 	// float max_neg_ind = 0;
 	// float32_t max_pos_freq = 0.0;
 	// float32_t max_neg_freq = 0.0;
 	float32_t max_fft = 1.0;
 	uint32_t max_ind = 0;

 	// Velocities
 	// float pos_vel, neg_vel;

 	// Complex FFT structure initializations	
 	arm_cfft_radix2_instance_f32 fft;
	arm_status status = arm_cfft_radix2_init_f32(&fft, FFT_N, FORWARD_FFT, 1);
	if (status != ARM_MATH_SUCCESS)
		flagerror(MEMORY_ALLOCATION_ERROR);
 	
 	// Error check memory allocation
 	if (input1==NULL || input2==NULL || output1==NULL || buffer_re==NULL || buffer_im==NULL || fft_in==NULL || mag==NULL) {
 		flagerror(MEMORY_ALLOCATION_ERROR);
 		while(1);
 	}
  
 	// Filter initializations
	BIQUAD_T *low1_re = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, MY_NSAMP);
	BIQUAD_T *low1_im = init_biquad(lowpass_num_stages, lowpass_g, lowpass_a_coef, lowpass_b_coef, MY_NSAMP);

	// Windowing 
	float kaiser[776] = {0.0023,
		    0.0025,
		    0.0027,
		    0.0030,
		    0.0032,
		    0.0034,
		    0.0036,
		    0.0039,
		    0.0041,
		    0.0044,
		    0.0047,
		    0.0050,
		    0.0052,
		    0.0055,
		    0.0058,
		    0.0062,
		    0.0065,
		    0.0068,
		    0.0072,
		    0.0075,
		    0.0079,
		    0.0083,
		    0.0087,
		    0.0091,
		    0.0095,
		    0.0099,
		    0.0104,
		    0.0108,
		    0.0113,
		    0.0118,
		    0.0122,
		    0.0127,
		    0.0133,
		    0.0138,
		    0.0143,
		    0.0149,
		    0.0154,
		    0.0160,
		    0.0166,
		    0.0172,
		    0.0178,
		    0.0185,
		    0.0191,
		    0.0198,
		    0.0205,
		    0.0212,
		    0.0219,
		    0.0226,
		    0.0234,
		    0.0241,
		    0.0249,
		    0.0257,
		    0.0265,
		    0.0273,
		    0.0282,
		    0.0290,
		    0.0299,
		    0.0308,
		    0.0317,
		    0.0326,
		    0.0336,
		    0.0346,
		    0.0355,
		    0.0366,
		    0.0376,
		    0.0386,
		    0.0397,
		    0.0408,
		    0.0419,
		    0.0430,
		    0.0441,
		    0.0453,
		    0.0465,
		    0.0477,
		    0.0489,
		    0.0501,
		    0.0514,
		    0.0527,
		    0.0540,
		    0.0553,
		    0.0566,
		    0.0580,
		    0.0594,
		    0.0608,
		    0.0622,
		    0.0637,
		    0.0652,
		    0.0667,
		    0.0682,
		    0.0697,
		    0.0713,
		    0.0729,
		    0.0745,
		    0.0762,
		    0.0778,
		    0.0795,
		    0.0812,
		    0.0830,
		    0.0847,
		    0.0865,
		    0.0883,
		    0.0901,
		    0.0920,
		    0.0939,
		    0.0958,
		    0.0977,
		    0.0997,
		    0.1016,
		    0.1036,
		    0.1057,
		    0.1077,
		    0.1098,
		    0.1119,
		    0.1140,
		    0.1162,
		    0.1184,
		    0.1206,
		    0.1228,
		    0.1251,
		    0.1273,
		    0.1296,
		    0.1320,
		    0.1343,
		    0.1367,
		    0.1391,
		    0.1416,
		    0.1440,
		    0.1465,
		    0.1490,
		    0.1516,
		    0.1542,
		    0.1567,
		    0.1594,
		    0.1620,
		    0.1647,
		    0.1674,
		    0.1701,
		    0.1729,
		    0.1756,
		    0.1784,
		    0.1813,
		    0.1841,
		    0.1870,
		    0.1899,
		    0.1928,
		    0.1958,
		    0.1988,
		    0.2018,
		    0.2048,
		    0.2079,
		    0.2110,
		    0.2141,
		    0.2172,
		    0.2204,
		    0.2236,
		    0.2268,
		    0.2300,
		    0.2333,
		    0.2366,
		    0.2399,
		    0.2432,
		    0.2466,
		    0.2499,
		    0.2534,
		    0.2568,
		    0.2602,
		    0.2637,
		    0.2672,
		    0.2708,
		    0.2743,
		    0.2779,
		    0.2815,
		    0.2851,
		    0.2887,
		    0.2924,
		    0.2961,
		    0.2998,
		    0.3035,
		    0.3073,
		    0.3110,
		    0.3148,
		    0.3186,
		    0.3225,
		    0.3263,
		    0.3302,
		    0.3341,
		    0.3380,
		    0.3419,
		    0.3459,
		    0.3499,
		    0.3538,
		    0.3579,
		    0.3619,
		    0.3659,
		    0.3700,
		    0.3741,
		    0.3782,
		    0.3823,
		    0.3864,
		    0.3906,
		    0.3947,
		    0.3989,
		    0.4031,
		    0.4073,
		    0.4115,
		    0.4158,
		    0.4200,
		    0.4243,
		    0.4286,
		    0.4328,
		    0.4371,
		    0.4415,
		    0.4458,
		    0.4501,
		    0.4545,
		    0.4588,
		    0.4632,
		    0.4676,
		    0.4720,
		    0.4764,
		    0.4808,
		    0.4852,
		    0.4896,
		    0.4941,
		    0.4985,
		    0.5030,
		    0.5074,
		    0.5119,
		    0.5164,
		    0.5208,
		    0.5253,
		    0.5298,
		    0.5343,
		    0.5388,
		    0.5433,
		    0.5478,
		    0.5523,
		    0.5568,
		    0.5613,
		    0.5658,
		    0.5703,
		    0.5748,
		    0.5793,
		    0.5838,
		    0.5883,
		    0.5928,
		    0.5973,
		    0.6018,
		    0.6063,
		    0.6108,
		    0.6153,
		    0.6198,
		    0.6242,
		    0.6287,
		    0.6332,
		    0.6377,
		    0.6421,
		    0.6466,
		    0.6510,
		    0.6554,
		    0.6599,
		    0.6643,
		    0.6687,
		    0.6731,
		    0.6775,
		    0.6818,
		    0.6862,
		    0.6906,
		    0.6949,
		    0.6992,
		    0.7035,
		    0.7078,
		    0.7121,
		    0.7164,
		    0.7206,
		    0.7249,
		    0.7291,
		    0.7333,
		    0.7375,
		    0.7417,
		    0.7458,
		    0.7500,
		    0.7541,
		    0.7582,
		    0.7623,
		    0.7663,
		    0.7704,
		    0.7744,
		    0.7784,
		    0.7823,
		    0.7863,
		    0.7902,
		    0.7941,
		    0.7980,
		    0.8019,
		    0.8057,
		    0.8095,
		    0.8133,
		    0.8170,
		    0.8207,
		    0.8244,
		    0.8281,
		    0.8317,
		    0.8354,
		    0.8389,
		    0.8425,
		    0.8460,
		    0.8495,
		    0.8530,
		    0.8564,
		    0.8598,
		    0.8632,
		    0.8665,
		    0.8698,
		    0.8731,
		    0.8763,
		    0.8795,
		    0.8827,
		    0.8858,
		    0.8889,
		    0.8920,
		    0.8950,
		    0.8980,
		    0.9009,
		    0.9038,
		    0.9067,
		    0.9096,
		    0.9124,
		    0.9151,
		    0.9179,
		    0.9205,
		    0.9232,
		    0.9258,
		    0.9284,
		    0.9309,
		    0.9334,
		    0.9358,
		    0.9382,
		    0.9406,
		    0.9429,
		    0.9452,
		    0.9474,
		    0.9496,
		    0.9517,
		    0.9538,
		    0.9559,
		    0.9579,
		    0.9599,
		    0.9618,
		    0.9637,
		    0.9655,
		    0.9673,
		    0.9690,
		    0.9707,
		    0.9724,
		    0.9740,
		    0.9755,
		    0.9771,
		    0.9785,
		    0.9799,
		    0.9813,
		    0.9826,
		    0.9839,
		    0.9851,
		    0.9863,
		    0.9875,
		    0.9885,
		    0.9896,
		    0.9906,
		    0.9915,
		    0.9924,
		    0.9932,
		    0.9940,
		    0.9948,
		    0.9955,
		    0.9961,
		    0.9967,
		    0.9973,
		    0.9978,
		    0.9982,
		    0.9986,
		    0.9989,
		    0.9992,
		    0.9995,
		    0.9997,
		    0.9998,
		    0.9999,
		    1.0000,
		    1.0000,
		    0.9999,
		    0.9998,
		    0.9997,
		    0.9995,
		    0.9992,
		    0.9989,
		    0.9986,
		    0.9982,
		    0.9978,
		    0.9973,
		    0.9967,
		    0.9961,
		    0.9955,
		    0.9948,
		    0.9940,
		    0.9932,
		    0.9924,
		    0.9915,
		    0.9906,
		    0.9896,
		    0.9885,
		    0.9875,
		    0.9863,
		    0.9851,
		    0.9839,
		    0.9826,
		    0.9813,
		    0.9799,
		    0.9785,
		    0.9771,
		    0.9755,
		    0.9740,
		    0.9724,
		    0.9707,
		    0.9690,
		    0.9673,
		    0.9655,
		    0.9637,
		    0.9618,
		    0.9599,
		    0.9579,
		    0.9559,
		    0.9538,
		    0.9517,
		    0.9496,
		    0.9474,
		    0.9452,
		    0.9429,
		    0.9406,
		    0.9382,
		    0.9358,
		    0.9334,
		    0.9309,
		    0.9284,
		    0.9258,
		    0.9232,
		    0.9205,
		    0.9179,
		    0.9151,
		    0.9124,
		    0.9096,
		    0.9067,
		    0.9038,
		    0.9009,
		    0.8980,
		    0.8950,
		    0.8920,
		    0.8889,
		    0.8858,
		    0.8827,
		    0.8795,
		    0.8763,
		    0.8731,
		    0.8698,
		    0.8665,
		    0.8632,
		    0.8598,
		    0.8564,
		    0.8530,
		    0.8495,
		    0.8460,
		    0.8425,
		    0.8389,
		    0.8354,
		    0.8317,
		    0.8281,
		    0.8244,
		    0.8207,
		    0.8170,
		    0.8133,
		    0.8095,
		    0.8057,
		    0.8019,
		    0.7980,
		    0.7941,
		    0.7902,
		    0.7863,
		    0.7823,
		    0.7784,
		    0.7744,
		    0.7704,
		    0.7663,
		    0.7623,
		    0.7582,
		    0.7541,
		    0.7500,
		    0.7458,
		    0.7417,
		    0.7375,
		    0.7333,
		    0.7291,
		    0.7249,
		    0.7206,
		    0.7164,
		    0.7121,
		    0.7078,
		    0.7035,
		    0.6992,
		    0.6949,
		    0.6906,
		    0.6862,
		    0.6818,
		    0.6775,
		    0.6731,
		    0.6687,
		    0.6643,
		    0.6599,
		    0.6554,
		    0.6510,
		    0.6466,
		    0.6421,
		    0.6377,
		    0.6332,
		    0.6287,
		    0.6242,
		    0.6198,
		    0.6153,
		    0.6108,
		    0.6063,
		    0.6018,
		    0.5973,
		    0.5928,
		    0.5883,
		    0.5838,
		    0.5793,
		    0.5748,
		    0.5703,
		    0.5658,
		    0.5613,
		    0.5568,
		    0.5523,
		    0.5478,
		    0.5433,
		    0.5388,
		    0.5343,
		    0.5298,
		    0.5253,
		    0.5208,
		    0.5164,
		    0.5119,
		    0.5074,
		    0.5030,
		    0.4985,
		    0.4941,
		    0.4896,
		    0.4852,
		    0.4808,
		    0.4764,
		    0.4720,
		    0.4676,
		    0.4632,
		    0.4588,
		    0.4545,
		    0.4501,
		    0.4458,
		    0.4415,
		    0.4371,
		    0.4328,
		    0.4286,
		    0.4243,
		    0.4200,
		    0.4158,
		    0.4115,
		    0.4073,
		    0.4031,
		    0.3989,
		    0.3947,
		    0.3906,
		    0.3864,
		    0.3823,
		    0.3782,
		    0.3741,
		    0.3700,
		    0.3659,
		    0.3619,
		    0.3579,
		    0.3538,
		    0.3499,
		    0.3459,
		    0.3419,
		    0.3380,
		    0.3341,
		    0.3302,
		    0.3263,
		    0.3225,
		    0.3186,
		    0.3148,
		    0.3110,
		    0.3073,
		    0.3035,
		    0.2998,
		    0.2961,
		    0.2924,
		    0.2887,
		    0.2851,
		    0.2815,
		    0.2779,
		    0.2743,
		    0.2708,
		    0.2672,
		    0.2637,
		    0.2602,
		    0.2568,
		    0.2534,
		    0.2499,
		    0.2466,
		    0.2432,
		    0.2399,
		    0.2366,
		    0.2333,
		    0.2300,
		    0.2268,
		    0.2236,
		    0.2204,
		    0.2172,
		    0.2141,
		    0.2110,
		    0.2079,
		    0.2048,
		    0.2018,
		    0.1988,
		    0.1958,
		    0.1928,
		    0.1899,
		    0.1870,
		    0.1841,
		    0.1813,
		    0.1784,
		    0.1756,
		    0.1729,
		    0.1701,
		    0.1674,
		    0.1647,
		    0.1620,
		    0.1594,
		    0.1567,
		    0.1542,
		    0.1516,
		    0.1490,
		    0.1465,
		    0.1440,
		    0.1416,
		    0.1391,
		    0.1367,
		    0.1343,
		    0.1320,
		    0.1296,
		    0.1273,
		    0.1251,
		    0.1228,
		    0.1206,
		    0.1184,
		    0.1162,
		    0.1140,
		    0.1119,
		    0.1098,
		    0.1077,
		    0.1057,
		    0.1036,
		    0.1016,
		    0.0997,
		    0.0977,
		    0.0958,
		    0.0939,
		    0.0920,
		    0.0901,
		    0.0883,
		    0.0865,
		    0.0847,
		    0.0830,
		    0.0812,
		    0.0795,
		    0.0778,
		    0.0762,
		    0.0745,
		    0.0729,
		    0.0713,
		    0.0697,
		    0.0682,
		    0.0667,
		    0.0652,
		    0.0637,
		    0.0622,
		    0.0608,
		    0.0594,
		    0.0580,
		    0.0566,
		    0.0553,
		    0.0540,
		    0.0527,
		    0.0514,
		    0.0501,
		    0.0489,
		    0.0477,
		    0.0465,
		    0.0453,
		    0.0441,
		    0.0430,
		    0.0419,
		    0.0408,
		    0.0397,
		    0.0386,
		    0.0376,
		    0.0366,
		    0.0355,
		    0.0346,
		    0.0336,
		    0.0326,
		    0.0317,
		    0.0308,
		    0.0299,
		    0.0290,
		    0.0282,
		    0.0273,
		    0.0265,
		    0.0257,
		    0.0249,
		    0.0241,
		    0.0234,
		    0.0226,
		    0.0219,
		    0.0212,
		    0.0205,
		    0.0198,
		    0.0191,
		    0.0185,
		    0.0178,
		    0.0172,
		    0.0166,
		    0.0160,
		    0.0154,
		    0.0149,
		    0.0143,
		    0.0138,
		    0.0133,
		    0.0127,
		    0.0122,
		    0.0118,
		    0.0113,
		    0.0108,
		    0.0104,
		    0.0099,
		    0.0095,
		    0.0091,
		    0.0087,
		    0.0083,
		    0.0079,
		    0.0075,
		    0.0072,
		    0.0068,
		    0.0065,
		    0.0062,
		    0.0058,
		    0.0055,
		    0.0052,
		    0.0050,
		    0.0047,
		    0.0044,
		    0.0041,
		    0.0039,
		    0.0036,
		    0.0034,
		    0.0032,
		    0.0030,
		    0.0027,
		    0.0025,
		    0.0023};

    MIXER_T *window_mix = init_mixer(kaiser,MY_NSAMP,MY_NSAMP);

	// Infinite Loop to process the data stream "MY_NSAMP" samples at a time
	while(1){
		DIGITAL_IO_SET();

		// Collect a block of samples from the ADC; waits until the input
		// buffer is filled
		getblockstereo(input1,input2);
    
    	DIGITAL_IO_RESET();

    	/*
    	 * Stage 1:  Complete processing at the incoming sample frequency fs.
    	 *           (Array size MY_NSAMP samples)
    	 */
		// DIGITAL_IO_SET();

    	// Lowpass filter
		calc_biquad(low1_re,input1,input1);
		calc_biquad(low1_im,input2,input2);

    	// Decimate by D1
    	for (i=0; i<MY_NSAMP/D1; i++) {
    		buffer_re[i] = input1[i*D1];
    		buffer_im[i] = input2[i*D1];
    	}

    	// We are going to wait for several blocks of input samples, so keep
    	// track of the index to start inserting new samples at
		start = block_count*2*MY_NSAMP/D1;
    	// Interleave real and complex arrays for arm function format purposes
    	for (i=0; i<MY_NSAMP/D1; i++) {
    		fft_in[start+2*i] = buffer_re[i];
    		fft_in[start+2*i+1] = buffer_im[i];
    	}
    	block_count++;

    	if (block_count == D2) {
    		block_count = 0;

	    	// Zero the rest of the fft_in array
	    	for (i=2*MY_NSAMP; i<2*FFT_N; i++) {
	    		fft_in[i] = 0.0;
	    	}
			
			// Windowing here may be helpful to discern between frequencies if the velocities are close together
	    	calc_mixer(window_mix, fft_in, fft_in);

	    	// Calculate complex fft
	    	arm_cfft_radix2_f32(&fft, fft_in);

	    	// DIGITAL_IO_RESET();

	    	// Calculate complex magnitude
	    	arm_cmplx_mag_f32(fft_in, mag, FFT_N);

	    	// Find the frequencies corresponding to the highest positive and
	    	// negative velocities. Positive frequencies are in the first half
	    	// of the 'mag' array, negatives in the second

	    	// Only search frequencies corresponding to 1 m/s or above; 1 m/s
	    	// corresponds to 38.667 Hz
	    	// one_ms = (int) 38.667/MY_FS/D1*FFT_N;	// Calculate index of 1 m/s

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
	    	// max_pos_freq = (max_pos_ind / ((float)FFT_N) ) * MY_FS / ((float)D1);
	    	// max_neg_freq = ( (max_neg_ind / ((float)FFT_N) ) - 1) * MY_FS / ((float)D1);
	    	// max_pos_freq = (max_pos_ind/1024.0) * 48000.0 / 10.0;
	    	// max_neg_freq = ( (max_neg_ind - 1024.0)/1024.0 ) * 48000.0 / 10.0;

	    	// Calculate velocity
			// pos_vel = max_pos_freq * 3e8 / (2*5.8e9)
			// pos_vel = max_pos_freq * 0.025862068966;
			// neg_vel = max_neg_freq * 0.025862068966;

	    
	    	// Detect button presses to switch what is being sent to the DAC
	    	if(UserButtonPressed==Button_Pressed) {		// Button Press?
				positives = !positives;

				UserButtonPressed = Button_Ready;
		    }
		    
		    arm_max_f32(mag,FFT_N,&max_fft,&max_ind);

		    // Write output values to the DAC
		    for(i=0; i<FFT_N/2; i++) {
		    	if (positives) {
		    		// arm_max_f32(&mag[0],FFT_N/2,&max_fft,&max_ind);
		    		output1[i] = (mag[i]/(max_fft/2.0)) - 1.0;
		    	} else {
		    		// arm_max_f32(&mag[FFT_N/2],FFT_N/2,&max_fft,&max_ind);
		    		output1[i] = (mag[FFT_N-1-i]/(max_fft/2.0)) - 1.0;
		    	}
		    }
		    for(i=FFT_N/2; i<MY_NSAMP; i++) {
		    	output1[i] = -1.0;
		    }
    	}

    	// Pass output buffer to the DAC
    	putblock(output1);
      
  }
}