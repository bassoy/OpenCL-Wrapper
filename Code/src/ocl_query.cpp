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

#include <sstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory>

#include <ocl_query.h>

#include <utl_assert.h>

/*
 * Since CL_KHR_ICD is an extension, some platforms (e. g. OS X) may not
 * include it.
 */
#ifdef CL_PLATFORM_NOT_FOUND_KHR
#define HAS_CL_KHR_ICD
#endif

using namespace std;


/*! \brief Returns true if the OpenCL platform exists. */
bool ocl::exists(cl_platform_id id)
{
    TRUE_ASSERT(id != 0, "Invalid Platform");
    const vector<cl_platform_id>& pltfs = platforms();
    return find(pltfs.begin(), pltfs.end(), id) != pltfs.end();
}


/*! \brief Returns all available OpenCL platforms. */
vector<cl_platform_id> ocl::platforms()
{
    cl_uint numPlatforms = 0;

    // Query for the number of recongnized platforms
    OPENCL_SAFE_CALL( clGetPlatformIDs(0, NULL, &numPlatforms));
    TRUE_ASSERT(numPlatforms > 0, "No Platforms detected.");

    vector<cl_platform_id> pltfs(numPlatforms, 0);
    OPENCL_SAFE_CALL( clGetPlatformIDs(numPlatforms, pltfs.data(), NULL));

    return pltfs;
}

/*! \brief Returns all available OpenCL devices for the specified OpenCL platform. */
vector<cl_device_id> ocl::devices(cl_platform_id id)
{
    TRUE_ASSERT(id != 0, "Invalid Platform");
    cl_uint numDevices = 0;
    // Retrieve the number of devices present on the specified platform of the specified deviceType
    OPENCL_SAFE_CALL( clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, 0, NULL,  &numDevices) );
    TRUE_ASSERT(numDevices > 0, "No devices detected.");

    vector<cl_device_id> devs(numDevices,0);
    OPENCL_SAFE_CALL( clGetDeviceIDs(id, CL_DEVICE_TYPE_ALL, numDevices, &devs[0], NULL) );
    return devs;
}



/*! \brief Prints all OpenCL platforms available on the system */
void ocl::printPlatforms(bool print_devices,
                         bool print_profile,
                         bool print_version,
                         bool print_name,
                         bool print_vendor,
                         bool print_extensions,
                         ostream& out)
{
    const std::vector<cl_platform_id> &pltfs = ocl::platforms();

//    out << pltfs.size() << " platform(s) detected!" << endl;
    for(auto it = pltfs.begin(); it != pltfs.end(); ++it){
        const cl_platform_id &id = *it;

        out << "Platform " << (it - pltfs.begin()) << endl;
        ocl::printPlatform(id,print_devices, print_profile, print_version, print_name, print_vendor, print_extensions, out);
    }
    out << endl;
}

/*! \brief Prints the specified OpenCL platform */
void ocl::printPlatform(cl_platform_id id,
                        bool print_devices,
                        bool print_profile,
                        bool print_version,
                        bool print_name,
                        bool print_vendor,
                        bool print_extensions,
                        ostream& out)
{
    TRUE_ASSERT(id != 0, "Invalid Platform");
    if(print_profile)    out << "Platform Profile    : " << ocl::profile(id)    << endl;
    if(print_vendor)     out << "Platform Vendor     : " << ocl::vendor(id)     << endl;
    if(print_name)       out << "Platform Name       : " << ocl::name(id)       << endl;
    if(print_version)    out << "Platform Version    : " << ocl::version(id)    << endl;
    if(print_extensions) out << "Platform Extensions : " << ocl::extensions(id) << endl;
    if(print_devices){
        out << "Devices : " << endl;
        ocl::printDevices(id, print_profile, print_version, print_name, print_vendor, print_extensions, out);
    }
}

