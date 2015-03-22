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

#include filter2_coef.h

float filter2_b_coef[2][3] = {{1, 1, 0},{1, 1.93369, 1}};
float filter2_a_coef[2][3] = {{1, -0.528901, 0},{1, -0.727876, 0.637883}};
float filter2_g = 0.0544912;
int filter2_num_stages = 2;