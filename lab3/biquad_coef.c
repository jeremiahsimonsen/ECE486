/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 3 - Source file containing 
 * definitions of the filter coefficients for external use.
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 *
 * @date Mar 10, 2015
 *
 * This file contains the definitions of the filter coefficients
 * for external use. The coefficients are the output of the 
 * zp2biquad() function. This implementation allows for the
 * testing of new filters simply by re-running the zp2biquad()
 * function and then re-compiling the task 4 test program.
 *
 */

#include "biquad_coef.h"

float_b_coef[5][3] = {{1, 2, 1},{1, -2, 1},{1, 0.851559, 1},{1, -1.90211, 1},{1, 1.17557, 1}};
float a_coef[5][3] = {{1, -0.224858, 0.36},{1, -0.790406, 0.3844},{1, -1.08994e-16, 0.7921},{1, -1.50477, 0.8649},{1, 1.15206, 0.9604}};
float g = 0.0264722;
int num_stages = 5;