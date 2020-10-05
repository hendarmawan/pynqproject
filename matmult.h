#ifndef _XF_AXIS_CONFIG_
#define _XF_AXIS_CONFIG_

#include "ap_int.h"
#include "hls_stream.h"
#include <iostream>
#include <math.h>

#define N 128
#define SIZE 16384 // N*N

struct axis_t {
  ap_uint<32> data;
  ap_int<1> last;
};

template <typename T> void mmult_hw(T a[N][N], T b[N][N], T out[N][N]);

#endif
