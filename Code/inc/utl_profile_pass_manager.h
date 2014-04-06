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

#ifndef UTL_PROFILE_PASS_MANAGER_H
#define UTL_PROFILE_PASS_MANAGER_H

#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <new>
#include <memory>

#include <utl_profile_pass.h>


namespace utl {

/*! \class ProfilePassManager utl_profile_pass_manager.h "inc/utl_profile_pass_manager.h"
  * \brief Utility class to handle profile classes.
  *
  * ProfilePassManager objects are used to collect ProfilePass objects
  * and to run them collectively.
  */


template<class T>
class ProfilePassManager
{
    typedef typename std::vector<std::shared_ptr<ProfilePass<T>>>::const_iterator const_iterator;
    typedef typename std::vector<std::shared_ptr<ProfilePass<T>>>::iterator       iterator;
    typedef typename std::vector<std::shared_ptr<ProfilePass<T>>>::pointer        pointer;
    typedef typename std::vector<std::shared_ptr<ProfilePass<T>>>::reference      reference;
public:
    ProfilePassManager() {};
	
	/*~ProfilePassManager()
	{
		for(iterator it = _passes.begin(); it != _passes.end(); ++it){
			delete *it;
		}
	}*/

    ProfilePassManager& operator<<(std::shared_ptr<ProfilePass<T>> __p)
	{
        _passes.push_back(__p);
        return *this;
	}

    void setIter(size_t iter)
    {
        for (auto it : _passes){
            it->setIter(iter);
        }
    }

    void setPrint(bool print_n, bool print_t, bool print_o, bool print_p)
    {
        for (auto it : _passes){
            it->setPrint(print_n, print_t, print_o, print_p);
        }
    }

	void run()
	{
		for (auto it : _passes){
			std::cout << "Profiling " << it->name() << std::endl;
			it->prof();
		}
	}

	void write(std::ostream& out) const
	{
		for (auto it : _passes)
			out << it;
		out << std::endl;
	}

	void write(const std::string &filename) const
	{
		std::ofstream out(filename.c_str());
		_passes.write(out);
	}

private:

    std::vector<std::shared_ptr<ProfilePass<T>>> _passes;

};

}


#endif // PROFILE_PASS_ProfilePassManager_H
