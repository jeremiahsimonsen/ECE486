#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>


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







MSV_STRUCT_T * init_msv(uint32_t M, uint32_t blocksize){

	MSV_STRUCT_T * s;

	s = (MSV_STRUCT_T *) malloc(sizeof(MSV_STRUCT_T));

	s->M = M;
	s->blocksize = blocksize;
	s->prevSquared = 0;
	s->histIndex = 0;
	s->history = calloc(M-1,sizeof(float));

  return s;

}

float * calc_msv(float * x, MSV_STRUCT_T * s){

	//Allocate the output array
	float * y;
	y = (float *) calloc(s->blocksize,sizeof(float));

	uint32_t i;

	for(i=0;i<s->blocksize;i++){

			y[i] = (s->prevSquared + x[i]*x[i]) / s->M;

			s->prevSquared -= (s->history[s->histIndex])*(s->history[s->histIndex]);
			s->prevSquared += x[i]*x[i];
			s->history[s->histIndex] = x[i];
			s->histIndex += 1;

			if(s->histIndex == (s->M-1)){
				s->histIndex -= (s->M-1);
			}

	}

	return y;

}

void destroy_msv(MSV_STRUCT_T * s, float * y){

	uint32_t i;

	//Free output array memory
	free(y);

	//Free circular buffer
	free(s->history);

	//Free the rest of the structure
	free(s);

}






int main(int argc, char const *argv[])	{

	uint32_t M = atoi(argv[1]);
	float x[5] = {1,2,3,4,5};
	
	// test when M < blocksize (blocksize is 5)
	if(M < 5){	
		MSV_STRUCT_T * s = init_msv(M,5);
		float * y = calc_msv(x,s);

		for (int i = 0; i < 5; i++)	{
			printf("%f ", y[i]);
		}
	}



	float x1[5] = {1,2,3,4,5};
	float x2[5] = {4,5,6,7,8};
	float x3[5] = {7,8,9,10,11};
	// test when M > blocksize
	if (M > 5) {
		MSV_STRUCT_T * s = init_msv(M,5);

		float * y = calc_msv(x1,s);
		float * y2 = calc_msv(x2,s);
		float * y3 = calc_msv(x3,s);

		for (int j = 0; j < M; j++)	{
			printf("%f ", y[j]);
		}
		for (int k = 0; k < M; k++)	{
			printf("%f ", y2[k]);
		}
		for (int l = 0; l < M; l++)	{
			printf("%f ", y3[l]);
		}
	}


}









