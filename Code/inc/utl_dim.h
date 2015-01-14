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

#ifndef UTL_DIM_H
#define UTL_DIM_H


#include <vector>
#include <initializer_list> // c++11
#include <utility>

#include <utl_assert.h>


namespace utl{

/*! \class Dim utl_dim.h "inc/utl_dim.h"
  * \brief Utility class to store the shape of multidimensional structures.
  *
  * The shape of a multidimensional structure is stored within a vector which store
  * the size of each dimension. A shape can be known or unkown. In case of a known shape
  * all vector entries must be natural numbers. If there is an unkown dimension
  * the vector entry of the corresponding element is equal -1. There are
  * predefined matrix shapes such as scalars, vectors or matrices.
  * Any shape vector with more 2 dimension entries is called multidimensional array.
  */


class Dim
{
public:
    typedef /*typename*/ std::vector<int>::iterator             iterator;
    typedef /*typename*/ std::vector<int>::const_iterator const_iterator;

    typedef /*typename*/ std::vector<int>::reverse_iterator             reverse_iterator;
    typedef /*typename*/ std::vector<int>::const_reverse_iterator const_reverse_iterator;

    typedef /*typename*/ std::vector<int>::value_type        value_type;
    typedef /*typename*/ std::vector<int>::pointer           pointer;
    typedef /*typename*/ std::vector<int>::const_pointer     const_pointer;
    typedef /*typename*/ std::vector<int>::reference         reference;
    typedef /*typename*/ std::vector<int>::const_reference   const_reference;

	explicit Dim();


    template<typename HeadType, typename ... TailTypes>
    Dim(HeadType head, TailTypes ... tail): _vector()
    {
        if(sizeof...(tail) == 0)
            _vector.resize(2,head);
        else
            insert(head,tail...);
    }

    template<typename HeadType, typename ... TailTypes>
    void insert(HeadType head, TailTypes ... tail)
    {
        _vector.push_back(head);
        insert(tail...);
    }

    template<typename HeadType>
    void insert(HeadType head)
    {
        _vector.push_back(head);
    }

	Dim(const Dim&);
    Dim(Dim&&);
	~Dim();


    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;

    const_iterator find_last_not_equal_one() const;
          iterator find_last_not_equal_one();


    inline reference operator[](size_t pos) { return _vector[pos]; }
    inline const_reference operator[](size_t pos) const { return _vector[pos]; }

    inline reference at(size_t pos) { return _vector.at(pos); }
    inline const_reference at(size_t pos) const { return _vector.at(pos); }

    void push_back(const value_type& value);
    void pop_back(const value_type& value);
    iterator insert(iterator pos, const value_type& __x);
    void insert(iterator pos, size_t num, const value_type& value);
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void resize(size_t newsize);
    void resize(size_t newsize, const value_type &value);

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    inline size_t size() const { return _vector.size(); }
    bool empty() const;
    void clear();

    Dim  operator+(const Dim&) const;
    Dim  operator+(Dim&&);
    Dim  operator+(int) const;
    Dim  operator-(const Dim&) const;
    Dim  operator-(Dim&&);
    Dim  operator-(int) const;

    Dim&  operator+=(const Dim&);
    Dim&  operator+=(Dim&&);
    Dim&  operator+=(int);
    Dim&  operator-=(const Dim&);
    Dim&  operator-=(Dim&&);
    Dim&  operator-=(int);

    Dim&  operator++();
    Dim&  operator--();
    Dim   operator++(int);
    Dim   operator--(int);

	Dim& operator<<(int);
    Dim& operator=(const Dim&);
    Dim& operator=(Dim&&);

	bool operator==(const Dim&) const;
	bool operator!=(const Dim&) const;
    bool operator> (const Dim&) const;
    bool operator< (const Dim&) const;
    bool operator>=(const Dim&) const;
    bool operator<=(const Dim&) const;

    int operator==(int) const;
    int operator!=(int) const;
    int operator> (int) const;
    int operator< (int) const;
    int operator>=(int) const;
    int operator<=(int) const;

    value_type operator()(int pos) const;

	void setUnknown();
	void setScalar();

	bool allVar() const;
	bool allConst() const;
	bool hasVar() const;
	bool hasConst() const;
	size_t numVar() const;
	size_t numConst() const;

	int rows() const;
	int cols() const;

	bool isVar(size_t pos) const;
	bool isConst(size_t pos) const;

	bool isScalar() const;
	bool isVector() const;
	bool isMatrix() const;
	bool isEqualLessMatrix() const;
	bool isEqualLessVector() const;

    int maxPos() const;
    int minPos() const;

    int max() const;
    int min() const;

    void squeeze();
    Dim squeeze() const;
    static Dim squeeze(const Dim&);

    size_t prod(size_t __start, size_t __end) const;
	size_t prod(size_t __end) const;
	size_t prod() const;

    void print(std::ostream& out = std::cout) const;
    std::string toString() const;

    const std::vector<int> &vector() const;


private:
    std::vector<int> _vector;
	bool isDef() const;
};
}



#endif

