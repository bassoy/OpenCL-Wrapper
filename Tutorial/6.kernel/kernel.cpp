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

__kernel void copy(int num, __global float *dst, __global float *src)
{
    int id = get_global_id(0);
    if(id >= num) return;
    dst[id] = src[id];
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
    ocl::Program program1(context1);
    ocl::Program program2(context2);


    // insert kernels into the program.
    program1 << kernel_strings::copy;
    program2 << kernel_strings::copy;


    // kernels are created and program is built for the context.
    program1.build();
    program2.build();


    // get the kernels.
    ocl::Kernel &kernel1 = program1.kernel("copy");
    ocl::Kernel &kernel2 = program2.kernel("copy");

    std::string s;
    kernel1.created() ? s = "created!" : s = "not created!";
    std::cout << "Kernel 1 is on context 1 " << s << std::endl;
    kernel2.created() ? s = "created!" : s = "not created!";
    std::cout << "Kernel 2 is on context 2 " << s << std::endl;

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

    // set the index space for the kernels
    kernel1.setWorkSize(256, elements);
    kernel2.setWorkSize(256, elements);

    // execute the kernels
    kernel1(queue1, int(elements), d_buffer1_out.id(), d_buffer1_in.id());
    kernel2(queue2, int(elements), d_buffer2_out.id(), d_buffer2_in.id());

//    // copy data from device buffers to device buffers
//    d_buffer1_in.copyTo(queue1, 0, size_bytes, d_buffer1_out,0);
//    d_buffer2_in.copyTo(queue2, 0, size_bytes, d_buffer2_out,0);

    // copy data from device buffers to host buffers
    d_buffer1_out.read(queue1,h_buffer1_out.data(), size_bytes);
    d_buffer2_out.read(queue2,h_buffer2_out.data(), size_bytes);

    std::cout << "Vector1 after transmission : " << utl::toString(h_buffer1_out) << std::endl;
    std::cout << "Vector2 after transmission : " << utl::toString(h_buffer2_out) << std::endl;

	return 0;
}
