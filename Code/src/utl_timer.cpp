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

#include <utl_timer.h>



utl::Timer::point utl::Timer::_start;
utl::Timer::point utl::Timer::_stop;

// need tic for toc in seconds
double utl::Timer::tic()
{
    _start = clock::now();
    // duration in resolution
    const duration &dur = std::chrono::duration_cast<resolution>(_start.time_since_epoch());
     // duration in seconds
    double sec = double(dur.count()) / double(resolution::period::den);
    return sec;
}

// elapsed time since tic in seconds
double utl::Timer::toc()
{
    _stop = clock::now();
    // duration in resolution
    const duration &dur = std::chrono::duration_cast<resolution>(_stop - _start);
    // duration in seconds
    double sec = double(dur.count()) / double(resolution::period::den);
    return sec;
}

double utl::Timer::elapsed()
{
    const duration &dur = std::chrono::duration_cast<resolution>(_stop - _start);
    // duration in seconds
    double sec = double(dur.count()) / double(resolution::period::den);
    return sec;
}

utl::Timer::point utl::Timer::start()
{
    return _start;
}

utl::Timer::point utl::Timer::stop()
{
    return _stop;
}
