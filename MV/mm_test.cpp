#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mm.h"
/*
extern void mm( unsigned int a[M][M], unsigned int b[M][M], unsigned int c[M][M] );
int main( )
{
    unsigned int a[M][M];
    unsigned int b[M][M];
    unsigned int c[M][M];
    int i,j,k;
    for( i = 0; i < M; i++ )
    {
        for( j = 0; j < M; j++ )
        {
            a[i][j] = (unsigned int)( i * M + j );
            b[i][j] = (unsigned int)( i * M + j );
        }
    }
    mm( a, b, c );
    unsigned int c_test[M][M]={0};
    for( i = 0; i < M; i++ )
    {
        for( j = 0; j < M; j++ )
        {
            for( k = 0; k < M; k++ )
            {
                c_test[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    for( i = 0; i < M; i++ )
    {
        for( j = 0; j < M; j++ )
        {
            if( c[i][j] == c_test[i][j] )
            {
                printf("error @(%d,%d): %f --> %f\n", i, j, c_test[i][j], c[i][j]);
            }
        }
    }
    return 0;
}
*/
extern void mm( unsigned int a[M][M], unsigned int b[M], unsigned int c[M] );
int main( )
{
    unsigned int a[M][M];
    unsigned int b[M];
    unsigned int c[M];
    int i,j,k;
    //unsigned int LO = -3.14e17;
    //unsigned int HI = 3.14e17;
    unsigned int LO = 0;
    unsigned int HI = 4294967294;
    for( i = 0; i < M; i++ )
    {
        for( j = 0; j < M; j++ )
        {
            //a[i][j] = (unsigned int)( i * M + j );
            //b[i][j] = (unsigned int)( i * M + j );

            //a[i][j] = LO + (rand() % static_cast<unsigned int>(HI - LO + 1));
            //b[i] = LO + (rand() % static_cast<unsigned int>(HI - LO + 1));
            //
            a[i][j] = i*M + j+1;
            b[j] = j + 1;

            //a[i] = LO + static_cast <unsigned int> (rand()) /( static_cast <unsigned int> (RAND_MAX/(HI-LO)));
            //b[i] = LO + static_cast <unsigned int> (rand()) /( static_cast <unsigned int> (RAND_MAX/(HI-LO)));
        }
    }
    mm( a, b, c );
    unsigned int c_test[M]={0};
    for( i = 0; i < M; i++ )
    {
        for( j = 0; j < M; j++ )
        {
          //  for( k = 0; k < M; k++ )
          //  {
                c_test[i] += a[i][j] * b[j];
          //  }
        }
    }
    for( i = 0; i < M; i++ )
    {
     //   for( j = 0; j < M; j++ )
     //   {
            //if( c[i][j] == c_test[i][j] )
        //    {
                printf("error @(%d): %d --> %d\n", i, c_test[i], c[i]);
         //   }
        }
   // }
    return 0;
}

