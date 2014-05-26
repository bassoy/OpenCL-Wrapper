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

#ifndef UTL_TIMER_H
#define UTL_TIMER_H

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

template<class _Duration>
class Timer
{
	typedef std::chrono::high_resolution_clock clock;
	typedef typename clock::time_point point;
public :
	using Duration = _Duration;

    Timer() = delete;
    Timer(const Timer&) = delete;
    Timer& operator =(const Timer&) = delete;

    static void tic();
    static void toc();

	static Duration elapsed();

	template<class OtherDuration>
	static OtherDuration elapsed()
	{
		return std::chrono::duration_cast<OtherDuration>( _stop - _start );
	}

private:    

	static point start();
	static point stop();

    static point _start;
    static point _stop;
};

}

std::ostream& operator<< (std::ostream & out, utl::Seconds d);
std::ostream& operator<< (std::ostream & out, utl::MilliSeconds d);
std::ostream& operator<< (std::ostream & out, utl::MicroSeconds d);
std::ostream& operator<< (std::ostream & out, utl::NanoSeconds d);




#endif
