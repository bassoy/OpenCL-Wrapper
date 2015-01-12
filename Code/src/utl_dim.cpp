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

#include <algorithm>
#include <functional>
#include <climits>
#include <sstream>

#include <utl_dim.h>
#include <utl_assert.h>

/*! \brief Konstruktor der Dimklasse
 *
 *  Erzeugt ein Dimensionsobjekt mit unbekannter Dimension.
 */
utl::Dim::Dim() : _vector(2,-1)
{
}

/*! \brief Destruktor der Dimklasse */
utl::Dim::~Dim() {}

utl::Dim::Dim(const Dim& dim) :
    _vector(dim._vector)
{
}

utl::Dim::Dim(Dim&& dim) :
    _vector(std::move(dim._vector))
{
}

int utl::Dim::operator()(int pos) const
{
    return this->at(pos);
}

utl::Dim& utl::Dim::operator<<(int dim)
{
	this->push_back(dim);
	return *this;
}

void utl::Dim::resize(size_t newsize)
{
    this->_vector.resize(newsize);
}

void utl::Dim::resize(size_t newsize, const value_type &value)
{
    this->_vector.resize(newsize, value);
}


void utl::Dim::push_back(const value_type& value)
{
    this->_vector.push_back(value);
}

void utl::Dim::pop_back(const value_type& value)
{
    this->_vector.push_back(value);
}

utl::Dim::iterator utl::Dim::insert(iterator pos, const value_type& value)
{
    return this->_vector.insert(pos, value);
}

void utl::Dim::insert(iterator pos, size_t num, const value_type& value)
{
    this->_vector.insert(pos, num, value);
}

utl::Dim::iterator utl::Dim::erase(iterator pos)
{
    return this->_vector.erase(pos);
}

utl::Dim::iterator utl::Dim::erase(iterator first, iterator last)
{
    return this->_vector.erase(first,last);
}

utl::Dim::reference utl::Dim::front()
{
    return *this->begin();
}

utl::Dim::const_reference utl::Dim::front() const
{
    return *this->begin();
}


utl::Dim::reference utl::Dim::back()
{
    return *(this->end() - 1);
}


utl::Dim::const_reference utl::Dim::back() const
{
    return *(this->end() - 1);
}


utl::Dim& utl::Dim::operator=(const Dim& other)
{
    _vector = other._vector;
    return *this;
}


utl::Dim& utl::Dim::operator=(Dim&& other)
{
    _vector = std::move(other._vector);
    return *this;
}

utl::Dim::iterator utl::Dim::begin()
{
    return _vector.begin();
}


utl::Dim::const_iterator utl::Dim::begin() const
{
    return _vector.begin();
}

utl::Dim::iterator utl::Dim::end()
{
    return _vector.end();
}


utl::Dim::const_iterator utl::Dim::end() const
{
    return _vector.end();
}


utl::Dim::reverse_iterator utl::Dim::rbegin()
{
    return reverse_iterator(end());
}

utl::Dim::const_reverse_iterator utl::Dim::rbegin() const
{
    return const_reverse_iterator(end());
}

utl::Dim::reverse_iterator utl::Dim::rend()
{
    return reverse_iterator(begin());
}

utl::Dim::const_reverse_iterator utl::Dim::rend() const
{
    return const_reverse_iterator(begin());
}

bool utl::Dim::empty() const
{
    return _vector.empty();
}


void utl::Dim::clear()
{
    this->_vector.clear();
}

utl::Dim::const_iterator
utl::Dim::find_last_not_equal_one() const
{
    const const_reverse_iterator &this_rend = std::find_if(this->rbegin(), this->rend(), std::bind(std::not_equal_to<int>(), std::placeholders::_1, 1));
    return const_iterator(this_rend.base());
}

utl::Dim::iterator
utl::Dim::find_last_not_equal_one()
{
    const reverse_iterator &this_rend = std::find_if(this->rbegin(), this->rend(), std::bind(std::not_equal_to<int>(), std::placeholders::_1, 1));
    return iterator(this_rend.base());
}


/*! \brief Überprüfung auf Gleichheit.
 *
 *  Zwei Dimensionen sind gleich, wenn bei variabler Dimension die Knoten im Sinne
 *  der Vergleichsoperation in der Operationsklasse \ref Operation gleich sind und
 *  bei konstanter Dimension die Werte übereinstimmen.
 * \param dim Dimension
 */
