/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 4 - Source file containing 
 * definitions of filter coefficients for external use.
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 *
 * @date Mar 22, 2015
 *
 * This file contains the definitions of filter coefficients
 * for external use. The coefficients are the output of the 
 * zp2biquad() function. This implementation allows for the
 * testing of new filters simply by re-running the zp2biquad()
 * function and then re-compiling.
 *
 */

#include "lowpass1_coef.h"

float lowpass_b_coef[3][3] = {{1, -0.352766, 1},{1, -1.63082, 1},{1, -1.7776, 1}};
float lowpass_a_coef[3][3] = {{1, -1.86694, 0.876615},{1, -1.87964, 0.919766},{1, -1.90684, 0.973663}};
float lowpass_g = 0.000171036;
int lowpass_num_stages = 3;