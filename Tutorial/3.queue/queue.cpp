#include <iostream>

#include <ocl_wrapper.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif


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

    std::string s;
    context1.activeQueue() == queue1 ? s = "active!" : s = "inactive!";
    std::cout << "Queue 1 is on context 1 " << s << std::endl;

    context2.activeQueue() == queue2 ? s = "active!" : s = "inactive!";
    std::cout << "Queue 2 is on context 2 " << s << std::endl;

    // create command queue which executes the commands out-of-order.
    ocl::Queue queue3(context1, device, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE);


	return 0;
}
