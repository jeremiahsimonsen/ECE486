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

float_b_coef[5][3] = {{0, 0, 1},{1, -2, 1},{1, 2, 1},{1, -1.61803, 1},{1, 1.98423, 1}};
float a_coef[5][3] = {{1, 0.509939, 0.16},{1, -6.12323e-17, 0.25},{1, 0.309017, 0.25},{1, -0.562411, 0.8281},{1, 1.50477, 0.8649}};
float g = 0.0240748;