bool utl::Dim::operator==(const Dim& other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());

	auto this_it  = this->begin(), this_it_end  = this->find_last_not_equal_one();
	auto other_it = other.begin(), other_it_end = other.find_last_not_equal_one();

    size_t this_diff  = this_it_end  - this_it;
    size_t other_diff = other_it_end - other_it;

    if(this_diff != other_diff) return false;

    return std::equal(this_it, this_it_end, other_it);
}

int utl::Dim::operator==(int d) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();
    int i = 0;
    for(; it != it_end; ++it){ if(*it == d) ++i; }
    return i;
}

/*! \brief Überprüfung auf Ungleichheit.
 *
 *  Zwei Dimensionen sind gleich, wenn bei variabler Dimension die Knoten im Sinne
 *  der Vergleichsoperation in der Operationsklasse \ref Operation ungleich sind und
 *  bei konstanter Dimension die Werte nicht übereinstimmen.
 * \param dim Dimension
 */
bool utl::Dim::operator!=(const Dim& other) const
{
	return !( (*this) == other ) ;
}

int utl::Dim::operator!=(int d) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();
    int i = 0;
    for(; it != it_end; ++it){ if(*it != d) ++i; }
    return i;
}


bool utl::Dim::operator>(const Dim &other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());

    auto this_it  = this->begin(), this_it_end  = this->find_last_not_equal_one();
    auto other_it = other.begin(), other_it_end = other.find_last_not_equal_one();

    size_t this_diff  = this_it_end  - this_it;
    size_t other_diff = other_it_end - other_it;


    if(this_diff > other_diff) return std::equal(other_it, other_it_end, other_it, std::greater<int>());
    else                       return std::equal(this_it, this_it_end, other_it, std::greater<int>());

}

int utl::Dim::operator>(int d) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();

    int i = 0;
    for(; it != it_end; ++it){ if(*it > d) ++i; }
    return i;
}

bool utl::Dim::operator<(const Dim &other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());

    auto this_it  = this->begin(), this_it_end  = this->find_last_not_equal_one();
    auto other_it = other.begin(), other_it_end = other.find_last_not_equal_one();

    size_t this_diff  = this_it_end  - this_it;
    size_t other_diff = other_it_end - other_it;


    if(this_diff > other_diff) return std::equal(other_it, other_it_end, other_it, std::less<int>());
    else                       return std::equal(this_it, this_it_end, other_it, std::less<int>());
}

int utl::Dim::operator<(int d) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();
    int i = 0;
    for(; it != it_end; ++it){ if(*it < d) ++i; }
    return i;
}


bool utl::Dim::operator>=(const Dim &other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot compare dimensions " << this->toString() << ", " << other.toString());

    auto this_it  = this->begin(), this_it_end  = this->find_last_not_equal_one();
    auto other_it = other.begin(), other_it_end = other.find_last_not_equal_one();

    size_t this_diff  = this_it_end  - this_it;
    size_t other_diff = other_it_end - other_it;

    if(this_diff > other_diff) return std::equal(other_it, other_it_end, other_it, std::greater_equal<int>());
    else                       return std::equal(this_it, this_it_end, other_it, std::greater_equal<int>());
}

int utl::Dim::operator>=(int d) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();
    int i = 0;
    for(; it != it_end; ++it){ if(*it >= d) ++i; }
    return i;
}

bool utl::Dim::operator<=(const Dim &other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());

    auto this_it  = this->begin(), this_it_end  = this->find_last_not_equal_one();
	auto other_it = other.begin(), other_it_end = other.find_last_not_equal_one();

    size_t this_diff  = this_it_end  - this_it;
    size_t other_diff = other_it_end - other_it;

//    DEBUG_COMMENT("other " << other.toString() << ", this " << this->toString());
//    DEBUG_COMMENT("other_diff " << other_diff << ", this_diff " << this_diff << ", " << (this_it - (this_it+2)));

    if(this_diff > other_diff) return std::equal(other_it, other_it_end, other_it, std::less_equal<int>());
    else                       return std::equal(this_it, this_it_end, other_it, std::less_equal<int>());
}

int utl::Dim::operator<=(int d) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined");
    //DEBUG_ASSERT(!this->hasVar(), "Cannot compare dimensions " << this->toString());

    auto it = this->begin();
    auto it_end = this->end();
    int i = 0;
    for(; it != it_end; ++it){ if(*it <= d) ++i; }
    return i;
}



utl::Dim
utl::Dim::operator+(const Dim& other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());


    Dim res; res.resize(this->size(),0);
    std::transform(this->begin(), this->end(), other.begin(), res.begin(), std::plus<int>());
    return res;
}

