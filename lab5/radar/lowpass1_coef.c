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

float lowpass_b_coef[4][3] = {{1, -0.704424, 1},{1, -1.7473, 1},{1, -1.8619, 1},{1, -1.8882, 1}};
float lowpass_a_coef[4][3] = {{1, -1.88557, 0.892758},{1, -1.89528, 0.925868},{1, -1.90774, 0.962513},{1, -1.92152, 0.989109}};
float lowpass_g = 0.000143594;
int lowpass_num_stages = 4;