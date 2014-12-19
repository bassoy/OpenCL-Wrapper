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
	Type Undefined ("undefined",          typeid(bool));
}
}
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////

utl::Types::Types() :
	types_()
{
}

utl::Types::Types(const Type& other) :
	types_()
{
	types_.insert(&other);
}

utl::Types::Types(const Types& other) :
	types_(other.types_)
{

	DEBUG_COMMENT(this->toString());
}

utl::Types::Types(Types&& other) :
	types_()
{
	std::swap( other.types_, types_ );

	DEBUG_COMMENT(this->toString());
}

bool utl::Types::contains(const Type &type)   const
{
	return types_.find(&type) != types_.end();
}

bool utl::Types::contains(const Types &other) const
{
	return std::includes(types_.begin(), types_.end(), other.begin(), other.end());
}

std::set<std::string> utl::Types::names() const
{
	std::set<std::string> __s;
	for(const_iterator it = types_.begin(); it != types_.end(); ++it){
		__s.insert((*it)->name() );
	}
	return __s;
}

std::string utl::Types::toString() const
{
	std::string str("");
	if(types_.empty()) return str;
	const_iterator it;
	for(it = types_.begin(); it != types_.end(); ++it){
		str += (*it)->name();
		str += ", ";
	}
	return str;
}

utl::Types& utl::Types::operator=(const Types &other)
{
	if( this != &other) types_ = other.types_;
	return *this;
}

utl::Types& utl::Types::operator=(Types &&other)
{
	if( this != &other) types_ = std::move(other.types_);
	return *this;
}

bool utl::Types::operator==(const Types &other) const
{
	return types_ == other.types_;
}

bool utl::Types::operator!=(const Types &other) const
{
	return !(*this == other);
}

utl::Types& utl::Types::operator|(const Type  &type)
{
	types_.insert(&type);
	return *this;
}

utl::Types& utl::Types::operator|(const Types &other)
{
	types_.insert(other.begin(), other.end());
	return *this;
}


utl::Types& utl::Types::operator<<(const Type &type)
{
	return  ( (*this) | type );
}
utl::Types& utl::Types::operator<<(const Types &other)
{
	return ( (*this) | other );
}


