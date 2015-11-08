#include <iostream>

#include <ocl_wrapper.h>
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

    // can set an active context.
    // note that there can only be one active context in one platform.
    platform.setActiveContext(context1);

    // create command queue.
    ocl::Queue queue1(context1, device);
    ocl::Queue queue2(context2, device);

    // can set an active queue.
    // note that there can only be one active queue within a context.
    context1.setActiveQueue(queue1);
    context2.setActiveQueue(queue2);

    // create program on a context.
    ocl::Program program1(context1, utl::type::Single);
    ocl::Program program2(context2, utl::type::Single);


    // insert kernels into the program.
    program1 << kernel_strings::copy;
    program2 << kernel_strings::copy;

    // kernels are created and program is built for the context.
    program1.build();
    program2.build();

    if(program1.isBuilt())
        std::cout << "Program 1 on context 1 is built" << std::endl;

    if(program2.isBuilt())
        std::cout << "Program 2 on context 2 is built" << std::endl;

    // can have an active program for the context
    context1.setActiveProgram(program1);
    context2.setActiveProgram(program2);

    std::string s;
    context1.activeProgram() == program1 ? s = "active!" : s = "inactive!";
    std::cout << "Program 1 is on context 1 " << s << std::endl;

    context2.activeProgram() == program2 ? s = "active!" : s = "inactive!";
    std::cout << "Program 2 is on context 2 " << s << std::endl;

	return 0;
}
