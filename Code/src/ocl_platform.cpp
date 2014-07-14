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
#include <ocl_context.h>
#include <ocl_device_type.h>
#include <utl_stream.h>
#include <utl_assert.h>

#include <algorithm>
#include <sstream>

using namespace std;

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

/*! \brief Instantiates this Platform.
  *
  * Do not forget to set and ID for this
  * object. Use the setID() member function.
  * Do not forget to set an active Context
  * for this Device.
  */
ocl::Platform::Platform() :
    _devices(), _id(0), _activeContext(), _contexts()
{
}

/*! \brief Instantiates this Platform.
  *
  * All Devices within a Platform are loaded into the list of Devices.
  *
  * \param id is the Platform ID with which this
  */
ocl::Platform::Platform(cl_platform_id id) :
    _devices(), _id(0), _activeContext(), _contexts()
{
    TRUE_ASSERT(ocl::exists(id), "Platform does not exist");
    this->create(id);
    const std::vector<cl_device_id>& devs = ocl::devices(id);
    for(auto it = devs.begin(); it != devs.end(); ++it){
        this->_devices.push_back(ocl::Device(*it));
    }
}

/*! \brief Instantiates this Platform which has the specified DeviceType.
  *
  * A Platform is chosen which has the given DeviceType. Outputs an
  * error if no such a DeviceType exists.
  *
  * \param type DeviceType which is searched for within all Platform objects.
  */
ocl::Platform::Platform(const ocl::DeviceType &type) :
    _devices(), _id(0), _activeContext(), _contexts()
{
    cl_platform_id id = ocl::Platform::id(type);
    TRUE_ASSERT(id != 0, "Could not find any platform for type " << type.name());
    this->create(id);

//	this->create(type);
}

/*! \brief Instantiates this Platform which has the specified DeviceTypes.
  *
  * A Platform is chosen which has the given DeviceTypes. Outputs an
  * error if no such DeviceTypes exist.
  *
  * \param types DeviceTypes which are searched for within all Platform objects.
  */

ocl::Platform::Platform(const ocl::DeviceTypes &types) :
    _devices(), _id(0), _activeContext(), _contexts()
{
    cl_platform_id id = ocl::Platform::id(types);
    TRUE_ASSERT(id != 0, "Could not find any platform for type");
    this->create(id);
}



/*! \brief Destructes this Platform.  */
ocl::Platform::~Platform()
{
    for(auto c : this->_contexts)
        c->release();
}


/*! \brief Assigns the specified OpenCL platform to this Platform and
  *  finds all Device objects associated with this Platform.
  *
  * Note that this Platform should not be created before.
*/
void ocl::Platform::create(cl_platform_id id)
{
    TRUE_ASSERT(_id == 0, "Platform is already constructed.");
    TRUE_ASSERT(id != 0, "Platform is not valid");
    TRUE_ASSERT(ocl::exists(id), "Platform does not exist");
    TRUE_ASSERT(this->_contexts.empty(), "Cannot create an already created platform. Context not empty.");
    TRUE_ASSERT(this->_devices.empty(), "Cannot create an already created platform.");

    this->_id = id;

    const vector<cl_device_id>& devs = ocl::devices(this->id());
    for(auto dev : devs)
        this->_devices.push_back(ocl::Device(dev));
}

/*! \brief Assigns an OpenCL platform which contains the DeviceType and
  *  finds all Device objects associated with this Platform.
  *
  * Note that this Platform should not be created before.
  */
void ocl::Platform::create(const ocl::DeviceType& type)
{
    cl_platform_id id = ocl::Platform::id(type);
    TRUE_ASSERT(id != 0, "Could not find any platform for type");
    this->create(id);
}

/*! \brief Assigns an OpenCL platform which contains the DeviceTypes and
  *  finds all Device objects associated with this Platform.
  *
  * Note that this Platform should not be created before.
  */
