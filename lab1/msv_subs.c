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
#include <stdlib.h>

MSV_STRUCT_T * init_msv(uint32_t M, uint32_t blocksize){

	MSV_STRUCT_T s;

	s.M = M;
	s.blocksize = blocksize;
	s.prevSquared = 0;
	s.histIndex = 0;
	s.history = calloc(M-1,sizeof(float));

  return &s;

}

float * calc_msv(float * x, MSV_STRUCT_T * s){

	//Allocate the output array
	float * y;
	y = calloc(s.blocksize,sizeof(float));

	uint32_t i;

	for(i=0;i<s.blocksize;i++){

			y[i] = (s.prevSquared + x[i]^2) / s.M;

			s.prevSquared -= s.history(s.histIndex);
			s.prevSquared += x[i]^2;
			s.history(s.histIndex) = x[i];
			s.histIndex += 1;

			if(s.histIndex == (s.M-1){
				s.histIndex -= (s.M-1);
			}

	}



	return &y;

}

