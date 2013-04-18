#include <iostream>
#include <algorithm>

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
    ocl::Context context(device);

    // insert contexts into the platform
    platform.insert(context);

    // create command queue.
    ocl::Queue queue1(context, device);
    ocl::Queue queue2(context, device);

    // create program on a context.
    ocl::Program program(context);

    // insert kernels into the program.
    program << kernel_strings::copy;

    // kernels are created and program is built for the context.
    program.build();

    // get the kernels.
    ocl::Kernel &kernel = program.kernel("copy");

    size_t size_bytes = 128 * (1 << 20); // 100 MB
    size_t elements = size_bytes / sizeof(float);

    // set the index space for the kernels
    kernel.setWorkSize(256, elements);

    // create host buffers
    std::vector<float> h_buffer_in (elements,1.0);
    std::vector<float> h_buffer1_out(elements,0.0);
    std::vector<float> h_buffer2_out(elements,0.0);

    // create device buffers on the specified context
    ocl::Buffer d_buffer_in (context, size_bytes);
    ocl::Buffer d_buffer1_out(context, size_bytes);
    ocl::Buffer d_buffer2_out(context, size_bytes);

    // copy data from host buffers to device buffers
    const ocl::Event& event_write = d_buffer_in.writeAsync(queue1, 0, h_buffer_in.data(), size_bytes);

    // create an eventlist to track the write command.
    ocl::EventList list_write(event_write);

    // execute both kernels only if the event_write is completed.
    // note that kernel executions are always asynchronous.
    const ocl::Event& event_exec1 = kernel(queue1, list_write, int(elements), d_buffer1_out.id(), d_buffer_in.id());
    const ocl::Event& event_exec2 = kernel(queue2, list_write, int(elements), d_buffer2_out.id(), d_buffer_in.id());

    // create an event list which tracks the executions.
    ocl::EventList list_exec; list_exec << event_exec1 << event_exec2;

    // copy data from device buffers to host buffers
    d_buffer1_out.read(queue1,h_buffer1_out.data(), size_bytes, list_exec);
    d_buffer2_out.read(queue2,h_buffer2_out.data(), size_bytes);

    float one = 1.0;
    bool eq1 = h_buffer1_out.end() == std::find_first_of(h_buffer1_out.begin(), h_buffer1_out.end(), &one, &one+1, std::not_equal_to<float>());
    bool eq2 = h_buffer2_out.end() == std::find_first_of(h_buffer2_out.begin(), h_buffer2_out.end(), &one, &one+1, std::not_equal_to<float>());

    if(eq1) std::cout << "Computation1 is correct" << std::endl;
    if(eq2) std::cout << "Computation2 is correct" << std::endl;

	return 0;
}
