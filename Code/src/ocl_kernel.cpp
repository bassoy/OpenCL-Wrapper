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
#include <string>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <cmath>


#include <ocl_program.h>
#include <ocl_query.h>
#include <ocl_context.h>
#include <ocl_kernel.h>
#include <ocl_queue.h>
#include <ocl_event_list.h>

#include <utl_assert.h>
#include <utl_type.h>


//using namespace std;

/*! \brief Instantiates an empty Kernel object without a kernel function.*/
ocl::Kernel::Kernel() :
    _program(0),  _id(0), _workDim(1), _kernelfunc(), _name(), _memlocs()
{
}

/*! \brief Instantiates this Kernel given a Program and the kernel function within a string.
  *
  * Note that it is assumed that the Program is instantiating
  * this Kernel. Thus it is assumed that comments are erased and
  * that the Kernel is not templated any more. The Program
  * should not be built yet.
  */
ocl::Kernel::Kernel(const ocl::Program &p, const std::string &kernel) :
    _program(&p), _id(0), _workDim(1), _kernelfunc(), _name(), _memlocs()
{
    TRUE_ASSERT(!_program->isBuilt(), "Program is already built.");
    this->_kernelfunc = kernel;
    this->_name = this->extractName(kernel);
    this->_memlocs = this->extractMemlocs(kernel);
}

/*! \brief Instantiates this Kernel given the kernel function within a string.
  *
  * It is assumed that comments are erased and
  * that the Kernel is not templated any more.
  * Do not forget to specify a Program for this
  * Kernel and built it.
  */
ocl::Kernel::Kernel(const std::string &kernel) :
    _program(0), _id(0), _workDim(1), _kernelfunc(), _name(), _memlocs()
{
    this->_kernelfunc = kernel;
    this->_name = this->extractName(kernel);
    this->_memlocs = this->extractMemlocs(kernel);
}

/*! \brief Instantiates this Kernel given a Program, the kernel function within a string and a Type.
  *
  * Note that it is assumed that the Program is instantiating
  * this Kernel. Thus it is assumed that comments are erased.
  * The Program should not be built yet.
*/
ocl::Kernel::Kernel(const ocl::Program &p, const std::string &kernel, const utl::Type & type) :
    _program(&p), _id(0), _workDim(1), _kernelfunc(), _name(), _memlocs()
{

    if(this->templated(kernel))  this->_kernelfunc = this->specialize(kernel, type.name());
    else                         this->_kernelfunc = kernel;

    this->_name = this->extractName(_kernelfunc);
    this->_memlocs = this->extractMemlocs(_kernelfunc);

}

/*! \brief Instantiates this Kernel given a Program, the kernel function within a string and a Type.
  *
  * It is assumed that comments are erased and
  * that the Kernel is not templated any more.
  * Do not forget to specify a Program for this
  * Kernel and built it.
*/
ocl::Kernel::Kernel(const std::string &kernel, const utl::Type & type) :
    _program(0), _id(0), _workDim(1), _kernelfunc(), _name(), _memlocs()
{

    if(this->templated(kernel))  this->_kernelfunc = this->specialize(kernel, type.name());
    else                         this->_kernelfunc = kernel;

    this->_name = this->extractName(_kernelfunc);
    this->_memlocs = this->extractMemlocs(_kernelfunc);

}


/*! \brief Destructs this Kernel. */
ocl::Kernel::~Kernel()
{

    release();
}

/*! \brief Creates this Kernel for execution.
  *
  * Note that a valid Program must be specified
  * for this Kernel so that this Kernel can
  * be create. Note also that usually the Program
  * in which this Kernel is residing calls this function.
  *
*/
void ocl::Kernel::create()
{
    if(this->created()) return;
    TRUE_ASSERT(this->_program  != 0, "program == 0");
    TRUE_ASSERT(this->program().isBuilt(), "Program not yet built");
    const char * __t = this->name().c_str();
    
#if 0
    std::cout << "Looking for kernel with name: " << __t << std::endl;
#endif
    
    cl_int err;
    _id = clCreateKernel(this->_program->id(), __t, &err);
	OPENCL_SAFE_CALL(err);
    TRUE_ASSERT(_id != 0, "id == 0");
}

/*! \brief Returns true if this Kernel is created and ready for execution. */
bool ocl::Kernel::created() const
{
    return _id != 0;
}

