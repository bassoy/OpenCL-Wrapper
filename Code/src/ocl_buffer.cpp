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
#include <ocl_buffer.h>
#include <ocl_context.h>
#include <ocl_query.h>
#include <ocl_queue.h>
#include <ocl_platform.h>
#include <ocl_device.h>
#include <ocl_queue.h>

#include <utl_assert.h>


/*! \brief Instantiates this Buffer within a context with size_bytes.
  *
  * No Memory is allocated but only an object created which can be used within
  * the specified Context. Allocation takes place when data is transfered.
  * The existence of an active Queue within the Context is assumed.
  *
  * \param size_bytes is the size in bytes which are needed for the Memory.
  */
ocl::Buffer::Buffer (Context& ctxt, size_t size_bytes, Access access ) :
    Memory(ctxt)
{
    create(size_bytes,access);
}

/*! \brief Instantiates this Buffer within a context with size_bytes.
  *
  * No Memory is allocated but only an object created which can be used within
  * the active Context. Allocation takes place when data is transfered.
  * The existence of an active Context and an active Queue is assumed.
  *
  * \param size_bytes is the size in bytes which are needed for the Memory.
  */
ocl::Buffer::Buffer (size_t size_bytes, Access access ) :
	Memory()
{
	create(size_bytes,access);
}

/*! \brief Instantiates this Buffer within a context with size_bytes from an OpenGL Buffer.
  *
  * No Memory is allocated but only an object created which can be used within
  * the specified Context. Allocation takes place when data is transfered.
  * This Buffer ist created from an OpenGL Buffer, so the data can be shared between OpenGL and OpenGL.
  * A shared context between OpenGL and OpenCL is assumed.
  * The existence of an active Queue within the Context is assumed.
  *
  * \param ctxt is a shared context between OpenGL and OpenCL
  * \param size_bytes is the size in bytes which are needed for the Memory.
  * \param vbo_desc is the VBO description of the OpenGL Buffer from which this Buffer is created.
  */
#ifdef __OPENGL__
ocl::Buffer::Buffer(Context &ctxt, GLuint vbo_desc) :
    Memory(ctxt)
{
    this->create(vbo_desc);
}
#endif

/*! \brief Instantiates this Buffer without a Context
  *
  * No Buffer is created. Use Buffer::create for the creation of such an object.
*/
ocl::Buffer::Buffer () :
	Memory()
{
}

ocl::Buffer::~Buffer()
{
}

/*! \brief Instantiates this Buffer from another Buffer.
  *
  * Note that Memory is allocated and transfered from other to this.
  * The Context of the other Buffer is taken.
  *
  * \param other Buffer to copy from.
  */
ocl::Buffer::Buffer ( const Buffer & other ) :
	Memory(other)
{
    TRUE_ASSERT(this->context() == other.context(), "context are not equal.");
    this->create(other.size_bytes());
    other.copyTo(0, other.size_bytes(), *this, 0);
}

/*! \brief Instantiates this Buffer from another Buffer within one context.
  *
  * Note that the data is moved without copying the data from other. Only
  * pointer copy is performed.
  *
  * \param other Buffer to move from.
  */
ocl::Buffer::Buffer (Buffer && other ) :
    Memory(std::move(other))
{
}

/*! \brief Creates cl_mem for this Buffer.
  *
  * Note that no Memory is allocated. Allocation takes place when data is transfered.
  * It is assumed that an active Queue exists.
  *
  *
  * \param size_bytes Number of bytes to be reserved.
  */
void ocl::Buffer::create(size_t size_bytes, Access access )
{
    TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create buffer");

	TRUE_ASSERT(this->id() == nullptr, "Cannot create buffer twice. Please release buffer.");

    cl_mem_flags flags = access;

    if(this->context()->devices().size() == 1 &&
       this->context()->devices().at(0).type() == ocl::device_type::CPU){
       flags |= ocl::Buffer::AllocHost;
    }	

    cl_int status;
	_id = clCreateBuffer(this->_context->id(), flags,  size_bytes, NULL, &status);
	OPENCL_SAFE_CALL( status );
    TRUE_ASSERT(_id != 0, "could not create buffer");
}

