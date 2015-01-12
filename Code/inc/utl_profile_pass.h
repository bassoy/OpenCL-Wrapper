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

#ifndef UTL_PROFILE_PASS_H
#define UTL_PROFILE_PASS_H


#include <ostream>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>

#include <utl_stream.h>
#include <utl_dim.h>
#include <utl_timer.h>




namespace utl{


/*! \class ProfilePass utl_profile_pass.h "inc/utl_profile_pass.h"
  * \brief Utility class to profile functions.
  *
  * The ProfilePass class can be inherited to profile a function.
  * Objects from inherited classes of ProfilePass can be stored in a
  * ProfilePassManager object which runs all ProfilePass functions.
  * Inhertied objects need to implement the prof and ops functions.
  */

class ProfilePass
{
public:
	using _Timer = Timer<Seconds>;

	ProfilePass(const std::string& str, const Dim& start, const Dim& step, const Dim& end, size_t __iter = 10);
	ProfilePass() = delete;
	ProfilePass(const ProfilePass&);
	virtual ~ProfilePass() = default;

	template<class F>
	Seconds call(F&& func)
	{
		_Timer::tic();
		for (size_t j = 0; j < _iter; j++){
			func();
		}
		_Timer::toc();
		return _Timer::elapsed() / _iter;
	}


	virtual Seconds prof(const Dim&) = 0;
    virtual double ops(const Dim&) = 0;

	void run();

	const std::vector<double> & dims() const;
	const std::vector<Seconds>& times() const;
	const std::vector<double> & ops() const;
	const std::vector<double> & perf() const;
	const std::string &name() const;

	void setCountUp();
	void setCountDown();

	bool printNumber() const;
	bool printTime() const;
	bool printOperation() const;
	bool printPerformance() const;

	std::string toString(const std::vector<double>& v) const;
	std::string toString(const std::vector<Seconds>& v) const;

protected:
	std::string _name;
    utl::Dim _start, _step, _end;
	size_t _iter;
    bool _print_n, _print_t, _print_o, _print_p;

    std::vector<double> _elems;
	std::vector<double> _ops;   /*!< Number of operations at each dimension [ ops ]*/
	std::vector<Seconds> _times; /*!< Time needed for the operation [ s ] */
	std::vector<double> _perf;  /*!< Number of operations per Time [ Ops / s ]*/

    bool _countUp;

private :
  template< typename S >
  static std::ostream& indirection( std::ostream& os, S const& t )
  { return os << t; }
  
  template< typename S, typename U >
  static std::ostream& indirection( std::ostream& os, std::chrono::duration<S,U> const& t )
  { return os << std::chrono::duration_cast< std::chrono::duration< S > >( t ).count(); }
};

}

std::ostream& operator <<(std::ostream &out, const utl::ProfilePass& pass);



#endif
