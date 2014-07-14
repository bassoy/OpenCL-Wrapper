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

#include <sstream>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include <ocl_query.h>
#include <ocl_context.h>
#include <ocl_program.h>
#include <ocl_kernel.h>
#include <ocl_queue.h>
#include <ocl_platform.h>
#include <ocl_device.h>
#include <ocl_device_type.h>
#include <ocl_buffer.h>
#include <ocl_sampler.h>

#include <utl_assert.h>


/*! \brief Instantiates this Context.
  *
  * Instantiates this Context with a given cl_context. Device objects
  * associated with cl_context will be taken and this is created. Otherwise
  * Do not forget to provide Device objects and create this Context.
  * Do not forget to determine an active Queue.
  *
  * \param id is the cl_context with which this Context is created.
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
ocl::Context::Context(cl_context id, bool shared) :
    _id(id), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(), _activeQueue(NULL), _activeProgram(NULL)
{
    TRUE_ASSERT(_id != 0, "Context not valid");


    size_t num_devices = 0;
    OPENCL_SAFE_CALL( clGetContextInfo (_id, CL_CONTEXT_DEVICES, 0, NULL, &num_devices) );
    if(num_devices>0){
        cl_device_id* devices = new cl_device_id[num_devices];
        OPENCL_SAFE_CALL( clGetContextInfo (_id, CL_CONTEXT_DEVICES, num_devices, devices, NULL) );
        for(size_t i = 0; i < num_devices; ++i) this->_devices.push_back(ocl::Device(devices[i]));
        delete[] devices;
				this->create(shared);
    }

}

/*! \brief Instantiates this Context.
  *
  * Instantiates this Context with the given Device. Do not forget
  * to determine an active Queue. This Context is created.
  *
	* \param device is the Device for this Context.
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
ocl::Context::Context(const ocl::Device&  device, bool shared) :
    _id(NULL), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(), _activeQueue(NULL), _activeProgram(NULL)
{
		_devices.push_back(device);
	this->create(shared);
}


/*! \brief Instantiates this Context.
  *
  * Instantiates this Context with the given Device objects. Do not forget
  * to determine an active Queue. This Context is created.
  *
	* \param device1 is the Device for this Context.
	* \param device2 is the Device for this Context.
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
ocl::Context::Context(const ocl::Device&  device1, const ocl::Device& device2, bool shared) :
    _id(NULL), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(), _activeQueue(NULL), _activeProgram(NULL)
{
		_devices.push_back(device1);
		_devices.push_back(device2);
	this->create(shared);
}

/*! \brief Instantiates this Context.
  *
  * Do not forget to determine a Device and create this Context.
  * Also provide an active Queue.
  */
ocl::Context::Context() :
    _id(NULL), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(), _activeQueue(NULL), _activeProgram(NULL)
{}


/*! \brief Instantiates this Context.
  *
  * Instantiates this Context with the given Device objects. Do not forget
  * to determine an active Queue. This Context is created.
  *
	* \param devices is a list of Device objects for this Context.
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
ocl::Context::Context(const std::vector<Device> & devices, bool shared) :
		_id(NULL), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(devices), _activeQueue(NULL), _activeProgram(NULL)
{
	TRUE_ASSERT(!devices.empty(), "No Devices specified. Cannot create context without devices.");
	this->create(shared);
}

/*! \brief Instantiates this Context.
  *
  * Instantiates this Context with all the given Device objects within
  * the Platform. Do not forget to determine an active Queue.
  * This Context is created.
  *
  * \param p is a Platform from which all Device objects are taken.
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
ocl::Context::Context(const ocl::Platform &p, bool shared) :
    _id(NULL), _programs(), _queues(), _events(), _memories(), _samplers(), _devices(), _activeQueue(NULL), _activeProgram(NULL)
{
    this->_devices = p.devices();
	this->create(shared);
}

/*! \brief Sets all Device objects of the specified Platform for this Context.
  *
  * Do not forget to create this Context.
  * Do not forget to determine an active Queue.
  *
  * \param p is a Platform from which all Device objects are taken.
  */