/*! \brief Creates cl_mem for this Buffer.
  *
  * Note that no Memory is allocated. Allocation takes place when data is transfered.
  * It is assumed that an active Queue exists.
  * This Buffer is now shared between OpenGL and OpenCL.
  *
  * \param size_bytes Number of bytes to be reserved.
  */

#ifdef __OPENGL__
void ocl::Buffer::create(GLuint vbo_desc)
{
    cl_mem_flags flags = ocl::Buffer::ReadWrite;

    if (this->_context->devices().size() == 1 &&
            this->_context->devices().at(0).type() == ocl::device_type::CPU){
        flags |= ocl::Buffer::AllocHost;
    }

    cl_int status;

    //Create Buffer from GL Buffer
    this->_id = clCreateFromGLBuffer(this->_context->id(),
                                     flags, vbo_desc, &status);
    OPENCL_SAFE_CALL(status);
    TRUE_ASSERT(this->_id != 0, "Could not create shared buffer.");
}
#endif


/*! \brief Creates a new cl_mem for this Buffer.
  *
  *  Note that no Memory is allocated. Allocation takes place when data is transfered.
  *
  * \param size_bytes Number of bytes to be reserved.
  */
void ocl::Buffer::recreate(size_t size_bytes)
{
    if(this->size_bytes() == size_bytes) return;    
	this->release();
	this->create(size_bytes);
}

/*! \brief Copies from this Buffer to the destination Buffer.
  *
  * The operation assumes that all data are valid and no synchronization is necessary (active Queue executes in-order).
  * The operation forces that all commands within the active Queue including this one are completed.
  *
  * \param thisOffset is the offset in bytes of this Buffer.
  * \param size_bytes is the size in bytes which are transmitted.
  * \param dest is the destination Buffer in which the data is going to be copied.
  * \param destOffset is the offset in bytes of the destination Buffer.
  */
