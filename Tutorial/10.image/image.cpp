#include <iostream>
#include <algorithm>

#include <ocl_wrapper.h>
#include <utl_utils.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

typedef float Type;
typedef utl::Matrix <Type,utl::column_major_tag> Matrix;
typedef utl::Ones <Type,utl::column_major_tag> Ones;
typedef utl::Zeros <Type,utl::column_major_tag> Zeros;
typedef utl::Rand <Type,utl::column_major_tag, utl::uniform_dist_tag> Rand;

int main(int argc, char* argv[])
{

	// provides a convient access to the command line arguments
	utl::Args args(argc, argv);

	const size_t elements = args.size() > 1 ? args.toSizet(1) : 1 << 4;
	const size_t local_size = 256;
	


	ocl::Platform platform(ocl::device_type::GPU);
	ocl::Device device = platform.device(ocl::device_type::GPU);
	// creates a context for a decice or platform
	ocl::Context context(device);
	// inserts contexts into the platform
	platform.insert(context);
	// creates command queue.
	ocl::Queue queue(context, device);
	// create program on a context
	// as the kernel is templated, creates kernel for single and integer types
	ocl::Program program(context, utl::type::Single | utl::type::Int);       	
	// inserts kernels into the program.
	std::ifstream file("10.image/image.cl");
	program << file;
	// kernels are created and program is built for the context.
	program.build();
	// get the kernels.
	ocl::Kernel &kernel = program.kernel("reverse4");
	// set the index space for the kernels
	kernel.setWorkSize(local_size/4, elements/4);


	// create host matrices
	auto h_matrix_in  = Zeros(elements,1);
	auto h_matrix_out = Zeros(elements,1);

	float i = 0;
	for(auto &v : h_matrix_in)
		v = ++i;

	std::cout << "Matrix(): " << std::endl << h_matrix_in << std::endl;

	// create device buffers on the specified context

	ocl::Image image_in(context, elements/4, 1);
	ocl::Image image_out(context, elements/4, 1);

	// copy data from host buffers to device image    
	const size_t region[3] = {elements/4,1,1};
	image_in.write(queue, h_matrix_in.data(), region);

	ocl::Sampler sampler(context);

	kernel(queue, int(elements/4), image_out.id(), image_in.id(), sampler.id());

	image_out.read(queue, h_matrix_out.data(), region);

	std::cout << "Matrix(reversed): " << std::endl << h_matrix_out << std::endl;
    
	return 0;
}
