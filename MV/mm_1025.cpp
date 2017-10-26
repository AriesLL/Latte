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

#include <ap_int.h>
#include <hls_stream.h>

using namespace hls;

//#define bool int
//#define false 0
//#define true 1
//#include "ap_int.h"

//extern "C" {


void data_feeder(unsigned int a[M][M], unsigned int b[M]
  , stream<unsigned int> &fifo_a0
  , stream<unsigned int> &fifo_a1
  , stream<unsigned int> &fifo_b0
  )
{
  #pragma HLS INLINE off

  for(int counter = 0; counter < M; counter++)
  {
    #pragma HLS PIPELINE
    fifo_a0.write(a[0][counter]);
    fifo_a1.write(a[1][counter]);
    fifo_b0.write(b[counter]);

  }
}


//for(int counter = 0; counter < M; counter++)
//#pragma HLS DEPENDENCE variable="local_c_temp" inter false
    //if(counter==M){
      //break;
    //}
//fifo_cout.write(local_c_temp);

void PE0(stream<unsigned int> &fifo_ain, stream<unsigned int> &fifo_bin, stream<unsigned int> &fifo_bout, stream<unsigned int> &fifo_cout)
{
  #pragma HLS INLINE off
  unsigned int local_c_temp = 0;
  int counter = 0;
  while(1)
  
  {
    #pragma HLS PIPELINE II=1 
    

    if(!fifo_ain.empty() && !fifo_bin.empty()){
      unsigned int reg_a = fifo_ain.read();
      unsigned int reg_b = fifo_bin.read();
      fifo_bout.write(reg_b);
      unsigned int reg_mul = reg_a * reg_b;
      local_c_temp += reg_mul;
      
      counter++;
      if(counter==M){
        fifo_cout.write(local_c_temp);
        break;
      }

    }

    

  }
  

}

void PE1(stream<unsigned int> &fifo_ain, stream<unsigned int> &fifo_bin, stream<unsigned int> &fifo_cout)
{
  #pragma HLS INLINE off
  unsigned int local_c_temp = 0;
  int counter = 0;
  while(1)
  //for(int counter = 0; counter < M; counter++)
  {
    #pragma HLS PIPELINE II=1 
    //#pragma HLS DEPENDENCE variable="local_c_temp" inter false

    if(!fifo_ain.empty() && !fifo_bin.empty()){
      unsigned int reg_a = fifo_ain.read();
      unsigned int reg_b = fifo_bin.read();
      unsigned int reg_mul = reg_a * reg_b;

      local_c_temp += reg_mul;

      //fifo_bout.write(reg_b);
      counter++;
      if(counter == M){
        fifo_cout.write(local_c_temp);
        break;
      }
    }
    

  }
  

}

void data_collect(stream<unsigned int> &fifo_c0, stream<unsigned int> &fifo_c1, unsigned int local_c[M])
{
  //for(int counter = 0; counter < M; counter++)
  //{
    //#pragma HLS UNROLL
  local_c[0] = fifo_c0.read();
  local_c[1] = fifo_c1.read();

/*
  int counter = 0;
  while(1){
    if(!fifo_c0.empty()) {
      local_c[0] = fifo_c0.read();
    }
    if(!fifo_c1.empty()) local_c[1] = fifo_c1.read();
  }
*/
  //}
}

#if KK == 0
    #define kernel kernel0
#endif
void kernel0( unsigned int a[M][M], unsigned int b[M], unsigned int local_c[M] )
{
  #pragma HLS DATAFLOW
  

  hls::stream<unsigned int> fifo_a0;
#pragma HLS STREAM variable=fifo_a0 depth=2

  hls::stream<unsigned int> fifo_a1;
#pragma HLS STREAM variable=fifo_a1 depth=2

  hls::stream<unsigned int> fifo_b0;
#pragma HLS STREAM variable=fifo_b0 depth=2

  hls::stream<unsigned int> fifo_b1;
#pragma HLS STREAM variable=fifo_b1 depth=2

  hls::stream<unsigned int> fifo_c0;
#pragma HLS STREAM variable=fifo_c0 depth=2

  hls::stream<unsigned int> fifo_c1;
#pragma HLS STREAM variable=fifo_c1 depth=2

  data_feeder(a, b, fifo_a0, fifo_a1, fifo_b0);
  PE0(fifo_a0, fifo_b0, fifo_b1, fifo_c0);
  PE1(fifo_a1, fifo_b1, fifo_c1);
  data_collect(fifo_c0, fifo_c1, local_c);

  /*
#pragma HLS INLINE off
//k_loop:    for(int k = 0; k < M; k++) {
i_loop:        for(int i = 0; i < M; i++) {
                   //#pragma HLS PIPELINE II = 2
p_loop:            for(int p = 0; p < M; p += 2) {
#pragma HLS PIPELINE II = 1
j_loop:            for(int j = 0; j < 2; j++) {
#pragma HLS DEPENDENCE variable="local_c" inter false
                       local_c[j+p] += a[j+p][i] * b[i];
                   }
                   }
               }
//           }
    */



} 

void mm( unsigned int a[M][M], unsigned int b[M], unsigned int c[M] )
{
    unsigned int local_a[M][M];
    unsigned int local_b[M];
//#pragma HLS ARRAY_PARTITION variable="local_b" cyclic factor=2 dim="1" partition
    unsigned int local_c[M];
#pragma HLS ARRAY_PARTITION variable="local_c" cyclic factor=2 dim="1" partition
#pragma HLS ARRAY_PARTITION variable="local_a" cyclic factor=2 dim="1" partition
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

