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

#include <utl_args.h>


#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ostream>

std::stringstream utl::Args::_ss;


utl::Args::Args(int argc, char **argv) : _strings()
{
	for(int i = 0; i < argc; ++i)
        this->push_back(argv[i]);
}

void utl::Args::push_back(const std::string& s)
{
    this->_strings.push_back(s);
}

void utl::Args::push_back(const char* s)
{
    this->_strings.push_back(s);
}

size_t utl::Args::size() const
{
    return this->_strings.size();
}


std::string& utl::Args::at(size_t pos)
{
    return this->_strings.at(pos);
}

const std::string& utl::Args::at(size_t pos) const
{
    return this->_strings.at(pos);
}


const std::vector<std::string>& utl::Args::args() const
{
    return _strings;
}

std::string utl::Args::str() const
{
    std::string s;
    for(size_t i = 0; i < this->size()-1; ++i)
	{
        s+= this->at(i); s+=(", ");
	}
    s+= this->at(this->size()-1);
	return s;
}


std::vector<std::string> utl::Args::toString(int argc, char **argv) const
{
    std::vector<std::string> strings;
	for(int i = 1; i < argc; ++i)
        strings.push_back(argv[i]);
	return strings;
}

/*! \brief Clear static stream. */
void utl::Args::_cs() { _ss.str(""); _ss.clear(); }


void utl::Args::print() const
{
    for(size_t i= 0; i < this->size()-1; ++i)
        std::cout << this->at(i) << ", ";
    std::cout << this->at(this->size()-1) << std::endl;
}

utl::Args& utl::Args::operator <<(double a)      { _cs(); _ss << a; this->push_back(_ss.str()); return *this;}
utl::Args& utl::Args::operator <<(float a)       { _cs(); _ss << a; this->push_back(_ss.str()); return *this;}
utl::Args& utl::Args::operator <<(int a)         { _cs(); _ss << a; this->push_back(_ss.str()); return *this;}
utl::Args& utl::Args::operator <<(unsigned int a){ _cs(); _ss << a; this->push_back(_ss.str()); return *this;}
utl::Args& utl::Args::operator <<(char a)        { _cs(); _ss << a; this->push_back(_ss.str()); return *this;}
utl::Args& utl::Args::operator <<(std::string a) { _cs(); _ss << a; this->push_back(_ss.str()); return *this;}

int          utl::Args::toInt(size_t pos)      const   { _cs(); _ss << this->at(pos); int n;          _ss >> n; return n;}
float        utl::Args::toFloat(size_t pos)    const   { _cs(); _ss << this->at(pos); float n;        _ss >> n; return n;}
double       utl::Args::toDouble(size_t pos)   const   { _cs(); _ss << this->at(pos); double n;       _ss >> n; return n;}
unsigned int utl::Args::toUnsigned(size_t pos) const   { _cs(); _ss << this->at(pos); unsigned int n; _ss >> n; return n;}
std::string  utl::Args::toString(size_t pos)   const   { _cs(); _ss << this->at(pos); std::string n;  _ss >> n; return n;}
char         utl::Args::toChar(size_t pos)     const   { _cs(); _ss << this->at(pos); char n;         _ss >> n; return n;}
size_t       utl::Args::toSizet(size_t pos)    const   { _cs(); _ss << this->at(pos); size_t n;       _ss >> n; return n;}

