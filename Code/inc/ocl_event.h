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


#ifndef OCL_EVENT_H
#define OCL_EVENT_H


#include <vector>
#include <string>
#include <memory>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif



using namespace std;


namespace ocl{
class Context;

/*! \class Event ocl_event.h "inc/ocl_event.h"
  * \brief Wrapper for cl_event
  *
  * Event object created either by the user (User event) or by the corresponding command Queue.
  * An event object can be used to track the execution status of a command. The API calls that
  * enqueue commands to a command Queue create a new event object that is returned. Event objects also serve
  * as synchronization points when created as user Event objects.The Event objects correspond to
  * a synchronization points. Be carefull with Memory usage. Only free and
  * reuse Memory objects if the Event objects are completed.
  */
class Event
{
public:
    Event(cl_event, ocl::Context*);
    Event(ocl::Context&);
    Event();
    Event(const Event & other);
	~Event ();
	cl_command_type commandType () const;
    cl_event id () const;
    size_t startTime() const;
    size_t finishTime() const;
    size_t submitTime() const;
    size_t queueTime() const;

	bool isErrored () const;
	bool isCompleted () const;
	bool isQueued () const;
	bool isRunning () const;
	bool isSubmitted () const;

    void release();

    Context& context() const;
    size_t reference_count() const;

	void 	waitUntilCompleted() const;

	bool 	operator!= ( const Event & other ) const;
	bool 	operator== ( const Event & other ) const;
	
	Event& operator =(const Event & other); 

private:
	cl_event _id;
    Context* _ctxt;
};

}

#endif
