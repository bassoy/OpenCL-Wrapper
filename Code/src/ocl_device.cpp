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

#include <ocl_device.h>
#include <ocl_query.h>
#include <ocl_queue.h>
#include <ocl_platform.h>

#include <utl_assert.h>

#include <algorithm>


/*! \brief Instantiates this Device.
  *
  * Usually the devices are created using the Platform class.
  * \param dev is a OpenCL device which is identified with cl_device_id.
  */
ocl::Device::Device(cl_device_id dev) :
    _id(dev), _type(ocl::device_type::ALL)
{
	cl_device_type t;
	OPENCL_SAFE_CALL( clGetDeviceInfo (_id,CL_DEVICE_TYPE, sizeof(t), &t, NULL) );
    _type = ocl::DeviceType::type(t);

}

/*! \brief Instantiates this Device.
  *
  * No OpenCL device specified. Must do this later.
  */
ocl::Device::Device() :
    _id(0), _type(ocl::device_type::ALL)
{

}

/*! \brief Destructs this Device.
  *
  */
ocl::Device::~Device()
{

}


/*! \brief Instantiates this Device using anther device.
  *
  * Note that both Device instance dev and this refer to
  * the same OpenCL device cl_device_id.
  * \param dev Device from which this Device is created.
  */
ocl::Device::Device(const Device& dev) :
    _id(dev._id), _type(dev._type)
{
}

/*! \brief Copies from other device to this device
  *
  * Note that both Device instance dev and this refer to
  * the same OpenCL device cl_device_id.
  * \param dev Device from which this Device is created.
  */
ocl::Device& ocl::Device::operator =(const ocl::Device &dev)
{
    _id = dev._id;
    _type = dev._type;
    return *this;
}


/*! \brief Sets the id of this Device
  *
  * Note that you have to change the type if
  * the cl_device_id does not refer to the same type.
  * \param id new cl_device_id to be set.
  */
void ocl::Device::setId(cl_device_id id)
{
    this->_id = id;
}

/*! \brief Returns the id of this Device
  *
  */
cl_device_id ocl::Device::id() const
{
	return this->_id;
}

/*! \brief Returns the type of this Device
  *
  */
const ocl::DeviceType& ocl::Device::type() const
{
	return this->_type;
}


/*! \brief Returns true if the ids are the same.
  *
  */
bool ocl::Device::operator == (const Device & dev) const
{
	return dev.id() == this->id();
}

/*! \brief Returns false if the ids are the same.
  *
  */
bool ocl::Device::operator != (const Device & dev) const
{
	return dev.id() != this->id();
}

/*! \brief Returns true if the ids are the same.
  *
  */
bool ocl::Device::operator == (cl_device_id  dev) const
{
	return dev == this->id();
}

/*! \brief Returns false if the ids are the same.
  *
  */
bool ocl::Device::operator != (cl_device_id  dev) const
{
	return dev != this->id();
}

/*! \brief Returns true if the types are the same.
  *
  */
bool ocl::Device::operator == (const ocl::DeviceType&  type) const
{
	return type == this->type();
}

/*! \brief Returns false if the types are the same.
  *
  */
bool ocl::Device::operator != (const ocl::DeviceType&  type) const
{
	return type != this->type();
}

/*! \brief Returns the true if this Device is a GPU . */
bool ocl::Device::isGpu() const
{
    return this->type() == CL_DEVICE_TYPE_GPU;
}

/*! \brief Returns the true if this Device is a CPU. */
bool ocl::Device::isCpu() const
{
	return this->type() == CL_DEVICE_TYPE_CPU;
}

/*! \brief Returns the true if this Device is an accelerator (Cell). */
bool ocl::Device::isAccelerator() const
{
	return this->type() == CL_DEVICE_TYPE_ACCELERATOR;
}


/*! \brief Returns the maximum compute units for a given device. */
size_t ocl::Device::maxComputeUnits() const
{
	cl_uint maxCompUnits;
	OPENCL_SAFE_CALL(  clGetDeviceInfo (_id, CL_DEVICE_MAX_COMPUTE_UNITS , sizeof(maxCompUnits), &maxCompUnits, NULL) );
	return size_t(maxCompUnits);
}

/*! \brief Returns the maximum dimensions that specify the global and local work-item IDs for *this. */
size_t ocl::Device::maxWorkItemDim() const
{
	cl_uint maxWorkItemDim;
	OPENCL_SAFE_CALL( clGetDeviceInfo (_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS , sizeof(maxWorkItemDim), &maxWorkItemDim, NULL) );
	return size_t(maxWorkItemDim);
}

