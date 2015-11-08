#include <iostream>
#include <fstream>
#include <iterator>

#include <ocl_wrapper.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

namespace kernel_strings {

const std::string reverse =
R"(

__kernel void reverse(int num, __global float *dst, __global float *src)
{
    int id = get_global_id(0);
    if(id >= num) return;
    dst[num-id-1] = src[id];
}
)";
}

int main()
{
  // It is hard to test on Xeon Phi, if the device_type is GPU.
  ocl::DeviceType deviceType = 
#if 0
  ocl::device_type::GPU;
#else
  ocl::device_type::CPU;
#endif

    ocl::Platform platform(deviceType);
    ocl::Device device = platform.device(deviceType);

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
    
    std::ifstream file("6.kernel/reverse4.cl");

    // insert kernels into the program.
    program1 << kernel_strings::reverse;
    program2 << file;


    // kernels are created and program is built for the context.
    program1.build();
    program2.build();


    // get the kernels.
    ocl::Kernel &kernel1 = program1.kernel("reverse");
    ocl::Kernel &kernel2 = program2.kernel("reverse4");

    std::string s;
    kernel1.created() ? s = "created!" : s = "not created!";
    std::cout << "Kernel 1 is on context 1 " << s << std::endl;
    kernel2.created() ? s = "created!" : s = "not created!";
    std::cout << "Kernel 2 is on context 2 " << s << std::endl;

    // size_t size_bytes = 10 * (1 << 20); // 10 MB

    size_t size_bytes = 128; 
    size_bytes *= 4;
    size_t elements = size_bytes / sizeof(float); // 8 Elements

    // create host buffers
    std::vector<float> h_buffer1_in (elements,1.0);
    std::vector<float> h_buffer1_out(elements,0.0);

    std::vector<float> h_buffer2_in (elements,1.0);
    std::vector<float> h_buffer2_out(elements,0.0);

    float i = 0.0;
    for (auto &v : h_buffer1_in)
    	v = ++i;

    i = 0.0;
    for (auto &v : h_buffer2_in)
    	v = ++i;

	std::cout << "Vector1 before transmission : ";
	std::copy(h_buffer1_out.begin(), h_buffer1_out.end(), std::ostream_iterator<float>(std::cout, ", "));
	std::cout << std::endl;

	std::cout << "Vector1 before transmission : ";
	std::copy(h_buffer2_out.begin(), h_buffer2_out.end(), std::ostream_iterator<float>(std::cout, ", "));
	std::cout << std::endl;

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
    kernel2.setWorkSize(256/4, elements/4);

    // execute the kernels
    kernel1(queue1, int(elements), d_buffer1_out.id(), d_buffer1_in.id());
    kernel2(queue2, int(elements)/4, d_buffer2_out.id(), d_buffer2_in.id());

//    // copy data from device buffers to device buffers
//    d_buffer1_in.copyTo(queue1, 0, size_bytes, d_buffer1_out,0);
//    d_buffer2_in.copyTo(queue2, 0, size_bytes, d_buffer2_out,0);

    // copy data from device buffers to host buffers
    d_buffer1_out.read(queue1,h_buffer1_out.data(), size_bytes);
    d_buffer2_out.read(queue2,h_buffer2_out.data(), size_bytes);
        

	std::cout << "Vector1 after transmission : ";
	std::copy(h_buffer1_out.begin(), h_buffer1_out.end(), std::ostream_iterator<float>(std::cout, ", "));
	std::cout << std::endl;

	std::cout << "Vector1 after transmission : ";
	std::copy(h_buffer2_out.begin(), h_buffer2_out.end(), std::ostream_iterator<float>(std::cout, ", "));
	std::cout << std::endl;

	return 0;
}
