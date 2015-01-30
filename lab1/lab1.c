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
  // Test when M < blocksize
  float x1[7] = {1,2,3,4,5,6,7};      // Initialize sample array of blocksize 7
  MSV_STRUCT_T *s1 = init_msv(3,7);   // Initialize struct for storing calculation info
  float *y1 = calc_msv(x1,s1);          // Calculate the mean-square value and store in y

  // Display output
  // Should match: 
  // 0.333333 1.666667 4.666667 9.666667 16.666666 25.666666 36.666668
  int i;
  for (i = 0; i < 7; i++)  {
      printf("%f ", y1[i]);
  }
  printf("\n\n");

  /********************************************************************/
  // Test when M = blocksize
  // Should match:
  // 0.333333 1.666667 4.666667 
  // 9.666667 16.666666 25.666666 
  // 36.666668 49.666668 64.666664
  /********************************************************************/

  float x2[3] = {1,2,3};
  float x3[3] = {4,5,6};
  float x4[3] = {7,8,9};
  MSV_STRUCT_T *s2 = init_msv(3,3);
  float *y2 = calc_msv(x2,s2);
  float *y3 = calc_msv(x3,s2);
  float *y4 = calc_msv(x4,s2);
  for(i=0; i<3; i++) {
    printf("%f ", y2[i]);
  }
  printf("\n");
  for(i=0; i<3; i++) {
    printf("%f ", y3[i]);
  }
  printf("\n");
  for(i=0; i<3; i++) {
    printf("%f ", y4[i]);
  }
  printf("\n");
  

  return 0;
}