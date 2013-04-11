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

#ifndef UTL_STREAM_H
#define UTL_STREAM_H

#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cstdlib>
#include <typeinfo>


namespace utl {

class Dim;

class Storage;

extern Storage ColMajor;

template<class T>
void write(const std::string &file_name, T *A, size_t size, std::ios::openmode mode = std::ios::out);

template<class T>
void read(const std::string &file_name, T *A, size_t size, std::ios::openmode mode = std::ios::in);

template<class T>
void randn(T *data, size_t num, T maxData = (T)RAND_MAX);

template<class T>
void print(const T *A, const Dim &dims,  const std::string &name = std::string(), const Storage& s = ColMajor, std::ostream &out = std::cout);

template<class T>
std::string toString(const T *A, const Dim &dims,  const std::string &name = std::string(), const Storage &s = ColMajor);

template<class T>
std::string toString(const std::vector<T>& A)
{
    std::stringstream ss;
    ss << "[";
    for(auto it = A.begin(), it_end = --A.end(); it != it_end; ++it)
        ss << *it << ", ";
    ss << A.back() << "]";
    return ss.str();
}


template<class T>
void octaveErrorPrint(const std::string &v1, const std::string &v2, std::ostream &out = std::cout, const std::string &max = "");

template<class T>
std::string typeToString();

}
#endif
