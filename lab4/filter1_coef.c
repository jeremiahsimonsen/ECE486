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

float filter1_b_coef[6][3] = {{1, -1.1764, 1},{1, -1.9997, 1},{1, 0.5368, 1},{1, -2, 1},{1, -1.4668, 0.999995},{1, -1.9995, 1}};
float filter1_a_coef[6][3] = {{1, -1.7687, 0.8927},{1, -1.9824, 0.9848},{1, -1.8212, 0.8630},{1, -1.9368, 0.9435},{1, -1.7732, 0.9634},{1, -1.995, 1.9966}};
float filter1_g = 0.000300412;
int filter1_num_stages = 6;