void ocl::Platform::create(const ocl::DeviceTypes& types)
{
    cl_platform_id id = ocl::Platform::id(types);
    TRUE_ASSERT(id != 0, "Could not find any platform for type");
    this->create(id);
}


/*! \brief Returns true if this Platform has the specified DeviceType.  */
bool ocl::Platform::has(const ocl::DeviceType& t) const
{
    for (auto d: this->_devices)
	{
        if(d == t) return true;
	}
	return false;
}

/*! \brief Returns true if this Platform has the specified DeviceTypes.  */
bool ocl::Platform::has(const ocl::DeviceTypes& types) const
{
    if(_devices.empty()) return false;
    auto devices = _devices;
    for(auto type : types()){
        if(devices.empty()) return false;
        bool found = false;
        for(auto it = devices.begin(); it != devices.end(); ++it){
            if(*it == type){
                found = true;
                devices.erase(it);
                break;
            }            
        }
        if(!found) return false;
    }


	return true;
}

/*! \brief Returns true if this Platform has the specified Device.  */
bool ocl::Platform::has(const ocl::Device &dev) const
{
    return std::find(_devices.begin(), _devices.end(), dev) != _devices.end();
}


/*! \brief Returns an OpenCL platform for a specified DeviceType.
  *
  * Searches the system for a Platform which supports the
  * specified DeviceType.
  *
  * \returns a valid OpenCL platform if the Platform supports the
  * specified DeviceType. Otherwise a NULL is returned.
*/
cl_platform_id ocl::Platform::id(const ocl::DeviceTypes& types)
{
    const auto &platforms = ocl::platforms();
    for(auto p_id : platforms)
    {
        const auto &devs = ocl::devices(p_id);
        ocl::DeviceTypes others;
      	for(auto d_id : devs){
      		others |= DeviceType::type(ocl::deviceType(d_id));
      	}
        if(others.contains(types)) return p_id;
    }
    return 0;
}

/*! \brief Returns an OpenCL platform for a specified DeviceType.
  *
  * Searches the system for a Platform which supports the
  * specified DeviceType.
  *
  * \returns a valid OpenCL platform if the Platform supports the
  * specified DeviceType. Otherwise a NULL is returned.
*/
cl_platform_id ocl::Platform::id(const ocl::DeviceType& type)
{
    const auto &platforms = ocl::platforms();
    for(auto p_id : platforms)
    {
        const auto &devs = ocl::devices(p_id);
        for(auto d_id : devs){
            if(type == ocl::deviceType(d_id)) return p_id;
        }
    }
    return 0;
}



/*! \brief Returns the ID of this Platform. */
cl_platform_id ocl::Platform::id() const
{
	return this->_id;
}

/*! \brief Returns a Device within this Platform with the specified device ID. */
ocl::Device ocl::Platform::device(cl_device_id id) const
{
    auto it = std::find(_devices.begin(), _devices.end(), id);
    TRUE_ASSERT(it != this->devices().end(), "Could not find device");
    return *it;
}

/*! \brief Returns a Device within this Platform with the specified device ID. */
ocl::Device ocl::Platform::device(const ocl::DeviceType &type) const
{
    auto it = std::find(_devices.begin(), _devices.end(), type);
    TRUE_ASSERT(it != this->devices().end(), "Could not find device");
    return *it;
}

/*! \brief Returns all Device objects within this Platform with the DeviceTypes. */
std::vector<ocl::Device> ocl::Platform::devices(const ocl::DeviceTypes& types) const
{
    std::vector<ocl::Device> devices;
	for(auto d : this->devices()){
        if(types.contains(d.type())) devices.push_back(d);
	}
	return devices;
}

/*! \brief Returns all Device objects within this Platform with the DeviceType. */
std::vector<ocl::Device> ocl::Platform::devices(const ocl::DeviceType& type) const
{
    std::vector<ocl::Device> devices;
	for(auto d : this->devices()){
        if(d == type) devices.push_back(d);
	}
	return devices;
}

