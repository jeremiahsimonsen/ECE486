/**
 * @file frequency_estimation.c
 * 
 * @brief ECE 486 Spring 2015 Lab 4 Frequency Estimation routine
 * 
 * @author ECE 486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 22, 2015
 * 
 * This file contains the subroutine declarations necessary for calculating
 * the frequency of the input signal and hence the binary bit that the 
 * detector receives.
 * 
 */


#define PI 3.141592653589

#include <stdlib.h>

#include "frequency_estimation.h"
#include <math.h>
#include "arm_math.h"


/**
 * @brief Performs frequency estimation on a block of samples
 * @details Uses the parameters described below to return a high or 
 * low delta-f value to determine whether the detector reveived a 1 or
 * a 0.
 * 
 * @param *df is the array of delta-f values
 * @param *y_re is the array of real coefs used for calculations
 * @param *y_im is the array of imaginary coefs used for calculations
 * @param blocksize is the number of samples to calculate the delta-f for
 * @returns void. output values are found in df array
 */
void delta_f(float *df, float *y_re, float *y_im, int blocksize) {
	// Store last value of complex a(n) for continuity between sample blocks
	static float last_a_re = 0.0, last_a_im = 0.0;

	float mag = 0.0;
	int n;

	for(n = 0; n < blocksize; n++) {
		// Calculate complex a(n) by normalizing y(n)
		mag = sqrtf(y_re[n]*y_re[n] + y_im[n]*y_im[n]);
		y_re[n] = y_re[n] / mag;	// find real part
		y_im[n] = y_im[n] / mag;	// find imaginary part

		if (n == 0) {
			df[n] = -y_re[n]*last_a_im + y_im[n]*last_a_re;
		} else {
			df[n] = -y_re[n]*y_im[n-1] + y_im[n]*y_re[n-1];
		}		

		// Calculate delta-f 
		// for the FSK detector, delta-f should be +- 0.0521 (+- 500 / 9.6k)
		df[n] = df[n] / (2 * PI) * 6.4;
	}
	// Update last values
	last_a_re = y_re[blocksize-1];
	last_a_im = y_im[blocksize-1];
}

