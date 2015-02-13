/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 2 IIR source file
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Feb 10, 2015
 * 
 * This file contains the subroutine definitions necessary for performing the
 * IIR filter calculations on a block of samples.
 *
 * The init_biquad() function initializes an BIQUAD_T variable, allocating memory for
 * all fields. The function takes an integer 'sections' which is 
 * the number of biquad filter sections to be used for calculating H(z).
 * The float 'g' is passed as the gain to be multiplied by every biquad section.
 * Two 2-dimensional arrays are passed as the 'a' coefficients and 'b' coefficients.
 * An integer 'blocksize' indicates the number of samples in each block of 
 * data to be processed. 
 * A 2-dimensional array 'v_buff' that keeps the 2 previous v(n) samples is 
 * initialized to 0.
 * The function returns a pointer to a BIQUAD_T structure.
 *
 * The calc_biquad() function performs the IIR filtering on 'blocksize' samples of
 * input 'x', storing the corresponding outputs in 'y'. The information needed
 * to perform the calculation is passed to the function with the BIQUAD_T struct
 * 's'.
 *
 * The destroy_biquad() function de-allocates all memory pointed to by BIQUAD_T *'s' 
 * allocated by init_biquad().
 * 
 */

#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>

#define DEBUG_INIT 0
#define DEBUG_CALC 0

/*!
 * @brief Initializes a BIQUAD_T structure.
 *
 * @returns A pointer to a structure of type BIQUAD_T is returned containing the
 *          fields necessary for IIR filter implementation
 */

BIQUAD_T * init_biquad(int sections, float g, float a[][3], float b[][3], int blocksize) {
  
  // Allocate memory for BIQUAD_T structure
  BIQUAD_T *s;
  s = malloc(sizeof(BIQUAD_T));
  if (s == NULL) return NULL;

  // intialize variables
  s->sections = sections;
  s->g = g;
  s->bSize = blocksize;

  ///////////////////////
  // Memory allocation //
  ///////////////////////
  int i;

  for (i = 0; i < sections; i++) {
  	s->v_buff[i] = (float *) malloc(sizeof(float) * 2);
    if (s->v_buff[i] == NULL) return NULL;
    s->v_buff[i][0] = 0;
    s->v_buff[i][1] = 0;
  }

  for (i = 0; i < sections; i++) {
  	s->a[i] = (float *) malloc(sizeof(float) * 3);
    if (s->a[i] == NULL) return NULL;
    s->a[i] = a[i];
  }

  for (i = 0; i < sections; i++) {
    s->b[i] = (float *) malloc(sizeof(float) * 3);
    if (s->b[i] == NULL) return NULL;
    s->b[i] = b[i];
  }

  return s;
}

/*!
 * @brief Performs IIR filter calculation on block of samples
 *
 * @returns From input sample array @x, the output array @y is calculated using
 *          the IIR filter information in @s.
 */

void calc_biquad(BIQUAD_T *s, float *x, float *y) {

  DEBUG_CALC && printf("Entering calc_biquad()\n");

  int bq, n;
  float v_tmp;

  // Run samples through each biquad filter section
  for(bq = 0; bq < s->sections; bq++) {
    // find corresponding output for each input n for blocksize samples
    for(n = 0; n < s->bSize; n++) {
      // v[n] = a0*x[n] - a1*v[n - 1] - a2*v[n - 2]
      v_tmp = s->a[bq][0]*x[n] - s->a[bq][1]*s->v_buff[bq][1] - s->a[bq][2]*s->v_buff[bq][0]; // find intermediate value for calculating x[n]
      // y[n] = b0*v[n] + b1*v[n - 1] + b2*v[n - 2]
      x[n] = s->b[bq][0]*v_tmp + s->b[bq][1]*s->v_buff[bq][1] + s->b[bq][2]*s->v_buff[bq][0]; // calculate x[n]
      // update previous v samples array
      s->v_buff[bq][0] = s->v_buff[bq][1];
      s->v_buff[bq][1] = v_tmp;

    }
    // multiply by gain to achieve output 
    for(n = 0; n < s->bSize; n++){
    	y[n] = (s->g)*x[n];
  	}

  }

}

/*!
 * @brief De-allocates the memory required for an BIQUAD_T struct.
 *
 */

void destroy_biquad(BIQUAD_T *s) {
  int i;
  // free memory allocated for coefficient arrays
  for (i = 0; i < s->bSize; i++) {
    free(s->a[i]);
    free(s->b[i]);
  }
  free(s->a);
  free(s->b);
  // free memory allocated for BIQUAD_T struct 's'
  free(s);
}
