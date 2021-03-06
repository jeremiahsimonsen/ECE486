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

#include "dcblock.h"

float dcblock_b_coef[2][3] = {{1, -1.99999, 1},{1, -1.99994, 1}};
float dcblock_a_coef[2][3] = {{1, -1.90826, 0.912301},{1, -1.986, 0.987446}};
float dcblock_g = 0.895743;
int dcblock_num_stages = 2;