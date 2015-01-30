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









