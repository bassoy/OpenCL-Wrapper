__kernel void gemm( size_t const N, size_t const L, __global float const* A, __global float const* B, __global float* C )
{
#define BLOCKSIZE 16
  
  private float c[BLOCKSIZE] = { 0.0f }; // This is part of a row of C.
  local   float b[BLOCKSIZE][BLOCKSIZE]; // This is a block in B.
  
  // Matrices are stored in column major layout.
  
  A += get_global_id( 1 );
  B += L * BLOCKSIZE * get_group_id( 0 );
  C += N * BLOCKSIZE * get_group_id( 0 ) + get_global_id( 1 );
  
  global float const* end = B + L;
  
  do
  {
    {
      int       x = get_local_id( 1 ) / BLOCKSIZE;
      int const y = get_local_id( 1 ) % BLOCKSIZE;
      
      b[y][x] = B[x * L + y]; x += 4;
      b[y][x] = B[x * L + y]; x += 4;
      b[y][x] = B[x * L + y]; x += 4;
      b[y][x] = B[x * L + y];
    }
    
    barrier( CLK_LOCAL_MEM_FENCE );
    
    #pragma unroll 16
    for ( int i = 0; i < BLOCKSIZE; ++i )
    {
      private float const a = *A;
      
      A += N;
      
      c[0]  += a * b[i][0];
      c[1]  += a * b[i][1];
      c[2]  += a * b[i][2];
      c[3]  += a * b[i][3];
      c[4]  += a * b[i][4];
      c[5]  += a * b[i][5];
      c[6]  += a * b[i][6];
      c[7]  += a * b[i][7];
      c[8]  += a * b[i][8];
      c[9]  += a * b[i][9];
      c[10] += a * b[i][10];
      c[11] += a * b[i][11];
      c[12] += a * b[i][12];
      c[13] += a * b[i][13];
      c[14] += a * b[i][14];
      c[15] += a * b[i][15];
    }
    
    barrier( CLK_LOCAL_MEM_FENCE );
    
    B += BLOCKSIZE;
  }
  while ( B < end );
  
  C[0 * N] = c[0];
  C[1 * N] = c[1];
  C[2 * N] = c[2];
  C[3 * N] = c[3];
  C[4 * N] = c[4];
  C[5 * N] = c[5];
  C[6 * N] = c[6];
  C[7 * N] = c[7];
  C[8 * N] = c[8];
  C[9 * N] = c[9];
  C[10 * N] = c[10];
  C[11 * N] = c[11];
  C[12 * N] = c[12];
  C[13 * N] = c[13];
  C[14 * N] = c[14];
  C[15 * N] = c[15];
  
#undef BLOCKSIZE
}
