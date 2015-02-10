#include "ece486_fir.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	FIR_T *filt;
	float h[3] = {1,2,3};
	float x[3] = {1,2,3};
	float y[3] = {0,0,0};

	filt = init_fir(h,3,3);
	calc_fir(filt,x,y);

	int i;
	for (i = 0; i < 3; i++) {
		printf("y(%d) = %f\n",i,y[i]);
	}
	printf("\n");

  return 0;
}