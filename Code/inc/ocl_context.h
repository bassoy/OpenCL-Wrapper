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

#ifndef OCL_CONTEXT_H
#define OCL_CONTEXT_H

#include <vector>
#include <map>
#include <set>
#include <string>

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
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif

#include <ocl_device.h>


namespace ocl{

class Kernel;
class Program;
class Queue;
class Platform;
class Event;
class Memory;
class Sampler;


/*! \class Context ocl_context.h "inc/ocl_context.h"
  * \brief Wrapper for cl_context.
  *
  * A Context can be set up for one or several Device objects in the same Platform
  * with several command Queue objects attached to Device objects. There might several Context objects
  * for a Platoform. A Context is responsible for
  * Kernel, Program, Event and Memory objects. Manipulation on these are performed using
  * command Queue objects.
  * <br>
  * Kernel, Program, Event and Memory objects do insert themselves
  * into a Context when their constructed either by taking
  * the current active Context which has to be set by the user or
  * when it is provided in their argument list.
  *
  * Note that there can only be one active Context within one Platform.
*/

class Context
{

public:

	Context(cl_context, bool shared = false);
	Context(const Platform &, bool shared = false);
	Context(const std::vector<Device>&, bool shared = false);
	Context(const Device&, bool shared = false);
	Context(const Device&, const Device&, bool shared = false);
	Context();

	~Context();

	void setDevices(const Platform&);
	void setDevices(const std::vector<Device>&);
	void setDevices(const Device&);

	cl_context id() const;

	bool operator==(const Context &) const;
	bool operator!=(const Context &) const;

	void create(bool shared = false);
	bool created() const;
	void release();

	void insert(Program*);
	void insert(Queue*);
	void insert(Memory*);
	void insert(Event*);
	void insert(Sampler*);

	void release(Event*);
	void release(Program*);
	void release(Queue*);
	void release(Memory*);
	void release(Sampler*);

	void remove(Event*);
	void remove(Program*);
	void remove(Queue*);
	void remove(Memory*);
	void remove(Sampler*);

	bool has(const Device&)  const;
	bool has(DeviceType) const;

	bool has(const Program&) const;
	bool has(const Queue&)   const;
	bool has(const Memory&)  const;
	bool has(const Event&)   const;
	bool has(const Sampler&) const;


	Program& activeProgram() const;
	void setActiveProgram(Program&);

	Queue& activeQueue() const;
	void setActiveQueue(Queue&);


	const std::set<Event*>    & events() const;
	const std::set<Memory*>   & memories() const;
	const std::set<Queue*>    & queues() const;
	const std::set<Sampler*>  & samplers() const;
	const std::vector<Device> & devices() const;

	std::vector<cl_device_id> cl_devices() const;


protected:

	cl_context _id;                  /**< OpenCL context. */

	std::set<Program*>  _programs;    /**< OpenCL programs which shall run on the context. */
	std::set<Queue*>    _queues;
	std::set<Event*>    _events;
	std::set<Memory*>   _memories;
	std::set<Sampler*>  _samplers;
	std::vector<Device> _devices;

	Queue* _activeQueue;
	Program* _activeProgram;

};

}

#endif
