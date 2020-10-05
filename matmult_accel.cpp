#include "matmult.h"

// --------------------------------------------------------
// function to be accelerated in HW
template <typename T> void mmult_hw(T a[N][N], T b[N][N], T out[N][N]) {
L1:
  for (int ia = 0; ia < N; ++ia)
  L2:
    for (int ib = 0; ib < N; ++ib) {
      T sum = 0;
    L3:
      for (int id = 0; id < N; ++id)
        sum += a[ia][id] * b[id][ib];
      out[ia][ib] = sum;
    }

  return;
}

template <typename T> void axis2Mat(axis_t *src, T A[N][N], T B[N][N]) {
#pragma HLS inline off
  union {
    int ival;
    T oval;
  } converter;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
      int k = i * N + j;
      converter.ival = src[k].data;
      A[i][j] = converter.oval;
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
      int k = i * N + j;
      converter.ival = src[k + SIZE].data;
      B[i][j] = converter.oval;
    }
  }
}

template <typename T> void Mat2axis(T C[N][N], axis_t *dst) {
#pragma HLS inline off
  union {
    int oval;
    T ival;
  } converter;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
#pragma HLS pipeline
#pragma HLS loop_flatten off
      ap_uint<1> tmp = 0;
      if ((i == N - 1) && (j == N - 1)) {
        tmp = 1;
      }
      dst[i * N + j].last = tmp;
      converter.ival = C[i][j];
      dst[i * N + j].data = converter.oval;
    }
  }
}

extern "C" {
void matmult_accel(axis_t *src, axis_t *dst) {

#pragma HLS INTERFACE axis port = src
#pragma HLS INTERFACE axis port = dst
#pragma HLS INTERFACE s_axilite port = return

#pragma HLS dataflow

  float A[N][N];
  float B[N][N];
  float C[N][N];

  axis2Mat(src, A, B);

  mmult_hw(A, B, C);

  Mat2axis(C, dst);
}
}
