#include <string.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mm.h"

float vecadd(float a, float b, int j)
{
    float constJ=6857684243440066058811403635611664384.000000f;
#pragma HLS INLINE off
    if(j==0) 
        return a+constJ;
    else
        return a+b;
}

#if KK == 0
#define kernel kernel0
#endif
void kernel0( float a[M], float b[M], float local_c[M] )
{

l_loop:		   for(int l = 0; l < 25*myFactor; l++){

#pragma HLS INLINE off
p_loop:            for(	int p = 0; p < M; p += myFactor) {
#pragma HLS PIPELINE II = 1
j_loop:            for(int j = 0; j < myFactor; j++) {
#pragma HLS DEPENDENCE variable="local_c" inter false
			   //local_c[p+j] = a[p] + b[p];
                local_c[p+j]=vecadd(a[p],b[p],j);
		   }
		   }
}
}

 

void mm( float a[M], float b[M], float c[M] )
{
	float local_a[M];
	float local_b[M];
	float local_c[M];
#pragma HLS ARRAY_PARTITION variable="local_a" cyclic factor=myFactor dim="1" partition
#pragma HLS ARRAY_PARTITION variable="local_b" cyclic factor=myFactor dim="1" partition
#pragma HLS ARRAY_PARTITION variable="local_c" cyclic factor=myFactor dim="1" partition
	int i,j;
	for(i = 0; i < M; i++)
	{
#pragma HLS PIPELINE
		local_b[i] = b[i];
		local_a[i] = a[i];
		local_c[i] = 0.0; 
	}

//for(i = 0; i < 10; i++)
{
	kernel(local_a,local_b,local_c);
}
for(i=0; i < M; i++)
{
	//   for(j=0; j < M; j++)
	//  {
#pragma HLS PIPELINE
	c[i]=local_c[i];
	// }
}
}

