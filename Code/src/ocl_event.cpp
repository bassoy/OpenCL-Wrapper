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

#include <ocl_event.h>
#include <ocl_query.h>

#include <utl_assert.h>

#include <ocl_platform.h>
#include <ocl_context.h>

/*! \brief Instantiates an Event returned by an command Queue instruction.
  *
  * Do not instantiate user events with this constructor.
  *
  * \param id is an OpenCL event id provided by the creating command Queue instruction.
  * \param ctxt is a valid Context provided which is the same as the command queue Context.
  */
ocl::Event::Event(cl_event id, ocl::Context* ctxt) : _id(id), _ctxt(ctxt)
{
    TRUE_ASSERT(id != 0, "Event not valid.");
    TRUE_ASSERT(ctxt != 0, "Context not valid");

    cl_context cl_ctxt = 0;
    OPENCL_SAFE_CALL( clGetEventInfo (this->id(), CL_EVENT_CONTEXT , sizeof(cl_ctxt), &cl_ctxt, NULL));
    TRUE_ASSERT(_ctxt->id() == cl_ctxt, "Context must be the same");
}


/*! \brief Instantiates a user Event.
  *
  * You can add this event to track a command within a command Queue by adding this
  * Event into the EventList of the command.
  * If there is an active Platform and an active Context
  * this Event is created. Otherwise do not forget
  * to provide a Context and to create this Event.
  */
ocl::Event::Event() : _id(0), _ctxt(0)
{
    if(ocl::Platform::hasActivePlatform() && ocl::Platform::activePlatform()->hasActiveContext()){
        _ctxt = ocl::Platform::activePlatform()->activeContext();
        TRUE_ASSERT(_ctxt != 0, "No active context");
        cl_int err;
        _id =  clCreateUserEvent (_ctxt->id(), &err);
        OPENCL_SAFE_CALL(err);
        TRUE_ASSERT(_id != 0, "Could not create user event");
    }
}

/*! \brief Instantiates a user Event.
  *
  * You can add this event to track a command within a command Queue by adding this
  * Event into the EventList of the command. This
  * Event is created using the provided Context.
  */
ocl::Event::Event(ocl::Context& ctxt) : _id(0), _ctxt(&ctxt)
{
    TRUE_ASSERT(this->_ctxt != 0, "Context not valid");

    cl_int err;
    _id =  clCreateUserEvent (_ctxt->id(), &err);
    OPENCL_SAFE_CALL(err);
    TRUE_ASSERT(_id != 0, "Could not create user event");
}


/*! \brief Copies the Event.
  *
  * Note that no deep copy is performed. Both Event Objects
  * refer to the same OpenCL event.
  *
  * \param other Event from which the OpenCL Event and Context is taken from.
  */
ocl::Event::Event( const Event & other ) : _id(other._id), _ctxt(other._ctxt)
{
    TRUE_ASSERT(_id != 0, "Event not valid (id == 0)");
    TRUE_ASSERT(_ctxt != 0, "Event not valid (ctxt == 0)");

	OPENCL_SAFE_CALL( clRetainEvent( _id ) );
}


ocl::Event& ocl::Event::operator =( ocl::Event const& other )
{

	if ( this == &other ) return *this;

	TRUE_ASSERT(other._id != 0, "Event not valid (id == 0)");
	TRUE_ASSERT(other._ctxt != 0, "Event not valid (ctxt == 0)");

	release();

	_id   = other._id;
	_ctxt = other._ctxt;

	clRetainEvent( _id );
	return *this;
}


/*! \brief Destructs the Event.
  *
  */
ocl::Event::~Event ()
{
    this->release();
}

/*! \brief Releases the Event.
  */
void ocl::Event::release()
{
    if(_id != 0 ){
        OPENCL_SAFE_CALL( clReleaseEvent (this->_id) );
    }
    _ctxt = 0;
    _id = 0;
}

/*! \brief Returns the corresponding command type associated with this event. */
cl_command_type ocl::Event::commandType () const
{
	cl_command_type __type;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(), CL_EVENT_COMMAND_TYPE, sizeof(__type), &__type, NULL) );
	return __type;
}


/*! \brief Returns the OpenCL Event for this Event. */
cl_event ocl::Event::id () const
{
	return this->_id;
}

