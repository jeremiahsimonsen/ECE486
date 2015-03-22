// frequency_estimation.h

/*************************************

This file contains the function "frequency_estimation"
that calculates the delta-f to determine whether the 
output should be a 1 or a 0.

The inputs are two pointers to the real and imaginary
terms of the input, and then length of the input arrays

*************************************/

int frequency_estimation(
		int *a;		// pointer to array of real cos terms
		int *b;		// pointer to array of imaginary sin terms
		int length  // number of terms in a array
	);