/*! \brief Releases this Kernel. */
void ocl::Kernel::release()
{
    if(created()){
        OPENCL_SAFE_CALL(clReleaseKernel(_id));
    }
	_globalSize[0] = 1; _globalSize[1] = 1; _globalSize[2] = 1;
	_localSize[0] = 1; _localSize[1] = 1; _localSize[2] = 1;
	_id = 0;
	_workDim = 1;
}


/*! \brief Returns the OpenCL kernel ID of this Kernel. */
cl_kernel ocl::Kernel::id() const
{
	return _id;
}


/*! \brief Returns Program of this Kernel. */
const ocl::Program& ocl::Kernel::program() const
{
    TRUE_ASSERT(_program != NULL, "Program not valid.");
    return *_program;
}

/*! \brief Set Program of this Kernel. */
void ocl::Kernel::setProgram(const ocl::Program &program)
{
    TRUE_ASSERT(_program == NULL, "There should not be a program.");
    _program = &program;
}


/*! \brief Returns the Context for this Kernel. */
ocl::Context& ocl::Kernel::context() const
{
    TRUE_ASSERT(_program != NULL, "Program not valid.");
    return this->_program->context();
}


/*! \brief Executes this Kernel and returns an Event by which the execution can be tracked.
  *
  * Executes this Kernel on the active Queue
  * of the Context given by the associated
  * Program.
*/
ocl::Event ocl::Kernel::callKernel(const Queue& queue, const EventList& list)
{
    TRUE_ASSERT(queue.context() == this->context(), "Context must be equal.");
    cl_event event_id;

    OPENCL_SAFE_CALL( clEnqueueNDRangeKernel(queue.id(), this->id(), this->workDim(), 0, this->globalSize(), this->localSize(), list.size(), list.events().data(), &event_id) );

    return ocl::Event(event_id, &this->context());
}

/*! \brief Executes this Kernel and returns an Event by which the execution can be tracked.
  *
  * Executes this Kernel on the active Queue
  * of the Context given by the associated
  * Program.
*/
ocl::Event ocl::Kernel::callKernel(const Queue& queue)
{
    TRUE_ASSERT(queue.context() == this->context(), "Context must be equal.");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueNDRangeKernel(queue.id(), this->id(), this->workDim(), 0, this->globalSize(), this->localSize(), 0, NULL, &event_id) );
    return ocl::Event(event_id, &this->context());
}

/*! \brief Executes this Kernel and returns an Event by which the execution can be tracked.
  *
  * Executes this Kernel on the active Queue
  * of the Context given by the associated
  * Program.
*/
ocl::Event ocl::Kernel::callKernel()
{

    cl_event event_id;

    const ocl::Queue &queue = this->program().context().activeQueue();

    OPENCL_SAFE_CALL( clEnqueueNDRangeKernel(queue.id(), this->id(), this->workDim(), 0, this->globalSize(), this->localSize(), 0, NULL, &event_id) );
    return ocl::Event(event_id, &this->context());
}

#if 0
/*! \brief Executes this Kernel and returns an Event by which the execution can be tracked.
  *
  * Executes this Kernel on the active Queue
  * of the Context given by the associated
  * Program.
*/
ocl::Event ocl::Kernel::operator ()()
{
    return this->callKernel();
}
#endif

/*! \brief Sets the working dimension of the index space.
  *
  * Note that the working dimension cannot be larger than three.
*/
void ocl::Kernel::setWorkDim(size_t dim)
{
	_workDim = dim;
}

/*! \brief Returns the number of working dimension of the index space.
  *
  * Note that the working dimension cannot be larger than three.
*/
size_t ocl::Kernel::workDim() const
{
	return _workDim ;
}

/*! \brief Returns the localSizes for the index space. */
const size_t* ocl::Kernel::localSize() const
{
	return _localSize;
}

/*! \brief Returns the globalSizes for the index space. */
const size_t* ocl::Kernel::globalSize() const
{
	return _globalSize;
}

/*! \brief Returns the localSize of the index space for the specified dimension.*/
size_t ocl::Kernel::localSize(size_t pos) const
{
	TRUE_ASSERT(pos < 3, "Cannot have more than three dims : " << pos);
	return _localSize[pos];
}

/*! \brief Returns the globalSize of the index space for the specified dimension.*/
size_t ocl::Kernel::globalSize(size_t pos) const
{
	TRUE_ASSERT(pos < 3, "Cannot have more than three dims : " << pos);
	return _globalSize[pos];
}

