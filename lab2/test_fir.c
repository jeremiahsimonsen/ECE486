#include "ece486_fir.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	
	//////////////////////////////////////////////////////////////////////
	// Test 4 filter coefficients, where n_coefs > blocksize 						//
	// Should match (as verified using MATLAB):
	// 		y(0) = 1.000000
	// 		y(1) = 4.000000
	// 		y(2) = 10.000000
	// 		y(3) = 20.000000
	// 		y(4) = 30.000000
	// 		y(5) = 40.000000
	//////////////////////////////////////////////////////////////////////

	FIR_T *filt;											// declare a filter
	float h0[4] = {1,2,3,4};					// impulse response
	float x1[2] = {1,2};							// inputs
	float x2[2] = {3,4};
	float x3[2] = {5,6};

	float y1[2] = {0,0};							// outputs
	float y2[2] = {0,0};
	float y3[2] = {0,0};
	// float *y = x;

	filt = init_fir(h0,4,2);					// initialize the filter
	calc_fir(filt,x1,y1);							// calculate filter outputs
	calc_fir(filt,x2,y2);
	calc_fir(filt,x3,y3);

	// Display values
	int i;
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i,y1[i]);
	}
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i+2,y2[i]);
	}
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i+4,y3[i]);
	}
	printf("\n");

	destroy_fir(filt);

	//////////////////////////////////////////////////////////////////////
	// Test with 20 filter coefficients, where n_coefs equals blocksize //
	// Should match (as verified using MATLAB):
	// 		y(0) = 1.000000
	// 		y(1) = 3.000000
	// 		y(2) = 6.000000
	// 		y(3) = 10.000000
	// 		y(4) = 15.000000
	// 		y(5) = 21.000000
	// 		y(6) = 28.000000
	// 		y(7) = 36.000000
	// 		y(8) = 45.000000
	// 		y(9) = 55.000000
	// 		y(10) = 66.000000
	// 		y(11) = 78.000000
	// 		y(12) = 91.000000
	// 		y(13) = 105.000000
	// 		y(14) = 120.000000
	// 		y(15) = 136.000000
	// 		y(16) = 153.000000
	// 		y(17) = 171.000000
	// 		y(18) = 190.000000
	// 		y(19) = 210.000000
	//////////////////////////////////////////////////////////////////////
	FIR_T *f;		// Declare filter

	float h[20] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};	// impulse response
	float x[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};	// input
	float *y = x;			// output

	f = init_fir(h,20,20);	// initialize the filter
	calc_fir(f,x,y);				// calculate outputs

	// Display values
	for (i=0; i<20; i++) {
		printf("y(%d) = %f\n", i, y[i]);
	}
	printf("\n");

	destroy_fir(f);

	/////////////////////////////////////////////////////////////////////
	// Test with 2 filter coefficients, where n_coefs < blocksize			 //
	// Should match (as verified using MATLAB):
	// 		y(0) = 1.000000
	//  	y(1) = 4.000000
	//  	y(2) = 7.000000
	//  	y(3) = 10.000000
	/////////////////////////////////////////////////////////////////////

	FIR_T *f2;		// Declare filter

	float h2[2] = {1,2};			// impulse response
	float x4[4] = {1,2,3,4};	// input
	float *y4 = x4;						// output

	f2 = init_fir(h2, 2, 4);	// initialize the filter
	calc_fir(f2,x4,y4);				// calculate filter outputs

	// Display values
	for(i=0; i<4; i++) {
		printf("y(%d) = %f\n", i, y4[i]);
	}
	printf("\n");

	destroy_fir(f2);

  return 0;
}