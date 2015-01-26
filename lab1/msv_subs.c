/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 1 Subroutine Definitions
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Jan 26, 2015
 * 
 * This file contains the subroutine definitions necessary for calculating the 
 * running mean-square-values of a block of samples.
 *
 * The init_msv() function initializes a struct of type MSV_STRUCT_T. It stores
 * the number of samples to average over 'M', the number of samples in a block
 * 'blocksize', and a circular buffer of past samples 'history'. It returns a
 * pointer to said struct.
 *
 * The calc_msv() function performs the running mean-square-value calculations
 * on a block of samples. It takes a float pointer 'x' to indicate the array of 
 * input samples and a MSV_STRUCT_T pointer 's' to indicate the struct with the 
 * information necessary to perform the calcuations. It returns a float pointer
 * to indicate the array of output running mean-square-values.
 * 
 */

#include "msv_subs.h"
#include <stdio.h>

MSV_STRUCT_T * init_msv(uint M, uint blocksize){

	uint i;
	MSV_STRUCT_T s;

	s.M = M;
	s.blocksize = blocksize;
	s.history = malloc(sizeof(float)*(M-1));
	
	for(i=0;i<M;i++){
		s.history[i] = 0;
	}

  

}