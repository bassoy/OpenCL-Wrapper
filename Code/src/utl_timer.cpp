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


utl::Seconds::Seconds (long double m) : _m(m){}
utl::Seconds::Seconds (const Seconds &s) : _m(s._m){}
utl::Seconds::Seconds (const MilliSeconds &s) : _m(s._m*1e-3){}
utl::Seconds::Seconds (const MicroSeconds &s) : _m(s._m*1e-6){}




utl::MilliSeconds::MilliSeconds (long double m) : _m(m){}
utl::MilliSeconds::MilliSeconds (const Seconds &s) : _m(s._m*1e3){}
utl::MilliSeconds::MilliSeconds (const MilliSeconds &s) : _m(s._m){}
utl::MilliSeconds::MilliSeconds (const MicroSeconds &s) : _m(s._m*1e-3){}	



utl::MicroSeconds::MicroSeconds (long double m) : _m(m){}
utl::MicroSeconds::MicroSeconds (const Seconds &s) : _m(s._m*1e6){}
utl::MicroSeconds::MicroSeconds (const MilliSeconds &s) : _m(s._m*1e3){}
utl::MicroSeconds::MicroSeconds (const MicroSeconds &s) : _m(s._m){}		



utl::Timer::point utl::Timer::_start;
utl::Timer::point utl::Timer::_stop;

void utl::Timer::tic()
{
    _start = clock::now();
}

void utl::Timer::toc()
{
    _stop = clock::now();
}

utl::MicroSeconds utl::Timer::elapsed()
{
    const duration &dur = std::chrono::duration_cast<resolution>(_stop - _start);
    MicroSeconds s = double(dur.count());
    return s;
}

utl::MicroSeconds utl::Timer::elapsed(size_t run)
{
    const duration &dur = std::chrono::duration_cast<resolution>(_stop - _start);
    // duration in seconds
    MicroSeconds s = (double(dur.count()) / double(run));
    return s;
}


utl::Timer::point utl::Timer::start()
{
    return _start;
}

utl::Timer::point utl::Timer::stop()
{
    return _stop;
}