static std::string getPlatformInfo(cl_platform_id id, cl_platform_info info)
{
	TRUE_ASSERT(id != 0, "Invalid Platform");
  size_t size = 0;
  OPENCL_SAFE_CALL( clGetPlatformInfo(id, info,  0, 0, &size));
  TRUE_ASSERT(size > 0, "Cannot retrieve platform profile");

	std::unique_ptr< char[] > buffer( new char[size] );
	OPENCL_SAFE_CALL( clGetPlatformInfo(id, info, size, buffer.get(), NULL));
	return buffer.get();
}

/*! \brief Returns the profile of the specified OpenCL platform. */
std::string ocl::profile(cl_platform_id id)
{
	return getPlatformInfo(id,CL_PLATFORM_PROFILE);
}

/*! \brief Returns the version of the specified OpenCL platform. */
std::string ocl::version(cl_platform_id id)
{
	return getPlatformInfo(id,CL_PLATFORM_VERSION);
}

/*! \brief Returns the name of the specified OpenCL platform. */
std::string ocl::name(cl_platform_id id)
{
	return getPlatformInfo(id,CL_PLATFORM_NAME);
}

/*! \brief Returns the vendor of the specified OpenCL platform. */
std::string ocl::vendor(cl_platform_id id)
{
	return getPlatformInfo(id,CL_PLATFORM_VENDOR);
}

/*! \brief Returns the extensions of the specified OpenCL platform. */
std::string ocl::extensions(cl_platform_id id)
{
	return getPlatformInfo(id,CL_PLATFORM_EXTENSIONS);
}



/*! \brief Prints all OpenCL devices available on the OpenCL platform. */
void ocl::printDevices(cl_platform_id id,
                       bool print_profile,
                       bool print_version,
                       bool print_name,
                       bool print_vendor,
                       bool print_extensions,
                       ostream& out)
{
    const vector<cl_device_id> &devs = ocl::devices(id);

//    out << devs.size() << " device(s) detected!" << endl;
    for(auto it = devs.begin(); it != devs.end(); ++it){
        const cl_device_id &id = *it;

        out << "Device " << (it - devs.begin()) << endl;
        ocl::printDevice(id, print_profile, print_version, print_name, print_vendor, print_extensions, out);
    }
    out << endl;
}

void ocl::printDevice(cl_device_id id,
                      bool print_profile,
                      bool print_version,
                      bool print_name,
                      bool print_vendor,
                      bool print_extensions,
                      ostream& out)
{
    TRUE_ASSERT(id != 0, "Invalid device");
    if(print_profile)    out << "Device Profile    : " << ocl::profile(id)    << endl;
    if(print_vendor)     out << "Device Vendor     : " << ocl::vendor(id)     << endl;
    if(print_name)       out << "Device Name       : " << ocl::name(id)       << endl;
    if(print_version)    out << "Device Version    : " << ocl::version(id)    << endl;
    if(print_extensions) out << "Device Extensions : " << ocl::extensions(id) << endl;
}

static std::string getDeviceInfo(cl_device_id id, cl_device_info info)
{
	TRUE_ASSERT(id != 0, "Invalid device");
  size_t size = 0;
  OPENCL_SAFE_CALL( clGetDeviceInfo(id, info,  0, 0, &size));
  TRUE_ASSERT(size > 0, "Cannot retrieve device information");

	std::unique_ptr< char[] > buffer( new char[size] );
	OPENCL_SAFE_CALL( clGetDeviceInfo(id, info, size, buffer.get(), NULL));
	return buffer.get();
}


/*! \brief Returns the profile of the specified OpenCL device. */
std::string ocl::profile(cl_device_id id)
{
	return getDeviceInfo(id,CL_DEVICE_PROFILE);
}

/*! \brief Returns the version of the specified OpenCL device. */
std::string ocl::version(cl_device_id id)
{
	return getDeviceInfo(id,CL_DEVICE_VERSION);	
}

/*! \brief Returns the name of the specified OpenCL device. */
std::string ocl::name(cl_device_id id)
{
	return getDeviceInfo(id,CL_DEVICE_NAME);
}

/*! \brief Returns the vendor of the specified OpenCL device. */
std::string ocl::vendor(cl_device_id id)
{
	return getDeviceInfo(id,CL_DEVICE_VENDOR);
}

