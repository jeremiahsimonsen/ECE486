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

#include "filter1_coef.h"

float filter1_b_coef[3][3] = {{1, 0.696317, 1},{1, -1.07606, 1},{1, -1.4074, 1}};
float filter1_a_coef[3][3] = {{1, -1.7706, 0.797853},{1, -1.7554, 0.868469},{1, -1.76713, 0.956562}};
float filter1_g = 0.000352401;
int filter1_num_stages = 3;
