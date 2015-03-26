/**
 * @file
 * 
 * @brief ECE 486 Spring 2015 DC rejection routine source file
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 23, 2015
 * 
 * This file contains subroutine definitions necessary for
 * running the DC rejection filter routine.
 */

 #include "rejectDC.h"
 #include <stdlib.h>


DCBLOCK_T *init_dcblock(int bSize) {

	// Allocate memory for DCBLOCK_T structure
	DCBLOCK_T *s;
	s = (DCBLOCK_T *)malloc(sizeof(DCBLOCK_T));
	if (s == NULL) return NULL;

	// initialize variables
	s->blockSize = bSize;
	// s->x_prev[0] = 0.0;
	// s->x_prev[1] = 0.0;
	s->x_prev = 0.0;
	s->y_prev = 0.0;

	return s;
}


void calc_dcblock(DCBLOCK_T *s,	float *x, float *y) {

	int n;
	for (n = 0; n < s->blockSize; n++) {
		if (n == 0) {
			// y[n] = 0.99*s->y_prev + 0.995*(s->x_prev[0] - s->x_prev[1]);
			y[n] = 0.99*s->y_prev + x[n] - s->x_prev;

		} else {
			// y[n] = 0.99*y[n-1] + 0.995*(x[n-1] - x[n-2]);
			y[n] = 0.99*y[n-1] + x[n] - x[n-1];
		}
	}

	// Update last values
	// s->x_prev[0] = x[s->blockSize-1];
	// s->x_prev[1] = x[s->blockSize-2];
	s->x_prev = x[s->blockSize-1];
	s->y_prev = y[s->blockSize-1];

}


void destroy_dcblock(DCBLOCK_T *s) {
	free(s);
	s = NULL;
}