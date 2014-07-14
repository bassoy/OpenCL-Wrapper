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

#include <utl_storage.h>


size_t utl::Storage::_globalid = 0;

utl::Storage utl::RowMajor("row_major");
utl::Storage utl::ColMajor("col_major");


size_t utl::Storage::id() const { return _myid;}
bool utl::Storage::equal(size_t id) const {return id == this->id();}
bool utl::Storage::equal(const utl::Storage &f) const { return this->id() == f.id(); }
bool utl::Storage::operator ==(const utl::Storage &f) const { return this->id() == f.id(); }
bool utl::Storage::operator !=(const utl::Storage &f) const { return ! this->operator ==(f); }
const std::string& utl::Storage::name() const { return this->_name; }
utl::Storage::Storage(const std::string& n) : _name(n), _myid(_globalid++) {}

utl::Storage::Storage(): _name(), _myid() {}
utl::Storage::Storage(const utl::Storage &): _name(), _myid() {}

