/*!
 * @file
 * 
 * @brief ECE 486 Spring 2014 Lab 1
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Jan 30, 2015
 * 
 * A simple program to test the mean squared value calculation of
 * an input array of 'blocksize' samples, averaging over 'M' samples.
 * 
 */

#include "msv_subs.c"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*!
 * @brief Test routine for the #msv_init, #msv_calc, and #destroy_msv functions.
 * 
 * Three test arrays @x1, @x2, and @x3 of length @blocksize1, @blocksize2, and
 * @blocksize3 are initialized. An @MSV_STRUCT_T is initialized for each sample array
 * using the #init_msv function. The mean squared value is calculated for each sample array
 * using the #calc_msv function, averaged over @M1, @M2, and @M3 samples respectively. Three
 * output arrays @y1, @y2, and @y3 are returned by #calc_msv containing the output samples
 * of the mean squared value calculation of @x1, @x2, and @x3, respectively. Each of these arrays
 * is printed to standard output. Lastly, all memory allocated is freed by calling #destroy_msv for
 * each @MSV_STRUCT_T and output array.
 * 
 * @returns	Returns 0 on success.
 */

int main(int argc, char *argv[]) {

  return 0;
}