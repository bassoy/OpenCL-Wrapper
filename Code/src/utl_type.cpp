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

#include <utl_type.h>
#include <utl_assert.h>
#include <utl_stream.h>

#include <algorithm>

std::set<const utl::Type*> utl::Type::_allTypes;


utl::Type::Type(const std::string &__n, const std::type_info& __i) :
	_name(__n), _info(&__i)
{
    _allTypes.insert(this);
}

const std::string& utl::Type::name() const
{
	return this->_name;
}

const std::type_info& utl::Type::info() const
{
	return *this->_info;
}

utl::Types utl::Type::operator|(const utl::Type  &__rhs)
{
	Types __lhs(*this);
	__lhs << __rhs;
	return __lhs;
}

utl::Types utl::Type::operator|(const utl::Types &__rhs)
{
	Types __lhs(*this);
	__lhs << __rhs;
	return __lhs;
}

bool utl::Type::operator==(const Type &t) const
{
	return this->info() == t.info();
}

bool utl::Type::operator!=(const Type &t) const
{
	return this->info() != t.info();
}

bool utl::Type::operator==(const std::type_info& __info) const
{
	return this->info() == __info;
}

bool utl::Type::operator!=(const std::type_info& __info) const
{
	return this->info() != __info;
}

const utl::Type* utl::Type::operator ()() const
{
    return this;
}



namespace utl{
namespace type{
    Type Double  ("double",        typeid(double));
    Type Single  ("float",         typeid(float));
    //Type Single4 ("float4",        typeid(float));
    Type Int     ("int",           typeid(int));
    Type UInt    ("unsigned int",  typeid(unsigned int));
    Type SChar    ("signed char",   typeid(signed char));
    Type UChar   ("unsigned char", typeid(unsigned char));
    Type Bool    ("bool",          typeid(bool));
}
}
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////

utl::Types::Types() :
    set<const Type*>()
{
}

utl::Types::Types(const Type& _Type) :
    set<const Type*>()
{
    this->insert(&_Type);
}

utl::Types::Types(const Types& _Types) :
    set<const Type*>(_Types)
{
}

utl::Types::Types(Types&& _Types) :
    set<const Type*>(std::move(_Types))
{
}

bool utl::Types::contains(const Type &_Type)   const
{
    return this->find(&_Type) != this->end();
}

bool utl::Types::contains(const Types &_Types) const
{
    return std::includes(this->begin(), this->end(), _Types.begin(), _Types.end());
}

std::set<std::string> utl::Types::names() const
{
	std::set<std::string> __s;
    for(const_iterator it = this->begin(); it != this->end(); ++it){
		__s.insert((*it)->name() );
	}
	return __s;
}

std::string utl::Types::toString() const
{
    std::string __s;
    if(this->empty()) return __s;
    const_iterator it;
    for(it = this->begin(); it != this->end(); ++it){
        __s += (*it)->name();
        __s += ", ";
    }
    return __s;
}

utl::Types& utl::Types::operator=(const Types &t)
{
    std::set<const Type*>::operator =(t);
    return *this;
}

utl::Types& utl::Types::operator=(Types &&t)
{
    std::set<const Type*>::operator =(std::move(t));
    return *this;
}

bool utl::Types::operator==(const Types &t) const
{
    return *this == t;
}

bool utl::Types::operator!=(const Types &t) const
{
    return *this != t;
}

utl::Types& utl::Types::operator|(const Type  &__rhs)
{
    this->insert(&__rhs);
	return *this;
}

utl::Types& utl::Types::operator|(const Types &__rhs)
{
    this->insert(__rhs.begin(), __rhs.end());
	return *this;
}


utl::Types& utl::Types::operator<<(const Type &__t)
{
    this->insert(&__t);
	return *this;
}
utl::Types& utl::Types::operator<<(const Types &__t)
{
    this->insert(__t.begin(), __t.end());
	return *this;
}


