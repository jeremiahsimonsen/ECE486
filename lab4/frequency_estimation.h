/**
 * @file
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


/**
 * @brief Performs frequency estimation on a block of samples
 * 
 * @return A pointer to a float array containing the calculated delta-f values
 * for the block of samples
 */
float *delta_f(
		float *y_re,		// pointer to array of real terms
		float *y_im,		// pointer to array of imaginary sin terms
		int blocksize	// number of terms in a array
);