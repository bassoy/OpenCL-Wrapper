#include <iostream>

#include <ocl_wrapper.h>
#include <utl_utils.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

namespace kernel_strings {

const std::string copy =
R"(

__kernel void copy(int num, __global float *lhs, __global float *rhs)
{
    int id = get_global_id(0);
    if(id >= num) return;
    lhs[id] = rhs[id];
}

)";

}

int main()
{

    ocl::Platform platform(ocl::device_type::CPU);
    ocl::Device device = platform.device(ocl::device_type::CPU);

    // creates a context for a decice or platform
    ocl::Context context1(device);
    ocl::Context context2(platform);

    // insert contexts into the platform
    platform.insert(context1);
    platform.insert(context2);

    // create command queue.
    ocl::Queue queue1(context1, device);
    ocl::Queue queue2(context2, device);

    // create program on a context.
    ocl::Program program1(context1, utl::type::Single);
    ocl::Program program2(context2, utl::type::Single);


    // insert kernels into the program.
    program1 << kernel_strings::copy;
    program2 << kernel_strings::copy;

    // kernels are created and program is built for the context.
    program1.build();
    program2.build();

    // size_t size_bytes = 10 * (1 << 20); // 10 MB

    size_t size_bytes = 24; // 24 Bytes
    size_t elements = size_bytes / sizeof(float); // 6 Elements

    // create host buffers
    std::vector<float> h_buffer1_in (elements,1.0);
    std::vector<float> h_buffer1_out(elements,0.0);

    std::vector<float> h_buffer2_in (elements,1.0);
    std::vector<float> h_buffer2_out(elements,0.0);

    std::cout << "Vector1 before transmission : " << utl::toString(h_buffer1_out) << std::endl;
    std::cout << "Vector2 before transmission : " << utl::toString(h_buffer2_out) << std::endl;

    // create device buffers on the specified context
    ocl::Buffer d_buffer1_in (context1, size_bytes);
    ocl::Buffer d_buffer1_out(context1, size_bytes);

    ocl::Buffer d_buffer2_in (context2, size_bytes);
    ocl::Buffer d_buffer2_out(context2, size_bytes);

    // copy data from host buffers to device buffers
    d_buffer1_in.write(queue1, h_buffer1_in.data(), size_bytes);
    d_buffer2_in.write(queue2, h_buffer2_in.data(), size_bytes);


    // copy data from device buffers to device buffers
    d_buffer1_in.copyTo(queue1, 0, size_bytes, d_buffer1_out,0);
    d_buffer2_in.copyTo(queue2, 0, size_bytes, d_buffer2_out,0);

    // copy data from device buffers to host buffers
    d_buffer1_out.read(queue1,h_buffer1_out.data(), size_bytes);
    d_buffer2_out.read(queue2,h_buffer2_out.data(), size_bytes);

    std::cout << "Vector1 after transmission : " << utl::toString(h_buffer1_out) << std::endl;
    std::cout << "Vector2 after transmission : " << utl::toString(h_buffer2_out) << std::endl;

	return 0;
}
