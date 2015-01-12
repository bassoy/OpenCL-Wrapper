//Copyright (C) 2013 Cem Bassoy.
//
//This file is part of the OpenCL Utility Toolkit.
//
//OpenCL Utility Toolkit is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//OpenCL Utility Toolkit is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with OpenCL Utility Toolkit.  If not, see <http://www.gnu.org/licenses/>.


#ifndef OCL_KERNEL_H
#define OCL_KERNEL_H


#include <string>
#include <typeinfo>
#include <set>
#include <vector>

#include <ocl_event.h>
#include <utl_assert.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
namespace utl
{
class Type;
}

namespace ocl
{

class Program;
class Context;
class Queue;
class EventList;

/*! \class Kernel ocl_kernel.h "inc/ocl_kernel.h"
  *
  * \brief Wrapper for cl_kernel.
  *
  * A Kernel is a wrapper for an OpenCL cl_kernel. It contains
  * the complete kernel function string and the function name. Additionally
  * It stores its calling parameters such as the global, local size
  * and working dimension.
  * Usually the user does not have to create Kernel objects. Instead
  * the Program is responsible for creating such from an input stream
  * or string and storing them with the possibility to acces Kernel functions
  * via their name. The Kernel can be executed if the corresponding
  * Program is built.
  * In order to execute a Kernel, arguments such as global and local
  * work size must be first set. The Kernel can then be called
  * by providing its arguments. Memory locations are extracted
  * autmatically by analyzing the kernel string.
  * A Kernel is destroyed by its Program.
  */
class Kernel
{
public:
    /*! \brief Enumeration for the memory locations of the arguments of this Kernel.*/
    enum mem_loc {global,local,host,constant,image,sampler};
    Kernel(const std::string &kernel);
    Kernel(const Program&, const std::string &kernel);
    Kernel(const std::string &kernel, const utl::Type &);
    Kernel(const Program&, const std::string &kernel, const utl::Type &);
    Kernel(const Kernel&) = delete;
		Kernel& operator =( Kernel const& ) = delete;
		~Kernel();
    void create();
    bool created() const;
    void release();
    cl_kernel id() const;
    const Program& program() const;
    void setProgram(const Program&);
    Context& context() const;

    template< typename ... Types >
    ocl::Event operator ()(Types const&... args)
    {
      pushArg(args...);
      return callKernel();
    }

    /*! \brief Executes this Kernel with arguments and returns an Event by which the execution can be tracked.
    *
    * The number of arguments must be equal to the number of arguments
    * defined in the kernel function of this Kernel. Also the types
    * must match.
    */
    template<typename ... Types>
    ocl::Event operator()(const ocl::Queue &queue, const ocl::EventList& list, const Types& ... args)
    {
      //int pos = 0;
      //if(sizeof...(args) > 0) setArg(pos, args ... );
      pushArg(args...);
      return callKernel(queue, list);
    }

    /*! \brief Executes this Kernel with arguments and returns an Event by which the execution can be tracked.
    *
    * The number of arguments must be equal to the number of arguments
    * defined in the kernel function of this Kernel. Also the types
    * must match.
    */
    template<typename ... Types>
    ocl::Event operator()(const ocl::Queue &queue, const Types& ... args)
    {
      //int pos = 0;
      //if(sizeof...(args) > 0) setArg(pos, args ... );
      pushArg(args ...);
      return callKernel(queue);
    }

	void setWorkSize(size_t lSizeX, size_t gSizeX);
	void setWorkSize(size_t lSizeX, size_t lSizeY, size_t gSizeX, size_t gSizeY);
	void setWorkSize(size_t lSizeX, size_t lSizeY, size_t lSizeZ, size_t gSizeX, size_t gSizeY, size_t gSizeZ);

	void setWorkDim(size_t dim);
	size_t workDim() const;

	const size_t* localSize() const;
	const size_t* globalSize() const;
	size_t localSize(size_t pos) const;
	size_t globalSize(size_t pos) const;

	void setLocalSize(size_t *localSize);
	void setGlobalSize(size_t *globalSize);
	void setLocalSize(size_t localSize, size_t pos);
	void setGlobalSize(size_t globalSize, size_t pos);

	const std::string& name() const;
	const std::string& toString() const;
	size_t numberOfArgs() const;
	mem_loc memoryLocation(size_t pos) const;


	static std::string specialize(const std::string &kernel, const std::string &type); //const utl::Type &);
	static std::vector<mem_loc> extractMemlocs(const std::string &kernel);
	static std::string extractName(const std::string &kernel);
	static std::string extractParameter(const std::string& kernel);
	static bool templated(const std::string& kernel);



    template<class T>
    void setArg(int pos, const T& data);
    void setArg(int pos, cl_mem);    
    void setArg(int pos, cl_sampler);

private:
    template< typename... Types > void pushArg( Types const& ... args )
    {
      ArgPusher< sizeof...(args), 0, Types... >( *this, args... );
    }
    
    void pushArg() { /* Do nothing for zero arguments. */ }
    
    template< size_t NumArgs, size_t ArgIndex, typename Type, typename... Types >
    struct ArgPusher
    {
      ArgPusher( Kernel& kernel, Type const& arg, Types const&... args )
      {
        kernel.setArg( ArgIndex, arg );
        
        ArgPusher< sizeof...( args ), ArgIndex + 1, Types... >( kernel, args... );
      }
    };
    
    template< size_t ArgIndex, typename Type>
    struct ArgPusher< 1, ArgIndex, Type >
    {
      ArgPusher( Kernel& kernel, Type const& arg )
      {
        kernel.setArg( ArgIndex, arg );
        
        // End recursion.
      }
    };
  
    Kernel();
    const Program * _program;
    cl_kernel _id;
    size_t _workDim;
    size_t _globalSize[3];
    size_t _localSize[3];
    ocl::Event callKernel();
    ocl::Event callKernel(const Queue&, const EventList&);
    ocl::Event callKernel(const Queue&);

    std::string _kernelfunc;
    std::string _name;
    std::vector<mem_loc> _memlocs;

};

/**
* Round @c x to the next multiple of @c y.
*
* This is usefull if one wants to fully populate all wavefronts/warps.
*/
template< typename T >
T roundNextMultiple( T x, T y )
{
 auto const tmp = x % y;

 return tmp ? x + y - tmp : x;
}

}

#endif

