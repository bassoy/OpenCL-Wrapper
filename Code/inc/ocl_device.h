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

#ifndef OCL_DEVICE_H
#define OCL_DEVICE_H

#include <vector>
#include <string>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif
#include <ocl_device_type.h>

namespace ocl{

/*! \class Device ocl_device.h "inc/ocl_device.h"
  * \brief Wrapper for cl_device_id.
  *
  * A Device can be of different types and is located in one platform.
  * It can be a central processing unit(e.g. Intel Xeon), graphics processing unit (e.g. Nvidia Fermi) or an accelerator (e.g. Cell).
  * In OpenCL a device consists of compute units which in turn have processing elements. The number of processing elements are
  * the same for all compute units. All compute units share a global memory and posses their own local memory. Depending on the
  * architecture of the device, the compute units work in a lock-step single instruction multiple data or single program
  * multiple data fashion.
  */
class  Device
{
public:	
	
	Device(const Device&);
	explicit Device(cl_device_id);
	Device();
	~Device();


	Device& operator =(const Device&);

	bool operator == (const Device &) const;
	bool operator != (const Device &) const;
	bool operator == (cl_device_id) const;
	bool operator != (cl_device_id) const;
	bool operator == (const DeviceType&) const;
	bool operator != (const DeviceType&) const;

	bool isGpu() const;
	bool isCpu() const;
	bool isAccelerator() const;
	void print() const;

	void setId(cl_device_id);
	cl_device_id id() const;
	const DeviceType& type() const;

	std::vector<size_t> maxWorkItemSizes() const;
	size_t maxWorkItemDim() const;
	size_t maxComputeUnits() const;
	size_t maxMemAllocSize() const;
	size_t maxConstantBufferSize() const;
	size_t globalMemSize() const;
	size_t localMemSize() const;
	size_t maxWorkGroupSize() const;

	cl_platform_id platform() const;
	std::string version()    const;
	std::string name()       const;
	std::string vendor()     const;
	std::string extensions() const;
	
	bool imageSupport() const;
  bool doubleSupport() const;
	bool supportsExtension( std::string const& extension ) const;
  bool supportsVersion( int major, int minor ) const;

private:
	cl_device_id _id;
	DeviceType _type;


};

}

#endif
