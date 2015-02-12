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

#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>


#define DEBUG_INIT 0
#define DEBUG_CALC 1

/*!
 * @brief Initializes a BIQUAD_T structure.
 *
 * @returns A pointer to a structure of type BIQUAD_T is returned containing the
 *          fields necessary for FIR filter implementation
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
  s->v_ind = 0;

  // s->a = (float *[3]) malloc(sizeof(float) * 3);
  // s->b = malloc(sizeof(float) * 3);

  ///////////////////////
  // Memory allocation //
  ///////////////////////
  int i;
  for (i = 0; i < blocksize; i++) {
    s->a[i] = (float *) malloc(sizeof(float) * 3);
    if (s->a[i] == NULL) return NULL;
    s->a[i] = a[i];
  }
  for (i = 0; i < blocksize; i++) {
    s->b[i] = (float *) malloc(sizeof(float) * 3);
    if (s->b[i] == NULL) return NULL;
    s->b[i] = b[i];
  }

  s->in_buff = (float *) malloc(blocksize*sizeof(float));
  if (s->in_buff == NULL) return NULL;

  ///////////////////////////////////////////////////////
  // Initialization of dynamically allocated variables //
  ///////////////////////////////////////////////////////
  // s->a = a;
  // s->b = b;
  for (i=0; i<blocksize; i++) {
    s->in_buff[i] = 0.0;
  }


  if (DEBUG_INIT) {
    printf("Biquad initialized\n");
    printf("\tsections = %d,\tg = %f,\tbSize = %d,\tv_ind = %d\n",s->sections,
            s->g, s->bSize, s->v_ind);
    for (i = 0; i < sections; i++) {
      printf("Filter %d coefficients:\n", i);
      int j;
      for (j = 0; j < 3; j++) {
        printf("b[%d][%d] = %f\t", i,j,b[i][j]);
      }
      printf("\n");
      for (j = 0; j < 3; j++) {
        printf("a[%d][%d] = %f\t", i,j,a[i][j]);
      }
      printf("\n");
    }
    for (i = 0; i < 3; i++) {
      printf("v_buff[%d] = %f\t", i,s->v_buff[i]);
    }
    printf("\n");
    for (i = 0; i < s->bSize; i++) {
      printf("in_buff[%d] = %f\n", i,s->in_buff[i]);
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
  DEBUG_CALC && printf("Entering calc_biquad()\n");
  int bq,n;
  float * stage = (float *) malloc((s->sections) * sizeof(float));
  for(bq = 0; bq < s->sections; bq++) {
    DEBUG_CALC && printf("bq = %d\n", bq);
    for(n = 0; n < s->bSize; n++) {
      DEBUG_CALC && printf("n = %d\n",n);
      int z1 = s->v_ind - 1;
      z1 = (z1 >= 0) ? z1 : z1 + 3;
      int z2 = s->v_ind - 2;
      z2 = (z2 >= 0) ? z2 : z2 + 3;


      if (bq == 0) {
        DEBUG_CALC && printf("x[n] = %f\n", x[n]);
        s->v_buff[s->v_ind] = s->a[bq][0]*x[n] - s->a[bq][1]*s->v_buff[z1] - s->a[bq][2]*s->v_buff[z2];
      } else {
        s->v_buff[s->v_ind] = s->a[bq][0]*s->in_buff[n] - s->a[bq][1]*s->v_buff[z1] - s->a[bq][2]*s->v_buff[z2];
      }


      if (bq == s->sections-1) {
        y[n] = s->g * (s->b[bq][0]*s->v_buff[s->v_ind] + s->b[bq][1]*s->v_buff[z1] + s->b[bq][2]*s->v_buff[z2]);
      } else {
        s->in_buff[n] = s->b[bq][0]*s->v_buff[s->v_ind] + s->b[bq][1]*s->v_buff[z1] + s->b[bq][2]*s->v_buff[z2];
      }

      s->v_ind++;
      if (s->v_ind == 3) {
        s->v_ind = 0;
      }
    }


  }
  DEBUG_CALC && printf("Exiting calc_biquad()\n");
}

/*!
 * @brief De-allocates the memory required for an BIQUAD_T struct.
 *
 */

void destroy_biquad(BIQUAD_T *s) {
  int i;
  for (i = 0; i < s->bSize; i++) {
    free(s->a[i]);
    free(s->b[i]);
  }
  free(s->a);
  free(s->b);
  free(s);
}
