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

template<class D>
typename utl::Timer<D>::point
utl::Timer<D>::_start;

template<class D>
typename utl::Timer<D>::point utl::Timer<D>::_stop;

/*! \brief Use tic() to start time.*/
template<class D>
void utl::Timer<D>::tic()
{
    _start = clock::now();
}


/*! \brief Use toc() to stop time. */
template<class D>
void utl::Timer<D>::toc()
{
    _stop = clock::now();
}


/*! \brief Returns the duration between tic() and toc()
 *
 * Note that after calling elapsed, you need to call count()
 *
 * \param OtherResolution Resolution of the duration
*/

template<class D>
D utl::Timer<D>::elapsed()
{
	return std::chrono::duration_cast<D>( _stop - _start );
}

template<class D>
typename utl::Timer<D>::point
utl::Timer<D>::start()
{
    return _start;
}

template<class D>
typename utl::Timer<D>::point
utl::Timer<D>::stop()
{
    return _stop;
}

template class utl::Timer<utl::NanoSeconds>;
template class utl::Timer<utl::MicroSeconds>;
template class utl::Timer<utl::MilliSeconds>;
template class utl::Timer<utl::Seconds>;

std::ostream& operator<< (std::ostream & out, utl::Seconds d)
{
	out << d.count();
	return out;
}

std::ostream& operator<< (std::ostream & out, utl::MilliSeconds d)
{
	out << d.count();
	return out;
}

std::ostream& operator<< (std::ostream & out, utl::MicroSeconds d)
{
	out << d.count();
	return out;
}

std::ostream& operator<< (std::ostream & out, utl::NanoSeconds d)
{
	out << d.count();
	return out;
}