/*! \brief Sets the local size for all three dimensions.
  *
  * Note that the array provided must be equal three.
  *
  * By the specification of the localSizes for each three
  * dimension, the number of threads within a single
  * working group is determined.
  *
  * \param localSize Pointer to an array specifying the llobalSizes for each three dimensions of a working group.
*/
void ocl::Kernel::setLocalSize(size_t *localSize)
{
	for(size_t i = 0; i < 3; ++i)
		_localSize[i] = localSize[i];
}

/*! \brief Sets the local size for the specified dimension.
  *
  * Note that position can not be larger than three, as
  * the index space cannot have more than three dimensions.
  *
  * Note that localSize must be smaller or equal than
  * the globalSize.
  *
  * By the specification of the localSizes for each three
  * dimension, the number of threads within a single
  * working group is determined.
  *
  * \param localSize Size within the index space at dimension pos of a working group.
  * \param pos Specifies the dimension of the index space for which the localSize is set.
*/
void ocl::Kernel::setLocalSize(size_t localSize, size_t pos)
{
	TRUE_ASSERT(pos < 3, "Cannot have more than three dims : " << pos);
	_localSize[pos] = localSize;
}

/*! \brief Sets the global size for all three dimensions.
  *
  * Note that the array provided must be equal three.
  *
  * By the specification of the globalSizes for each three
  * dimension, the number of threads for the kernels are
  * determined. Note that the globalSize at dimension pos
  * must be a multiple of the localSize at the same dimension.
  * The round-up is done in this function automatically.
  *
  * \param globalSize Pointer to an array specifying the globalSizes for each three dimensions of the index space.
*/
void ocl::Kernel::setGlobalSize(size_t *globalSize)
{
	for(size_t i = 0; i < 3; ++i)
		setGlobalSize(globalSize[i], i);
}

/*! \brief Sets the global size for the specified dimension.
  *
  * Note that position can not be larger than three, as
  * the index space cannot have more than three dimensions.
  *
  * By the specification of the globalSizes for each three
  * dimension, the number of threads for the kernels are
  * determined. Note that the globalSize at dimension pos
  * must be a multiple of the localSize at the same dimension.
  * The round-up is done in this function automatically.
  *
  * \param globalSize Size of the index space at dimension pos.
  * \param pos Specifies the dimension of the index space for which the globalSize is set.
*/
void ocl::Kernel::setGlobalSize(size_t globalSize, size_t pos)
{
	// round up
	_globalSize[pos] = (size_t)(ceil((double)globalSize / (double)_localSize[pos]) * (double)_localSize[pos]) ;
}

/*! \brief Sets a 1D Working Size for this Kernel.
  *
  * First parameter is the local size (x) of the 1D-index space.
  * Second parameter is the global size (x) of the 1D-index space.
*/
void ocl::Kernel::setWorkSize(size_t lSizeX, size_t gSizeX)
{
	setLocalSize(lSizeX,0);
	setGlobalSize(gSizeX,0);
	setWorkDim(1);
}

/*! \brief Sets a 2D Working Size for this Kernel.
  *
  * First two parameters are the local sizes (x,y) of the 2D-index space.
  * Last two parameters are the global sizes (x,y) of the 2D-index space.
*/
void ocl::Kernel::setWorkSize(size_t lSizeX, size_t lSizeY, size_t gSizeX, size_t gSizeY)
{
	setLocalSize(lSizeX,0);
	setLocalSize(lSizeY,1);
	setGlobalSize(gSizeX,0);
	setGlobalSize(gSizeY,1);
	setWorkDim(2);
}

/*! \brief Sets a 3D Working Size for this Kernel.
  *
  * First three parameters are the local sizes (x,y,z) of the 3D-index space.
  * Last three parameters are the global sizes (x,y,z) of the 3D-index space.
*/
void ocl::Kernel::setWorkSize(size_t lSizeX, size_t lSizeY, size_t lSizeZ, size_t gSizeX, size_t gSizeY, size_t gSizeZ)
{
	setLocalSize(lSizeX,0);
	setLocalSize(lSizeY,1);
	setLocalSize(lSizeZ,2);
	setGlobalSize(gSizeX,0);
	setGlobalSize(gSizeY,1);
	setGlobalSize(gSizeZ,2);
	setWorkDim(3);
}

