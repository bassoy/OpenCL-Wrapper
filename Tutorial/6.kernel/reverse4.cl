__kernel void reverse4(int num, __global float4 *dst, __global float4 *src)
{
    int id = get_global_id(0);
    if(id >= num) return;
    dst[num-id-1].wzyx = src[id].xyzw;
}
