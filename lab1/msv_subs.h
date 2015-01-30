/*!
 * @file
 * 
 * @brief ECE 486 Spring 2015 Lab 1 Subroutine Header file
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Jan 24, 2015
 * 
 * This file contains subroutine and data-type declarations necessary for 
 * running mean-square-value calculations.
 * 
 * The MSV_STRUCT_T data-type is a struct containing the necessary fields for
 * calculating the running mean-square-value.
 *
 * The init_msv() function initializes an MSV_STRUCT_T variable, allocating
 * memory for all fields. The function takes an integer 'M' indicating the
 * number of samples to average over and an integer 'blocksize' indicating the
 * number of samples in each block of data to be processed. A 'history' array 
 * of 'M-1' floating point numbers is allocated to store the last 'M-1' input 
 * samples. The index of 'history' is held by the variable 'histIndex', which is
 * initialized to 0. The variable 'prevSquared', which holds the running sum of 
 * squared inputs, is also initialized to 0.
 * The function returns a pointer to an MSV_STRUCT_T.
 *
 * The calc_msv() function performs the running mean-square-value calculation on
 * a block of samples. It takes a pointer to a float 'x' - the block of samples -
 * and a pointer to an MSV_STRUCT_T 's' - the struct containing the fields 
 * necessary for the calculations. The function returns a pointer to an array
 * of floats, which contains the output mean-square-values.
 * 
 * The destroy_msv() function de-allocates all memory allocated by init_msv()
 * and calc_msv(). 
 * 
 */

#ifndef ECE486_MSV_SUBS_H
#define ECE486_MSV_SUBS_H

#include <stdint.h>

 /*!
  * @brief Structure for mean squared value calculation
  */

typedef struct msv_struct {

	 	uint32_t M;          /*!< The number of samples to average over */
		uint32_t blocksize;  /*!< The number of samples per block */
		float prevSquared;   /*!< Running sum of squared values */
		float * history;     /*!< History of the last M samples */
		uint32_t histIndex;  /*!< Index of history[] */

} MSV_STRUCT_T;

/*!
 * @brief Initializes an MSV_STRUCT_T structure.
 *
 * @returns A pointer to a structure of type MSV_STRUCT_T is returned
 * 			    containing the fields necessary for a mean squared value
 * 			    calculation of @a blocksize samples, averaged over @a M
 *			    samples.
 */

MSV_STRUCT_T * init_msv(
	uint32_t M, 				   /*!< [in] The number of samples to average over */
	uint32_t blocksize		 /*!< [in] The number of samples in each block */
);

/*!
 * @brief Calculates the mean squared value of a block of samples.
 * 
 * @returns A pointer to an array of running mean-square-values calculated from
 *          the block of samples @a x.
 */

float * calc_msv(
	float * x,         /*!< [in] Pointer to an array of floats to be operated upon */
	MSV_STRUCT_T * s   /*!< [in,out] Pointer to an MSV_STRUCT_T containing additional
                          fields necessary for the calculation */
);

/*!
 * @brief De-allocates the memory required for a mean squared value calculation
 *
 */

void destroy_msv(
	MSV_STRUCT_T * s,		/*!< [in] Pointer to an MSV_STRUCT_T containing required
								fields for a mean squared value calculation. */
	float * y				/*!< [in] Pointer to an array of floating point output samples */
);

#endif


