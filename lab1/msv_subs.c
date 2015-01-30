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
 * The destroy_msv() function de-allocates all memory required for he mean squared
 * value calculation. The memory allocated for the MSV_STRUCT_T structure and output
 * array y[] is freed.
 * 
 */

#include "msv_subs.h"
#include <stdio.h>
#include <stdlib.h>

MSV_STRUCT_T * init_msv(uint32_t M, uint32_t blocksize){

	MSV_STRUCT_T * s;

	s = (MSV_STRUCT_T *) malloc(sizeof(MSV_STRUCT_T));

	s->M = M;
	s->blocksize = blocksize;
	s->prevSquared = 0;
	s->histIndex = 0;
	s->history = calloc(M-1,sizeof(float));

  return s;

}

float * calc_msv(float * x, MSV_STRUCT_T * s){

	//Allocate the output array
	float * y;
	y = (float *) calloc(s->blocksize,sizeof(float));

	uint32_t i;

	for(i=0;i<s->blocksize;i++){

			//Store the output value
			y[i] = (s->prevSquared + x[i]*x[i]) / s->M;

			//Update the circular buffer and history variable
			s->prevSquared -= (s->history[s->histIndex])*(s->history[s->histIndex]);
			s->prevSquared += x[i]*x[i];
			s->history[s->histIndex] = x[i];
			s->histIndex += 1;

			//Check to see if index is at end of circular buffer
			if(s->histIndex == (s->M-1)){
				s->histIndex -= (s->M-1);
			}

	}

	return y;

}

void destroy_msv(MSV_STRUCT_T * s, float * y){

	//Free output array memory
	free(y);

	//Free circular buffer
	free(s->history);

	//Free the rest of the structure
	free(s);

}

