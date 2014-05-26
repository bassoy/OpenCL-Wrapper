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
		
    const size_t elements_in = args.size() > 1 ? args.toSizet(1) : 1 << 22;
    const size_t size_bytes_in  = elements_in * sizeof(Type);
    const size_t local_size = 256;
    const size_t elements_out = std::max(elements_in/local_size, 1ul);
	const size_t size_bytes_out = elements_out * sizeof(Type);
	const size_t execute = 100;

	using Timer = utl::Timer<utl::Seconds>;
			
		

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
    std::ifstream file("9.minimum/minimum.cl");
    program << file;
    // kernels are created and program is built for the context.
    program.build();
    // get the kernels.
    ocl::Kernel &kernel = program.kernel("minimum", utl::type::Single);
    // set the index space for the kernels
    kernel.setWorkSize(local_size, elements_in);
    

    // create host matrices
    auto h_matrix_in  = Rand(1,elements_in);
    auto h_matrix_out = Zeros(1,elements_out);

    //std::cout << "Matrix(out) before computation: " << std::endl << h_matrix_out << std::endl;

    // create device buffers on the specified context
    ocl::Buffer d_matrix_in (context, size_bytes_in);
    ocl::Buffer d_matrix_out(context, size_bytes_out);

    // copy data from host buffers to device buffers
    d_matrix_in.write(queue, 0, h_matrix_in.data(), size_bytes_in);

    // execute both kernels only if the event_write is completed.
    // note that kernel executions are always asynchronous.
	Timer::tic();
    for(size_t i = 0; i < execute; ++i){
	    kernel(queue, int(elements_in), d_matrix_in.id(), 0, 1, d_matrix_out.id(), local_size * sizeof(Type));
    	queue.finish();
    }        
    // copy data from device buffers to host buffers
    d_matrix_out.read(queue, h_matrix_out.data(), size_bytes_out);       
    float min_gpu = std::min_element(h_matrix_out.begin(), h_matrix_out.end())[0];
	Timer::toc();
	std::cout << "Minimum[GPU]: " << min_gpu << ", Time[GPU] = " << Timer::elapsed() << std::endl;
	
		
	float min_cpu;
	min_cpu = std::min_element(h_matrix_in.begin(), h_matrix_in.end())[0];
	min_cpu = std::min_element(h_matrix_in.begin(), h_matrix_in.end())[0];

	Timer::tic();
	for(size_t i = 0; i < execute; ++i){
		min_cpu = std::min_element(h_matrix_in.begin(), h_matrix_in.end())[0];
	}
	Timer::toc();
	std::cout << "Minimum[CPU]: " << min_cpu << ", Time[CPU] = " << Timer::elapsed() << std::endl;
    

	return 0;
}
