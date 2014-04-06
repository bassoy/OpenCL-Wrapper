__kernel void gemm( size_t const N, size_t const L2CacheSize, __global float const* A, __global float const* B, __global float* C )
{
  size_t const bx = get_global_id( 0 );
  size_t const m  = L2CacheSize / (N * sizeof (float));
  
  int jj = 0;
  
  while ( jj < (N + m - 1) / m )
  {
    int i = 0;
    
    int l = (bx + i * get_local_size( 0 )) * m;
  
    while ( l < N )
    {
      for ( int k1 = 0; k1 < min( m, N - jj * m ); ++k1 )
      {
        for ( int k2 = 0; k2 < min( m, N - jj * m ); ++k2 )
        {
          float c = 0.0f;
          
          for ( int n = 0; n < N; ++n )
            c += A[l + k1 + N * n] * B[k2 * N + jj * m * N + n];
          
          C[l + k1 + k2 * N + jj * m * N] = c;
        }
      }
    
      ++i;
      
      l = (bx + i * get_local_size( 0 )) * m;
    }
    
    ++jj;
  }
}
