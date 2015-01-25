/*!
 * @file
 * 
 * @brief ECE 486 Spring 2014 Lab 1 Subroutines
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Jan 24, 2015
 * 
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
 * 			containing the fields necessary for a mean squared value
 * 			calculation of 'blocksize' samples, averaged over 'M'
 *			samples.
 */

MSV_STRUCT_T * init_msv(
	uint M, 				/*!< [in] The number of samples to average over */
	uint blocksize			/*!< [in] The number of samples in each block */
);

/*!
 * @brief Calculates the mean squared value of a block of samples.
 * 
 * @returns On return, an array of output samples 
 */

float * calc_msv(
	float * x, 
	MSV_STRUCT_T * s
);

#endif


