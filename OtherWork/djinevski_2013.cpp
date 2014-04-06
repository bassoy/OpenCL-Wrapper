/**
 * GEMM reconstructed from "Superlinear Speedup for Matrix Multiplication in GPU Devices"
 * by Djineski, Ristov and Gusev, 2013
 * 
 * @see http://link.springer.com/chapter/10.1007%2F978-3-642-37169-1_28
 * 
 * @author Felix Patschkowski
 * @date April 2014
 */

#include <cstdlib>

#include <ocl_buffer.h>
#include <ocl_context.h>
#include <ocl_device.h>
#include <ocl_device_type.h>
#include <ocl_kernel.h>
#include <ocl_platform.h>
#include <ocl_program.h>
#include <ocl_queue.h>
#include <utl_args.h>
#include <utl_matrix.h>
#include <utl_profile_pass.h>
#include <utl_profile_pass_manager.h>


typedef float Type;



class Djinevski2013Pass : public utl::ProfilePass< Type >
{
public :
  Djinevski2013Pass( std::istream& source, utl::Dim const& start, utl::Dim const& step, utl::Dim const& end, std::size_t iter = 10 );
    
  double prof( utl::Dim const& ) override;
  
  double ops( utl::Dim const& dim ) override;
  
private :
  typedef utl::Matrix< Type, utl::column_major_tag > Matrix;
  typedef utl::Zeros< Type, utl::column_major_tag > Zeros;
  
  bool          testing_;
  ocl::Platform platform_;
  ocl::Device   device_;
  ocl::Context  context_;
  ocl::Queue    queue_;
  ocl::Program  program_;
};



Djinevski2013Pass::Djinevski2013Pass( std::istream& source, utl::Dim const& start, utl::Dim const& step, utl::Dim const& end, std::size_t iter ):
  ProfilePass< Type >( "Djinevski2013", start, step, end, iter ),
  testing_( true ),
  platform_( ocl::device_type::CPU ),
  device_( platform_.device( ocl::device_type::CPU ) ),
  context_( device_ ),
  queue_( (platform_.insert( context_ ), platform_.setActiveContext( context_ ), context_), device_, CL_QUEUE_PROFILING_ENABLE ),
  program_( (context_.setActiveQueue( queue_ ), context_), utl::type::Single )
{
  program_ << source;
  
  std::ostringstream oss;
  oss << "-cl-std=CL1.2 -w -Werror";
  
  ocl::CompileOption const opts( oss.str() );
  
  program_.setCompileOption( ocl::compile_option::FAST_MATH | ocl::compile_option::NO_SIGNED_ZERO | opts );
  program_.build();
  
  if ( program_.isBuilt() )
  {
    context_.setActiveProgram( program_ );
    
    ocl::Kernel& kernel( program_.kernel( "gemm" ) );
    
    if ( !kernel.created() )
    {
      throw std::runtime_error( "kernel not created" );
    }
  }
  else
  {
    throw std::runtime_error( "program not built" );
  }
}



double Djinevski2013Pass::prof( utl::Dim const& dim )
{
  std::size_t const N = dim[0];
  
  Zeros result( N, N );
  
  Matrix lhs( N, N );
  Matrix rhs( N, N );
  
  // for ( size_t i = 0; i < N * L; ++i ) lhs[i] = i + 1; for ( size_t i = 0; i < L * M; ++i ) rhs[i] = i + 1;
  for ( size_t i = 0; i < N * N; ++i ) lhs[i] = i % N;
  for ( size_t i = 0; i < N * N; ++i ) rhs[i] = i / N;
  
  std::chrono::nanoseconds totalRuntime{ 0 };
  
  ocl::Kernel& kernel( program_.kernel( "gemm" ) );
  
  size_t const numProcessingElements = device_.maxComputeUnits();
      
  /*
   * Create one thread per processing element 
   * to store a part of matrix B in cache memory.
   */
  kernel.setWorkSize( 1, numProcessingElements );
  
  size_t constexpr typeSize = sizeof (Type);
  size_t const numResultBytes = typeSize * result.size();
  size_t const numLhsBytes = typeSize * lhs.size();
  size_t const numRhsBytes = typeSize * rhs.size();
  
  ocl::Buffer bufResult( context_, numResultBytes, ocl::Buffer::WriteOnly ),
              bufLhs( context_, numLhsBytes, ocl::Buffer::ReadOnly ),
              bufRhs( context_, numRhsBytes, ocl::Buffer::ReadOnly );

  for ( std::size_t i = 0; i < this->_iter; ++i )
  {
    // Copy data from host to device.
    ocl::Event const lhsWritten = bufLhs.writeAsync( queue_, 0u, lhs.data(), numLhsBytes );
    ocl::Event const rhsWritten = bufRhs.writeAsync( queue_, 0u, rhs.data(), numRhsBytes );
    
    ocl::EventList operandsWritten;
    operandsWritten << lhsWritten << rhsWritten;
    
    // Execute kernel when both operands have been loaded.
    size_t const L2CacheSize = 1024 * 768;
    ocl::Event const multiplyDone = kernel( queue_, operandsWritten, N, L2CacheSize, bufLhs.id(), bufRhs.id(), bufResult.id() );
    
    // Copy result from device to host.
    ocl::Event const resultRead = bufResult.readAsync( queue_, 0u, result.data(), numResultBytes, ocl::EventList( multiplyDone ) );
    
    // Wait for all commands being executed.
    queue_.finish();
    
    size_t const kernelRuntime_ns = multiplyDone.finishTime() - multiplyDone.startTime();

    totalRuntime += std::chrono::nanoseconds( kernelRuntime_ns );
  }
  
   if( testing_ )
  {
    auto const ref = lhs * rhs;
    auto const diff = result - ref;
    auto const iMax = std::max_element( diff.begin(), diff.end(), []( Type a, Type b ){
      return std::fabs( a ) < std::fabs( b );
    } );
    
    std::cout /*<< lhs << '*' << rhs << " = " */ << "ref = " << ref << std::endl;
    std::cout << "result = " << result << std::endl;
    std::cout << "Maximal error: " << *iMax << std::endl;
    
    if ( *iMax != 0 )
    {
      size_t const index = iMax - diff.begin();
      std::cout << "ref[" << index << "] = " << ref[index] << " != result[" << index << "] = " << result[index] << std::endl;
    }
  }
  
  // Return average time.
  return std::chrono::duration_cast< std::chrono::microseconds >( totalRuntime / this->_iter ).count();
}



double Djinevski2013Pass::ops( utl::Dim const& dim )
{
  // N * M * (L + (L - 1))
  return dim[0] * dim[0] * (2.0 * dim[0] - 1.0);
}



int main( int argc, char** argv )
{
  utl::Args args( argc, argv );
  
  if ( args.size() == 2 )
  {
    std::string const filename( args.toString( 1 ) );
    std::ifstream file( filename );
    
    if ( file.is_open() )
    {
      utl::ProfilePassManager< Type > mgr;
  
      mgr << std::make_shared<Djinevski2013Pass>( file, utl::Dim( 256 ), utl::Dim( 16 ), utl::Dim( 256 ) );
  
      mgr.run();
      mgr.write( std::cout );
    }
    else
    {
      std::cerr << "Failed opening file " << filename << std::endl;
    }
  }
  else
  {
    std::cout << "Usage: " << args.at( 0 ) << " <kernel.cl>" << std::endl;
  }
  
  return EXIT_SUCCESS;
}
