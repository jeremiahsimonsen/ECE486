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

  for(bq = 0; bq < s->sections; bq++) {

    for(n = 0; n < s->bSize; n++) {

      v_tmp = s->a[bq][0]*x[n] - s->a[bq][1]*s->v_buff[bq][1] - s->a[bq][2]*s->v_buff[bq][0];
      x[n] = s->b[bq][0]*v_tmp + s->b[bq][1]*s->v_buff[bq][1] + s->b[bq][2]*s->v_buff[bq][0];
      s->v_buff[bq][0] = s->v_buff[bq][1];
      s->v_buff[bq][1] = v_tmp;

    }

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
  for (i = 0; i < s->bSize; i++) {
    free(s->a[i]);
    free(s->b[i]);
  }
  free(s->a);
  free(s->b);
  free(s);
}