utl::Dim
utl::Dim::operator+(int d) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());


    Dim res; res.resize(this->size(),0);
    using namespace std::placeholders;
    std::transform(this->begin(), this->end(), res.begin(), std::bind(std::plus<int>(), _1, d));
    return res;
}


utl::Dim
utl::Dim::operator+(Dim&& other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());


    Dim res; res.resize(this->size(),0);
    std::transform(this->begin(), this->end(), other.begin(), res.begin(), std::plus<int>());
    return res;
}

utl::Dim
utl::Dim::operator-(const Dim &other) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    Dim res; res.resize(this->size(),0);
    std::transform(this->begin(), this->end(), other.begin(), res.begin(), std::minus<int>());
    return res;
}

utl::Dim
utl::Dim::operator-(int d) const
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    Dim res; res.resize(this->size(),0);
    using namespace std::placeholders;
    std::transform(this->begin(), this->end(), res.begin(), std::bind(std::minus<int>(), _1, d));
    return res;
}

utl::Dim
utl::Dim::operator-(Dim&& other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    Dim res; res.resize(this->size(),0);
    std::transform(this->begin(), this->end(), other.begin(), res.begin(), std::minus<int>());
    return res;
}

utl::Dim&
utl::Dim::operator+=(const Dim &other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), this->toString() << " != " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::plus<int>());
    return *this;
}

utl::Dim&
utl::Dim::operator+=(int d)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    using namespace std::placeholders;
    std::transform(this->begin(), this->end(), this->begin(), std::bind(std::plus<int>(), _1, d));
    return *this;
}

utl::Dim&  utl::Dim::operator+=(Dim &&other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), this->toString() << " != " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::plus<int>());
    return *this;
}

utl::Dim&  utl::Dim::operator-=(const Dim & other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), this->toString() << " != " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::minus<int>());
    return *this;
}

utl::Dim&
utl::Dim::operator-=(int d)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    using namespace std::placeholders;
    std::transform(this->begin(), this->end(), this->begin(), std::bind(std::minus<int>(), _1, d));
    return *this;
}

utl::Dim&  utl::Dim::operator-=(Dim &&other)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(other.isDef(), "Dimension is not defined");
    DEBUG_ASSERT(this->size() == other.size(), this->toString() << " != " << other.toString());
    DEBUG_ASSERT(!this->hasVar() && !other.hasVar(), "Cannot add dimensions " << this->toString() << ", " << other.toString());
    std::transform(this->begin(), this->end(), other.begin(), this->begin(), std::minus<int>());
    return *this;
}

utl::Dim&
utl::Dim::operator--()
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    std::for_each(this->begin(), this->end(), [](int n){--n;});
    return *this;
}

utl::Dim&
utl::Dim::operator++()
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    std::for_each(this->begin(), this->end(), [](int n){++n;});
    return *this;
}

utl::Dim
utl::Dim::operator--(int)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    std::for_each(this->begin(), this->end(), [](int n){--n;});
    return utl::Dim(*this);
}

utl::Dim
utl::Dim::operator++(int)
{
    DEBUG_ASSERT(this->isDef(), "Dimension is not defined");
    DEBUG_ASSERT(!this->hasVar(), "Cannot add dimensions " << this->toString());
    std::for_each(this->begin(), this->end(), [](int n){++n;});
    return utl::Dim(*this);
}



bool utl::Dim::allVar() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::all_of(this->begin(), this->end(), [](int i){ return i == -1; });
}


bool utl::Dim::allConst() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::all_of(this->begin(), this->end(), [](int i){ return i > -1; });
}


/*! \brief Prüft ob variable Dimensionen enthalten sind.*/
bool utl::Dim::hasVar() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
        return std::any_of(this->begin(), this->end(), [](int i){ return i < 0; });
}

/*! \brief Prüft ob konstante Dimensionen enthalten sind.*/
bool utl::Dim::hasConst() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::any_of(this->begin(), this->end(), [](int i){ return i > -1; });
}

/*! \brief gibt die Anzahl variabler Dimensionen zurück.*/
size_t utl::Dim::numVar() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::count(this->begin(), this->end(), -1);
}

/*! \brief Gibt die Anzahl konstanter Dimensionen */
size_t utl::Dim::numConst() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::count_if(this->begin(), this->end(), [](int i){return i > -1;});
}


