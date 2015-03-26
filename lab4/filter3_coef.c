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

#include "filter3_coef.h"

float filter3_b_coef[3][3] = {{1, 1.49562, 1},{1, 0.00648781, 1},{1, -0.527189, 1}};
float filter3_a_coef[3][3] = {{1, -1.58056, 0.663587},{1, -1.44727, 0.785228},{1, -1.36754, 0.929594}};
float filter3_g = 0.0013607;
int filter3_num_stages = 3;