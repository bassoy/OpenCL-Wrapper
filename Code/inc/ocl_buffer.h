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

#ifndef OCL_BUFFER_H
#define OCL_BUFFER_H

#include <vector>
#include <set>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#ifdef __OPENGL__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#else
#include <CL/opencl.h>
#ifdef __OPENGL__
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif

#include <ocl_event.h>
#include <ocl_event_list.h>
#include <ocl_memory.h>






namespace ocl{


/*! \class Buffer ocl_buffer.h "inc/ocl_buffer.h"
  * \brief Wrapper for buffer to manipulate cl_mem. Inherits from Memory.
  *
  * A buffer object stores a one-dimensional collection of elements. Elements of a buffer object can
  * be a scalar data type (such as an int, float), vector data type, or a user-defined structure.
  * Buffer objects are device memory objects in which data can be transfered from the host memory.
  * It is also possible to map and unmap the host memory regions if the device memory is located
  * on the host.
  */


class Memory;
class Context;
class Queue;

class Buffer : public Memory
{
public:
    /*! \brief Type of access to the memory and memory usage in a kernel. */
    enum 	Access { ReadWrite = CL_MEM_READ_WRITE,     /*!< memory object will be read and written within a kernel.*/
                     WriteOnly = CL_MEM_WRITE_ONLY ,    /*!< memory object will only be written wihtin a kernel.*/
                     ReadOnly  = CL_MEM_READ_ONLY,      /*!< memory object will only be read within a kernel.*/
                     UseHost   = CL_MEM_USE_HOST_PTR,   /*!< memory object will provided by a host pointer and must not be allocated by any OpenCL API call.*/
                     AllocHost = CL_MEM_ALLOC_HOST_PTR, /*!< memory object in the host memory will be created by instantiating a buffer object.*/
                     CopyHost  = CL_MEM_COPY_HOST_PTR   /*!< memory object will be created on the device and copied from the host memory.*/
                   };
	explicit Buffer();
	Buffer (Context&, size_t size_bytes);
	#ifdef __OPENGL__
	Buffer (Context &, GLuint vbo_desc);
	#endif
	explicit Buffer (size_t size_bytes);
	~Buffer();
	Buffer ( const Buffer & other );
	Buffer ( Buffer && other);

	void create(size_t size_bytes);
	#ifdef __OPENGL__
	void create(GLuint vbo_desc);
	#endif
	void recreate(size_t size_bytes);

	void 	copyTo ( size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const EventList & list = EventList()  ) const;
	Event copyToAsync( size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const EventList & list = EventList() );

	void 	copyTo (const Queue&, size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const EventList & list = EventList() ) const;
	Event copyToAsync(const Queue&, size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const EventList & list = EventList() );



	void * map ( size_t thisOffset, size_t size_bytes, Memory::Access access ) const;
	void * map ( Memory::Access access ) const;
	Event mapAsync ( void ** ptr, size_t offset, size_t size_bytes, Memory::Access access, const EventList & list = EventList() ) const;

	void 	read ( size_t offset, void *ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	void 	read ( void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	Event 	readAsync ( size_t offset, void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;

	void 	read (const Queue&, size_t offset, void *ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	void 	read (const Queue&, void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	Event 	readAsync (const Queue&, size_t offset, void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;

	void 	write ( const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	void 	write ( size_t offset, const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	Event 	writeAsync ( size_t offset, const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;

	void 	write (const Queue&, const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	void 	write (const Queue&, size_t offset, const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;
	Event 	writeAsync (const Queue&, size_t offset, const void * ptr_to_host_data, size_t size_bytes, const EventList & list = EventList() ) const;

	Buffer & 	operator= ( const Buffer & other );
	Buffer & 	operator= ( Buffer && other );

	#ifdef __OPENGL__
	cl_int acquireAccess(Queue&);
	cl_int releaseAccess(Queue&, const EventList& = EventList());
	#endif
};

}




#endif