/*! \brief Sets the OpenCL memory object into the argument list of this Kernel at the specified position.
  *
  * Note that the memory location is automatically extracted. This
  * function is called when this Kernel is excuted.
*/
void ocl::Kernel::setArg(int pos, cl_mem data)
{
    TRUE_ASSERT(this->numberOfArgs() > size_t(pos), "Position " << pos << " >= " << this->_memlocs.size());
    //TRUE_ASSERT(this->memoryLocation(pos) == global, "Argument must be of type GLOBAL at pos " << pos);
	cl_int stat = clSetKernelArg(_id, pos, sizeof(cl_mem), &data);
	if(stat != CL_SUCCESS) cerr << "Error setting kernel "<< this->name() << " argument " << pos << endl;
	OPENCL_SAFE_CALL( stat );
}

/*! \brief Sets the OpenCL sampler object into the argument list of this Kernel at the specified position.
  *
*/
void ocl::Kernel::setArg(int pos, cl_sampler data)
{
	TRUE_ASSERT(this->numberOfArgs() > size_t(pos), "Position " << pos << " >= " << this->_memlocs.size());
	cl_int stat = clSetKernelArg(_id, pos, sizeof(cl_sampler), &data);
	if(stat != CL_SUCCESS) cerr << "Error setting kernel "<< this->name() << " argument " << pos << endl;
	OPENCL_SAFE_CALL( stat );
}

/*! \brief Sets a scalar datum into the argument list of this Kernel at the specified position.
  *
  * This function is called when this Kernel is excuted.
*/
template<class T>
void ocl::Kernel::setArg(int pos, const T& data)
{
  TRUE_ASSERT(this->numberOfArgs() > size_t(pos), "Position " << pos << " >= " << this->_memlocs.size());

	cl_int stat ;

  if(this->memoryLocation(pos) == host)
  {
		stat = clSetKernelArg(_id, pos, sizeof(T), (void*)&data);
	}
  else if(this->memoryLocation(pos) == local)
	{
    TRUE_ASSERT(typeid(data) == typeid(size_t), "data: "<< data << " at pos " << pos << " must be of type size_t");

    // TODO This is a hack and should be better implemented using compile time type erasure.
		stat = clSetKernelArg(_id, pos, static_cast<size_t>( data ), NULL);
	}
	else
  {
    TRUE_ASSERT(0,"Location Unkown - Error setting kernel "<< name() << " at pos = " << pos << ", arg = " << data);
	}
    
  if(stat != CL_SUCCESS) cerr << "Error setting kernel "<< name() << " at pos = " << pos << ", arg = " << data << endl;
	
  OPENCL_SAFE_CALL( stat );
}

template void ocl::Kernel::setArg<int>   (int pos, const int& data);
template void ocl::Kernel::setArg<size_t>(int pos, const size_t& data);
template void ocl::Kernel::setArg<float> (int pos, const float& data);
template void ocl::Kernel::setArg<double>(int pos, const double& data);
template void ocl::Kernel::setArg<char>  (int pos, const char& data);
template void ocl::Kernel::setArg<unsigned int>( int pos, unsigned int const& data );


/*! \brief Returns the kernel function of this Kernel. */
const std::string& ocl::Kernel::toString() const
{
    return this->_kernelfunc;
}

/*! \brief Returns the kernel function name of this Kernel. */
const std::string& ocl::Kernel::name() const
{
	return this->_name;
}

/*! \brief Returns the number of arguments of this Kernel. */
size_t ocl::Kernel::numberOfArgs() const
{
	return this->_memlocs.size();
}

/*! \brief Returns the memory location of this Kernel at the specified position. */
ocl::Kernel::mem_loc ocl::Kernel::memoryLocation(size_t pos) const
{
    return this->_memlocs.at(pos);
}

bool ocl::Kernel::templated(const std::string &kernel)
{
    return !ocl::Kernel::extractParameter(kernel).empty();
}