/*! \brief Prüft ob die Dimension an der angegebenen Position variabel ist.*/
bool utl::Dim::isVar(size_t pos) const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return !isConst(pos);
}

/*! \brief Prüft ob die Dimension an der angegebenen Position konstant ist.*/
bool utl::Dim::isConst(size_t pos) const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return this->at(pos) != -1;
}


int utl::Dim::rows() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return this->at(0);
}

int utl::Dim::cols() const
{
	TRUE_ASSERT(isDef(), "Dimension is not defined");
	return at(1);
}

/*! \brief Dimension wird auf unbekannt gesetzt.*/
void utl::Dim::setUnknown()
{
	this->clear();
	this->push_back(-1);
	this->push_back(-1);
}

void utl::Dim::setScalar()
{
	this->clear();
	this->push_back(1);
	this->push_back(1);
}


/*! \brief Prüft ob die Dimensionsgrößen die eines Scalars entsprechen.
 *
 * Definition eines Skalars:\n
 * Gibt es variable Dimensionsgrößen oder konstante Größen größer 1, so wird falsch
 * ansonsten wahr ausgegeben:
 */
bool utl::Dim::isScalar() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");
	return std::all_of(this->begin(), this->end(), [](int i){ return i == 1; });
}

/*! \brief Prüft ob die Dimensionsgrößen die eines Vektors entsprechen.
 *
 * Definition eines Vektors:\n
 * Alle Dimensionsgrößen müssen einer 1 entsprechen, ausser die erste oder zweite
 * Richtung, die größer 1 oder variabel sein kann.
 * \li Beispiel für die Form eines Vektor: Dim=N,1 oder Dim=1,6.
 * \li Kein Vektor: Dim=1,1,N oder Dim=3,1,N.
 *
 * <em> Achtung : Die variable Größe könnte eine 1 sein, wodurch keine
 * Vektorform gegeben wäre, sondern einem Skalar entspreche. </em>
 */
bool utl::Dim::isVector() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");

	if( std::any_of(this->begin()+2, this->end(), [](int i){ return i != 1; }) ) return false;


  // row vector  
	if(at(0) == 1 && at(1) > 1 ) return true;

  // col vector
	if(at(1) == 1 && at(0) > 1 ) return true;

  return false;
}

/*!
	Wahr wenn.
	( 1,  3, 1...1)
	( 3,  1, 1,..1)
	(-1,  1, 1,..1)
	( 1, -1, 1,..1)
*/
bool utl::Dim::isEqualLessVector() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");

	if( std::any_of(this->begin()+2, this->end(), [](int i){ return i != 1; }) ) return false;

	if(at(0) == 1 && at(1) >= 1 ) return true;
	if(at(1) == 1 && at(0) >= 1 ) return true;

	if(at(0) == -1 && at(1) == 1 ) return true;
	if(at(1) == -1 && at(0) == 1 ) return true;

	return false;
}

/*! \brief Prüft ob die Dimensionsgrößen die eines Vektors entsprechen.
 *
 * Definition eines Vektors:\n
 * Alle Dimensionsgrößen müssen einer 1 entsprechen, ausser die beiden ersten
 * Richtungen, die größer 1 oder variabel sein müssen.
 * \li Beispiel für die Form einer Matrix: Dim=N,6 oder Dim=N,M.
 * \li Keine Matrix: Dim=1,N,M oder Dim=3,1,N.
 *
 * <em> Achtung : Die variablen Größen könnten einer 1 entsprechen, wodurch keine
 * Matrixform gegeben wäre, sondern einem Vektor oder Skalar entspreche.</em>
 */
bool utl::Dim::isMatrix() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");

	if( std::any_of(this->begin()+2, this->end(), [](int i){ return i != 1; }) ) return false;

	if(at(0) > 1 && at(1) > 1 ) return true;

  return false;
}


/*!
	Wahr wenn.
	( 3, 3, 1...1)
	( 3,-1, 1,..1)
	(-1, 3, 1,..1)
	(-1,-1, 1,..1)
*/
bool utl::Dim::isEqualLessMatrix() const
{
	DEBUG_ASSERT(isDef(), "Dimension is not defined");

	if( std::any_of(this->begin()+2, this->end(), [](int i){ return i != 1; }) ) return false;


	if(at(0) >= 1 && at(1) >=  1 ) return true;
	if(at(0) ==-1 && at(1) == -1 ) return true;
	if(at(0) >= 1 && at(1) == -1 ) return true;
	if(at(1) >= 1 && at(0) == -1 ) return true;


	return false;
}

