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

#include <ocl_device_type.h>

#include <utl_assert.h>

#include <algorithm>
#include <sstream>

using namespace std;


/*! \brief Instantiates this DeviceTypes without any DeviceType objects. */
ocl::DeviceTypes::DeviceTypes() : _deviceTypes()
{
}

/*! \brief Instantiates this DeviceTypes with the specified DeviceType. */
ocl::DeviceTypes::DeviceTypes(const ocl::DeviceType &t) : _deviceTypes()
{
	this->push_back(t);
}

/*! \brief Instantiates this DeviceTypes with the specified DeviceType objects. */
ocl::DeviceTypes::DeviceTypes(const ocl::DeviceType &t1, const ocl::DeviceType &t2) : _deviceTypes()
{
	this->push_back(t1);
	this->push_back(t2);
}

/*! \brief Creates a new DeviceTypes object from this DeviceTypes with the inserted DeviceType.
  *
  * Note that this DeviceType does not contain the specified DeviceType but
  * only the new created one.
*/
ocl::DeviceTypes ocl::DeviceTypes::operator |(const ocl::DeviceType &type) const
{
    ocl::DeviceTypes types = *this;
	types.push_back(type);
	return types;
}

/*! \brief Insertes a DeviceType to this DeviceTypes. */
void ocl::DeviceTypes::operator |=(const ocl::DeviceType &type)
{
    this->push_back(type);
}

/*! \brief Returns the DeviceType objects within this DeviceTypes. */
const std::vector<ocl::DeviceType>& ocl::DeviceTypes::operator ()() const
{
    return this->_deviceTypes;
}



/*! \brief Returns true if this and the specified DeviceTypes have the same DeviceType objects. */
bool ocl::DeviceTypes::operator == (const ocl::DeviceTypes& types) const
{
	return this->contains(types);
}

/*! \brief Returns true if this and the specified DeviceTypes do not have the same DeviceType objects. */
bool ocl::DeviceTypes::operator != (const ocl::DeviceTypes& types) const
{
	return !this->contains(types);
}

/*! \brief Returns true if this DeviceTypes object contains all DeviceType objects which are in the specified DeviceTypes. */
bool ocl::DeviceTypes::contains(const ocl::DeviceTypes &types) const
{
	for(auto type : types()){
        if(!this->contains(type)) return false;
	}
	return true;
}

/*! \brief Returns true if this DeviceTypes object contains the specified DeviceType. */
bool ocl::DeviceTypes::contains(const ocl::DeviceType &type) const
{
    return std::find(_deviceTypes.begin(), _deviceTypes.end(), type) != _deviceTypes.end();
}

/*! \brief Returns true if this DeviceTypes object contains the specified OpenCL device type. */
bool ocl::DeviceTypes::contains(cl_device_type type) const
{
    return std::find(_deviceTypes.begin(), _deviceTypes.end(), type) != _deviceTypes.end();
}

/*! \brief Returns the number of DeviceType objects within this DeviceTypes. */
size_t ocl::DeviceTypes::size() const
{
    return this->_deviceTypes.size();
}

/*! \brief Inserts the specified DeviceType into this DeviceTypes. */
void ocl::DeviceTypes::push_back(const ocl::DeviceType &t)
{
    this->_deviceTypes.push_back(t);
}


/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////


std::vector<ocl::DeviceType> ocl::DeviceType::_allTypes;


/*! \brief Instantiates this DeviceType from an OpenCL device type
  *
  */
ocl::DeviceType::DeviceType(cl_device_type type, std::string name) :
    _type(type), _name(name)
{
	_allTypes.push_back(*this);
}

/*! \brief Instantiates this DeviceType from another DeviceType
  *
  */
ocl::DeviceType::DeviceType(const ocl::DeviceType &type) :
    _type(type._type), _name(type._name)
{
}

/*! \brief Aggregates this DeviceType with another DeviceType
  *
  * \param type is a DeviceType
  * \returns DeviceTypes which contains this DeviceType with the other DeviceType
  */
ocl::DeviceTypes ocl::DeviceType::operator | (const DeviceType &type) const
{
	return ocl::DeviceTypes(*this,type);
}

/*! \brief Returns the OpenCL device type
  *
  */
cl_device_type ocl::DeviceType::operator()() const
{
	return _type;
}

/*! \brief Returns the OpenCL device type
  *
  */
std::string ocl::DeviceType::name() const
{
	return _name;
}


/*! \brief Returns true if this and the specified DeviceType have the same OpenCL device type. */
bool ocl::DeviceType::operator == (const ocl::DeviceType& t) const
{
	return ((*this)() == t());
}

/*! \brief Returns true if this and the specified DeviceType do not have the same OpenCL device type. */
bool ocl::DeviceType::operator != (const ocl::DeviceType& t) const
{
	return (*this)() != t();
}

/*! \brief Returns true if this DeviceType has the same OpenCL device type as the specified one. */
bool ocl::DeviceType::operator == (cl_device_type t) const
{
	return (*this)() == t;
}

/*! \brief Returns true if this DeviceType does not have the same OpenCL device type as the specified one. */
bool ocl::DeviceType::operator != (cl_device_type t) const
{
	return (*this)() != t;
}

/*! \brief Returns the DeviceType of a given OpenCL device type. */
const ocl::DeviceType& ocl::DeviceType::type(cl_device_type t)
{

    std::vector<DeviceType>::const_iterator it = std::find(_allTypes.begin(), _allTypes.end(), t);
    TRUE_ASSERT(it != _allTypes.end(), "Could not find that type.");
    return *it;
}





namespace ocl{
namespace device_type{
	DeviceType GPU(CL_DEVICE_TYPE_GPU, "GPU");
	DeviceType CPU(CL_DEVICE_TYPE_CPU, "CPU");
	DeviceType ACC(CL_DEVICE_TYPE_ACCELERATOR, "ACC");
	DeviceType ALL(CL_DEVICE_TYPE_ALL, "ALL");
}
}
