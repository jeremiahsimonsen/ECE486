#include <stdio.h>
#include <stdlib.h>
#include "msv_subs.c"

int main(){

	float x1[6] = {1,2,3,4,5,6};
	float x2[6] = {7,8,9,10,11,12};
	float * y1;
	float * y2;
	uint32_t i;

	MSV_STRUCT_T * s;
	s = init_msv(3,6);
	y1 = calc_msv(x1,s);
	y2 = calc_msv(x2,s);


	for(i=0;i<s->blocksize;i++){
		printf("%f ", y1[i]);
	}
	printf("\n");
	for(i=0;i<s->blocksize;i++){
		printf("%f ", y2[i])
	}

	return 0;

}