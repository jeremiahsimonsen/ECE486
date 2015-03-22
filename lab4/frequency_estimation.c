// frequency_estimation.c

/*************************************

This file contains the function "frequency_estimation"
that calculates the delta-f to determine whether the 
output should be a 1 or a 0.

The delta-f values should be either deltaf = +- .0521  
(+- 500Hz / 9.6ksps)

*************************************/

#define PI 3.141592653589

#include "frequency_estimation.h"



int frequency_estimation(int *a, int *b, int length) {

	int n;
	for(n = 0; n < length; n++) {
		// A(n) = real cos 	B(n) = imaginary sin
		// divide by magnitude which is sqrt(y(Re) * y(Re) + y(Im) * y (Im))
		a(n) = a(n) / sqrt((a(n)^2 + b(n)^2));
		b(n) = b(n) / sqrt((a(n)^2 + b(n)^2));

		// a(n) * a^*(n - 1)
		im = b(n) * a(n - 1) - a(n) * b(n - 1);
		deltaf = im / (2 * PI); 
		// deltaf = +- .0521  (+- 500Hz / 9.6ksps)
		return deltaf;

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




}

