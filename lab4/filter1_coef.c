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

<<<<<<< HEAD
float filter1_b_coef[3][3] = {{1, 0.696317, 1},{1, -1.07606, 1},{1, -1.4074, 1}};
float filter1_a_coef[3][3] = {{1, -1.7706, 0.797853},{1, -1.7554, 0.868469},{1, -1.76713, 0.956562}};
float filter1_g = 0.000352401;
int filter1_num_stages = 3;
=======
float filter1_b_coef[6][3] = {{1, 0.592425, 1},{1, -1.1434, 1},{1, -1.99995, 0.999987},{1, -1.44755, 1},{1, -1.99992, 1.00002},{1, -1.99976, 0.999994}};
float filter1_a_coef[6][3] = {{1, -1.81414, 0.849941},{1, -1.76499, 0.885452},{1, -1.94495, 0.948642},{1, -1.77132, 0.961235},{1, -1.98819, 0.988376},{1, -1.99957, 1.0011}};
float filter1_g = 0.998565025836217;
int filter1_num_stages = 6;
>>>>>>> ded0b7f45fb9b501e47dcd1f20c9a721b4b085df
