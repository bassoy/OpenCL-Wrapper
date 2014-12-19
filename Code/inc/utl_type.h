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

#ifndef UTL_TYPE_H
#define UTL_TYPE_H

#include <typeinfo>
#include <type_traits>
#include <string>
#include <set>
#include <limits>
#include <utility>
#include <vector>

#include <utl_assert.h>



namespace utl{

class Type;

/*! \class Types
  * \brief Utility class for storing and handling Type objects.
  *
  * Define your type in order to query types at runtime.
  */
class Types
{
  std::set< Type const* > types_;
public:
	typedef /*typename*/ std::set<const Type*>::iterator iterator;
	typedef /*typename*/ std::set<const Type*>::const_iterator const_iterator;
	typedef /*typename*/ std::set<const Type*>::const_pointer const_pointer;
	typedef /*typename*/ std::set<const Type*>::const_reference const_reference;

	 //std::set< Type const* > types_;
   
	Types();
	Types(const Type &);
	Types(const Types&);
	Types(Types&&);

	template<typename ... Ts>
	Types(Ts ... args)
	{
		std::vector<const Type*> __v = {args...};
		for(auto e : __v)
			types_.insert(e);
	}
	

	bool empty() const { return types_.empty(); }
	const_iterator begin() const { return types_.begin(); }
	const_iterator end() const { return types_.end(); }


	bool contains(const Type &) const;
	bool contains(const Types &) const;

	std::set<std::string> names() const;

	std::string toString() const;

	Types& operator=(const Types&);
	Types& operator=(Types&&);

	bool operator==(const Types &) const;
	bool operator!=(const Types &) const;

	Types& operator|(const Type  &);
	Types& operator|(const Types &);

	Types& operator<<(const Type &);
	Types& operator<<(const Types &);
};


/*! \class Type
  * \brief Utility class for the definition of single types.
  *
  * Define your type in order query types at runtime.
  */

class Type
{
public:
	Type(const std::string &name, const std::type_info &info);

	Type( Type const& ) = delete;
	Type( Type&& ) = delete;

	Type& operator =( Type const& ) = delete;
	Type& operator =( Type&& ) = delete;

	bool operator==(const Type &) const;
	bool operator!=(const Type &) const;
	bool operator==(const std::type_info&) const;
	bool operator!=(const std::type_info&) const;

	const std::string& name() const;
	const std::type_info& info() const;

	Types operator|(const Type  &__rhs);
	Types operator|(const Types &__rhs);

	const Type* operator()() const;

	template <class T>
	static const Type& type(){
		for(auto __t : _allTypes) { if(*__t == typeid(T)) return *__t; }
		TRUE_ASSERT(0, "Type with id " << typeid(T).name() << " not found");
	}
private:
	std::string _name;
	const std::type_info* _info;
	static std::set<const Type*> _allTypes;

};

namespace type{
	extern Type Double;
	extern Type Single;
	extern Type Int;
	extern Type UInt;
	extern Type SChar;
	extern Type UChar;
	extern Type Undefined;
	//extern Type Bool;
}

}


#endif
