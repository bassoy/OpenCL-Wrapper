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

#ifndef UTL_TIME_H
#define UTL_TIME_H

#include <chrono>
#include <iostream>

namespace utl
{

/*! \class Timer
  * \brief Utility class for timing functions.
  *
  * Timer class encapsulates the chono clock
  * functions for an easier clock query.
  * Use tic, toc methods (similar to Matlab) to
  * query the runtime in seconds of a function.
  */

using NanoSeconds  = std::chrono::duration<double, std::nano>;
using MicroSeconds = std::chrono::duration<double, std::micro>;
using MilliSeconds = std::chrono::duration<double, std::milli>;
using Seconds      = std::chrono::duration<double, std::ratio<1,1>>;

template<class _Resolution>
class Timer
{
	typedef std::chrono::high_resolution_clock clock;
	typedef typename clock::duration duration;
	typedef typename clock::time_point point;
public :
	using Resolution = _Resolution;

    static void tic();
    static void toc();

	static Resolution elapsed();

	template<class OtherResolution>
	static OtherResolution elapsed()
	{
		return std::chrono::duration_cast<OtherResolution>( _stop - _start );
	}

private:
    explicit Timer(){}

	static point start();
	static point stop();

    static point _start;
    static point _stop;
};

}

std::ostream& operator<< (std::ostream & out, const utl::Seconds&);

//template<class T>
//std::ostream& operator<< (std::ostream & out, const T&);
#endif
