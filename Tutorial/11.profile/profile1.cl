// c Zeros( M );
// A Ones ( M, N );
// b Ones ( N );


// thread m is responsible to calculate one inner product of row a and b.
// thread m, thread m+1 address contiguous column elements of A -> coalesced memory access
template<class Type>
__kernel void matvec1_cmajor(__global Type *c, __global Type *A, __global Type *b)
{
	int m = get_global_id(0); 

	if(m >= M) return;
	
	__global Type* a = A + m;

	Type s = 0;
	for(size_t n = 0; n < N; ++n)
		s += a[n*M] * b[n];
		
	c[m] = s;
}

// thread m is responsible to calculate one inner product of row a and b.
// thread m, thread m+1 address not contiguous column elements of A
template<class Type>
__kernel void matvec1_rmajor(__global Type *c, __global Type *A, __global Type *b)
{
	int m = get_global_id(0);

	if(m >= M) return;
	
	__global Type* a = A + m*N;

	Type s = 0;
	for(size_t n = 0; n < N; ++n)
		s += a[n] * b[n];
		
	c[m] = s;
}
