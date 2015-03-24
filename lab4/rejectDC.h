/**
 * @file
 * 
 * @brief ECE 486 Spring 2015 DC rejection routine header file
 * 
 * @author ECE486 Lab Group 2
 * @author Jacob Allenwood, Travis Russell, Jeremiah Simonsen
 * 
 * @date Mar 23, 2015
 * 
 * This file contains subroutine and data-type declarations necessary for
 * running the DC rejection filter routine.
 */

#ifndef REJECTDC_H
#define REJECTDC_H

/**
 * @brief Structure for FIR DC rejection filter implementation
 * @details This structure contains the number of samples per block,
 * 'blockSize', and the state information - previous values of x(n) and y(n)
 * 
 */
typedef struct dc_block_struct {

	int blockSize;			/*!< Number of samples to process */
	float x_prev[2];			/*!< Previous value of input; x(n-1) */
	float y_prev;			/*!< Previous value of output; y(n-1) */

} DCBLOCK_T;

/**
 * @brief Initializes a DCBLOCK_T structure
 * @details Initializes variables required for DC blocking filter
 * @returns A pointer to a structure of type DCBLOCK_T containing the fields
 * necessary for DC blocking filter implementation
 */
DCBLOCK_T *init_dcblock(	
	int bSize				/*!< Number of samples to process */
);

/**
 * @brief Performs DC blocking filter calculation on a block of samples
 * @details An input sample array, @x, is filtered to reject DC and the filter
 * output is sent to output sample array, @y.
 * @returns Filter output - input signal with DC rejected
 */
void calc_dcblock(
	DCBLOCK_T *s,			/*!< Structure containing necessary filter info */
	float *x,				/*!< Array of input samples */
	float *y 				/*!< Array of outputs */
);

/**
 * @brief De-allocates the memory required for a DCBLOCK_T
 */
void destroy_dcblock(
	DCBLOCK_T *s 			/*!< Pointer to a DCBLOCK_T to be destroyed */
);

#endif