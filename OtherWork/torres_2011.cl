// This is the less sophisticated implementation mentioned in the paper.
__kernel void gemm( size_t const N, size_t const L, __global int const* A, __global int const* B, __global int* C )
{
  int c = 0;
  
  size_t n = get_global_id( 0 );
  size_t m = get_global_id( 1 );
  
  for ( int i = 0; i < L; ++i )
    c += A[n + i * N] * B[m * L + i];
  
  C[n + m * N] = c;
}
