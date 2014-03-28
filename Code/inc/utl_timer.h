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
#include <ostream>

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

class MilliSeconds;
class MicroSeconds;

class Seconds 
{
public:
	Seconds (long double m);
	Seconds (const Seconds &s);
	Seconds (const MilliSeconds &s);
	Seconds (const MicroSeconds &s);
	friend std::ostream& operator<< (std::ostream & out, const Seconds & s) { out << s._m << "[s]"; return out;}
	friend MilliSeconds;
	friend MicroSeconds;
private:
	long double _m;
};

class MilliSeconds 
{
public:
	MilliSeconds (long double m);
	MilliSeconds (const Seconds &s);
	MilliSeconds (const MilliSeconds &s);
	MilliSeconds (const MicroSeconds &s);
	friend std::ostream& operator<< (std::ostream & out, const MilliSeconds & s) { out << s._m << "[ms]"; return out;}
	friend Seconds;
	friend MicroSeconds;	
private:
	long double _m;
};

class MicroSeconds 
{
public:
	MicroSeconds (long double m);
	MicroSeconds (const Seconds &s);
	MicroSeconds (const MilliSeconds &s);
	MicroSeconds (const MicroSeconds &s);
	friend std::ostream& operator<< (std::ostream & out, const MicroSeconds & s) { out << s._m << "[us]"; return out;}
	friend Seconds;
	friend MilliSeconds;	
private:
	long double _m;
};

/*
Seconds operator "" _s(long double d)
{
	return Seconds(d);
}

MilliSeconds operator "" _ms(long double d)
{
	return MilliSeconds(d);
}

MicroSeconds operator "" _us(long double d)
{
	return MicroSeconds(d);
}
*/

class Timer
{
public :
		
    typedef std::chrono::high_resolution_clock clock;
    typedef typename clock::duration duration;
    typedef typename clock::time_point point;
    typedef typename clock::period period;
    typedef std::chrono::microseconds resolution;

    static void tic();
    static void toc();

    static point start();
    static point stop();

    static MicroSeconds elapsed();
    static MicroSeconds elapsed(size_t run);


private:
    explicit Timer(){}

    static point _start;
    static point _stop;
};
}
#endif
