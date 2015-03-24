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

float filter1_b_coef[6][3] = {{1, 0.536782, 1},{1, -1.17636, 1},{1, -1.99996, 1},{1, -1.46681, 1},{1, -1.99969, 0.999995},{1, -1.99949, 1}};
float filter1_a_coef[6][3] = {{1, -1.82119, 0.863015},{1, -1.76869, 0.892714},{1, -1.9373, 0.944014},{1, -1.77323, 0.963391},{1, -1.97833, 0.980678},{1, -1.99864, 1.00022}};
float filter1_g = 0.999875;
int filter1_num_stages = 6;