void ocl::Context::setDevices(const ocl::Platform &p)
{
    this->_devices = p.devices();
}

/*! \brief Sets the specified Device objects for this Context.
  *
  * Do not forget to create this Context.
  * Do not forget to determine an active Queue.
  *
  * \param d is the list of Device objects.
  */
void ocl::Context::setDevices(const std::vector<Device> &d)
{
    _devices = d;
}

/*! \brief Sets the specified Device object for this Context.
  *
  * Do not forget to create this Context.
  * Do not forget to determine an active Queue.
  *
  * \param d is a Device.
  */
void ocl::Context::setDevices(const ocl::Device &d)
{
    _devices.clear();
    _devices.push_back(d);

}

/*! \brief Creates this Context.
  *
  * A Context is not automatically created when instantiated - only
  * when Device objects are not provided. Call this function
  * when Device objects are inserted after instantiation.
  *
  * Do not forget to determine an active Queue.
  *
	* \param shared if true, creates a shared Context for OpenGL interoperability.
  */
void ocl::Context::create(bool shared)
{
	TRUE_ASSERT(_id == 0, "Cannot create a context twice");
	TRUE_ASSERT(!this->_devices.empty(), "No devices specified");
	cl_int status;
	std::vector<cl_device_id> dev = this->cl_devices();
	if(shared == false)
	{
		_id = clCreateContext(NULL, dev.size(), dev.data(), NULL, NULL, &status);
		OPENCL_SAFE_CALL(status);
		TRUE_ASSERT(_id != 0, "Could not create Context");
		return;
	}

	TRUE_COMMENT("Creating a shared context");

	// TODO: check if shared context possible.

	#ifdef __OPENGL__

	std::vector<cl_context_properties> properties;
	#ifdef __APPLE__
			CGLContextObj glContext = CGLGetCurrentContext();
			CGLShareGroupObj shareGroup = CGLGetShareGroup(glContext);
			properties.push_back(CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE);
			properties.push_back((cl_context_properties)shareGroup);
			properties.push_back(0);
	#else
			properties.push_back(CL_GL_CONTEXT_KHR);
			properties.push_back((cl_context_properties) glXGetCurrentContext());
			properties.push_back(CL_GLX_DISPLAY_KHR);
			properties.push_back((cl_context_properties) glXGetCurrentDisplay());
			properties.push_back(CL_CONTEXT_PLATFORM);
			properties.push_back((cl_context_properties) this->devices().front().platform());
			properties.push_back(0);
	#endif

	//size_t ext_size = 1024;
	//char *ext_string = (char*) malloc(ext_size);
	//std::string ext_string(ext_size);
	//int err = clGetDeviceInfo(this->_devices.front().id(), CL_DEVICE_EXTENSIONS, ext_size, ext_string, &ext_size);
	//std::cout << ext_string << std::endl;
	//std::cout << ext_size << std::endl;
	_id = clCreateContext(properties.data(), dev.size(), dev.data(), NULL, NULL, &status);
	OPENCL_SAFE_CALL(status);
	TRUE_ASSERT(_id != 0, "Could not create Context");
	#endif
}

/*! \brief Destructs this Context.
  *
  * Releases all Program, Queue, Event and Memory objects
  * associated with this Context.
  */
ocl::Context::~Context()
{
    this->release();
}


/*! \brief Destructs this Context.
  *
  * Releases all Program, Queue, Event and Memory objects
  * associated with this Context.
  */

void ocl::Context::release()
{
    if(this->_id == 0) return;

    for(auto it = _programs.begin(); it != _programs.end();){
        ocl::Program *p = *it; ++it;
        this->release(p);
    }
    for(auto it = _queues.begin(); it != _queues.end();){
        ocl::Queue *q = *it; ++it;
        this->release(q);
    }
    for(auto it = _events.begin(); it != _events.end();){
        ocl::Event *e = *it; ++it;
        this->release(e);
    }
    for(auto it = _memories.begin(); it != _memories.end();){
        ocl::Memory *m = *it; ++it;
        this->release(m);
    }
    for(auto it = _samplers.begin(); it != _samplers.end();){
        ocl::Sampler *m = *it; ++it;
        this->release(m);
    }

		OPENCL_SAFE_CALL( clReleaseContext( _id ) );

    this->_activeProgram = 0;
    this->_activeQueue = 0;
    this->_id = 0;
}

