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

#include <ocl_platform.h>
#include <ocl_query.h>
#include <ocl_device.h>
#include <ocl_queue.h>
#include <ocl_context.h>
#include <ocl_event_list.h>

#include <algorithm>
#include <sstream>

/*! \brief Instantiates a Queue
  *
  * Note: no Device and Context chosen. No OpenCL Queue is created. Must do this later.
  */
ocl::Queue::Queue() :
    _device(nullptr), _context(nullptr), _props(0), _id(nullptr)
{
}

/*! \brief Instantiates a Queue
  *
  * Note: no Context chosen. No OpenCL Queue is created. Must do this later.
  * \param dev Device on which the Queue is executed.
  * \param props Properties with which the Queue is created.
  */
ocl::Queue::Queue(const ocl::Device& dev, const Queue::props props) :
	_device(&dev), _context(nullptr), _props(props), _id(nullptr)
{
}

/*! \brief Instantiates a Queue ready for use
  *
  * No need to create the OpenCL queue. This is done in this constructor.
  *
  * \param ctxt Context within which the Queue is executed. The Device specified must be within the Context.
  * \param dev Device on which the Queue is executed.
  * \param props Properties with which the Queue is created.
  */
ocl::Queue::Queue(ocl::Context& ctxt, const ocl::Device& dev, Queue::props props) :
	_device(&dev), _context(&ctxt), _props(props), _id(nullptr)
{

	this->create();
}

/*! \brief Sets the device for this Queue.
  *
  * \param dev Device on which this Queue will run.
  */
void ocl::Queue::setDevice(const ocl::Device &dev)
{
	if(_device != nullptr) throw std::runtime_error("device already set for this queue");
	if(_context == nullptr) throw std::runtime_error("context for this queue not yet set");
	if(!_context->has(dev)) throw std::runtime_error("context does not include the dev");
    _device = &dev;
}

/*! \brief Sets the device for this Queue.
  *
  * \param ctxt Context for which this Queue will run.
  */
void ocl::Queue::setContext(ocl::Context &ctxt)
{
	if(_context != nullptr) throw std::runtime_error("context already set for this queue");
    _context = &ctxt;
}

#if CL_VERSION_2_0
static bool supportsAtLeast2Point0( cl_platform_id id )
{
  char version[128];
  size_t versionLen = 0;
  
  clGetPlatformInfo( id, CL_PLATFORM_VERSION, 128, version, &versionLen );
  
  int major = 0, minor = 0;
  
  std::sscanf( version, "OpenCL %i.%i", &major, &minor );
  
  return major > 1;
}
#endif

/*! \brief Creates cl_command_queue for this Queue.
  *
  * This is only needed when the Queue is not instantiated with a Device and a Queue.
  *
  * \param ctxt Context for which this Queue will run. If not specified the set context will be taken.
  */
void ocl::Queue::create(ocl::Context * ctxt)
{
	if(ctxt == 0){
		if(_context == nullptr) throw std::runtime_error("this queue must have a valid context");
	}
	else {
		if(_context != ctxt && _context != nullptr) throw std::runtime_error("cannot have different contexts for the same program");
		_context = ctxt;
	}

	cl_int status;
	
#if CL_VERSION_2_0
  if ( supportsAtLeast2Point0( device().platform() ) )
  {
    cl_queue_properties propties[] = {
      CL_QUEUE_PROPERTIES, this->properties(),
      0
    };
    
    _id = clCreateCommandQueueWithProperties( this->context().id(), this->device().id(), propties, &status );
  }
  else
#endif
  {
    _id = clCreateCommandQueue(this->context().id(), this->device().id(), this->properties(), &status);
  }
  
	OPENCL_SAFE_CALL(status);
	if(_id == nullptr) throw std::runtime_error("could not create command queue");
    _context->insert(this);
}

/*! \brief Releases the this Queue.
  *
  */
void ocl::Queue::release()
{
    if(this->created()){
        OPENCL_SAFE_CALL( clReleaseCommandQueue (_id));
    }
	_device = 0;
	_context = 0;
    _id = 0;
}

/*! \brief Returns true if this Queue is created.
  *
  */
bool ocl::Queue::created() const
{
	return _id != NULL;
}

/*! \brief Returns the number of reference counts.
  *
  */
size_t ocl::Queue::reference_count() const
{
	if(_id == nullptr) throw std::runtime_error("could not performance a reference count");

    cl_uint info;
	OPENCL_SAFE_CALL( clGetCommandQueueInfo (_id, CL_QUEUE_REFERENCE_COUNT, sizeof(info), &info, NULL)) ;
	return size_t(info);
}

/*! \brief Destructs this Queue.
  *
  */
ocl::Queue::~Queue()
{
    this->release();
}



/*! \brief Returns true if both Queues refer to the same OpenCL cl_command_queue
  *
  */
bool ocl::Queue::operator == (const Queue& q) const
{
	return this->_id == q.id();
}

/*! \brief Returns false if both Queues refer to the same OpenCL cl_command_queue
  *
  */
bool ocl::Queue::operator != (const Queue& q) const
{
	return this->_id != q.id();
}

/*! \brief Returns true if this Queue refers to the same OpenCL cl_command_queue
  *
  */
bool ocl::Queue::operator == (cl_command_queue q) const
{
	return this->_id == q;
}

/*! \brief Returns false if this Queue refers to the same OpenCL cl_command_queue
  *
  */
bool ocl::Queue::operator != (cl_command_queue q) const
{
	return this->_id != q;
}

/*! \brief Returns OpenCL cl_command_queue for this Queue.
  *
  */
cl_command_queue ocl::Queue::id() const
{
	return this->_id;
}

/*! \brief Issues all commands in this Queue are issued to the associated devices.
  *
  * clFlush only guarantees that all queued commands to command_queue
  * will eventually be submitted to the appropriate device. There is no guarantee that they will be
  * complete after clFlush returns.
*/
void ocl::Queue::flush() const
{
	OPENCL_SAFE_CALL(  clFlush(this->id() ) );
}

/*! \brief Blocks until all commands in this Queue are issued to the associated device and have completed.
  *
  * clFinish does not return until all previously queued commands in command_queue have been processed and completed.
*/
void ocl::Queue::finish() const
{
	OPENCL_SAFE_CALL(  clFinish(this->id() ) );
}

/*! \brief Blocks until all commands in this Queue are issued to the associated device and have completed.
  *
  * clFinish does not return until all previously queued commands in command_queue have been processed and completed.
*/
void ocl::Queue::barrier(const EventList&  list ) const
{
    OPENCL_SAFE_CALL(  clEnqueueBarrierWithWaitList (this->id(),  list.size(), list.events().data(), 0) );
}

/*! \brief Returns the Context for this Queue.
  *
  */
const ocl::Context& ocl::Queue::context() const
{
	if(_context == nullptr) throw std::runtime_error("there is no context for this queue");
	return *this->_context;
}

/*! \brief Returns the Device for this Queue.
  *
  */
const ocl::Device& ocl::Queue::device() const
{
	if(_device == nullptr) throw std::runtime_error("there is no device for this queue");
	return *this->_device;
}

/*! \brief Returns the properties for this Queue.
  *
  */
ocl::Queue::props ocl::Queue::properties() const
{
	return this->_props;
}



