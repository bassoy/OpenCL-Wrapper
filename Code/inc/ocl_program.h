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

#ifndef OCL_PROGRAM_H
#define OCL_PROGRAM_H

// #include <map>
#include <memory>
#include <string>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <utl_type.h>

namespace ocl{

/*! \class CompileOption ocl_program.h "inc/ocl_program.h"
  *
  * \brief Wrapper for string of command options used for the build process of the Program.
  *
  * Single CompileOption objects are previously declared and can accessed by
  * entering the namespace ocl::compile_option::*option*. They can be also combined
  * to create multiple compile options. See the OpenCL reference
  * for valid compile options.
  *
  */
class CompileOption
{
public:
    CompileOption();
    CompileOption(const std::string&);
    CompileOption(const CompileOption&);
    CompileOption(CompileOption&&);
    CompileOption operator | (const CompileOption&) const;
    CompileOption operator | (const std::string&) const;
    CompileOption& operator |= ( CompileOption const& opt ) { return *this = *this | opt; }
    const std::string& operator()() const;
    CompileOption& operator=(const CompileOption &);

private:
    std::string _options;
};


/*! \namespace compile_option ocl_program.h "inc/ocl_program.h"
* \brief Encapsulates predefined valid CompileOption objects.
*
*/
namespace compile_option{
	extern CompileOption SINGLE_CONSTANT;
	extern CompileOption NO_DENORMALS;
	extern CompileOption RND_CORRECTLY_DIVIDE_SQRT;
	extern CompileOption DISABLE_OPT;
	extern CompileOption ENABLE_MAD;
	extern CompileOption NO_SIGNED_ZERO;
	extern CompileOption UNSAFE_MATH_OPT;
	extern CompileOption FINITE_MATH;
	extern CompileOption FAST_MATH;
}
}


namespace ocl{
class Kernel;
class Context;


/*! \class Program ocl_program.h "inc/ocl_program.h"
  *
  * \brief Wrapper for cl_program.
  *
  * A Program object is a collection of kernels
  * and auxiliary functions within a single Context. It is used
  * for building, compiling and linking of Kernel objects together.
  * A Program creates Kernel objects by either reading them from a stream or string.
  * Kernel objects owned by the Program are stored within a map and
  * accessed via their function name. While a Program object is only valid for one Context,
  * a Context might have multiple Program objects. In order to
  * access Kernel objects within functions one can define an active
  * Program for a Context.
  *
  * cl_program is released if the associated context is released.
  * The Program objects becomes than invalid.
  */

class Program
{
// 	typedef std::map<std::string, Kernel*> Kernels;
// 	typedef std::vector<Kernel*> Kernels;
  typedef std::vector< std::unique_ptr< Kernel > > Kernels;
	typedef Kernels::const_iterator const_iterator;
	typedef Kernels::iterator iterator;
public:
    Program();
	~Program();
	explicit Program(ocl::Context& ctxt, const utl::Types&, const CompileOption & o = CompileOption());
    explicit Program(ocl::Context& ctxt, const CompileOption & o = CompileOption());

    void build();
	cl_program id() const;
    Context& context() const;
	void setContext(Context&);
    void print(std::ostream & out = std::cout) const;

    Program& operator << (const std::string &kernels);
    Program& operator << (std::istream& stream);

    void setTypes(const utl::Types &);
    void setTypes(utl::Types &&);
    const utl::Types& types() const;
    void setCompileOption(const ocl::CompileOption & o);
	void deleteKernel(const std::string &kernel_name);
    void removeKernels();
	bool exists(const std::string &kernel_name) const;
	void release();
    bool isBuilt() const;
    Kernel& kernel(const std::string &name);
    Kernel& kernel(const std::string &name, const utl::Type &);

    template<class T>
    Kernel& kernel(const std::string &name);

    bool operator==(const Program &other) const;
    bool operator!=(const Program &other) const;

    Program& operator =( Program const& ) = delete;
    
    Program( Program const& ) = delete;
     
private:



	cl_program _id; /**< OpenCL program for a OpenCL context. */
    Context *_context;
	Kernels _kernels;  /**< Set of kernels which are created and ready for command queue insertion. */
    utl::Types _types;
    ocl::CompileOption _options;

	std::string nextKernel(const std::string &kernels, size_t pos);
	void eraseComments(std::string &file_string) const;
    void checkBuild(cl_int buildErr) const;
    
    /**
     * Code common to all kernels.
     * 
     * Constraint: commonCodeBlocks_.size() == _kernels.size() + 1
     */
    std::vector< std::string > commonCodeBlocks_;
    
    void checkConstraints() const;
};
}

#endif
