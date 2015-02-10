/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 2 FIR header file
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

#ifndef ECE486_FIR_H
#define ECE486_FIR_H

#include <stdint.h>

 /*!
  * @brief Structure for FIR implementation containing coefficients and filter 
  * state
  */

typedef struct your_fir_struct {

  float *fir_coefs;   /*!< Array of filter coefficients */
  int n_coefs;        /*!< Number of filter coefficients */
  int blocksize;      /*!< Number of samples */
  float *history;     /*!< Buffer to store last n_coefs-1 samples */

} FIR_T;

/*!
 * @brief Initializes a FIR_T structure.
 *
 * @returns A pointer to a structure of type FIR_T is returned containing the
 *          fields necessary for FIR filter implementation
 */

FIR_T * init_fir(
  float *fir_coefs,   /*!< Array of filter coefficients */
  int n_coefs,         /*!< Number of filter coefficients */
  int blocksize      /*!< Number of samples */
);

/*!
 * @brief Performs FIR filter calculation on block of samples
 *
 * @returns From input sample array @x, the output array @y is calculated using
 *          the FIR filter information in @s.
 */

void calc_fir(
  FIR_T *s,           /*!< Structure containing necessary FIR filter info */
  float *x,           /*!< Array of input samples */
  float *y            /*!< Array of outputs */
);

/*!
 * @brief De-allocates the memory required for an FIR_T struct.
 *
 */

void destroy_fir(
  FIR_T *s            /*!< Pointer to a FIR_T struct to be destroyed */
);

#endif