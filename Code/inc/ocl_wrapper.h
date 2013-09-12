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

#ifndef OCL_WRAPPER_H
#define OCL_WRAPPER_H

/*! \file ocl_wrapper.h "inc/ocl_wrapper.h"
  * \brief Wrapper classes for better OpenCL handling
  *
  * Include this file in order to have accesses to the OpenCL classes.
  */

/*! \namespace ocl
* \brief Encapsulates the C++ Wrapper classes to prevent conflicts with other classes.
*/

#include <ocl_buffer.h>
#include <ocl_query.h>
#include <ocl_context.h>
#include <ocl_device.h>
#include <ocl_device_type.h>
#include <ocl_event.h>
#include <ocl_event_list.h>
#include <ocl_kernel.h>
#include <ocl_memory.h>
#include <ocl_platform.h>
#include <ocl_program.h>
#include <ocl_queue.h>
#include <ocl_image.h>

#endif