/*! \brief Returns the extensions of the specified OpenCL device. */
std::string ocl::extensions(cl_device_id id)
{
	return getDeviceInfo(id,CL_DEVICE_EXTENSIONS);
}


/*! \brief Returns the device_type for a given device. */
cl_device_type ocl::deviceType(cl_device_id id)
{
	TRUE_ASSERT(id != 0, "Invalid Device");
	cl_device_type t;
	OPENCL_SAFE_CALL( clGetDeviceInfo (id,CL_DEVICE_TYPE, sizeof(t), &t, NULL) );
	return t;
}



/*! brief Auxiliary function for measuring OpenCL kernels. */
double ocl::execTime(cl_event event)
{
    cl_ulong start, end;
    
	OPENCL_SAFE_CALL( clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL) );
	OPENCL_SAFE_CALL( clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL) );
    
    return double(end - start); // returns in nanoseconds
}

/*! brief Auxiliary function for calling OpenCL routines. */
void ocl::safe_call(cl_int status, const std::string file, const std::string function, const int line)
{
    if(status == CL_SUCCESS) return;
    
    std::ostringstream oss;
    
    // TODO This list is far from complete.
    switch(status)
    {
    case CL_DEVICE_NOT_AVAILABLE                       :  {oss << "CL_DEVICE_NOT_AVAILABLE."; break;}
    case CL_DEVICE_NOT_FOUND                           :  {oss << "CL_DEVICE_NOT_FOUND."; break;}
    case CL_COMPILER_NOT_AVAILABLE                     :  {oss << "CL_COMPILER_NOT_AVAILABLE."; break;}
    case CL_MEM_OBJECT_ALLOCATION_FAILURE              :  {oss << "CL_MEM_OBJECT_ALLOCATION_FAILURE."; break;}
    case CL_OUT_OF_RESOURCES                           :  {oss << "CL_OUT_OF_RESOURCES."; break;}
    case CL_OUT_OF_HOST_MEMORY                         :  {oss << "CL_OUT_OF_HOST_MEMORY."; break;}
    case CL_MEM_COPY_OVERLAP                           :  {oss << "CL_MEM_COPY_OVERLAP."; break;}
    case CL_PROFILING_INFO_NOT_AVAILABLE               :  {oss << "CL_PROFILING_INFO_NOT_AVAILABLE."; break;}
    case CL_IMAGE_FORMAT_MISMATCH                      :  {oss << "CL_IMAGE_FORMAT_MISMATCH."; break;}
    case CL_IMAGE_FORMAT_NOT_SUPPORTED                 :  {oss << "CL_IMAGE_FORMAT_NOT_SUPPORTED."; break;}
    case CL_BUILD_PROGRAM_FAILURE                      :  {oss << "CL_BUILD_PROGRAM_FAILURE."; break;}
    case CL_MAP_FAILURE                                :  {oss << "CL_MAP_FAILURE."; break;}
    case CL_MISALIGNED_SUB_BUFFER_OFFSET               :  {oss << "CL_MISALIGNED_SUB_BUFFER_OFFSET."; break;}
    case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST  :  {oss << "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST."; break;}
#ifdef HAS_CL_KHR_ICD
    case CL_PLATFORM_NOT_FOUND_KHR                     :  {oss << "CL_PLATFORM_NOT_FOUND_KHR."; break;}
    case CL_DEVICE_PARTITION_FAILED_EXT                :  {oss << "CL_DEVICE_PARTITION_FAILED_EXT."; break;}
    case CL_INVALID_PARTITION_COUNT_EXT                :  {oss << "CL_INVALID_PARTITION_COUNT_EXT."; break;}
    case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR        :  {oss << "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR."; break;}
#endif
    case CL_INVALID_EVENT_WAIT_LIST                    :  {oss << "CL_INVALID_EVENT_WAIT_LIST."; break;}
    case CL_INVALID_PLATFORM                           :  {oss << "CL_INVALID_PLATFORM."; break;}
    case CL_INVALID_DEVICE_TYPE                        :  {oss << "CL_INVALID_DEVICE_TYPE."; break;}
    case CL_INVALID_DEVICE                             :  {oss << "CL_INVALID_DEVICE."; break;}
    case CL_INVALID_VALUE                              :  {oss << "CL_INVALID_VALUE."; break;}
    case CL_INVALID_PROGRAM                            :  {oss << "CL_INVALID_PROGRAM."; break;}
    case CL_INVALID_QUEUE_PROPERTIES                   :  {oss << "CL_INVALID_QUEUE_PROPERTIES."; break;}
    case CL_INVALID_CONTEXT                            :  {oss << "CL_INVALID_CONTEXT."; break;}
    case CL_INVALID_BUILD_OPTIONS                      :  {oss << "CL_INVALID_BUILD_OPTIONS."; break;}
    case CL_INVALID_KERNEL_NAME                        :  {oss << "CL_INVALID_KERNEL_NAME."; break;}
    case CL_INVALID_ARG_INDEX                          :  {oss << "CL_INVALID_ARG_INDEX."; break;}
    case CL_INVALID_COMMAND_QUEUE                      :  {oss << "CL_INVALID_COMMAND_QUEUE."; break;}
    case CL_INVALID_WORK_DIMENSION                     :  {oss << "CL_INVALID_WORK_DIMENSION."; break;}
    case CL_INVALID_GLOBAL_WORK_SIZE                   :  {oss << "CL_INVALID_GLOBAL_WORK_SIZE."; break;}
    case CL_INVALID_MEM_OBJECT                         :  {oss << "CL_INVALID_MEM_OBJECT."; break;}
    case CL_INVALID_HOST_PTR                           :  {oss << "CL_INVALID_HOST_PTR."; break;}
    case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR            :  {oss << "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR."; break;}
    case CL_INVALID_IMAGE_SIZE                         :  {oss << "CL_INVALID_IMAGE_SIZE."; break;}
    case CL_INVALID_SAMPLER                            :  {oss << "CL_INVALID_SAMPLER."; break;}
    case CL_INVALID_BINARY                             :  {oss << "CL_INVALID_BINARY."; break;}
    case CL_INVALID_PROGRAM_EXECUTABLE                 :  {oss << "CL_INVALID_PROGRAM_EXECUTABLE."; break;}
    case CL_INVALID_KERNEL_DEFINITION                  :  {oss << "CL_INVALID_KERNEL_DEFINITION."; break;}
    case CL_INVALID_KERNEL                             :  {oss << "CL_INVALID_KERNEL."; break;}
    case CL_INVALID_ARG_VALUE                          :  {oss << "CL_INVALID_ARG_VALUE."; break;}
    case CL_INVALID_ARG_SIZE                           :  {oss << "CL_INVALID_ARG_SIZE."; break;}
    case CL_INVALID_KERNEL_ARGS                        :  {oss << "CL_INVALID_KERNEL_ARGS."; break;}
    case CL_INVALID_WORK_GROUP_SIZE                    :  {oss << "CL_INVALID_WORK_GROUP_SIZE."; break;}
    case CL_INVALID_GLOBAL_OFFSET                      :  {oss << "CL_INVALID_GLOBAL_OFFSET."; break;}
    case CL_INVALID_EVENT                              :  {oss << "CL_INVALID_EVENT."; break;}
    case CL_INVALID_OPERATION                          :  {oss << "CL_INVALID_OPERATION."; break;}
    case CL_INVALID_GL_OBJECT                          :  {oss << "CL_INVALID_GL_OBJECT."; break;}
    case CL_INVALID_MIP_LEVEL                          :  {oss << "CL_INVALID_MIP_LEVEL."; break;}
    case CL_INVALID_WORK_ITEM_SIZE                     :  {oss << "CL_INVALID_WORK_ITEM_SIZE."; break;}
    default                                            :  {oss << "Unknown error (0x" << std::hex << status << std::dec << "or " << status << ")."; break;}
    }
    oss << "Error in file=" << file << ", in function=" << function << ", in line="<< line << endl;
    
    throw std::runtime_error( oss.str() );
}
