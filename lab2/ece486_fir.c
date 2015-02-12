/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 2 FIR Subroutine Definitions
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Feb 9, 2015
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

#include "ece486_fir.h"
#include <stdio.h>
#include <stdlib.h>

FIR_T * init_fir(float *fir_coefs, int n_coefs, int blocksize){
  // Allocate memory for an FIR_T structure
  FIR_T * s;
  s = (FIR_T *) malloc(sizeof(FIR_T));

  // Initialize variables
  s->fir_coefs = fir_coefs;
  s->n_coefs = n_coefs;
  s->blocksize = blocksize;
  s->history = (float *) malloc((n_coefs)*sizeof(float));

  int i;
  for (i = 0; i < n_coefs; i++) {
  	s->history[i] = 0.0;
  }

  s->histInd = 0;
  s->f_calls = 0;

  return s;
}

// In order to have this work when x and y point to the same location, the 
// FIR_T struct will have to store the past 'n_coefs' values of x(n), in a
// circular buffer. This is really only more efficient when n_coefs < blocksize
void calc_fir(FIR_T *s, float *x, float *y){
  int k,n,i;

  int start_ind = (s->f_calls*s->blocksize);
  int end_ind = start_ind + s->blocksize;
  for (n = start_ind, i=0; n < end_ind; n++, i++) {
  	s->history[s->histInd] = x[i];
    // s->histInd++;
  	y[i] = 0.0;
    // printf("n = %d, i = %d, histInd = %d\n",n,i,s->histInd);
  	for (k = 0; k < s->n_coefs; k++) {
  		// if ((0 <= n-k) && (n-k <= s->n_coefs)) {
  			int index = s->histInd - k;
        index = index>=0 ? index : index+s->n_coefs;
        y[i] += (s->fir_coefs[k] * s->history[index]);
        // printf("index = %d, history[index] = %f, k = %d, fir_coefs[k] = %f\n", index,s->history[index],k,s->fir_coefs[k]);
  		// }
  	}
    s->histInd++;
    if(s->histInd == (s->n_coefs)) {
      s->histInd = 0;
    }
    // printf("histInd = %d\n",s->histInd);
  }
  s->f_calls++;
}

void destroy_fir(FIR_T *s){
  // free any dynamically allocated elements of s first
	free(s->history);
  // Free structure
  free(s);
}