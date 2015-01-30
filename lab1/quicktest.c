#include <stdio.h>
#include <stdlib.h>
#include "msv_subs.c"

int main(){

	float x1[6] = {1,2,3,4,5,6};
	float x2[6] = {7,8,9,10,11,12};
	float * y;
	uint32_t i;

	MSV_STRUCT_T * s;
	s = init_msv(3,6);
	y = calc_msv(x1,s);

	for(i=0;i<s->blocksize;i++){
		printf("%f ", y[i]);
	}

	y = calc_msv(x2,s);

	for(i=0;i<s->blocksize;i++){
		printf("%f ", y[i]);
	}

	//De-allocate memory
	destroy_msv(s, y);
	
	return 0;

}