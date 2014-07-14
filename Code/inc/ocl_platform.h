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

#ifndef OCL_PLATFORM_H
#define OCL_PLATFORM_H


#include <vector>
#include <string>
#include <set>
#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif


/**
* @mainpage C++ Wrapper for OpenCL
*
* This API provides a C++ interface to the common OpenCL API routines.
* It is built for a convenience of the OpenCL framework and uses the object-oriented paradigms for
* wrapping C routines. Yet, it is still possible to use the C OpenCL functions and
* mix it with this library. Have fun.
*
*/





namespace ocl
{

class DeviceType;
class DeviceTypes;
class Device;
class Context;
class Program;
class Queue;

/*! \class Platform ocl_platform.h "inc/ocl_platform.h"
  * \brief Wrapper for cl_platform_id
  *
  * The Platform model consists of a host connected to one or more Device objects.
  * A Device is divided into one or more compute units (CUs) which are further
  * divided into one or more processing elements (PEs). Computations on a Device occur
  * within the processing elements. Note that within one Platform different
  * DeviceTypes might exist.
  * <br>
  * A Platform has mutiple Context objects aggregating the Device objects
  * within the Platform. There can only be one active Context which can be
  * switched any time in the program flow. Within one Context multiple Queue objects
  * can be created. One of these must be selected as an active Queue on
  * which commands are enqueued. Multiple Platforms may exist.
  *
  */
class Platform
{
public:
    Platform();
		explicit Platform(cl_platform_id);
    explicit Platform(const DeviceType&);
    explicit Platform(const DeviceTypes&);
 	// Delete copying due to pointer data members.
	Platform& operator =( Platform const& ) = delete;
	Platform( Platform const& ) = delete;


	~Platform();

	bool has(const DeviceType&) const;
	bool has(const DeviceTypes&) const;
    bool has(const Device&) const;
	bool operator == (const Platform &) const;
	bool operator != (const Platform &) const;
	bool operator == (cl_platform_id) const;
	bool operator != (cl_platform_id) const;

    void create(const DeviceType&);
    void create(const ocl::DeviceTypes&);
    void create(cl_platform_id);

	cl_platform_id id() const;
    const std::vector<Device>& devices() const;
    Device device(cl_device_id) const;
    Device device(const DeviceType&) const;
    std::vector<Device> devices(const DeviceTypes&) const;
    std::vector<Device> devices(const DeviceType&) const;

    void print(bool print_devices = true,
               bool print_profile = true,
               bool print_version = true,
               bool print_name = true,
               bool print_vendor = true,
               bool print_extensions = true,
               std::ostream& out = std::cout);

    void insert(Context&);
    bool has(Context &) const;
    void setActiveContext(Context&);
    Context* activeContext() const;
    bool isActiveContext(Context&) const;
    bool hasActiveContext() const;


    static cl_platform_id id(const DeviceTypes&);
    static cl_platform_id id(const DeviceType&);

    static bool hasActivePlatform();
    static void setActivePlatform(Platform &);
    static Platform* activePlatform();


private:

    static Platform *_activePlatform;

    std::vector<Device> _devices;
    cl_platform_id _id;
	Context * _activeContext;
	std::set<Context*> _contexts;

};

}

#endif