/*! \brief Returns true if cl_context of both Context objects are the same. */
bool ocl::Context::operator==(const Context &__c) const
{
	return this->id() == __c.id();
}

/*! \brief Returns true if cl_context of both Context objects differ. */
bool ocl::Context::operator!=(const Context &__c) const
{
	return this->id() != __c.id();
}

/*! \brief Returns cl_context the id of this Context. */
cl_context ocl::Context::id() const
{
	return this->_id;
}


/*! \brief Inserts a Memory.
  *
  * Insertion is performed when the Memory
  * is created. This is done by the Memory
  * itself and must not be done by the user.
  *
  * \param mem Memory to be inserted into the set of Memory objects.
  */
void ocl::Context::insert(ocl::Memory *mem)
{
    TRUE_ASSERT(mem != 0, "Memory not valid");
    TRUE_ASSERT(*mem->context() == *this, "Memory has a different context.");
    _memories.insert(mem);
}

/*! \brief Releases a Memory if it belongs to this Context.
  *
  * Release is called if this Context is destructed. You do not
  * have to call this function. The release of a Memory
  * is controlled by the location of its declaration and thus its
  * scope.
  * \param mem Memory to be released and removed from the set of Memory objects.
  */
void ocl::Context::release(ocl::Memory *mem)
{
    TRUE_ASSERT(mem != 0, "Memory not valid");
    if(_memories.find(mem) == _memories.end()) return;
    _memories.erase(mem);
    mem->release();
}


/*! \brief Removes a Memory if it belongs to this Context.
  *
  * Remove is called from a Memory when its scope ends
  * and thus must be destructed. You do not have to call this function.
  *
  * \param mem Memory to be removed from the set of Memory objects.
  */
void ocl::Context::remove(ocl::Memory *mem)
{
    TRUE_ASSERT(mem != 0, "Memory not valid");
    if(_memories.find(mem) == _memories.end()) return;
    _memories.erase(mem);
}

/*! \brief Inserts a Sampler.
  *
  * Insertion is performed when the Sampler
  * is created. This is done by the Sampler
  * itself and must not be done by the user.
  *
  * \param sampler Sampler to be inserted into the set of Sampler objects.
  */
void ocl::Context::insert(ocl::Sampler *sampler)
{
    TRUE_ASSERT(sampler != 0, "Sampler not valid.");
    if(this->has(*sampler)) return;
    TRUE_ASSERT(sampler->context() == *this, "Cannot insert Sampler with a different Context and this Context");
    this->_samplers.insert(sampler);
}

/*! \brief Releases a Sampler if it belongs to this Context.
  *
  * Release is called if this Context is destructed. You do not
  * have to call this function. The release of a Sampler
  * is controlled by the location of its declaration and thus its
  * scope.
  * \param sampler Sampler to be released and removed from the set of Sampler objects.
  */
void ocl::Context::release(ocl::Sampler *sampler)
{
    TRUE_ASSERT(sampler != 0, "Sampler not valid");
    if(_samplers.find(sampler) == _samplers.end()) return;
    _samplers.erase(sampler);
    sampler->release();
}

/*! \brief Removes a Sampler if it belongs to this Context.
  *
  * Remove is called from a Sampler when its scope ends
  * and thus must be destructed. You do not have to call this function.
  *
  * \param sampler Sampler to be removed from the set of Sampler objects.
  */
void ocl::Context::remove(ocl::Sampler *sampler)
{
    TRUE_ASSERT(sampler != 0, "Sampler not valid");
    if(_samplers.find(sampler) == _samplers.end()) return;
    _samplers.erase(sampler);
}

/*! \brief Inserts a Queue.
  *
  * Insertion is performed when the Queue
  * is created. This is done by the Queue
  * itself and must not be done by the user.
  *
  * Do not forget to set an active Queue.
  * \param queue Queue to be inserted into the set of Queue objects.
  */
