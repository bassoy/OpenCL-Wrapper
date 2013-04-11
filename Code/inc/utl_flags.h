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

#ifndef UTL_FLAGS_H
#define UTL_FLAGS_H

#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <new>

namespace utl{
class Flag
{
public:
	explicit Flag(unsigned int flag) : _flag(flag) {}
	unsigned int flag() const { return _flag; }
private:
	unsigned int _flag;
};

template<class Enum>
class Flags
{
public:
	Flags ( const Flags & other ) : _flags(other._flags) {}
    Flags () : _flags(0) {}
    Flags ( Enum flag ) : _flags(flag) {}
    Flags ( Flag flag ) : _flags(flag.flag()) {}
    Flags ( unsigned int flags) : _flags(flags){}
	bool 	testFlag ( Enum flag ) const { return ( _flags & flag ) != 0; }
	operator unsigned int () const { return _flags; }
	bool      operator!  () const { return !_flags; }

	bool    operator==  ( const Flags& other ) const{ return _flags == other._flags; }
	bool 	  operator==  ( Enum other ) const        { return _flags == static_cast<unsigned int>(other); }
	

	Flags     operator&  ( unsigned int mask ) const { return Flags(_flags & mask); }
	Flags     operator&  ( Enum mask ) const         { return Flags(_flags & static_cast<unsigned int>(mask)); }
	
	Flags & 	operator&= ( unsigned int mask )       { _flags &= mask; return *this; }
	
    Flags & 	operator=  (  Flags  other )     { _flags = other._flags; return *this; }
    Flags & 	operator=  ( Enum other )              { _flags = static_cast<unsigned int>(other); return *this; }

	Flags     operator^  ( const Flags &other ) const{ return Flags(_flags ^ other._flags); }
	Flags 	  operator^  ( Enum other ) const        { return Flags(_flags ^ static_cast<unsigned int>(other)); }
	
	Flags & 	operator^= ( const Flags &other )      { _flags ^= other._flags; return *this; }
	Flags & 	operator^= ( Enum other )              { _flags ^= static_cast<unsigned int>(other); return *this; }
	
    Flags 	  operator|  (  Flags other ) const{ return Flags(_flags | other._flags); }
	Flags 	  operator|  ( Enum other ) const        { return Flags(_flags | static_cast<unsigned int>(other)); }

	Flags & 	operator|= ( const Flags &other )      { _flags |= other._flags; return *this; }
	Flags & 	operator|= ( Enum other )              { _flags |= static_cast<unsigned int>(other); return *this; }
	
	Flags 	  operator~  () const                    { return Flags(~_flags); }



private:
    unsigned int _flags;


};
}
    template<class Enum>
    utl::Flags<Enum> operator|(Enum other1, Enum other2)
    {
        return utl::Flags<Enum>(static_cast<unsigned int>(other1) | static_cast<unsigned int>(other2));
    }

    template<class Enum>
    utl::Flags<Enum> operator&(Enum other1, Enum other2)
    {
        return utl::Flags<Enum>(static_cast<unsigned int>(other1) & static_cast<unsigned int>(other2));
    }

#endif
