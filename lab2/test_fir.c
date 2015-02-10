#include "ece486_fir.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	FIR_T *filt;
	float h[4] = {1,2,3,4};
	float x[2] = {1,2};
	// float y[2] = {0,0};
	float *y = x;

	filt = init_fir(h,4,2);
	calc_fir(filt,x,y);

	int i;
	for (i = 0; i < 2; i++) {
		printf("y(%d) = %f\n",i,y[i]);
	}
	printf("\n");

  return 0;
}