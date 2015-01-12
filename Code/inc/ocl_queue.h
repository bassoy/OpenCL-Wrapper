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

#ifndef OCL_QUEUE_H
#define OCL_QUEUE_H

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/opencl.h>
#endif

/*! \file ocl_queue.h "inc/ocl_queue.h"
  * \brief Wrapper file for cl_command_queue  */


namespace ocl{


struct QueueProp
{

};


class Context;
class Device;
class EventList;

/*! \class Queue
  * \brief Wrapper for cl_command_queue
  *
  * OpenCL objects such as Memory, Program and Kernel objects are created using a context.
  * Operations on these objects are performed using a command-queue. The command-queue can be
  * used to queue a set of operations (referred to as commands) in order.
  * In order to work with command-queue a Context and Device has to be specified.
  */

class Queue
{
public:
    typedef cl_command_queue_properties props; /*!< Choose In-order/Out-of-order and/or Profiling for this Queue. */
    Queue();
    Queue(const Device&, props = 0);
    Queue(Context&, const Device&, props = 0);
	~Queue();

    void setDevice(const Device&);
    void setContext(Context&);

    void create(Context *c = 0);
    size_t reference_count() const;
	void release();
	bool created() const;


	bool operator == (const Queue &) const;
	bool operator != (const Queue &) const;
	bool operator == (cl_command_queue) const;
	bool operator != (cl_command_queue) const;

	cl_command_queue id() const;

	void flush() const;
	void finish() const;
	const Context& context() const;
	const Device& device() const;
    props properties() const;
    void barrier(const EventList&) const;

    Queue& operator =( Queue const& ) = delete;
    
    Queue( Queue const& ) = delete;

private:
	const Device *_device;
    Context *_context;
    props _props;
	cl_command_queue _id;

};

}

#endif
