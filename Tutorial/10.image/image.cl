__kernel void reverse4(int num, write_only image2d_t dst, read_only image2d_t src, sampler_t sampler)
{

    size_t x = get_global_id(0);
    
    if(x >= num) return;

    float4 tap = read_imagef(src, sampler, (int2)(x,0));

    write_imagef(dst, (int2)(num-x-1,0), tap.wzyx);
    
}


__kernel void add4(int cols, int rows, write_only image2d_t C, read_only image2d_t A, read_only image2d_t B, sampler_t sampler)
{
	size_t x = get_global_id(0); // col
	size_t y = get_global_id(1); // row

	if(x >= cols || y >= rows) return;

	float4 tap1 = read_imagef(A, sampler, (int2)(x,y));
	float4 tap2 = read_imagef(B, sampler, (int2)(x,y));

	float4 res = tap1 + tap2;

	write_imagef(C, (int2)(x,y), res);

}

