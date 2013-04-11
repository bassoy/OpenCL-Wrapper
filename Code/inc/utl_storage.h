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

#ifndef UTL_STORAGE_H
#define UTL_STORAGE_H

#include <cstddef> // size_t
#include <string>



namespace utl{

/*! \class Storage utl_storage.h "inc/utl_storage.h"
  * \brief Utility class for specifing the storage format.
  *
  * Use the objects RowMajor and ColMajor in order
  * to specify the storage format.
  */

class Storage
{
public :
	size_t id() const;
	bool equal(size_t id) const;
	bool equal(const Storage &f) const;
	bool operator ==(const Storage &f) const;
	bool operator !=(const Storage &f) const;
	const std::string& name() const;
	explicit Storage(const std::string& n);


private:
	explicit Storage();
	Storage(const Storage &);
	std::string _name;
	size_t _myid;
	static size_t _globalid;

};

extern Storage RowMajor;
extern Storage ColMajor;

}


#endif
