#include <iostream>
#include <algorithm>

#include <ocl_wrapper.h>
#include <utl_utils.h>

typedef float Type;


int main(int argc, char* argv[])
{

	// provides a convient access to the command line arguments
	// utl::Args args(argc, argv);
	// const size_t elements = args.size() > 1 ? args.toSizet(1) : 1 << 4;
	// const size_t local_size = 256;


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

	ocl::Sampler sampler(context);

	{

		typedef utl::Zeros  <Type,utl::column_major_tag> Zeros;
		typedef utl::Matrix  <Type,utl::column_major_tag> Matrix;

		const size_t elements  = 1 << 4;
		const size_t local_size = 256;

		// get the kernels.
		ocl::Kernel &kernel = program.kernel("reverse4");
		// set the index space for the kernels
		kernel.setWorkSize(local_size/4, elements/4);


		// create host matrices
		Matrix h_matrix_in  = Zeros(elements,1);
		Matrix h_matrix_out = Zeros(elements,1);

		float i = 0;
		for(auto &v : h_matrix_in)
			v = ++i;

		std::cout << "Vector(): " << std::endl << h_matrix_in << std::endl;

		// create device buffers on the specified context

		ocl::Image image_in(context, elements/4, 1);
		ocl::Image image_out(context, elements/4, 1);

		// copy data from host buffers to device image
		const size_t region[3] = {elements/4,1,1};
		image_in.write(queue, h_matrix_in.data(), region);

		ocl::Sampler sampler(context);

		kernel(queue, int(elements/4), image_out.id(), image_in.id(), sampler.id());

		image_out.read(queue, h_matrix_out.data(), region);

		std::cout << "Vector(reversed): " << std::endl << h_matrix_out << std::endl;
	}



	{

		typedef utl::Zeros  <Type,utl::column_major_tag> Zeros;
		typedef utl::Matrix  <Type,utl::column_major_tag> Matrix;

		const size_t rows  = 1 << 6;
		const size_t cols  = 1 << 7;
		const size_t localRows = 1 << 5;
		const size_t localCols = 1 << 4;


		// create host matrices
		Matrix A = Zeros(rows,cols);
		Matrix B = Zeros(rows,cols);
		Matrix C = Zeros(rows,cols);

		float i = 0;
		for(auto &a : A) a = ++i;

		B = A;
//		std::cout << "A: " << std::endl << A << std::endl;
//		std::cout << "B: " << std::endl << B << std::endl;


		{
			C = 0;
			// get the kernels.
			ocl::Kernel &kernel = program.kernel("add4");
			// set the index space for the kernels
			kernel.setWorkSize(localCols, localRows>>2, cols, rows>>2);

			// create device buffers on the specified context
			ocl::Image iA(context, A.cols() , A.rows()>>2);
			ocl::Image iB(context, B.cols() , B.rows()>>2);
			ocl::Image iC(context, C.cols() , C.rows()>>2);

			// copy data from host buffers to device image
			const size_t regionA[3] = {A.cols(), A.rows()>>2,1};
			const size_t regionB[3] = {B.cols(), B.rows()>>2,1};
			const size_t regionC[3] = {C.cols(), C.rows()>>2,1};

			iA.write(queue, A.data(), regionA);
			iB.write(queue, B.data(), regionB);

			kernel(queue, int(A.cols()), int(A.rows()), iC.id(), iA.id(), iB.id(), sampler.id());

			iC.read(queue, C.data(), regionC);

//			std::cout << "C: " << std::endl << C << std::endl;

			if(A+B != C) std::cout << "Computation result: FALSE" << std::endl;
			else         std::cout << "Computation result: TRUE" << std::endl;
		}



		{
			C = 0;

			// get the kernels.
			ocl::Kernel &kernel = program.kernel("add4");
			// set the index space for the kernels
			kernel.setWorkSize(localCols>>2, localRows, cols>>2, rows);

			// create device buffers on the specified context
			ocl::Image iA(context, A.cols()>>2 , A.rows());
			ocl::Image iB(context, B.cols()>>2 , B.rows());
			ocl::Image iC(context, C.cols()>>2 , C.rows());

			// copy data from host buffers to device image
			const size_t regionA[3] = {A.cols()>>2, A.rows(),1};
			const size_t regionB[3] = {B.cols()>>2, B.rows(),1};
			const size_t regionC[3] = {C.cols()>>2, C.rows(),1};

			iA.write(queue, A.data(), regionA);
			iB.write(queue, B.data(), regionB);


			kernel(queue, int(A.cols()), int(A.rows()), iC.id(), iA.id(), iB.id(), sampler.id());

			iC.read(queue, C.data(), regionC);

//			std::cout << "C: " << std::endl << C << std::endl;

			if(A+B != C) std::cout << "Computation result: FALSE" << std::endl;
			else         std::cout << "Computation result: TRUE" << std::endl;
		}
	}


	{

		typedef utl::Zeros  <Type,utl::row_major_tag> Zeros;
		typedef utl::Matrix  <Type,utl::row_major_tag> Matrix;

		const size_t rows  = 1 << 6;
		const size_t cols  = 1 << 7;
		const size_t localRows = 1 << 5;
		const size_t localCols = 1 << 4;


		// create host matrices
		Matrix A = Zeros(rows,cols);
		Matrix B = Zeros(rows,cols);
		Matrix C = Zeros(rows,cols);

		float i = 0;
		for(auto &a : A) a = ++i;

		B = A;
//		std::cout << "A: " << std::endl << A << std::endl;
//		std::cout << "B: " << std::endl << B << std::endl;


		{
			C = 0;
			// get the kernels.
			ocl::Kernel &kernel = program.kernel("add4");
			// set the index space for the kernels
			kernel.setWorkSize(localCols, localRows>>2, cols, rows>>2);

			// create device buffers on the specified context
			ocl::Image iA(context, A.cols() , A.rows()>>2);
			ocl::Image iB(context, B.cols() , B.rows()>>2);
			ocl::Image iC(context, C.cols() , C.rows()>>2);

			// copy data from host buffers to device image
			const size_t regionA[3] = {A.cols(), A.rows()>>2,1};
			const size_t regionB[3] = {B.cols(), B.rows()>>2,1};
			const size_t regionC[3] = {C.cols(), C.rows()>>2,1};

			iA.write(queue, A.data(), regionA);
			iB.write(queue, B.data(), regionB);

			kernel(queue, int(A.cols()), int(A.rows()), iC.id(), iA.id(), iB.id(), sampler.id());

			iC.read(queue, C.data(), regionC);

//			std::cout << "C: " << std::endl << C << std::endl;

			if(A+B != C) std::cout << "Computation result: FALSE" << std::endl;
			else         std::cout << "Computation result: TRUE" << std::endl;
		}



		{
			C = 0;

			// get the kernels.
			ocl::Kernel &kernel = program.kernel("add4");
			// set the index space for the kernels
			kernel.setWorkSize(localCols>>2, localRows, cols>>2, rows);

			// create device buffers on the specified context
			ocl::Image iA(context, A.cols()>>2 , A.rows());
			ocl::Image iB(context, B.cols()>>2 , B.rows());
			ocl::Image iC(context, C.cols()>>2 , C.rows());

			// copy data from host buffers to device image
			const size_t regionA[3] = {A.cols()>>2, A.rows(),1};
			const size_t regionB[3] = {B.cols()>>2, B.rows(),1};
			const size_t regionC[3] = {C.cols()>>2, C.rows(),1};

			iA.write(queue, A.data(), regionA);
			iB.write(queue, B.data(), regionB);


			kernel(queue, int(A.cols()), int(A.rows()), iC.id(), iA.id(), iB.id(), sampler.id());

			iC.read(queue, C.data(), regionC);

//			std::cout << "C: " << std::endl << C << std::endl;

			if(A+B != C) std::cout << "Computation result: FALSE" << std::endl;
			else         std::cout << "Computation result: TRUE" << std::endl;
		}
	}
    
	return 0;
}
