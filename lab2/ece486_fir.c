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
  FIR_T * filt;

  filt = (FIR_T *) malloc(sizeof(FIR_T));

  // Variable initializations ...

  return filt;
}

void calc_fir(FIR_T *s, float *x, float *y){
  
}

void destroy_fir(FIR_T *s){
  // free any dynamically allocated elements of s first
  free(s->fir_coefs);

  // Free structure
  free(s);
}