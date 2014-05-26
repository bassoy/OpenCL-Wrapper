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
#include <vector>
#include <memory>


namespace utl {

class ProfilePass;

/*! \class ProfilePassManager utl_profile_pass_manager.h "inc/utl_profile_pass_manager.h"
  * \brief Utility class to handle profile classes.
  *
  * ProfilePassManager objects are used to collect ProfilePass objects
  * and to run them collectively.
  */

class ProfilePassManager
{
	using uPPassPtr = std::unique_ptr<ProfilePass> ;
public:

    ProfilePassManager() = default;
    ~ProfilePassManager() = default;
	ProfilePassManager(const ProfilePassManager&) = delete;
	
	ProfilePassManager& operator<<(ProfilePass* p);
	void run();
	void write(std::ostream& out) const;

private:

	std::vector<uPPassPtr> _passes;

};

}


#endif // PROFILE_PASS_ProfilePassManager_H