std::vector<ocl::Kernel::mem_loc> ocl::Kernel::extractMemlocs(const std::string & kernel)
{
    const auto afterAttributes = kernel.find("void");

    const size_t start = kernel.find("(",afterAttributes) + 1;
    const size_t end   = kernel.find(")",start) - 1;
	//TRUE_ASSERT(start < end, "Function not correctly defined.");
	TRUE_ASSERT((start-1) < (end+1), "Function not correctly defined.");

    size_t pos_before = start;
    size_t pos_after = start;

    vector<mem_loc> locs;

    while(pos_after <= end)
    {
        pos_after = kernel.find(",", pos_before) - 1;
        if(pos_after > end) pos_after = kernel.find(")", pos_before) - 1;
        if(pos_after > end) return locs;

        const string& argument = kernel.substr(pos_before, pos_after - pos_before + 1);

        if( argument.find("global") != argument.npos || argument.find("__global") != argument.npos )
            locs.push_back(global);
        else if( argument.find("local") != argument.npos || argument.find("__local") != argument.npos)
            locs.push_back(local);
        else if( argument.find("image") != argument.npos)
            locs.push_back(image);
        else if( argument.find("sampler") != argument.npos)
            locs.push_back(sampler);
        else
            locs.push_back(host);
        pos_before = pos_after+2;
    }
    return locs;
}

std::string ocl::Kernel::extractParameter(const std::string& kernel)
{
    DEBUG_ASSERT(!kernel.empty(), "Function empty");
    size_t start, end;

    start = kernel.find("class");
    if(start == kernel.npos) return "";
    end = kernel.find(">");
    TRUE_ASSERT(start < kernel.length(), "Template not correctly defined.");
    TRUE_ASSERT(end < kernel.length(), "Template not correctly defined.");
    TRUE_ASSERT(start < end, "Template not correctly defined.");
    start += 5;
    end--;

    const string &substr = kernel.substr(start, end - start + 1);

    start = substr.find_first_not_of(" ");
    end = substr.find_last_not_of(" ");	

    //if(start == end) return "";
    return substr.substr(start, end - start + 1);
}

std::string ocl::Kernel::extractName(const string &kernel)
{
    size_t pos_void    = kernel.find("void")+4;
    size_t pos_bracket = kernel.find("(", pos_void)-1;
    TRUE_ASSERT(pos_bracket > pos_void, "Could not find function name.");
    string uname = kernel.substr(pos_void, pos_bracket - pos_void + 1);

//	DEBUG_COMMENT(uname);

    size_t start = uname.find_first_not_of(" ");
    size_t end = uname.find_last_not_of(" ");

//    DEBUG_COMMENT("start=" << start << "; end=" << end);

    TRUE_ASSERT(start < end, "Could not find function name.");

    return uname.substr(start, end - start + 1);
}


std::string ocl::Kernel::specialize(const std::string& kernel, const std::string& type)//const utl::Type &type)
{

    const string temp_param = ocl::Kernel::extractParameter(kernel);

    if(temp_param.empty()) return kernel;

    string fct = kernel;

    //    DEBUG_COMMENT("fct = " << fct);


    size_t start = 0, end = 0;

    start = fct.find("template", start);
    end   = fct.find(">",end,1);
    TRUE_ASSERT(start < end, "Template not correctly defined.");

    fct.erase(start, end - start + 2);

// #if defined OPENCL_V1_1 || defined OPENCL_V1_0
//if(type == "double") fct.insert(0, "#if !defined(__OPENCL_VERSION__) || __OPENCL_VERSION__ < 120\n#pragma OPENCL EXTENSION cl_khr_fp64: enable\n#endif\n");
// #endif


    size_t pos = 0;
    
    pos = fct.find("void", pos);

    pos = fct.find("(", pos, 1);
    TRUE_ASSERT(pos < fct.length(), "Could not find the fct.");
    fct.insert(pos++,"_",1);
    //fct.insert(pos,type.name());
    fct.insert(pos,type);
    //pos += type.name().length();
    pos += type.length();

    const string s1 = temp_param + " ";
    const string s2 = temp_param + "*";
    const string s3 = temp_param + "&";
    const string s4 = temp_param + ")";

    while(pos < fct.length()){
        int pos1 = fct.find(s1, pos); if(pos1 == (int)string::npos) pos1 = fct.length();
        int pos2 = fct.find(s2, pos); if(pos2 == (int)string::npos) pos2 = fct.length();
        int pos3 = fct.find(s3, pos); if(pos3 == (int)string::npos) pos3 = fct.length();
        int pos4 = fct.find(s4, pos); if(pos4 == (int)string::npos) pos4 = fct.length();

        pos = std::min(std::min(std::min(pos1,pos2),pos3),pos4);
        if(pos >= fct.length()) break;
        fct.replace(pos, temp_param.length() , type);//type.name());

        //pos += type.name().length();
        pos += type.length();
    }

	return fct;
}


