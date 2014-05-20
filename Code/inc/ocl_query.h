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

#ifndef OCL_QUERY_H
#define OCL_QUERY_H

#include <string>
#include <iostream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

#define OPENCL_SAFE_CALL(status) ocl::safe_call(status,__FILE__,__FUNCTION__,__LINE__);



namespace ocl
{

    bool exists(cl_platform_id);
    std::vector<cl_platform_id> platforms();
    std::vector<cl_device_id> devices(cl_platform_id);
    cl_device_type deviceType(cl_device_id);

    std::string profile(cl_platform_id);
    std::string version(cl_platform_id);
    std::string name(cl_platform_id);
    std::string vendor(cl_platform_id);
    std::string extensions(cl_platform_id);

    std::string profile(cl_device_id);
    std::string vendor(cl_device_id);
    std::string name(cl_device_id);
    std::string version(cl_device_id);
    std::string extensions(cl_device_id);

    /*! \brief Prints the device. */
    void printDevice(cl_device_id,
                     bool print_profile = true,
                     bool print_version = true,
                     bool print_name = true,
                     bool print_vendor = true,
                     bool print_extensions = false,
                     std::ostream& out = std::cout);

    /*! \brief Prints the devices for the specified platform. */
    void printDevices(cl_platform_id,
                      bool print_profile = true,
                      bool print_version = true,
                      bool print_name = true,
                      bool print_vendor = true,
                      bool print_extensions = false,
                      std::ostream& out = std::cout);

    /*! \brief Prints all platforms and the corresponding devices if wanted. */
    void printPlatforms(bool print_devices = true,
                        bool print_profile = true,
                        bool print_version = true,
                        bool print_name = true,
                        bool print_vendor = true,
                        bool print_extensions = false,
                        std::ostream& out = std::cout);

    /*! \brief Prints the specified platform and the corresponding devices if wanted. */
    void printPlatform(cl_platform_id id,
                       bool print_devices = true,
                       bool print_profile = true,
                       bool print_version = true,
                       bool print_name = true,
                       bool print_vendor = true,
                       bool print_extensions = false,
                       std::ostream& out = std::cout);


    void safe_call(cl_int status, const std::string file, const std::string function, const int line);
	double execTime(cl_event event);


}

#endif