/*! \brief Returns all Device objects within this Platform. */
const std::vector<ocl::Device>& ocl::Platform::devices() const
{
	return this->_devices;
}

/*! \brief Returns true if this and the specified Platform have the same OpenCL platform id. */
bool ocl::Platform::operator == (const Platform & p) const
{
	return p.id() == this->id();
}

/*! \brief Returns true if this and the specified Platform do not have the same OpenCL platform id. */
bool ocl::Platform::operator != (const Platform & p) const
{
	return p.id() != this->id();
}

/*! \brief Returns true if this Platform has the same OpenCL platform id as the specified one. */
bool ocl::Platform::operator == (cl_platform_id  p) const
{
	return p == this->id();
}

/*! \brief Returns true if this Platform does not have the same OpenCL platform id as the specified one. */
bool ocl::Platform::operator != (cl_platform_id  p) const
{
	return p != this->id();
}

/*! \brief Prints this Platform and if wanted with all its Device objects. */
void ocl::Platform::print(bool print_devices,
                          bool print_profile,
                          bool print_version,
                          bool print_name,
                          bool print_vendor,
                          bool print_extensions,
                          std::ostream& out)
{
    ocl::printPlatform(this->id(), print_devices, print_profile, print_version, print_name, print_vendor, print_extensions,out);
}



/*! \brief Returns true if this Platform contains the specified Context. */
bool ocl::Platform::has(ocl::Context & ctxt) const
{
    return this->_contexts.find(&ctxt) != this->_contexts.end();
}

/*! \brief Sets an active Context for this Platform
  *
  * An active Context is necessary if OpenCL objects such as
  * Program, Event, Buffer, Kernel, Queues
  * are constructed without a Context. In this case
  * an active Context within the active Platform is considered.
  * These objects cannot be created without a valid Context.
  *
*/
void ocl::Platform::setActiveContext(ocl::Context &ctxt)
{
    TRUE_ASSERT(this->has(ctxt), "Context not in Platform.");
    this->_activeContext = &ctxt;
}

/*! \brief Returns the active Context of this Platform. */
ocl::Context* ocl::Platform::activeContext() const
{
    TRUE_ASSERT(_activeContext != NULL, "There is no active Context");
    return this->_activeContext;
}

/*! \brief Returns true if the specified Context is an active Context of this Platform. */
bool ocl::Platform::isActiveContext(ocl::Context &ctxt) const
{
    return this->_activeContext->id() == ctxt.id();
}

/*! \brief Returns true if this Platform has an active Context. */
bool ocl::Platform::hasActiveContext() const
{
    return this->_activeContext != 0;
}

/*! \brief Inserts the specified Context into this Platform.
  *
  * Device objects of the Context must be also within
  * this Platform. In order to set a Context active
  * you have to perform this operation before.
  *
  */
void ocl::Platform::insert(ocl::Context &ctxt)
{
    const auto & devs = ctxt.devices();
    for(auto it = devs.begin(); it != devs.end(); ++it)
    {
        const auto &dev = *it;
        TRUE_ASSERT(this->has(dev), "Platform does not have this Device");
    }
    this->_contexts.insert(&ctxt);
}

/*! \brief Returns true if this System has an active Platform.
  *
  * Active Platforms are needed when a Context or
  * objects such as Queue, Program, Kernel and Event
  * are created and no Platform is specified.
  * In this case an active Platform is taken.
*/
bool ocl::Platform::hasActivePlatform()
{
    return _activePlatform != 0;
}

/*! \brief Sets an active Platform for this System. */
void ocl::Platform::setActivePlatform(Platform &plat)
{
    _activePlatform = &plat;
}

/*! \brief Returns true if this System has an active Platform. */
ocl::Platform* ocl::Platform::activePlatform()
{
    return _activePlatform;
}

ocl::Platform *ocl::Platform::_activePlatform = 0;

