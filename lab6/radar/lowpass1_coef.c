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

float lowpass_b_coef[3][3] = {{1, 0.974404, 1},{1, -0.814551, 1},{1, -1.21576, 1}};
float lowpass_a_coef[3][3] = {{1, -1.72821, 0.765629},{1, -1.69291, 0.847819},{1, -1.69008, 0.949722}};
float lowpass_g = 0.00048507;
int lowpass_num_stages = 3;