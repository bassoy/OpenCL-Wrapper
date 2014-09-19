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
	~Kernel();
    void create();
    bool created() const;
    void release();
    cl_kernel id() const;
    const Program& program() const;
    void setProgram(const Program&);
    Context& context() const;

#if !defined(_MSC_VER) || _MSC_VER >= 1900
#if 0
    ocl::Event operator()();


    /*! \brief Executes this Kernel with arguments and returns an Event by which the execution can be tracked.
    *
    * The number of arguments must be equal to the number of arguments
    * defined in the kernel function of this Kernel. Also the types
    * must match.
    */
    template<typename T, typename ... Types>
    ocl::Event operator()(const T& arg0, const Types& ... args)
    {
    //int pos = 0;
    //setArg(pos, args ... );
    //pushArg(arg0);
    pushArg( arg0, args ... );
    return callKernel();
    }

    template<typename T1, typename T2, typename ... Types>
    ocl::Event operator()(const T1& arg0, const T2& arg1, const Types& ... args)
    {
      //int pos = 0;
      //setArg(pos, args ... );
      //pushArg(arg0);
      //pushArg(arg1);
      pushArg( arg0, arg1, args ...);
      return callKernel();
    }
#else
    template< typename ... Types >
    ocl::Event operator ()(Types const&... args)
    {
      pushArg(args...);
      return callKernel();
    }
#endif

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
#else
    ocl::Event operator()()
    {
      return callKernel();
    }

    template< typename T >
    ocl::Event operator()( T const& arg0 )
    {
      pushArg(arg0);
      return callKernel();
    }

    template< typename T1, typename T2 >
    ocl::Event operator()(T1 const& arg0, T2 const& arg1)
    {
      pushArg(arg0,arg1);
      return callKernel();
    }

    template< typename T1, typename T2, typename T3 >
    ocl::Event operator()(T1 const& arg0, T2 const& arg1, T3 const& arg2)
    {
      pushArg(arg0, arg1,arg2);
      return callKernel();
    }

    template< typename T1, typename T2, typename T3, typename T4 >
    ocl::Event operator()(T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3)
    {
      pushArg(arg0, arg1, arg2, arg3);
      return callKernel();
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5 >
    ocl::Event operator()(T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4);
      return callKernel();
    }


    ocl::Event operator()( Queue const& queue )
    {
      return callKernel(queue);
    }

    template< typename T >
    ocl::Event operator()(Queue const& queue , T const& arg0)
    {
      pushArg(arg0);
      return callKernel(queue);
    }

    template< typename T1, typename T2 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1)
    {
      pushArg(arg0, arg1);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2)
    {
      pushArg(arg0, arg1, arg2);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3)
    {
      pushArg(arg0, arg1, arg2, arg3);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9, T11 const& arg10)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
      return callKernel(queue);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12 >
    ocl::Event operator()(Queue const& queue, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9, T11 const& arg10, T12 const& arg11)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
      return callKernel(queue);
    }



    ocl::Event operator()(Queue const& queue, EventList const& el)
    {
      return callKernel(queue, el);
    }

    template< typename T >
    ocl::Event operator()(Queue const& queue, EventList const& el, T const& arg0)
    {
      pushArg(arg0);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1)
    {
      pushArg(arg0, arg1);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2)
    {
      pushArg(arg0, arg1, arg2);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3)
    {
      pushArg(arg0, arg1, arg2, arg3);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9, T11 const& arg10)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
      return callKernel(queue, el);
    }

    template< typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12 >
    ocl::Event operator()(Queue const& queue, EventList const& el, T1 const& arg0, T2 const& arg1, T3 const& arg2, T4 const& arg3, T5 const& arg4, T6 const& arg5, T7 const& arg6, T8 const& arg7, T9 const& arg8, T10 const& arg9, T11 const& arg10, T12 const& arg11)
    {
      pushArg(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11);
      return callKernel(queue, el);
    }
#endif

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
    
    Kernel& operator =( Kernel const& ) = delete;

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