void ocl::Context::insert(ocl::Queue *queue)
{
    TRUE_ASSERT(queue != 0, "Queue not valid.");
    if(this->has(*queue)) return;
    TRUE_ASSERT(queue->context() == *this, "Cannot insert Queue with a different Context and this Context");
    this->_queues.insert(queue);
}

/*! \brief Releases a Queue if it belongs to this Context.
  *
  * Release is called if this Context is destructed. You do not
  * have to call this function. The release of a Queue
  * is controlled by the location of its declaration and thus its
  * scope.
  *
  * \param queue Queue to be released and removed from the set of Queue objects.
  */
void ocl::Context::release(ocl::Queue *queue)
{
    TRUE_ASSERT(queue != 0, "Queue not valid.");
    if(!this->has(*queue)) return;
    _queues.erase(queue);
    if(queue == _activeQueue) _activeQueue = 0;
    queue->release();
}

/*! \brief Removes a Queue if it belongs to this Context.
  *
  * Remove is called from a Queue when its scope ends
  * and thus must be destructed. You do not have to call this function.
  *
  * \param mem Queue to be removed from the set of Queue objects.
  */
void ocl::Context::remove(ocl::Queue *queue)
{
    TRUE_ASSERT(queue != 0, "Queue not valid");
    if(this->has(*queue)) return;
    if(queue == _activeQueue) _activeQueue = 0;
    _queues.erase(queue);
}


/*! \brief Inserts a Program.
  *
  * Insertion is performed when the Program
  * is created. This is done by the Program
  * itself and must not be done by the user.
  *
  * \param prog Program to be inserted into the set of Program objects.
  */
void ocl::Context::insert(ocl::Program *prog)
{
    TRUE_ASSERT(prog != 0, "Program not valid.");
    if(this->has(*prog)) return;
    TRUE_ASSERT(prog->context() == *this, "Cannot insert Program with a different Context and this Context");
    this->_programs.insert(prog);
}

/*! \brief Releases a Program if it belongs to this Context.
  *
  * Release is called if this Context is destructed. You do not
  * have to call this function. The release of a Program
  * is controlled by the location of its declaration and thus its
  * scope.
  *
  * \param prog Program to be released and removed from the set of Program objects.
  */
void ocl::Context::release(ocl::Program *prog)
{
    TRUE_ASSERT(prog != 0, "Program not valid.");
    if(!this->has(*prog)) return;    
    _programs.erase(prog);
    if(prog == _activeProgram) _activeProgram = 0;
    prog->release();
}

/*! \brief Removes a Program if it belongs to this Context.
  *
  * Remove is called from a Program when its scope ends
  * and thus must be destructed. You do not have to call this function.
  *
  * \param mem Program to be removed from the set of Program objects.
  */
void ocl::Context::remove(ocl::Program *prog)
{
    TRUE_ASSERT(prog != 0, "Program not valid");
    if(!this->has(*prog)) return;
    _programs.erase(prog);
    if(prog == _activeProgram) _activeProgram = 0;
}

/*! \brief Inserts a Event.
  *
  * Insertion is performed when the Event
  * is created. This is done by the Event
  * itself and must not be done by the user.
  *
  * \param event Event to be inserted into the set of Event objects.
  */
void ocl::Context::insert(ocl::Event *event)
{
    TRUE_ASSERT(event != 0, "Event not valid.");
    if(this->has(*event)) return;
    TRUE_ASSERT(event->context() == *this, "Cannot insert Event with a different Context and this Context");
    this->_events.insert(event);
}

/*! \brief Releases a Event if it belongs to this Context.
  *
  * Release is called if this Context is destructed. You do not
  * have to call this function. The release of a Event
  * is controlled by the location of its declaration and thus its
  * scope.
  *
  * \param event Event to be released and removed from the set of Event objects.
  */
void ocl::Context::release(ocl::Event *event)
{
    TRUE_ASSERT(event != 0, "Event not valid.");
    if(this->has(*event)) return;
    _events.erase(event);
    event->release();
}

