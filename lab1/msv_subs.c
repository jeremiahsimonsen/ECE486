#include "msv_subs.h"
#include <stdio.h>

MSV_STRUCT_T * init_msv(uint M, uint blocksize){

	uint i;
	MSV_STRUCT_T s;

	s.M = M;
	s.blocksize = blocksize;
	s.history = malloc(sizeof(float)*(M-1));
	
	for(i=0;i<M;i++){
		s.history[i] = 0;
	}

}