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

template<class R>
typename utl::Timer<R>::point
utl::Timer<R>::_start;

template<class R>
typename utl::Timer<R>::point
utl::Timer<R>::_stop;

/*! \brief Use tic() to start time.*/
template<class R>
void utl::Timer<R>::tic()
{
    _start = clock::now();
}


/*! \brief Use toc() to stop time. */
template<class R>
void utl::Timer<R>::toc()
{
    _stop = clock::now();
}


/*! \brief Returns the duration between tic() and toc()
 *
 * Note that after calling elapsed, you need to call count()
 *
 * \param OtherResolution Resolution of the duration
*/

template<class R>
R utl::Timer<R>::elapsed()
{
	return std::chrono::duration_cast<R>( _stop - _start );
}

//template<class OtherResolution, class R>
//OtherResolution utl::Timer<R>::elapsed()
//{
//	return std::chrono::duration_cast<OtherResolution>( _stop - _start );
//}



template<class R>
typename utl::Timer<R>::point
utl::Timer<R>::start()
{
    return _start;
}

template<class R>
typename utl::Timer<R>::point
utl::Timer<R>::stop()
{
    return _stop;
}

template class utl::Timer<utl::NanoSeconds>;
template class utl::Timer<utl::MicroSeconds>;
template class utl::Timer<utl::MilliSeconds>;
template class utl::Timer<utl::Seconds>;


//template<class T>
//std::ostream& operator<< (std::ostream & out, const T& d)
//{
//	out << d.count() << std::endl;
//	return out;
//}

std::ostream& operator<< (std::ostream & out, const utl::Seconds& d)
{
	out << d.count();
	return out;
}