/*! \brief Returns the starting time of the corresponding command in nanoseconds
  *
  * CL_QUEUE_PROFILING_ENABLE flag must be set when the corresponding
  * Queue is created in order to use this function.
  */
size_t ocl::Event::startTime () const
{
	cl_ulong info = 0;
	OPENCL_SAFE_CALL(  clGetEventProfilingInfo (this->id(), CL_PROFILING_COMMAND_START , sizeof(info), &info, NULL));
    return size_t(info);
}

/*! \brief Returns the end time of the corresponding command in nanoseconds
  *
  * CL_QUEUE_PROFILING_ENABLE flag must be set when the corresponding
  * Queue is created in order to use this function.
  */
size_t ocl::Event::finishTime () const
{
	cl_ulong info = 0;
	OPENCL_SAFE_CALL(  clGetEventProfilingInfo (this->id(), CL_PROFILING_COMMAND_END, sizeof(info), &info, NULL));
    return size_t(info);
}

/*! \brief Returns the submit time of the corresponding command in nanoseconds
  *
  * CL_QUEUE_PROFILING_ENABLE flag must be set when the corresponding
  * Queue is created in order to use this function.
  */
size_t ocl::Event::submitTime () const
{
	cl_ulong info = 0;
	OPENCL_SAFE_CALL(  clGetEventProfilingInfo (this->id(), CL_PROFILING_COMMAND_SUBMIT, sizeof(info), &info, NULL));
    return size_t(info);
}

/*! \brief Returns the queue time of the corresponding command in nanoseconds
  *
  * CL_QUEUE_PROFILING_ENABLE flag must be set when the corresponding
  * Queue is created in order to use this function.
  */
size_t ocl::Event::queueTime () const
{
	cl_ulong info = 0;
	OPENCL_SAFE_CALL(  clGetEventProfilingInfo (this->id(), CL_PROFILING_COMMAND_QUEUED, sizeof(info), &info, NULL));
    return size_t(info);
}

/*! \brief Returns true if the executing command returned an error.*/
bool ocl::Event::isErrored () const
{
	cl_int info;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(),CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(info), &info, NULL));
	return info < 0;
}

/*! \brief Returns true if the executing command is completed.*/
bool ocl::Event::isCompleted() const
{
	cl_int info;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(),CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(info), &info, NULL));
	return info == CL_COMPLETE;
}

/*! \brief Returns true if the executing command is queued.*/
bool ocl::Event::isQueued () const
{
	cl_int info;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(),CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(info), &info, NULL));
	return info == CL_QUEUED;
}

/*! \brief Returns true if the executing command is running.*/
bool ocl::Event::isRunning () const
{
	cl_int info;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(),CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(info), &info, NULL));
	return info == CL_RUNNING;
}

/*! \brief Returns true if the executing command is submitted.*/
bool ocl::Event::isSubmitted () const
{
	cl_int info;
	OPENCL_SAFE_CALL(  clGetEventInfo (this->id(),CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(info), &info, NULL));
	return info == CL_SUBMITTED;
}

/*! \brief Returns the context with which this Event was created.*/
ocl::Context& ocl::Event::context() const
{
    TRUE_ASSERT(this->_ctxt != 0, "No Context for Event");
    return *this->_ctxt;
}

/*! \brief Returns the reference count of this Event object.*/
size_t ocl::Event::reference_count() const
{
    TRUE_ASSERT(_id != 0, "Cannot get reference count for this. Not yet created.");
    cl_uint info;
    OPENCL_SAFE_CALL( clGetEventInfo (_id, CL_EVENT_REFERENCE_COUNT, sizeof(info), &info, NULL)) ;
    return size_t(info);
}

/*! \brief Forces commands within a command Queue associated with this Event to wait until
  * the commmands to be completed.
  */
void ocl::Event::waitUntilCompleted() const
{
	OPENCL_SAFE_CALL( clSetUserEventStatus (this->id(), CL_COMPLETE) );
}

/*! \brief Returns true if both Event s have the same OpenCL Event.*/
bool ocl::Event::operator!= ( const Event & other ) const
{
	return other.id() != this->id();
}

/*! \brief Returns true if both Event s do not have the same OpenCL Event.*/
bool ocl::Event::operator== ( const Event & other ) const
{
	return other.id() == this->id();
}
