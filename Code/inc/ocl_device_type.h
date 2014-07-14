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


#ifndef OCL_DEVICE_TYPE_H
#define OCL_DEVICE_TYPE_H

#include <string>
#include <vector>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

namespace ocl
{

class DeviceType;

/*! \class DeviceTypes ocl_device_type.h "inc/ocl_device_type.h"
  * \brief Aggregation of DeviceType objects.
  *
  * DeviceTypes contain DeviceType objects which are previously
  * defined by this framework. See DeviceType. DeviceTypes
  * are used when defining Platform objects.
  *
  */
class DeviceTypes
{
public:
	DeviceTypes();
	DeviceTypes(const DeviceType &, const DeviceType&);
	DeviceTypes operator |(const DeviceType&) const;
	void operator |=(const DeviceType&);
	bool operator == (const DeviceTypes&) const;
	bool operator != (const DeviceTypes&) const;
	bool contains (const DeviceTypes&) const;
	bool contains (const DeviceType&) const;
	bool contains (cl_device_type) const;
	const std::vector<DeviceType>& operator()() const;
	size_t size() const;
protected:
	void push_back(const DeviceType&);
	explicit DeviceTypes(const DeviceType&);
private:
    std::vector<DeviceType> _deviceTypes;
};

/*! \class DeviceType ocl_device_type.h "inc/ocl_device_type.h"
  * \brief Wrapper for cl_device_type.
  *
  * A DeviceType wraps the cl_device_type and provides easier comparisons
  * and conglomeration of types, see DeviceTypes. A DeviceType already provided
  * and must not be instantiated, GPU,CPU,ACC,ALL.
  */
class DeviceType
{
public:
	explicit DeviceType(cl_device_type, std::string name);
	DeviceType(const DeviceType&);
	DeviceTypes operator | (const DeviceType&) const;
	bool operator == (const DeviceType&) const;
	bool operator != (const DeviceType&) const;
	bool operator == (cl_device_type) const;
	bool operator != (cl_device_type) const;
	cl_device_type operator()() const;
	static const DeviceType& type(cl_device_type);
	~DeviceType(){}
	std::string name() const;
private:
	DeviceType(): _type(), _name() {}
	static std::vector<DeviceType> _allTypes;
	cl_device_type _type;
	std::string _name;
};


namespace device_type{
	extern DeviceType GPU;
	extern DeviceType CPU;
	extern DeviceType ACC;
	extern DeviceType ALL;
}
}

#endif
