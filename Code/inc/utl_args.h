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

#ifndef UTL_ARGS_H
#define UTL_ARGS_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ostream>





namespace utl{
/*! \class Args utl_args.h "inc/utl_args.h"
  * \brief Utility class to handle the arguments of the main() function.
  *
  * An Args object can be instantiated in order to conveniently store the arguments
  * from the console. Arguments are strored as strings which can be converted
  * to other data types.
  */


class Args
{
public :

    explicit Args() : _strings() {}
	Args(int argc, char **argv);
    ~Args(){}

    std::vector<std::string> toString(int argc, char **argv) const;
    const std::vector<std::string>& args() const;
    std::string str() const;
	void print() const;

    std::string& at(size_t pos);
    const std::string& at(size_t pos) const;
    size_t size() const;

    void push_back(const std::string&);
    void push_back(const char*);


	Args& operator<<(double a);
	Args& operator<<(float a);
	Args& operator<<(int a);
	Args& operator<<(unsigned int a);
	Args& operator<<(char a);
    Args& operator<<(std::string a);

	int          toInt(size_t pos)      const;
	float        toFloat(size_t pos)    const;
	double       toDouble(size_t pos)   const;
	unsigned int toUnsigned(size_t pos) const;
    std::string  toString(size_t pos)   const;
	char         toChar(size_t pos)     const;
	size_t       toSizet(size_t pos)    const;
	bool         toBool(size_t pos)     const;

private:

    std::vector<std::string> _strings;

    static void _cs();

    static std::stringstream _ss;
};
}
#endif
