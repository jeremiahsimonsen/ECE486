// frequency_estimation.c

/*************************************

This file contains the function "frequency_estimation"
that calculates the delta-f to determine whether the 
output should be a 1 or a 0.

The delta-f values should be either deltaf = +- .0521  
(+- 500Hz / 9.6ksps)

*************************************/

#define PI 3.141592653589

#include <stdlib.h>

#include "frequency_estimation.h"
#include "arm_math.h"


/**
 * @brief [brief description]
 * @details [long description]
 * 
 * @param a [description]
 * @param b [description]
 * @param length [description]
 * @return [description]
 */
float *delta_f(float *y_re, float *y_im, int blocksize) {
	// Store last value of complex a(n) for continuity between sample blocks
	static float last_a_re = 0.0, last_a_im = 0.0;

	float *df = (float *)malloc(sizeof(float)*blocksize);
	float mag = 0.0;
	int n;

	for(n = 0; n < blocksize; n++) {
		// Calculate complex a(n) by normalizing y(n)
		mag = sqrtf(y_re[n]*y_re[n] + y_im[n]*y_im[n]);
		y_re[n] = y_re[n] / mag;
		y_im[n] = y_im[n] / mag;

		// Calculate complex b(n) = a(n)a*(n-1)
		// b_re(n) = a_re(n)a_re(n-1) + a_im(n)a_im(n-1); not needed
		// b_im(n) = -a_re(n)a_im(n-1) + a_im(n)a_re(n-1)
		if (n == 0) {
			df[n] = -y_re[n]*last_a_im + y_im[n]*last_a_re;
		} else {
			df[n] = -y_re[n]*y_im[n-1] + y_im[n]*y_re[n-1];
		}		

		// Calculate delta-f 
		// for the FSK detector, delta-f should be +- 0.0521 (+- 500 / 9.6k)
		df[n] = df[n] / (2 * PI);

	// 	high_margin_deltaf = deltaf + (deltaf * 0.05);
	// 	low_margin_deltaf = deltaf - (deltaf * 0.05);
	// 	// if deltaf is + then output 0
	// 	if ((low_margin_deltaf < abs(deltaf)) && (abs(deltaf) < high_margin_deltaf) && (deltaf > 0)) {
	// 		y(n) = 0;
	// 	} else if((low_margin_deltaf < abs(deltaf)) && (abs(deltaf) < high_margin_deltaf)){
	// 		y(n) = 1;
	// 	} else {
	// 		printf("deltaf is out of range\n");
	// 	}

	}

	// Update last values
	last_a_re = y_re[blocksize-1];
	last_a_im = y_im[blocksize-1];

	return df;

}

