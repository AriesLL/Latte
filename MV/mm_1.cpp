/*******************************************************************************
Vendor: Xilinx 
Associated Filename: mmult1.c
Purpose: HLS C matrix multiply kernel example
Revision History: July 1, 2013 - initial release
                                                
*******************************************************************************
Copyright (C) 2013 XILINX, Inc.

This file contains confidential and proprietary information of Xilinx, Inc. and 
is protected under U.S. and international copyright and other intellectual 
property laws.

DISCLAIMER
This disclaimer is not a license and does not grant any rights to the materials 
distributed herewith. Except as otherwise provided in a valid license issued to 
you by Xilinx, and to the maximum extent permitted by applicable law: 
(1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX 
HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, 
INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR 
FITNESS FOR ANY PARTICULAR PURPOSE; and (2) Xilinx shall not be liable (whether 
in contract or tort, including negligence, or under any other theory of 
liability) for any loss or damage of any kind or nature related to, arising under 
or in connection with these materials, including for any direct, or any indirect, 
special, incidental, or consequential loss or damage (including loss of data, 
profits, goodwill, or any type of loss or damage suffered as a result of any 
action brought by a third party) even if such damage or loss was reasonably 
foreseeable or Xilinx had been advised of the possibility of the same.

CRITICAL APPLICATIONS
Xilinx products are not designed or intended to be fail-safe, or for use in any 
application requiring fail-safe performance, such as life-support or safety 
devices or systems, Class III medical devices, nuclear facilities, applications 
related to the deployment of airbags, or any other applications that could lead 
to death, personal injury, or severe property or environmental damage 
(individually and collectively, "Critical Applications"). Customer assumes the 
sole risk and liability of any use of Xilinx products in Critical Applications, 
subject only to applicable laws and regulations governing limitations on product 
liability. 

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT 
ALL TIMES.

*******************************************************************************/
#include <string.h>

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "mm.h"

//#define bool int
//#define false 0
//#define true 1
//#include "ap_int.h"

//extern "C" {

#if KK == 0
    #define kernel kernel0
#endif
void kernel0( unsigned int a[M][M], unsigned int b[M], unsigned int local_c[M] )
{
#pragma HLS INLINE off
//k_loop:    for(int k = 0; k < M; k++) {
i_loop:        for(int i = 0; i < M; i++) {
                   //#pragma HLS PIPELINE II = 2
p_loop:            for(int p = 0; p < M; p += myFactor) {
#pragma HLS PIPELINE II = 1
j_loop:            for(int j = 0; j < myFactor; j++) {
#pragma HLS DEPENDENCE variable="local_c" inter false
                       local_c[j+p] += a[j+p][i] * b[i];
                   }
                   }
               }
//           }
    
} 

void mm( unsigned int a[M][M], unsigned int b[M], unsigned int c[M] )
{
    unsigned int local_a[M][M];
    unsigned int local_b[M];
//#pragma HLS ARRAY_PARTITION variable="local_b" cyclic factor=myFactor dim="1" partition
    unsigned int local_c[M];
#pragma HLS ARRAY_PARTITION variable="local_c" cyclic factor=myFactor dim="1" partition
#pragma HLS ARRAY_PARTITION variable="local_a" cyclic factor=myFactor dim="1" partition
    int i,j;
    for(i = 0; i < M; i++)
    {
        for(j = 0; j < M; j++)
        {
#pragma HLS PIPELINE
            local_b[i] = b[i];
            local_a[i][j] = a[i][j];
            local_c[i] = 0.0; 
        }
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

