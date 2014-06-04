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

#ifndef OCL_MEMORY_H
#define OCL_MEMORY_H

#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <utility>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif


namespace ocl{

/*! \class Memory ocl_memory.h "inc/ocl_memory.h"
  * \brief Wrapper for cl_mem. Base class of the Buffer and Image class.
  *
  * In order to create Memory object an active Contex must exist. The Memory
  * object inserts itself automatically into the Context, while the liveness
  * depends on the range of the scope of its definition. When the scope is
  * left, the Memory object destructs itself and removes itself from
  * its Context. When a Context leaves its scope all corresponding Memory
  * objects will also be destructed and the Memory becomes invalid.
  * The buffer and image class are using the Memory class for common functions.
  * Do not instantiate this class.
  */


class Queue;
class Context;
class Device;

class Memory
{
public:

    /*! \brief Type of mapped memory usage. */
	enum Access {
        ReadWrite = CL_MAP_READ | CL_MAP_WRITE , /*!< Specifies that the region being mapped in the memory object is being mapped for reading and writing.*/
        WriteOnly = CL_MAP_WRITE,                /*!< Specifies that the region being mapped in the memory object is being mapped for writing.*/
        ReadOnly = CL_MAP_READ                   /*!< Specifies that the region being mapped in the memory object is being mapped for reading.*/
    };
    Context* context () const;
    void setContext(Context &);
	cl_mem_flags 	flags () const;
    void release();
	void unmap ( void * mapped_ptr ) const;
	cl_mem 	id () const;
	size_t 	size_bytes () const;
	bool 	operator!= ( const Memory & other ) const;
	bool 	operator== ( const Memory & other ) const;
	Memory& operator =(const Memory & other);
    Memory& operator =(Memory && other);

    size_t map_count() const;
    size_t reference_count() const;

    Queue &activeQueue() const;
//    const Device& activeDevice() const;

protected:
    Memory ();
    Memory (Context &context);
    Memory (const Memory & other);
    Memory (Memory && other);
    virtual ~Memory ();

protected:
    Context *_context;
    cl_mem _id;

};

}

#endif
