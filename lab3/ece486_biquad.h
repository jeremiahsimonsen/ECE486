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
 * This file contains subroutine and data-type declarations necessary for 
 * running the biquad filter transfer function calculations, which filters 
 * the input data in Direct Form II.
 * 
 * The BIQUAD_T data-type is a struct containing the necessary fields for
 * calculating the transfer function.
 *
 * The init_biquad() function initializes an BIQUAD_T variable, allocating
 * memory for all fields. The function takes an integer 'sections' which is 
 * the number of biquad filter sections to be used for calculating H(z).
 * The float 'g' is passed as the gain to be multiplied by every biquad section.
 * Two 2-dimensional arrays are passed as the 'a' coefficients and 'b' coefficients.
 * An integer 'blocksize' indicates the number of samples in each block of 
 * data to be processed.
 * The function returns a pointer to an BIQUAD_T.
 *
 * The calc_biquad() function performs the transfer function calculation on
 * a block of samples. It takes a pointer to a float 'x' - the block of samples -
 * and a pointer to an BIQUAD_T 's' - the struct containing the fields 
 * necessary for the calculations. The function returns a pointer to an array
 * of floats, which contains the output transfer function calculations.
 * 
 * The destroy_biquad() function de-allocates all memory allocated by init_biquad()
 * and calc_biquad(). 
 * 
 */

#ifndef ECE486_BIQUAD_H
#define ECE486_BIQUAD_H

#include <stdint.h>

 /*!
  * @brief Structure for IIR implementation containing coefficients and filter 
  * state
  */

typedef struct biquad_struct {

  int sections;         /*!< Number of biquad filter sections to be implemented */
  float g;              /*!< scale factor */
  int bSize;            /*!< Number of samples */
  float *v_buff[2];     /*!< Buffer to store last n_coefs samples */
  float (*a)[3];          /*!< array of 'a' coefficients arrays */
  float (*b)[3];          /*!< array of 'b' coefficients arrays */

} BIQUAD_T;

/*!
 * @brief Initializes a BIQUAD_T structure.
 *
 * @returns A pointer to a structure of type BIQUAD_T is returned containing the
 *          fields necessary for IIR filter implementation
 */

BIQUAD_T *init_biquad(
  int sections,      /*!< Number of biquad filter sections to be implemented */
  float g,           /*!< Gain to be mulitplied by each filter section */
  float a[][3],      /*!< Array of 'a' coefficients */
  float b[][3],      /*!< Array of 'b' coefficients */
  int blocksize      /*!< Number of samples */
);

/*!
 * @brief Performs IIR filter calculation on block of samples
 *
 * @returns From input sample array @x, the output array @y is calculated using
 *          the IIR filter information in @s.
 */

void calc_biquad(
  BIQUAD_T *s,        /*!< Structure containing necessary IIR filter info */
  float *x,           /*!< Array of input samples */
  float *y            /*!< Array of outputs */
);

/*!
 * @brief De-allocates the memory required for an BIQUAD_T struct.
 *
 */

void destroy_biquad(
  BIQUAD_T *s            /*!< Pointer to a BIQUAD_T struct to be destroyed */
);

#endif