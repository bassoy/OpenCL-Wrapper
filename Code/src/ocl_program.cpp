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

// System includes

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#include <ocl_query.h>
#include <ocl_program.h>
#include <ocl_context.h>
#include <ocl_kernel.h>
#include <ocl_device.h>
#include <ocl_platform.h>


#include <utl_assert.h>


/*! \brief Instantiates this empty CompileOption. */
ocl::CompileOption::CompileOption() :
    _options()
{
}

/*! \brief Instantiates this CompileOption with the specified string.
  *
  * Note that the string must be a valid OpenCL compile option.
  * The validity of the option is not checked here.
*/
ocl::CompileOption::CompileOption(const std::string& s) :
    _options(s)
{
}

/*! \brief Instantiates this CompileOption from another CompileOption. */
ocl::CompileOption::CompileOption(const ocl::CompileOption& c) :
    _options(c._options)
{
}

/*! \brief Instantiates this CompileOption from another CompileOption. */
ocl::CompileOption::CompileOption(ocl::CompileOption&& c) :
    _options(std::move(c._options))
{
}

/*! \brief Instantiating a new CompileOption from this CompileOption and another CompileOption.
  *
  * The strings of CompileOption objects are concatinated.
*/
ocl::CompileOption ocl::CompileOption::operator | (const CompileOption &other)
{
    return ocl::CompileOption(this->_options + " " + other._options);
}

/*! \brief Instantiating a new CompileOption from this CompileOption and another string.
  *
  * The strings are concatinated.
*/
ocl::CompileOption ocl::CompileOption::operator | (const std::string& other)
{
    //return ocl::CompileOption(this->_options + ", " + other);
  return *this | ocl::CompileOption( other );
}

/*! \brief Returns the string of the CompileOption. */
const std::string& ocl::CompileOption::operator ()() const
{
    return this->_options;
}

ocl::CompileOption& ocl::CompileOption::operator=(const CompileOption &other) {
    _options = other._options;
    return *this;
}

