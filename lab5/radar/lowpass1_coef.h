/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 4 - Header file declaring externally
 * defined filter coefficients.
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 22, 2015
 * 
 * This file contains the declaration of the filter coefficients that will
 * defined in filter1_coef.c The coefficients will be the output of the 
 * zp2biquad() MATLAB function. This implementation allows for the implementation
 * of a new filter by calling the zp2biquad() function with the new poles
 * and zeros and then re-compiling.
 *
 * This header file must be included by any program that uses the filter
 * coefficients output by the zp2biquad() function.
 * 
 */

#ifndef LOWPASS1_COEF_H
#define LOWPASS1_COEF_H

extern float lowpass1_a_coef[][3];
extern float lowpass1_b_coef[][3];
extern float lowpass1_g;
extern int lowpass1_num_stages;

#endif