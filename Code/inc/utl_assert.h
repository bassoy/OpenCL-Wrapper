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


#ifndef UTL_ASSERT_H
#define UTL_ASSERT_H

#include <iostream>


#ifdef DEBUG
    #define DEBUG_ASSERT(b,text)  if(!(b)) {std::cerr<<"(D)Error in "   << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush(); throw;} do {} while(0)
    #define DEBUG_WARNING(b,text) if(!(b)) {std::cerr<<"(D)Warning in " << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush();       } do {} while(0)
    #define DEBUG_COMMENT(text)            {std::cout<<"(D)Comment in " << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush();       } do {} while(0)
#else
	#define DEBUG_ASSERT(b,text)
	#define DEBUG_WARNING(b,text) 
	#define DEBUG_COMMENT(text)  
#endif

#define TRUE_ASSERT(b,text)  if(!(b)) {std::cerr<<"Error in "   << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush(); throw;} do {} while(0)
#define TRUE_WARNING(b,text) if(!(b)) {std::cerr<<"Warning in " << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush();       } do {} while(0)
#define TRUE_COMMENT(text)            {std::cout<<"Comment in " << __FILE__<<", "<<__FUNCTION__<<", "<<__LINE__<<": "<< text <<std::endl; std::cerr.flush();       } do {} while(0)

#endif 
