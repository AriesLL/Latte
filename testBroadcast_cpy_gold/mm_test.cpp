#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"

extern void mm( float a[M], float b[M], float c[M] );
int main( )
{
    float a[M];
    float b[M];
    float c[M];
    int i,j,k;
    float LO = -3.14e37;
    float HI = 3.14e37;
    for( i = 0; i < M; i++ )
    {
            //a[i][j] = (float)( i * M + j );
            //b[i][j] = (float)( i * M + j );
            a[i] = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
            b[i] = LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
        }
    
    mm( a, b, c );
    float c_test[M]={0};
    for( i = 0; i < M; i++ )
    {
          //  for( k = 0; k < M; k++ )
          //  {
                c_test[i] = a[i] + b[i];
          //  }
        
    }
    for( i = 0; i < M; i++ )
    {
     //   for( j = 0; j < M; j++ )
     //   {
            //if( c[i] == c_test[i] )
        //    {
                printf("error @(%d): %f --> %f\n", i, c_test[i], c[i]);
         //   }
        }
   // }
    return 0;
}

