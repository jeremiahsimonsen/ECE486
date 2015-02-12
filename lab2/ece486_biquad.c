/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 2 IIR header file
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Feb 10, 2015
 * 
 * Now describe the details of what's going on with the functions in this
 * file.  This can be several paragraphs long and gives the general
 * overview of what's going on.  Virtually every file (.h or .c or 
 * other source code) should include a comment block to describe 
 * its purpose or give an overview of the use of the code. 
 * 
 * Paragraphs are indicated by a blank line in the comment block
 * (except for the optional "*" character at the start of the line).
 * 
 * @verbatim
 *   If doxygen formatting starts to drive you nuts, you can use the 
 *   verbatim construct to avoid all the re-structuring.
 *       1) Who knows if it will help?
 *       2) Note that here are better ways to format lists within doxygen
 * @endverbatim
 * 
 */

#ifndef ECE486_FIR_H
#define ECE486_FIR_H

#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>



/*!
 * @brief Initializes a BIQUAD_T structure.
 *
 * @returns A pointer to a structure of type BIQUAD_T is returned containing the
 *          fields necessary for FIR filter implementation
 */

BIQUAD_T * init_biquad(int sections, float g, float **a, float **b, int blocksize) {
  // Allocate memory for BIQUAD_T structure
  BIQUAD_T *s;
  s = (BIQUAD_T *) malloc(sizeof(BIQUAD_T));

  // intialize variables
  s->sections = sections;
  s->g = g;
  s->a = a[sections][3];
  s->b = b[sections][3];
  s->blocksize = blocksize;

  s->in_buff = (float *) malloc(blocksize*sizeof(float));
  int i;
  for (i=0; i<blocksize; i++) {
    in_buff[i] = 0.0;
  }

  s->v_ind = 0;

  return s;
}

/*!
 * @brief Performs IIR filter calculation on block of samples
 *
 * @returns From input sample array @x, the output array @y is calculated using
 *          the IIR filter information in @s.
 */

void calc_biquad(BIQUAD_T *s, float *x, float *y) {
  // figure out n
  int bq,n;
  float * stage = (float *) malloc((s->sections) * sizeof(float))
  for(bq = 0; bq < s->sections; bq++) {
    for(n = 0; n < s->blocksize; n++) {
      int z1 = s->v_ind - 1;
      z1 = (z1 >= 0) ? z1 : z1 + 3;
      int z2 = s->v_ind - 2;
      z2 = (z2 >= 0) ? z2 : z2 + 3;

      if (bq == 0) {
        v[s->v_ind] = s->a[bq][0]*x[n] - s->a[bq][1]*s->v_buff[z1] - a[bq][2]*s->v_buff[z2];
      } else {
        v[s->v_ind] = s->a[bq][0]*in_buff[n] - s->a[bq][1]*s->v_buff[z1] - a[bq][2]*s->v_buff[z2];
      }

      if (bq == sections-1) {
        y[n] = s->g * (b[bq][0]*v_buff[s->v_ind] + b[bq][1]*v_buff[z1] + b[bq][2]*v_buff[z2]);
      } else {
        in_buff[n] = b[bq][0]*v_buff[s->v_ind] + b[bq][1]*v_buff[z1] + b[bq][2]*v_buff[z2];
      }

      s->v_ind++;
      if (s->v_ind == 3) {
        s->v_ind = 0;
      }
    }


  }
  // g times all stage elements will be H(z)
}

/*!
 * @brief De-allocates the memory required for an BIQUAD_T struct.
 *
 */

void destroy_biquad(
  BIQUAD_T *s            /*!< Pointer to a BIQUAD_T struct to be destroyed */
);

#endif