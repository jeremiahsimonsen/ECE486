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

float lowpass_b_coef[3][3] = {{1, 1, 0},{1, -1.09249, 1},{1, -1.57893, 1}};
float lowpass_a_coef[3][3] = {{1, -0.923153, 0},{1, -1.85562, 0.885138},{1, -1.89228, 0.958395}};
float lowpass_g = 0.000196222;
int lowpass_num_stages = 3;