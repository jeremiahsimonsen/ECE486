#include "ece486_biquad.h"
#include "filter1_coef.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	BIQUAD_T *f1;//, *f2;
	f1 = init_biquad(filter1_num_stages, filter1_g, filter1_a_coef, filter1_b_coef, 10);

	float x[10] =  {1,2,3,4,5,6,7,8,9,10};
	float y[10] = {0};

	calc_biquad(f1,x,y);
	int i;
	for (i = 0; i < 10; i++) {
		printf("y(%d) = %f\n", i, y[i]);
	}
}