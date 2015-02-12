#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	BIQUAD_T *f;

	float x[20] = {1, 0.5, -0.8, 0.2, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0,0, 0, 0, 0, 0};
	float y[20] = {0};

	float a[2][3] = {{1.0, -0.6088, 0.9702},
					 {1.0, -0.3500, -0.4250}};

	float b[2][3] = {{2.2044, 0.0, 2.2044},
					 {2.9658, -3.4865, 2.9658}};

	f = init_biquad(2,0.01718740,a,b,20);
	calc_biquad(f, x, y);

	int i;
	for (i = 0; i < 20; i++) {
		printf("y(%d) = %f\n", i, y[i]);
	}
	printf("\n");

	return 0;
}