/*! \brief Returns the maximum number of work-items that can be specified in each dimension of the work-group for *this. */
std::vector<size_t> ocl::Device::maxWorkItemSizes() const
{
    std::vector<size_t> a(3,0);
    //size_t a[3];
    OPENCL_SAFE_CALL( clGetDeviceInfo (this->id(), CL_DEVICE_MAX_WORK_ITEM_SIZES , sizeof(size_t)*3, a.data(), NULL) );
    //std::vector<size_t> b(a, a + sizeof(a) / sizeof(int));
    return a;
}

/*! \brief Returns the maximum number of work-items in a work- group executing a kernel on a single compute unit for *this. */
size_t ocl::Device::maxWorkGroupSize() const
{
    size_t a;
	OPENCL_SAFE_CALL( clGetDeviceInfo (this->id(), CL_DEVICE_MAX_WORK_GROUP_SIZE , sizeof(a), &a, NULL) );
	return a;
}

/*! \brief Returns the maximum size in bytes of a constant buffer allocation for *this. */
size_t ocl::Device::maxConstantBufferSize() const
{
    cl_ulong a;
	OPENCL_SAFE_CALL( clGetDeviceInfo (this->id(), CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE , sizeof(a), &a, NULL) );
	return size_t(a);
}


/*! \brief Returns the maximum size of memory object allocation in bytes for *this . */
size_t ocl::Device::maxMemAllocSize() const
{
    cl_ulong maxMemAllocSize;
    OPENCL_SAFE_CALL(  clGetDeviceInfo (_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE , sizeof(maxMemAllocSize), &maxMemAllocSize, NULL) );
    return size_t(maxMemAllocSize);
}

/*! \brief Returns the global memory size in bytes for *this . */
size_t ocl::Device::globalMemSize() const
{
    cl_ulong a;
    OPENCL_SAFE_CALL(  clGetDeviceInfo (_id, CL_DEVICE_GLOBAL_MEM_SIZE , sizeof(a), &a, NULL) );
    return size_t(a);
}

/*! \brief Returns the local memory size in bytes for *this . */
size_t ocl::Device::localMemSize() const
{
    cl_ulong a;
    OPENCL_SAFE_CALL(  clGetDeviceInfo (_id, CL_DEVICE_LOCAL_MEM_SIZE , sizeof(a), &a, NULL) );
    return size_t(a);
}

/*! \brief Returns the OpenCL platform on which this Device is located.*/
cl_platform_id ocl::Device::platform() const
{
	cl_platform_id _pl;
	OPENCL_SAFE_CALL( clGetDeviceInfo (this->id(), CL_DEVICE_PLATFORM , sizeof(_pl), &_pl, NULL) );
	TRUE_ASSERT(_pl != 0, "Platform not found");
	return _pl;
}

/*! \brief Returns the version of this Device .*/
std::string ocl::Device::version() const
{
// 	std::string buffer(100,0);
  char buffer[100];
  
	OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_VERSION,  sizeof buffer, buffer, NULL));
	return buffer;
}

/*! \brief Returns the name of this Device .*/
std::string ocl::Device::name() const
{
//     std::string buffer(100,0);
  char buffer[100];
  
	OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_NAME,  sizeof buffer, buffer, NULL));
	return buffer;
}

/*! \brief Returns the name of the vendor of this Device .*/
std::string ocl::Device::vendor() const
{
//     std::string buffer(100,0);
  char buffer[100];
  
	OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_VENDOR,  sizeof buffer, buffer, NULL));
	return buffer;
}

/*! \brief Returns all extensions of this Device (support of double precision?) .*/
std::string ocl::Device::extensions() const
{
//     std::string buffer(1000,0);
  char buffer[1000];
  
	OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_EXTENSIONS,  sizeof buffer, buffer, NULL));
	return buffer;
}

/*! \brief Prints this Device.*/
void ocl::Device::print() const
{
//     std::string buffer(100,0);
    char buffer[100];
    std::cout << "\tDevice " << std::endl;
    OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_VENDOR, sizeof buffer, buffer, NULL) );
    std::cout << "\t\tDevice: " << buffer << std::endl;
//     buffer.assign(100,0);
    OPENCL_SAFE_CALL( clGetDeviceInfo(this->id(), CL_DEVICE_NAME, sizeof buffer, buffer, NULL) );
    std::cout << "\t\tName: " <<  buffer << std::endl;

}


/*! \brief Return true if this Device support images.*/
bool ocl::Device::imageSupport() const
{
	cl_bool support = CL_FALSE;
	OPENCL_SAFE_CALL( clGetDeviceInfo( this->id(), CL_DEVICE_IMAGE_SUPPORT, sizeof support, &support, NULL ) );
	return support == CL_TRUE;
}
