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

template<class ValueType_>
class ProfilePassManager
{
public:
		using PassType = ProfilePass<ValueType_>;
		using ValueType = typename PassType::ValueType;
		
    using const_iterator = typename std::vector<std::shared_ptr<PassType>>::const_iterator;
    using iterator       = typename std::vector<std::shared_ptr<PassType>>::iterator;
    using pointer        = typename std::vector<std::shared_ptr<PassType>>::pointer;
    using reference      = typename std::vector<std::shared_ptr<PassType>>::reference;
        

    ProfilePassManager() = default;
    ~ProfilePassManager() = default;

    ProfilePassManager& operator<<(const std::shared_ptr<PassType>& __p)
	{
        _passes.push_back(__p);
        return *this;
	}
	
    ProfilePassManager& operator<<(PassType* __p)
	{
        _passes.push_back(std::shared_ptr<PassType>(__p));
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
			it->run();
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
	
	
  std::shared_ptr< PassType > getPass( size_t index )
  {
    return _passes.at( index );
  }

private:

    std::vector<std::shared_ptr<PassType>> _passes;

};

}


#endif // PROFILE_PASS_ProfilePassManager_H
