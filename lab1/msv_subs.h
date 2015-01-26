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
 * number of samples in each block of data to be processed. The function returns
 * a pointer to an MSV_STRUCT_T.
 *
 * The calc_msv() function performs the running mean-square-value calculation on
 * a block of samples. It takes a pointer to a float 'x' - the block of samples-
 * and a pointer to an MSV_STRUCT_T 's' - the struct containing the fields 
 * necessary for the calculations. The function returns a pointer to an float,
 * the array of running mean-square-values.
 * 
 */

#ifndef ECE486_MSV_SUBS_H
#define ECE486_MSV_SUBS_H

 /*!
  * @brief Structure for mean squared value calculation
  */
typedef struct msv_struct {

	 	uint M; /*!< The number of samples to average over */
		uint blocksize; /*!< The number of samples per block */
		float prevSquared = 0; /*!< Running sum of squared values */
		float * history; /*!< History of the last M samples */
		uint samples = 0; /*!< Tracks how many samples are in the history */
		uint histIndex = 1; /*!< Index of history[] */

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
	uint M, 				   /*!< [in] The number of samples to average over */
	uint blocksize		 /*!< [in] The number of samples in each block */
);

/*!
 * @brief Calculates the mean squared value of a block of samples.
 * 
 * @returns A pointer to an array of running mean-square-values calculated from
 *          the block of samples @a x.
 */

float * calc_msv(
	float * x,         //!< [in] Pointer to an array of floats to be operated upon
	MSV_STRUCT_T * s   /*!< [in] Pointer to an MSV_STRUCT_T containing additional
                          fields necessary for the calculation */
);

#endif


