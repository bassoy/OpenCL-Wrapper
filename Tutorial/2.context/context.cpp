#include <iostream>

#include <ocl_wrapper.h>
#include <utl_utils.h>
#include <CL/opencl.h>



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

    // can have an active context.
    // note that there can only be one active context in one platform.
    platform.setActiveContext(context1);

    std::string s;
    *platform.activeContext() == context1 ? s = "1" : s = "2";
    std::cout << "Context " << s << " is active " << std::endl;


	return 0;
}