void ocl::Buffer::copyTo ( size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const ocl::EventList &list) const
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(this->id() != dest.id(), "Buffer must not be equal this->id() " << this->id() << "; other.id " << dest.id());
    OPENCL_SAFE_CALL( clEnqueueCopyBuffer (this->activeQueue().id(), this->id(), dest.id(), thisOffset, destOffset, size_bytes, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/*! \brief Copies asynchronously from this Buffer to the destination Buffer.
  *
  * \param thisOffset is the offset in bytes of this Buffer.
  * \param size_bytes is the size in bytes which are transmitted.
  * \param dest is the destination Buffer in which the data is going to be copied.
  * \param destOffset is the offset in bytes of the destination Buffer.
  * \param list contains all events for which this command has to wait.
  * \return event which can be integrated into other EventList.
  */
ocl::Event ocl::Buffer::copyToAsync( size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const ocl::EventList &list)
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
	cl_event event_id;
    OPENCL_SAFE_CALL ( clEnqueueCopyBuffer (this->activeQueue().id(), this->id(), dest.id(), thisOffset, destOffset, size_bytes,
																				 list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}


/*! \brief Copies from this Buffer to the destination Buffer.
  *
  * The operation assumes that all data are valid and no synchronization is necessary (active Queue executes in-order).
  * The operation forces that all commands within the active Queue including this one are completed.
  *
  * \param queue is a command queue on which the command is executed.
  * \param thisOffset is the offset in bytes of this Buffer.
  * \param size_bytes is the size in bytes which are transmitted.
  * \param dest is the destination Buffer in which the data is going to be copied.
  * \param destOffset is the offset in bytes of the destination Buffer.
  */
void ocl::Buffer::copyTo (const ocl::Queue& queue, size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const ocl::EventList &list) const
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(this->id() != dest.id(), "Buffer must not be equal this->id() " << this->id() << "; other.id " << dest.id());
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL( clEnqueueCopyBuffer (queue.id(), this->id(), dest.id(), thisOffset, destOffset, size_bytes, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/*! \brief Copies asynchronously from this Buffer to the destination Buffer.
  *
  * \param queue is a command queue on which the command is executed.
  * \param thisOffset is the offset in bytes of this Buffer.
  * \param size_bytes is the size in bytes which are transmitted.
  * \param dest is the destination Buffer in which the data is going to be copied.
  * \param destOffset is the offset in bytes of the destination Buffer.
  * \param list contains all events for which this command has to wait.
  * \return event which can be integrated into other EventList.
  */
ocl::Event ocl::Buffer::copyToAsync(const ocl::Queue& queue, size_t thisOffset, size_t size_bytes, const Buffer & dest, size_t destOffset, const ocl::EventList &list)
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    cl_event event_id;
    OPENCL_SAFE_CALL ( clEnqueueCopyBuffer (queue.id(), this->id(), dest.id(), thisOffset, destOffset, size_bytes,
                                                                                 list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}


/*! \brief Maps the Buffer into the host memory.
  *
  *  No data transfer is performed. Note that in order to map data of the Buffer the active queue must be a cpu and must have been allocated
  *  with the Buffer access mode AllocHost. You cannot modify the Buffer with OpenCL until unmap.
  * \param offset is the offset in bytes from which the Buffer is read.
  * \param size_bytes is the number of bytes to be mapped.
  * \param access specifies in what way the host_mem is used.
  * \returns a void pointer to the mapped host memory location
  */
void * ocl::Buffer::map ( size_t offset, size_t size_bytes, Memory::Access access ) const
{
    TRUE_ASSERT(this->activeQueue().device().isCpu(), "Device " << this->activeQueue().device().name() << " is not a cpu!");
	cl_int status;
	cl_map_flags flags = access;
    void *pointer = clEnqueueMapBuffer(this->activeQueue().id(), this->id(), CL_TRUE, flags, offset, size_bytes,  0, NULL, NULL, &status);
	OPENCL_SAFE_CALL (status ) ;
	TRUE_ASSERT(pointer != NULL, "Could not map buffer");
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
	return pointer;
}

/*! \brief Maps the Buffer into the host memory.
  *
  *  No data transfer is performed. Note that in order to map data of the Buffer the active queue must be a cpu and must have been allocated
  *  with the Buffer access mode AllocHost. You cannot modify the Buffer with OpenCL until unmap.
  * \param offset is the offset in bytes from which the Buffer is read.
  * \param size_bytes is the number of bytes to be mapped.
  * \param access specifies in what way the host_mem is used.
  * \returns a void pointer to the mapped host memory location
  */
void * ocl::Buffer::map ( Memory::Access access ) const
{
    TRUE_ASSERT(this->activeQueue().device().isCpu(), "Device " << this->activeQueue().device().name() << " is not a cpu!");
	cl_int status;
	cl_map_flags flags = access;
    void *pointer = clEnqueueMapBuffer(this->activeQueue().id(), this->id(), CL_TRUE, flags, 0, this->size_bytes(),  0, NULL, NULL, &status);
	OPENCL_SAFE_CALL (status ) ;
	TRUE_ASSERT(pointer != NULL, "Could not map buffer");
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
	return pointer;
}

/*! \brief Maps the Buffer into the host memory.
  *
  *  No data transfer is performed. Note that in order to map data of the Buffer the active queue must be a cpu and must have been allocated
  *  with the Buffer access mode AllocHost. You cannot modify the Buffer with OpenCL until unmap. Eventlist specifies the events before
  *  map is performed.
  * \param host_mem is returned and contains the address of a pointer of the host memory.
  * \param offset is the offset in bytes from which the Buffer is read.
  * \param size_bytes is the number of bytes to be mapped.
  * \param access specifies in what way the host memory is used.
  * \param list contains all events for which this command has to wait.
  * \return event which can be integrated into other EventList.
  */
ocl::Event ocl::Buffer::mapAsync ( void ** host_mem, size_t offset, size_t size_bytes, Memory::Access access, const ocl::EventList & list) const
{
	cl_event event_id;
	cl_int status;
	cl_map_flags flags = access;
    *host_mem = clEnqueueMapBuffer(this->activeQueue().id(), this->id(), CL_FALSE, flags, offset, size_bytes,
																		 list.size(), list.events().data(), &event_id, &status);
	OPENCL_SAFE_CALL (status ) ;
	TRUE_ASSERT(*host_mem != NULL, "Could not map buffer");

    return ocl::Event(event_id, this->context());
}

/*! \brief Transfers data from this Buffer to the host memory.
  *
  * You can be sure that the data is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param offset is the offset in bytes from which the Buffer is read
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::read ( size_t offset, void * host_mem, size_t size_bytes, const EventList & list ) const
{
	TRUE_ASSERT(host_mem != NULL, "data == 0");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(this->activeQueue().id(), this->id(), CL_TRUE, offset, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/*! \brief Transfers data from this Buffer to the host memory.
  *
  * You can be sure that the data is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::read ( void * host_mem, size_t size_bytes, const EventList & list) const
{
	TRUE_ASSERT(host_mem != NULL, "data == 0");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(this->activeQueue().id(), this->id(), CL_TRUE, 0, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/*! \brief Transfers data from this Buffer to the host memory.
  *
  * You can be sure that the data is read. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param offset is the offset in bytes from which the Buffer is read
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::read (const ocl::Queue& queue, size_t offset, void * host_mem, size_t size_bytes, const EventList & list ) const
{
    TRUE_ASSERT(host_mem != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(queue.id(), this->id(), CL_TRUE, offset, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/*! \brief Transfers data from this Buffer to the host memory.
  *
  * You can be sure that the data is read. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::read (const ocl::Queue& queue, void * host_mem, size_t size_bytes, const EventList & list) const
{
    TRUE_ASSERT(host_mem != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(queue.id(), this->id(), CL_TRUE, 0, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}


/*! \brief Transfers data from this Buffer to the host memory.
  *
  * Waits until the event list is completed.
  * \param offset is the offset in bytes from which the Buffer is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered.
  * \param list contains all events for which this command has to wait.
  * \returns an event which can be further put into an event list for synchronization.
*/
ocl::Event
ocl::Buffer::readAsync (size_t offset, void *host_mem, size_t size_bytes, const ocl::EventList & list) const
{
	cl_event event_id;
	TRUE_ASSERT(host_mem != NULL, "data == 0");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(this->activeQueue().id(), this->id(), CL_FALSE, offset, size_bytes, host_mem, list.size(), list.events().data(), &event_id) );
    return Event(event_id, this->context());
}

/*! \brief Transfers data from this Buffer to the host memory.
  *
  * Waits until the event list is completed. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param offset is the offset in bytes from which the Buffer is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered.
  * \param list contains all events for which this command has to wait.
  * \returns an event which can be further put into an event list for synchronization.
*/
ocl::Event
ocl::Buffer::readAsync (const ocl::Queue& queue, size_t offset, void *host_mem, size_t size_bytes, const ocl::EventList & list) const
{
    cl_event event_id;
    TRUE_ASSERT(host_mem != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL ( clEnqueueReadBuffer(queue.id(), this->id(), CL_FALSE, offset, size_bytes, host_mem, list.size(), list.events().data(), &event_id) );
    return Event(event_id, this->context());
}

/*! \brief Transfers data from host memory to this Buffer.
  *
  * You can be sure that the data is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::write (const void * host_mem, size_t size_bytes, const EventList & list ) const
{
	TRUE_ASSERT(host_mem != NULL, "hostMem == 0");
    OPENCL_SAFE_CALL (  clEnqueueWriteBuffer(this->activeQueue().id(), this->id(), CL_TRUE, 0, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/*! \brief Transfers data from host_memory to this Buffer.
  *
  * You can be sure that the data is read.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param offset is the offset in bytes from which the Buffer is read
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::write (size_t offset, const void * host_mem, size_t size_bytes, const EventList & list ) const
{
	TRUE_ASSERT(host_mem != NULL, "hostMem == 0");
    OPENCL_SAFE_CALL (  clEnqueueWriteBuffer(this->activeQueue().id(), this->id(), CL_TRUE, offset, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/*! \brief Transfers data from host memory to this Buffer.
  *
  * You can be sure that the data is read. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::write (const ocl::Queue& queue, const void * host_mem, size_t size_bytes, const EventList & list ) const
{
    TRUE_ASSERT(host_mem != NULL, "hostMem == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL (  clEnqueueWriteBuffer(queue.id(), this->id(), CL_TRUE, 0, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/*! \brief Transfers data from host_memory to this Buffer.
  *
  * You can be sure that the data is read. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param offset is the offset in bytes from which the Buffer is read
  * \param size_bytes are the number of bytes which are transfered
*/
void ocl::Buffer::write (const ocl::Queue& queue, size_t offset, const void * host_mem, size_t size_bytes, const EventList & list ) const
{
    TRUE_ASSERT(host_mem != NULL, "hostMem == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL (  clEnqueueWriteBuffer(queue.id(), this->id(), CL_TRUE, offset, size_bytes, host_mem, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}


/*! \brief Transfers data from host memory to this Buffer.
  *
  * Waits until the event list is completed.
  * \param offset is the offset in bytes from which the Buffer is written.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered.
  * \param list contains all events for which this command has to wait.
  * \returns an event which can be further put into an EventList for synchronization.
*/
ocl::Event ocl::Buffer::writeAsync (size_t offset, const void * host_mem, size_t size_bytes, const ocl::EventList & list) const
{
	cl_event event_id;
	TRUE_ASSERT(host_mem != NULL, "data == 0");
    OPENCL_SAFE_CALL ( clEnqueueWriteBuffer(this->activeQueue().id(), this->id(), CL_FALSE, offset, size_bytes, host_mem,
                                                                                 list.size(), list.events().data(), &event_id) );
    return Event(event_id, this->context());
}

/*! \brief Transfers data from host memory to this Buffer.
  *
  * Waits until the event list is completed. Be sure that the queue
  * and this buffer are in the same context.
  * \param queue is a command queue on which the command is executed.
  * \param offset is the offset in bytes from which the Buffer is written.
  * \param host_mem must point to a memory location with size_bytes available.
  * \param size_bytes are the number of bytes which are transfered.
  * \param list contains all events for which this command has to wait.
  * \returns an event which can be further put into an EventList for synchronization.
*/
ocl::Event ocl::Buffer::writeAsync (const ocl::Queue& queue, size_t offset, const void * host_mem, size_t size_bytes, const ocl::EventList & list) const
{
    cl_event event_id;
    TRUE_ASSERT(host_mem != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL ( clEnqueueWriteBuffer(queue.id(), this->id(), CL_FALSE, offset, size_bytes, host_mem,
                                                                                 list.size(), list.events().data(), &event_id) );
    return Event(event_id, this->context());
}

/*! \brief Copies data from other Buffer to this Buffer.
  *
  * \param other Buffer which is copied.
  * \returns this Buffer.
  */
ocl::Buffer & ocl::Buffer::operator= ( const Buffer & other )
{
    TRUE_ASSERT(this->context() == other.context(), "context must be equal");
    this->recreate(other.size_bytes());
    other.copyTo(0, other.size_bytes(), *this, 0);
	return *this;
}

/*! \brief Moves data from other Buffer to this Buffer.
  *
  * No data is copied here.
  * \param other Buffer which is moved.
  * \returns this Buffer.
  */
ocl::Buffer & ocl::Buffer::operator= ( Buffer && other )
{
    if(this == &other) return *this;
    ocl::Memory::operator =(std::move(other));
    return *this;
}

/*! \brief Acquires access to this Buffer.
  *
  * Access is aquired to this Buffer. Only OpenCL can access this Buffer now.
  * \param q is the active OpenCL queue.
  * \returns whether acquiring was successful or not.
  */

#ifdef __OPENGL__
cl_int ocl::Buffer::acquireAccess(Queue &q) 
{
    glFinish();
    return clEnqueueAcquireGLObjects(q.id(), 1, &this->_id, NULL, NULL, NULL);
}
#endif

/*! \brief Releases access to this Buffer.
  *
  * Access is released to this Buffer.
  * \param q is the active OpenCL queue.
  * \returns whether releasing was successful or not.
  */
#ifdef __OPENGL__
cl_int ocl::Buffer::releaseAccess(Queue &q, const EventList& list) {
    cl_event event_id;
    return clEnqueueReleaseGLObjects(q.id(), 1, &this->_id, list.size(), list.events().data(), &event_id);
}
#endif