/*! \brief Removes a Event if it belongs to this Context.
  *
  * Remove is called from a Event when its scope ends
  * and thus must be destructed. You do not have to call this function.
  *
  * \param mem Event to be removed from the set of Event objects.
  */
void ocl::Context::remove(ocl::Event *event)
{
    TRUE_ASSERT(event != 0, "Event not valid");
    if(!has(*event)) return;
    _events.erase(event);
}




/*! \brief Returns the active Queue for this Context.
  *
  * User has to set the active Queue explicitly.
  * In case no active Queue, no command can be
  * executed.
  */
ocl::Queue& ocl::Context::activeQueue() const
{
    TRUE_ASSERT(this->_activeQueue != 0, "No active queue present");
    return *this->_activeQueue;
}

/*! \brief Sets the active Queue for this Context.
  *
  * User has to set the active Queue explicitly.
  * In case no active Queue, no command can be
  * executed.
  */
void ocl::Context::setActiveQueue(ocl::Queue &q)
{
    TRUE_ASSERT(this->has(q), "Queue is not within this Context");
    this->_activeQueue = &q;
}


/*! \brief Returns the active Program for this Context.
  *
  * User has to set the active Queue explicitly.
  */
ocl::Program& ocl::Context::activeProgram() const
{
    TRUE_ASSERT(this->_activeProgram != 0, "No active queue present");
    return *this->_activeProgram;
}

/*! \brief Sets the active Program for this Context.
  *
  * User has to set the active Program explicitly.
  */
void ocl::Context::setActiveProgram(Program &p)
{
    TRUE_ASSERT(this->has(p), "Program is not within this Context");
    TRUE_ASSERT(p.isBuilt(), "Program not yet created.");
    this->_activeProgram = &p;
}


/*! \brief Returns true if this Context has the specified Device. */
bool ocl::Context::has(const Device& d) const
{
    return std::find(this->devices().begin(),this->devices().end(), d) != this->devices().end();
}

/*! \brief Returns true if this Context has the specified Device. */
bool ocl::Context::has(const DeviceType t) const
{
	auto pred = [t](const Device &d){ return d.type() == t;};
	return std::find_if(this->devices().begin(),this->devices().end(), pred) != this->devices().end();
}




/*! \brief Returns true if this Context has the specified Sampler. */
bool ocl::Context::has(const ocl::Sampler& s) const
{
    return this->_samplers.find(const_cast<ocl::Sampler*>(&s)) != this->_samplers.end();
}


/*! \brief Returns true if this Context has the specified Queue. */
bool ocl::Context::has(const ocl::Queue& q) const
{
    return this->_queues.find(const_cast<ocl::Queue*>(&q)) != this->_queues.end();
}

/*! \brief Returns true if this Context has the specified Program. */
bool ocl::Context::has(const ocl::Program& p) const
{
    return this->_programs.find(const_cast<ocl::Program*>(&p)) != this->_programs.end();
}

/*! \brief Returns true if this Context has the specified Event. */
bool ocl::Context::has(const ocl::Event& e) const
{
    return this->_events.find(const_cast<ocl::Event*>(&e)) != this->_events.end();
}



/*! \brief Returns all Memory s for this Context. */
const std::set<ocl::Memory*>& ocl::Context::memories() const
{
    return this->_memories;
}

/*! \brief Returns all Memory s for this Context. */
const std::set<ocl::Sampler*>& ocl::Context::samplers() const
{
    return this->_samplers;
}



/*! \brief Returns all Queue s for this Context. */
const std::set<ocl::Queue*>& ocl::Context::queues() const
{
    return this->_queues;
}

/*! \brief Returns all Device s for this Context. */
const std::vector<ocl::Device>& ocl::Context::devices() const
{
	return _devices;
}

std::vector<cl_device_id> ocl::Context::cl_devices() const
{
	std::vector<cl_device_id> v;
    for(auto d : _devices){
        v.push_back(d.id());
	}
	return v;
}


/*! \brief True if a context is already created. */
bool ocl::Context::created() const
{
	return _id != NULL;
}