namespace ocl {
namespace compile_option{
    ocl::CompileOption SINGLE_CONSTANT("-cl-single-precision-constant");
    ocl::CompileOption NO_DENORMALS("-cl-denorms-are-zero");
    ocl::CompileOption RND_CORRECTLY_DIVIDE_SQRT("-cl-fp32-correctly-rounded-divide-sqrt");
    ocl::CompileOption DISABLE_OPT("-cl-opt-disable");
    ocl::CompileOption ENABLE_MAD("-cl-mad-enable");
    ocl::CompileOption NO_SIGNED_ZERO("-cl-no-signed-zeros");
    ocl::CompileOption UNSAFE_MATH_OPT("-cl-unsafe-math-optimizations");
    ocl::CompileOption FINITE_MATH("-cl-finite-math-only");
    ocl::CompileOption FAST_MATH("-cl-fast-relaxed-math");
}
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


/*! \brief Instantiates this Program for a given Context, predefined Types and CompileOption.
	*
    * This Program is not yet created, only initialized with the given Context, Types and CompileOptions.
    * In order to create it, load Kernel objects into this Program and call the appropriate create function.
    * Templated Kernel functions are then build for the given Types with the CompileOption.
    * The Kernel function name will be changed to kernel_<type>.
    *
    * \param context The Context for which this Program will be created.
    * \param types Types which consist of valid Type objects.
    * \param options defines a valid CompileOption for build process.
*/
ocl::Program::Program(ocl::Context& ctxt, const utl::Types &types, const ocl::CompileOption &options) :
    _id(NULL), _context(&ctxt), _types(types), _options(options)
{
    TRUE_ASSERT(!_types.empty(), "no types selected.");
    _context->insert(this);
}

/*! \brief Instantiates this Program for a given Context and CompileOption.
    *
    * This Program is not yet created, only initialized with the given Context and CompileOptions.
    * In order to create it, load Kernel objects into this Program and call the appropriate create function.
    * It is assumed that the Kernel functions are not templated. Thus no Types must be provided.
    *
    * \param context The Context for which this Program will be created.
    * \param options defines a valid CompileOption for build process.
*/
ocl::Program::Program(ocl::Context& ctxt, const ocl::CompileOption &options) :
    _id(NULL), _context(&ctxt), _types(), _options(options)
{
    _context->insert(this);
}

/*! \brief Instantiates this Program for a given Context and CompileOption.
    *
    * This Program is not yet created or initialized with the necessary Context.
    * Do not forget to provide at least a valid Context, Kernel objects or
    * functions and to build it.
*/
ocl::Program::Program() :
    _id(NULL), _context(), _types(), _options()
{
}


/*! \brief Destructs this Program.*/
ocl::Program::~Program()
{

	if(_context != nullptr)
		_context->release(this);

   removeKernels();
   this->release();
   _context = 0;
	_id = 0;
}

/*! \brief Releases this Program.
  *
  * Removes and destroys also all Kernel objects.
*/
void ocl::Program::release()
{
   if(this->isBuilt())
        OPENCL_SAFE_CALL( clReleaseProgram (_id));
    _id = 0;

//	if(_context)
//        _context->release(this);

//   if(this->isBuilt()){
//        removeKernels();
//        OPENCL_SAFE_CALL( clReleaseProgram (_id));

//    }
//    _context = 0;
//    _id = 0;
}


/*! \brief Removes all kernels created with this Program.
  *
  * Kernel objects are also deleted and not accessible any more from elsewhere.
*/
void ocl::Program::removeKernels()
{
    while(!_kernels.empty()){
        delete  _kernels.begin()->second;
        _kernels.erase( _kernels.begin());
    }
}

/*! \brief Sets the Types for the Kernel objects.
  *
  * Note that this Progam should not be built.
*/
void ocl::Program::setTypes(const utl::Types& types)
{
    TRUE_ASSERT(!types.empty(), "Types should not be empty");
    TRUE_ASSERT(!this->isBuilt(), "Program already built.");
    _types = types;

}

/*! \brief Sets the Types for the Kernel objects.
  *
  * Note that this Progam should not be built.
*/
void ocl::Program::setTypes(utl::Types&& types)
{
    TRUE_ASSERT(!types.empty(), "Types should not be empty");
    TRUE_ASSERT(!this->isBuilt(), "Program already built.");
    _types = std::move(types);
}

/*! \brief Returns the Types of the Kernel objects.
  *
*/
const utl::Types& ocl::Program::types() const
{
    return this->_types;
}

/*! \brief Sets the CompileOption for this Program.
  *
  * Note that this should not be built.
*/
void ocl::Program::setCompileOption(const ocl::CompileOption & o)
{
    TRUE_ASSERT(!this->isBuilt(), "Program already built.");
    _options = o;
}



/*! \brief Builds this Program.
	*
    * Do not forget to load Kernel objects into this
    * Program before executing this function.
    * This Program with all Kernel objects are built. Note that
    * compiling and linking in seperate stages are note supported
    * yet. Kernels built with this Program
    * can be executed on all Device objects within the Context
    * for which this Program is built.
*/
void ocl::Program::build()
{
    TRUE_ASSERT(this->_context != 0, "Program has no Context");
    TRUE_ASSERT(this->_id == 0, "Program already built");

    TRUE_ASSERT(!_kernels.empty(), "No kernels loaded for the program");
    std::stringstream stream;

    this->print(stream);
    const std::string &t = stream.str();

    cl_int status;
    const char * file_char = t.c_str(); // stream.str().c_str();
    _id = clCreateProgramWithSource(this->context().id(), 1, (const char**)&file_char,   NULL, &status);
    OPENCL_SAFE_CALL(status);
    cl_int buildErr = clBuildProgram(_id, 0, NULL, _options().c_str(), NULL, NULL);
    checkBuild(buildErr);

    for(auto k : _kernels){
        k.second->create();
    }
}


/*! \brief Returns the OpenCL ID of this Program. */
cl_program ocl::Program::id() const
{
	return _id;
}

/*! \brief Returns the Context of this Program. */
ocl::Context& ocl::Program::context() const
{
    TRUE_ASSERT(this->_context != 0, "Context not valid.");
    return *this->_context;
}

/*! \brief Set the Context of this Program.
  *
  * Note that you cannot change the context
  * once this Program has been built.
*/
void ocl::Program::setContext(ocl::Context &c)
{
    TRUE_ASSERT(!this->isBuilt(), "Context already built");
    _context = &c;
    _context->insert(this);

}

/*! \brief Return true if this Program is built. */
bool ocl::Program::isBuilt() const
{
	return _id != NULL;
}

/*! \brief Prints the Kernel functions of this Program. */
void ocl::Program::print(std::ostream& out) const
{
    for(auto k : _kernels)
    {
        const ocl::Kernel &kernel = *(k.second);
        out << kernel.toString() << std::endl;
    }
    out << std::endl;
}

/*! \brief Reads kernel functions from a string into this Program.
  *
  * The string object can contain multiple OpenCL kernel
  * functions. It deletes all comments and if the kernels
  * are templated, it substitutes the template parameter
  * with the provided types. For each Kernel function
  * a Kernel object is built and stored within a map.
  * The map stores the name of the kernel function and
  * the corresponding function.
  * Note that DEFINES are not supported yet.
*/
ocl::Program& ocl::Program::operator << (const std::string &k)
{
    std::string kernels = k;
    eraseComments(kernels);

    //DEBUG_COMMENT(kernels);

    size_t pos = 0;
    while(pos < kernels.npos){
        const std::string& next = nextKernel(kernels, pos);
        if(next.empty()) return *this;
        pos += next.length();

        if(_types.empty() || !ocl::Kernel::templated(next)){
            ocl::Kernel *kernel = new ocl::Kernel(*this, next);
            if(this->isBuilt()) kernel->create();
            //DEBUG_COMMENT("Creating kernel " << kernel->name() << std::endl << kernel->toString() );
            _kernels[kernel->name()] = kernel;
            continue;
        }

        for(utl::Types::const_iterator it = _types.begin(); it != _types.end(); ++it)
        {
            const utl::Type &type = **it;
            ocl::Kernel *kernel = new ocl::Kernel(*this, next, type);
            if(this->isBuilt()) kernel->create();
            //DEBUG_COMMENT("Creating kernel " << kernel->name() << std::endl << kernel->toString() );
            _kernels[kernel->name()] = kernel;
        }
    }
    return *this;
}

/*! \brief Reads kernel functions from an input stream into this Program.
  *
  * The input stream can contain multiple OpenCL kernel
  * functions. It deletes all comments and if the kernels
  * are templated, it substitutes the template parameter
  * with the provided types. For each Kernel function
  * a Kernel object is built and stored within a map.
  * The map stores the name of the kernel function and
  * the corresponding function.
  * Note that DEFINES are not supported yet.
*/
ocl::Program& ocl::Program::operator << (std::istream& stream)
{
	TRUE_ASSERT(!stream.fail(), "Error while opening file.");

    std::stringstream buffer;

    stream >> buffer.rdbuf();

	return (*this) << buffer.str();
}

/*! \brief Returns true if this and the specified Program have the same OpenCL program ID.*/
bool ocl::Program::operator ==(const Program& other) const
{
    return this->id() == other.id();
}

/*! \brief Returns true if this and the specified Program do not have the same OpenCL program ID.*/
bool ocl::Program::operator !=(const Program& other) const
{
    return this->id() != other.id();
}


/*! \brief Returns the Kernel from this Program by providing the Kernel's function name.*/
ocl::Kernel& ocl::Program::kernel(const std::string &name) const
{
    const_iterator it = _kernels.find(name);
    TRUE_ASSERT(it != _kernels.end(), "Kernel " << name << " does not exist yet");
	return *(it->second);
}

/*! \brief Returns the Kernel from this Program by providing the Kernel's function name and its Type.*/
template<class T>
ocl::Kernel& ocl::Program::kernel(const std::string &name) const
{
    const utl::Type& t = utl::Type::type<T>();
    return kernel(name, t);
}

template ocl::Kernel& ocl::Program::kernel<char>(const std::string &name) const;
template ocl::Kernel& ocl::Program::kernel<int>(const std::string &name) const;
template ocl::Kernel& ocl::Program::kernel<size_t>(const std::string &name) const;
template ocl::Kernel& ocl::Program::kernel<double>(const std::string &name) const;
template ocl::Kernel& ocl::Program::kernel<float>(const std::string &name) const;


/*! \brief Returns the Kernel from this Program by providing the Kernel's function name and its Type.*/
ocl::Kernel& ocl::Program::kernel(const std::string &name, const utl::Type &t) const
{
    TRUE_ASSERT(_types.contains(t), "Type "<< t.name() <<" not found.");
    std::string n = name; n+= "_"; n+= t.name();
	return this->kernel(n);
}

/*! \brief Returns true if the Kernel specified by its function name exist.*/
bool ocl::Program::exists(const std::string &name) const
{
    return _kernels.find(name) != _kernels.end();
}

/*! \brief Destroys the Kernel specified by its function name. */
void ocl::Program::deleteKernel(const std::string &name)
{
    iterator it = _kernels.find(name);
    TRUE_ASSERT(it != _kernels.end(), "Kernel " << name << " does not exist yet");
	const Kernel *__k = it->second;
	delete __k;
	_kernels.erase(it);
}



/*! \brief Returns the next available kernel function from the string.
  *
  * Note that this is a helper function and that
  * you do not have to call this function.
*/
std::string ocl::Program::nextKernel(const std::string &kernels, size_t pos)
{
    if(pos == kernels.npos) return "";



    size_t start_template = kernels.find("template", pos);
    size_t start_non_template = kernels.find("__kernel",pos);
    size_t start;

    bool template_found = false, non_template_found = false;
    if(start_template < start_non_template){
        template_found = true;
        start = start_template;
    }
    else{
        non_template_found = true;
        start = start_non_template;
    }

    if(start == kernels.npos) return "";

//    DEBUG_COMMENT("start_template = " << start << " pos = " << pos);

    if(template_found){
        size_t step = kernels.find("__kernel",start);
        size_t end1 = kernels.find("template",step+1);
        size_t end2 = kernels.find("__kernel",step+1);
        size_t end = std::min(end1, end2);
        return kernels.substr(start, end - start);
    }


//    DEBUG_COMMENT("start_kernel = " << start << " pos = " << pos << ", npos " << kernels.npos);
    if(non_template_found){
        size_t end1 = kernels.find("template",start + 8);
        size_t end2 = kernels.find("__kernel",start + 8);
        size_t end = std::min(end1, end2);
        return kernels.substr(start, end - start);
    }
    return "";

}


/*! \brief Erases comments within the string object containing kernel function.
  *
  * Note that this is a helper function and that
  * you do not have to call this function.
*/
void ocl::Program::eraseComments(std::string &kernels) const
{
	size_t end_pos = 0, pos = 0;
    while(pos < kernels.length()){
            pos = kernels.find("/*", pos,2);
            end_pos = kernels.find("*/", pos,2);
            if(pos >= kernels.length()) break;
            if(end_pos >= kernels.length()) break;
            TRUE_ASSERT(pos < end_pos, pos << " >= " << end_pos);
//		cout << "Erasing substring : " << kernels.substr(start_pos, end_pos-start_pos+2) <<  "-ENDEND" << endl;
            kernels.erase(pos, end_pos-pos+2);
            pos += 2;
	}
        pos = 0;
        while(pos < kernels.length()){
            pos = kernels.find("//", pos,2);
            end_pos = kernels.find("\n", pos); std::string s("\n");
            if(pos >= kernels.length()) break;
            if(end_pos >= kernels.length()) break;
            TRUE_ASSERT(pos < end_pos, pos << " >= " << end_pos);
            //		cout << "Erasing substring : " << kernels.substr(start_pos, end_pos-start_pos) <<  "-ENDEND" << endl;
            kernels.erase(pos, end_pos-pos);
            pos++;
	}
}




/*! \brief Checks whether the build process was successfull or not.*/
void ocl::Program::checkBuild(cl_int buildErr) const
{
	if(buildErr == CL_SUCCESS) return;
    std::cerr << "Program failed to build." << std::endl;
	cl_build_status buildStatus;
    std::string buildLog;
	size_t size = 0;
    for(auto device : _context->devices()){
        clGetProgramBuildInfo(_id, device.id(), CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &buildStatus, NULL);
		if(buildStatus == CL_SUCCESS) continue;

		clGetProgramBuildInfo(_id, device.id(), CL_PROGRAM_BUILD_LOG,  0, NULL, &size);
		buildLog.assign((unsigned int)size, 0);

		clGetProgramBuildInfo(_id, device.id(), CL_PROGRAM_BUILD_LOG,  size, &buildLog[0], NULL);
        std::cerr << "Device " << device.name() << " Build Log:" << std::endl << buildLog << std::endl;
	}
	exit(-1);
}

