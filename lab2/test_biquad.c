#include "ece486_biquad.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	BIQUAD_T *f;

	float x1[10] = {1, 0.5, -0.8, 0.2, 0, 0, 0, 0, 0, 0};
	float x2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	float y1[10] = {0};
	float y2[10] = {0};

	float a[2][3] = {{1.0, -0.6088, 0.9702},
					 {1.0, -0.3500, -0.4250}};

	float b[2][3] = {{2.2044, 0.0, 2.2044},
					 {2.9658, -3.4865, 2.9658}};

	int sections = 2;
	float g = 0.01718740;
	int blocksize = 10;
	f = init_biquad(sections,g,a,b,blocksize);
	calc_biquad(f, x1, y1);
	calc_biquad(f, x2, y2);

	int i;
	for (i = 0; i < 10; i++) {
		printf("y(%d) = %f\n", i, y1[i]);
	}
	printf("\n");
	for (i = 0; i < 10; i++) {
		printf("y(%d) = %f\n", i, y2[i]);
	}
	printf("\n");

	return 0;
}