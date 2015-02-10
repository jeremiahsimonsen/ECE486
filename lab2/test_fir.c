#include "ece486_fir.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	FIR_T *filt;
	float h[4] = {1,2,3,4};
	float x1[2] = {1,2};
	float x2[2] = {3,4};
	float x3[2] = {5,6};

	float y1[2] = {0,0};
	float y2[2] = {0,0};
	float y3[2] = {0,0};
	// float *y = x;

	filt = init_fir(h,4,2);
	calc_fir(filt,x1,y1);
	calc_fir(filt,x2,y2);
	calc_fir(filt,x3,y3);


	int i;
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i,y1[i]);
	}
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i+2,y2[i]);
	}
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i+4,y3[i]);
	}
	printf("\n");

	for (i = 0; i < filt->n_coefs; i++) {
		printf("%f ", filt->history[i]);
	}
	printf("\n");

  return 0;
}