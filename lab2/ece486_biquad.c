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
  s->a = a;
  s->b = b;
  s->bSize = blocksize;

  ///////////////////////
  // Memory allocation //
  ///////////////////////
  int i;

  // changed by TR 3-11-15
  s->v_buff = (float (*)[2]) malloc((s->sections) * sizeof(float));
  if (s->v_buff == NULL) return NULL;
  
  for(i=0; i<sections; i++){
	s->v_buff[i][0] = 0.0;
	s->v_buff[i][1] = 0.0;
  }

  // Allow for a0 != 1
  for (i=0; i < sections; i++) {
    if (s->a[i][0] != 1.0) {
      s->a[i][1] /= s->a[i][0];
      s->a[i][2] /= s->a[i][0];
      s->b[i][0] /= s->a[i][0];
      s->b[i][1] /= s->a[i][0];
      s->b[i][2] /= s->a[i][0];
    }
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

  int bq, n;
  float v_tmp;

  // Run samples through each biquad filter section
  for(bq = 0; bq < s->sections; bq++) {
    // find corresponding output for each input n for blocksize samples
    for(n = 0; n < s->bSize; n++) {
      // v[n] = x[n] - a1*v[n - 1] - a2*v[n - 2]
      v_tmp = x[n] - s->a[bq][1]*s->v_buff[bq][1] - s->a[bq][2]*s->v_buff[bq][0]; // find intermediate value for calculating x[n]
      // y[n] = b0*v[n] + b1*v[n - 1] + b2*v[n - 2]
      x[n] = s->b[bq][0]*v_tmp + s->b[bq][1]*s->v_buff[bq][1] + s->b[bq][2]*s->v_buff[bq][0]; // calculate x[n]
      // update previous v samples array
      s->v_buff[bq][0] = s->v_buff[bq][1];
      s->v_buff[bq][1] = v_tmp;

    }
  }
  // moved by JS 3-3-15
  // multiply by gain to achieve output 
  for(n = 0; n < s->bSize; n++){
    y[n] = (s->g)*x[n];
  }
}

/*!
 * @brief De-allocates the memory required for an BIQUAD_T struct.
 *
 */

void destroy_biquad(BIQUAD_T *s) {
  // changed by TR 3-11-15
  // free pointers to coefficient arrays
  free(s->a);
  s->a = NULL;
  free(s->b);
  s->b = NULL;
  // free memory allocated for BIQUAD_T struct 's'
  free(s);
  // NULL pointer for safety
  s = NULL;
}