/*! \brief Prüft ob die Dimensionen für alle Richtungen bekannt sind.
 *
 * Dimension ist definiert, wenn sie
 */
bool utl::Dim::isDef() const
{
	return this->size() >= 2;
}


/*! \brief Gibt die Position max Dimension zurück.
 *
 *  Alle Dimensionen müssen Konstanten sein. Ansonsten gibt es -1 zurück.
 */
int utl::Dim::maxPos() const
{
	DEBUG_ASSERT(isDef(), "this is not defined");
	if(this->hasVar()) return -1;
	return std::max_element(this->begin(), this->end()) - this->begin();
}


/*! \brief Gibt die min Dimension zurück.
 *
 *  Alle Dimensionen müssen Konstanten sein. Ansonsten gibt es -1 zurück.
 */
int utl::Dim::minPos() const
{
	DEBUG_ASSERT(isDef(), "this is not defined");
	if(this->hasVar()) return -1;
	return std::min_element(this->begin(), this->end()) - this->begin();
}

/*! \brief Gibt die Position max Dimension zurück.
 *
 *  Alle Dimensionen müssen Konstanten sein. Ansonsten gibt es -1 zurück.
 */
int utl::Dim::max() const
{
    DEBUG_ASSERT(isDef(), "this is not defined");
    if(this->hasVar()) return -1;
    return *std::max_element(this->begin(), this->end());
}


/*! \brief Gibt die min Dimension zurück.
 *
 *  Alle Dimensionen müssen Konstanten sein. Ansonsten gibt es -1 zurück.
 */
int utl::Dim::min() const
{
    DEBUG_ASSERT(isDef(), "this is not defined");
    if(this->hasVar()) return -1;
    return *std::min_element(this->begin(), this->end());
}


void utl::Dim::squeeze()
{
    if(this->size() == 2) return;
    for(auto it = this->begin(); it != this->end();)
    {
        if(*it != 1) { ++it; }
        else         { it = this->erase(it); }
    }
    while(this->size() < 2){ this->push_back(1); }
}

utl::Dim utl::Dim::squeeze() const
{
    Dim d = *this;
    d.squeeze();
    return d;
}

utl::Dim utl::Dim::squeeze(const Dim& dim)
{
    Dim d = dim;
    d.squeeze();
    return d;
}

/*! \brief Gibt das chartesiche Produkt der Dimension zurück
 *
 *  prod(p) = at(0) * at(1) * ... * at(p)
 */
size_t utl::Dim::prod(size_t __end) const
{
//    __comment("this.size() " << this->size() << ", __end " << __end);
	return this->prod(0,__end);
}

/*! \brief Gibt das chartesiche Produkt der Dimension zurück
 *
 *  prod(p) = at(s) * at(1) * ... * at(p)
 */
size_t utl::Dim::prod(size_t __start, size_t __end) const
{
    DEBUG_ASSERT(isDef(), "Dimension is not defined.");
    DEBUG_ASSERT(!hasVar(), "Kann nicht das chartesische Produkt der Dimensionen berechnen.");
    TRUE_ASSERT(__end <= this->size(), "End : " <<  __end << " > Size : " << this->size());
        TRUE_ASSERT(__start <= __end, "Start : " << __start << " > End : " << __end);

		const_iterator it_end = this->begin() + __end + 1;
		const_iterator it = this->begin() + __start;
		size_t p = 1;
		for(; it  != it_end; ++it)
			p *= *it;
		return p;

}

size_t utl::Dim::prod() const
{
//    __comment("this.size() " << this->size() << ", " << this->size() - 1);
	return this->prod(0,this->size()-1);
}


/*! \brief Ausgabe der Dimension unter Angabe des Ausgabestreams*/
void utl::Dim::print(std::ostream& out) const
{
	if(this->size() == 0) out << "()";
	out << "(";
	for(auto it = this->begin(); it != this->end()-1; ++it){
		out << (*it) << ", ";
	}
    out << this->back() << ")" << std::endl;
}

/*! \brief Ausgabe der Dimension unter Angabe des Ausgabestreams*/
std::string utl::Dim::toString() const
{
	if(this->size() == 0) return "()";
    std::stringstream ss;
	ss << "(";
	for(auto it = this->begin(); it != this->end()-1; ++it){
		ss << (*it) << ", ";
	}
    ss << this->back() << ")";
	return ss.str();
}

const std::vector<int> &utl::Dim::vector() const
{
    return this->_vector;
}

