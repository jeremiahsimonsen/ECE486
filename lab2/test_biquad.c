/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 2 Biquad/IIR test
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Feb 13, 2015
 * 
 * This file tests the BIQUAD subroutines
 * 
 */

#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	BIQUAD_T *f;

	float x1[10] = {1, 0.5, -0.8, 0.2, 0, 0, 0, 0, 0, 0};
	float x2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float y1[10] = {0};
	float y2[10] = {0};

	float a[2][3] = {{1.0, -0.6088, 0.9702},
					 {1.0, -0.3500, -0.4250}};

	float b[2][3] = {{2.2044, 0.0, 2.2044},
					 {2.9658, -3.4865, 2.9658}};

	int sections = 2;
	float g = 0.01718740;
	int blocksize = 10;
	f = init_biquad(sections,g,a,b,blocksize);
	calc_biquad(f, x1, y1);
	calc_biquad(f, x2, y2);

	int i;
	//////////////////////////////////////////////////////////////////////					
	// Should match (as verified from Homework 2):
	//    y(1) =   1.0    0.0379    0.1124
  	//    y(2) =   0.5    0.0420    0.0318
	//    y(3) =  -0.8   -0.0036    0.0141
	//    y(4) =   0.2   -0.0164    0.1069
	//    y(5) =   0.0   -0.0368   -0.0192
	//    y(6) =   0.0    0.0011    0.1216
	//    y(7) =   0.0    0.0364    0.0293
	//    y(8) =   0.0    0.0211    0.0009
	//    y(9) =   0.0   -0.0225   -0.0195
	//   y(10) =   0.0   -0.0341    0.0332
	//   y(11) =   0.0    0.0010    0.0587
	//   y(12) =   0.0    0.0337    0.0299
	//   y(13) =   0.0    0.0196   -0.0212
	//   y(14) =   0.0   -0.0208   -0.0246
	//   y(15) =   0.0   -0.0317    0.0191
	//   y(16) =   0.0    0.0009    0.0476
	//   y(17) =   0.0    0.0313    0.0204
	//   y(18) =   0.0    0.0181   -0.0251
	//   y(19) =   0.0   -0.0193   -0.0278
	//   y(20) =   0.0   -0.0293    0.0137
	//////////////////////////////////////////////////////////////////////
	for (i = 0; i < 10; i++) {
		printf("y(%d) = %f\n", i, y1[i]);
	}
	printf("\n");
	for (i = 0; i < 10; i++) {
		printf("y(%d) = %f\n", i, y2[i]);
	}
	printf("\n");

	return 0;
}