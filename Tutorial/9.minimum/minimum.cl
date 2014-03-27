template<class T>
__kernel void minimum(int num, __global T *x, int offset, int step, __global T *h, __local T *s)
{
	// load shared mem
	int lid = get_local_id(0);
	int gid = get_global_id(0);

	s[lid] = (gid < num) ? x[gid*step+offset] : x[0];

	barrier(CLK_LOCAL_MEM_FENCE);

	// do reduction in shared mem
	for(int j=get_local_size(0)>>1; j>0; j>>=1)
	{
			if (lid >= j) continue;			
			s[lid] = s[lid] < s[lid+j] ? s[lid] : s[lid+j];			
			barrier(CLK_LOCAL_MEM_FENCE);
	}

	// minmax_areduction(op, s);

	// write result for this block to global mem
	if (lid == 0) h[get_group_id(0)] = s[0];
}

