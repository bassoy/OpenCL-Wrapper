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

#include <ocl_memory.h>
#include <ocl_context.h>
#include <ocl_query.h>
#include <ocl_queue.h>
#include <ocl_platform.h>

#include <utl_assert.h>

/*! \brief Instantiates this Device Memory  within a Context with size_bytes.
  *
  * If no Context is provided the active Context of the Platform is taken if
  * there is an active Context.
  * Use Buffer and Image to allocate Memory. This is the base class.
  *
  */
ocl::Memory::Memory () :
    _context(0), _id(0)
{
    if(!ocl::Platform::hasActivePlatform()) return;
    if(!ocl::Platform::activePlatform()->hasActiveContext()) return;

    _context = ocl::Platform::activePlatform()->activeContext();
    _context->insert(this);
}

/*! \brief Instantiates this Device Memory  within a Context with size_bytes.
  *
  * If no Context is provided the active Context of the Platform is taken.
  * Use Buffer and Image to allocate Memory. This is the base class.
  * \param context is Context for which the Memory is created.
  */
ocl::Memory::Memory (ocl::Context& context) :
    _context(&context), _id(0)
{
    TRUE_ASSERT(_context != 0, "context == 0");
    _context->insert(this);
}


/*! \brief Instantiates this Device Memory from another Device Memory.
  *
  * Use Buffer and Image which calls this constructor. This is the base class.
  * \param other Device Memory from which the Context is taken from.
  */
ocl::Memory::Memory (const Memory &other ) :
    _context(other._context), _id(0)
{
    TRUE_ASSERT(_context != 0, "context == 0");
    _context->insert(this);
}

/*! \brief Instantiates this Device Memory from another Device Memory.
  *
  * Use Buffer and Image which calls this constructor. This is the base class.
  * \param other Device Memory from which the Context is taken.
  */
ocl::Memory::Memory (Memory && other ) :
    _context(other._context), _id(other._id)
{
    TRUE_ASSERT(_context != 0, "context == 0");
    _context->insert(this);    
    _context->remove(&other);
    other._id = 0;
}

/*! \brief Destructs this Device Memory.
  *
  * You do not have to call this function, the corresponding
  * Buffer and Image objects are.
  *
  * Device Memory is only released if there is no reference to the Device Memory.
  */
ocl::Memory::~Memory ()
{
    this->release();
}

/*! \brief Copies a Device Memory from another Device Memory.
  *
  * Note that the copy operation is performed by the Subclasses Buffer
  * and Image.
  * \param other Device Memory from which the Context is taken.
  */
ocl::Memory& ocl::Memory::operator =(const Memory & other)
{
    TRUE_ASSERT(other._context != 0, "No active Context");
    TRUE_ASSERT(this->context()== other.context(), "Context must be equal");
	this->_context = other._context;
    this->_context->insert(this);
	this->_id = 0;
	return *this;
}

/*! \brief Moves a Device Memory from another Memory.
  *
  * Note that this is a move operation which does not perform a copy.
  * Only the pointer of the Memory location is copied.
  *
  * \param other Device Memory from which the context and id is taken.
  */
ocl::Memory& ocl::Memory::operator =(Memory && other)
{
    if(this == &other) return *this;
    TRUE_ASSERT(other._context != 0, "No active Context");
    TRUE_ASSERT(this->context() == other.context(), "Context must be equal");
    this->release();

    this->_context = other._context;
    this->_id = other._id;

    this->_context->insert(this);
    this->_context->remove(&other);

    other._id = 0;

    return *this;
}

/*! \brief Returns the context for which this Device Memory was created.
  *
  */
ocl::Context* ocl::Memory::context () const
{
    return this->_context;
}

/*! \brief Sets a new context for this Device Memory.
  *
  */
void ocl::Memory::setContext(ocl::Context &ctxt)
{
    TRUE_ASSERT(_context == 0 || ctxt == *this->context(), "cannot switch context");
	_context = &ctxt;
}


/*! \brief Returns Device Memory flags with which the Device Memory was created.
  *
  */
cl_mem_flags ocl::Memory::flags () const
{
	cl_mem_flags info;
	OPENCL_SAFE_CALL( clGetMemObjectInfo (this->_id,CL_MEM_FLAGS, sizeof(info), &info, NULL)) ;
	return info;
}


/*! \brief Releases the Device Memory.
  *
  * Device Memory is freed if not referenced.
  */
void ocl::Memory::release()
{
	if(this->_id == 0) return;

	OPENCL_SAFE_CALL( clReleaseMemObject(_id) );
	_context->remove(const_cast<ocl::Memory*>(this));
	this->_id = 0;
}

/*! \brief Unmaps the Device Memory.
  *
  * Memory is if unmapped if not referenced.
  */
void ocl::Memory::unmap ( void * mapped_ptr ) const
{
    if(map_count() > 0){
        OPENCL_SAFE_CALL( clEnqueueUnmapMemObject (this->activeQueue().id(), this->_id, mapped_ptr, 0, NULL, NULL) );
        OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
    }
}

/*! \brief Returns the number of mappings of the Device Memory to the host Memory.
  *
  * Each time a Buffer or Image is mapped into the host Memory, the
  * mapping reference count is incremented.
  */
size_t ocl::Memory::map_count() const
{
    cl_uint info;
	OPENCL_SAFE_CALL( clGetMemObjectInfo (this->_id, CL_MEM_MAP_COUNT , sizeof(info), &info, NULL)) ;
	return size_t(info);
}

/*! \brief Returns the number of reference to the Device Memory to the host Memory.
  *
  * Each time a sub Buffer or sub image are created from Memory objects, the
  * reference count is incremented.
  */
size_t ocl::Memory::reference_count() const
{
    if(this->_id == 0) return 0;
    if(this->_context == 0) return 0;
    cl_uint info;
	OPENCL_SAFE_CALL( clGetMemObjectInfo (this->_id, CL_MEM_REFERENCE_COUNT, sizeof(info), &info, NULL) ) ;
	return size_t(info);
}

/*! \brief Returns cl_mem.
  *
  * cl_mem is the OpenCL transparent Device Memory structure.
  */
cl_mem 	ocl::Memory::id () const
{
	return this->_id;
}

/*! \brief Returns the number of bytes allocated for this Memory. */
size_t 	ocl::Memory::size_bytes () const
{
    if(this->_id == 0) return 0;
	size_t info;
	OPENCL_SAFE_CALL( clGetMemObjectInfo (this->_id,CL_MEM_SIZE, sizeof(info), &info, NULL)) ;
	return info;
}

/*! \brief Returns the active Queue of the Context with which this Memory was created. */
ocl::Queue& ocl::Memory::activeQueue() const
{
    return this->context()->activeQueue();
}

///*! \brief Returns the active Device of the active Queue with which this Memory was created. */
//const ocl::Device& ocl::Memory::activeDevice() const
//{
//    return this->activeQueue().device();
//}

/*! \brief Returns true if the Context of other and this Memory are the different or refer to other cl_mem. */
bool ocl::Memory::operator!= ( const Memory & other ) const
{
    return *this->context() != *other.context() || this->id() != other.id();
}

/*! \brief Returns true if the context of other and this Memory are the same and refer to the same cl_mem. */
bool ocl::Memory::operator== ( const Memory & other ) const
{
	return this->context() == other.context() && this->id() == other.id();
}

