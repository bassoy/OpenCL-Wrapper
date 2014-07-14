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

#ifndef UTL_MATRIX_H
#define UTL_MATRIX_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <ostream>
#include <algorithm>

#include <utl_assert.h>


namespace utl{



/*! \class Matrix utl_matrix.h "inc/utl_args.h"
  * \brief MatrixBase class which corresponds to a two-dimensional array.
  *
  * An Args object can be instantiated in order to conveniently store the arguments
  * from the console. Arguments are strored as strings which can be converted
  * to other data types.
  */

template<class T>
class __MatrixBase
{
public:

	typedef typename std::vector<T>::pointer           pointer;
	typedef typename std::vector<T>::const_pointer     const_pointer;
	typedef typename std::vector<T>::reference         reference;
	typedef typename std::vector<T>::const_reference   const_reference;
	typedef typename std::vector<T>::value_type        value_type;

	typedef typename std::vector<T>::iterator          iterator;
	typedef typename std::vector<T>::const_iterator    const_iterator;


    __MatrixBase& operator = (value_type value) { std::fill(this->begin(), this->end(), value); return *this; }
    __MatrixBase& operator = (const __MatrixBase& m) { _vector = m._vector; _rows = m._rows; _cols = m._cols; return *this;}
    __MatrixBase& operator = (__MatrixBase&& m) { _vector = std::move(m._vector); _rows = m._rows; _cols = m._cols; return *this; }



	bool operator==(const_reference value) const {
        return std::all_of(this->begin(), this->end(), [value](const_reference ref){return ref == value;});
    }

	bool operator!=(const_reference value) const {
		return std::all_of(this->begin(), this->end(), [value](const_reference ref){return ref != value;});
	}


    bool operator==(const __MatrixBase& m) const {
        if(m.dim() != this->dim()) return false;
        return std::equal(this->begin(), this->end(), m.begin());
    }

	bool operator!=(const __MatrixBase& m) const {
		if(m.dim() != this->dim()) return true;
		return std::equal(this->begin(), this->end(), m.begin(), std::not_equal_to<value_type>() );
	}

    bool operator==(__MatrixBase&& m) {
        if(m.dim() != this->dim()) return false;
        return std::equal(this->begin(), this->end(), m.begin());
    }

	bool operator!=(__MatrixBase&& m) {
		if(m.dim() != this->dim()) return true;
		return std::equal(this->begin(), this->end(), m.begin(), std::not_equal_to<value_type>());
	}

    __MatrixBase operator*(const_reference value) const
    {
        __MatrixBase res = *this;
        std::for_each(res.begin(), res.end(), [value]( reference r){ r *= value; });
        return res;
    }


    __MatrixBase operator+(const_reference value) const
    {
        __MatrixBase res = *this;
        std::for_each(res.begin(), res.end(), [value]( reference r){ r += value; });
        return res;
    }

    __MatrixBase operator+(const __MatrixBase& rhs) const
    {
        const __MatrixBase& lhs = *this;
        TRUE_ASSERT(lhs.dim() == rhs.dim(), "Dimensions must be equal");
        __MatrixBase res(lhs.rows(), lhs.cols());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), res.begin(), std::plus<T>());
        return res;
    }

    __MatrixBase operator-(const_reference value) const
    {
        __MatrixBase res = *this;
        std::for_each(res.begin(), res.end(), [value]( reference r){ r -= value; });
        return res;
    }

    __MatrixBase operator-(const __MatrixBase& rhs) const
    {
        const __MatrixBase& lhs = *this;
        TRUE_ASSERT(lhs.dim() == rhs.dim(), "Dimensions must be equal");
        __MatrixBase res(lhs.rows(), lhs.cols());
        std::transform(lhs.begin(), lhs.end(), rhs.begin(), res.begin(), std::minus<T>());
        return res;
    }

    __MatrixBase operator/(const_reference value) const
    {
        return this->operator *(value_type(1)/value);
    }


    void resize(size_t rows, size_t cols, value_type value = value_type()) { _rows = rows; _cols = cols; this->_vector.resize(rows*cols, value);}
    size_t size() const { return this->vector().size(); }

    pointer data() { return this->_vector.data(); }
    const_pointer data() const { return this->vector().data(); }

    size_t rows() const { return this->_rows; }
    size_t cols() const { return this->_cols; }

    std::pair<size_t,size_t> dim() const { return std::make_pair(_rows,_cols); }

    const std::vector<T>& vector() const { return this->_vector; }

    const_iterator begin() const { return const_iterator( this->data() ); }
    const_iterator end() const   { return const_iterator( this->data()+this->size() ); }
    iterator begin() { return iterator ( this->data()  ); }
    iterator end()   { return iterator( this->data()+this->size() ); }


    reference operator[](size_t index) { return _vector[index]; }
    const_reference operator[](size_t index) const { return _vector[index]; }

    reference front() { return this->_vector.front(); }
    const_reference front() const { return this->_vector.front(); }
    reference back() { return this->_vector.back(); }
    const_reference back() const { return this->_vector.back(); }

    bool is_scalar() const { return _rows == 1 && _cols == 1; }
    bool is_vector() const { return !is_scalar() && (_rows == 1 || _cols == 1); }
    bool is_matrix() const { return _rows > 1 && _cols > 1; }

    virtual ~__MatrixBase() {};

protected:

    __MatrixBase(size_t rows, size_t cols, const_reference value) :  _vector(rows*cols, value), _rows(rows), _cols(cols) {}
    __MatrixBase(size_t rows, size_t cols) :  _vector(rows*cols), _rows(rows), _cols(cols) {}
    __MatrixBase(const __MatrixBase& m) : _vector(m._vector), _rows(m._rows), _cols(m._cols) {}
    __MatrixBase(__MatrixBase&& m) : _vector(std::move(m._vector)), _rows(m._rows), _cols(m._cols) {}
		__MatrixBase() : _vector(), _rows(0u), _cols(0u) {}




    std::vector<T> _vector;

    size_t _rows;
    size_t _cols;
};

}




namespace utl{

///  Marking column_major matrices.
struct column_major_tag { };

///  Marking row_major matrices.
struct row_major_tag  { };

}





namespace utl{

template <class T, class F>
class Matrix : private __MatrixBase<T>
{
    Matrix() = delete;
};
}



namespace utl{


template <class T>
class Matrix<T, column_major_tag> : public __MatrixBase<T>
{
    using Base = __MatrixBase<T>;

public :

    typedef typename Base::pointer           pointer;
    typedef typename Base::const_pointer     const_pointer;
    typedef typename Base::reference         reference;
    typedef typename Base::const_reference   const_reference;
    typedef typename Base::value_type        value_type;

    typedef typename Base::iterator          iterator;
    typedef typename Base::const_iterator    const_iterator;

    Matrix(size_t rows, size_t cols, const_reference value) :  Base(rows,cols, value) {}
    Matrix(size_t rows, size_t cols) :  Base(rows,cols) {}
    Matrix(const Matrix& m) : Base(m) {}
    Matrix(Matrix&& m) : Base(std::move(m)) {}
	Matrix() : Base() {}
    ~Matrix() = default;

    Matrix& operator = (value_type value) { Base::operator=( value ); return *this; }
    Matrix& operator = (const Matrix& m) { Base::operator=( m ); return *this; }
    Matrix& operator = (Matrix&& m) { Base::operator=( std::move(m) ) ;  return *this; }


	bool operator==(const Matrix& m) const { return Base::operator ==( m ); }
	bool operator!=(const Matrix& m) const { return Base::operator !=( m ); }

	bool operator==(Matrix&& m) { return Base::operator ==( std::move(m) ); }
	bool operator!=(Matrix&& m) { return Base::operator !=( std::move(m) ); }

	bool operator==(const_reference m) const { return Base::operator ==( m ); }
	bool operator!=(const_reference m) const { return Base::operator !=( m ); }


    Matrix operator*(const_reference value) const { return Base::operator*(value);  }
    Matrix operator+(const_reference value) const { return Base::operator+(value);  }
    Matrix operator+(const Matrix& rhs) const     { return Base::operator+(rhs);   }
    Matrix operator-(const_reference value) const { return Base::operator-(value); }
    Matrix operator-(const Matrix& rhs) const     { return Base::operator-(rhs);   }
    Matrix operator/(const_reference value) const { return Base::operator/(value); }

	Matrix operator*(const Matrix& B) const
    {


		const Matrix& A = *this;
		TRUE_ASSERT(A.cols() == B.rows(), "LhsMatrix.cols() != RhsMatrix.rows()");

		const size_t M = A.rows();
		const size_t N = B.cols();
		const size_t K = A.cols();


		Matrix C(M, N);


//		auto nB = B.begin(), nBEnd = rhs.begin() + K*N;
//		auto nC = C.begin();
//		auto mBEnd = A.begin() + M;

//		for(; nB != nBEnd; nB += K, nC += M)
//		{
//			auto mA = A.begin();
//			auto mC = nC;

//			for(; mA != mAEnd; ++mA, ++mC)
//			{
//				auto kA = mA;
//				auto kB = nB;
//				auto kBEnd = kB + K;

//				for(; kB != kBEnd; kA += M, ++kB){
//					*mC += *kA * *kB;
//				}
//			}
//		}
//		return C;


		auto nB = B.begin();
		auto nBEnd = nB + K*N;
		auto nC = C.begin();

		for(; nB != nBEnd; nB += K, nC += M)
		{
			auto kA = A.begin();
			auto kB = nB;

			auto kBEnd = kB + K;

			for(; kB != kBEnd; kA += M, ++kB)
			{

				auto mA = kA;
				auto mC = nC;

				auto mAEnd = mA + M;

				for(; mA != mAEnd; ++mA, ++mC)
				{
					*mC += *mA * *kB;
				}
			}
		}
		return C;
    }


    reference at(size_t row, size_t col) { return this->_vector.at(row + col * this->_rows); }
    const_reference at(size_t row, size_t col) const { return this->_vector.at(row + col * this->_rows); }

    friend std::ostream& operator<< (std::ostream & out, const Matrix & m)
    {
        if(m.is_scalar()){
            out << "[" << m.front() << "];";
            return out;
        }

        if(m.is_vector()) {
            std::string cat = m.rows() > m.cols() ? "; " : ", ";
            out << "[";
            auto it = m.begin();
            for(auto it_end = m.end()-1; it != it_end; ++it)
                out << *it << cat;
            out << *it << "];";
            return out;
        }


        out << "[..." << std::endl;
        for(size_t row = 0, row_end = m.rows(); row < row_end; ++row)
        {
            size_t col = 0;
            for(size_t col_end = m.cols()-1; col < col_end; ++col)
                out << m.at(row,col) << " ";
            out << m.at(row,col) << ";..." << std::endl;
        }
        out << "];" << std::endl;
        return out;
    }

private:
    Matrix(Base &&base) :
        Base(base)
    {
    }
};
}


template<class T>
bool operator==(typename utl::Matrix<T,utl::column_major_tag>::const_reference value, const utl::Matrix<T,utl::column_major_tag>& matrix) {
	return matrix == value;
}

template<class T>
bool operator!=(typename utl::Matrix<T,utl::column_major_tag>::const_reference value, const utl::Matrix<T,utl::column_major_tag>& matrix) {
	return matrix != value;
}


namespace utl{
template <class T>
class Matrix<T, row_major_tag> : public __MatrixBase<T>
{
    using Base = __MatrixBase<T>;
public :

    typedef typename Base::pointer           pointer;
    typedef typename Base::const_pointer     const_pointer;
    typedef typename Base::reference         reference;
    typedef typename Base::const_reference   const_reference;
    typedef typename Base::value_type        value_type;

    typedef typename Base::iterator          iterator;
    typedef typename Base::const_iterator    const_iterator;

    Matrix(size_t rows, size_t cols, const_reference value) :  Base(rows,cols, value) {}
    Matrix(size_t rows, size_t cols) :  Base(rows,cols) {}
    Matrix(const Matrix& m) : Base(m) {}
    Matrix(Matrix&& m) : Base(std::move(m)) {}
	Matrix() : Base() {}
    ~Matrix() = default;

	bool operator==(const Matrix& m) const { return Base::operator ==( m ); }
	bool operator!=(const Matrix& m) const { return Base::operator !=( m ); }

	bool operator==(Matrix&& m) { return Base::operator ==( std::move(m) ); }
	bool operator!=(Matrix&& m) { return Base::operator !=( std::move(m) ); }

	bool operator==(const_reference m) const { return Base::operator ==( m ); }
	bool operator!=(const_reference m) const { return Base::operator !=( m ); }

    Matrix& operator = (value_type value) { Base::operator=( value ); return *this; }
    Matrix& operator = (const Matrix& m) { Base::operator=( m ); return *this; }
    Matrix& operator = (Matrix&& m) { Base::operator=( std::move(m) ) ;  return *this; }


    Matrix operator*(const_reference value) const { return Base::operator*(value); }
    Matrix operator+(const_reference value) const { return Base::operator+(value); }
    Matrix operator+(const Matrix& rhs) const     { return Base::operator+(rhs); }
    Matrix operator-(const_reference value) const { return Base::operator-(value); }
    Matrix operator-(const Matrix& rhs) const     { return Base::operator-(rhs); }
    Matrix operator/(const_reference value) const { return Base::operator/(value); }

	Matrix operator*(const Matrix& B) const
    {
		const Matrix& A = *this;
		TRUE_ASSERT(A.cols() == B.rows(), "LhsMatrix.cols() != RhsMatrix.rows()");


		const size_t M = A.rows();
		const size_t N = B.cols();
		const size_t L = A.cols();

		Matrix C(M, N);


//		auto nB = B.begin(), nBEnd = B.begin() + N;
//		auto nC = C.begin();
//		auto mAEnd = A.begin() + K * M;

//		for(; nB != nBEnd; ++nB, ++nC)
//		{
//			auto mA = A.begin();
//			auto mC = nC;
//			for(; mA != mAEnd; mA += K, mC += N)
//			{
//				auto kA = mA;
//				auto kB = nB;
//				auto kBEnd = kB + N * K;
//				for(; kB != kBEnd; ++kA, kB += N){
//					*mC += *kA * *kB;
//				}
//			}
//		}
//		return C;

		auto mA = A.begin();
		auto mC = C.begin();

		auto kBEnd = B.begin() + L * N;
		auto mAEnd = A.begin() + M * L;

		for(; mA != mAEnd; mA += L, mC += N)
		{
			auto kA = mA;
			auto kB = B.begin();

			for(; kB != kBEnd; ++kA, kB += N)
			{
				auto nC = mC;
				auto nB = kB;
				auto nBEnd = nB + N;

				for(; nB != nBEnd; ++nB, ++nC)
				{
					*nC += *kA * *nB;
				}

			}
		}

		return C;
    }



    reference at(size_t row, size_t col) { return this->_vector.at(row*this->_cols + col); }
    const_reference at(size_t row, size_t col) const { return this->_vector.at(row*this->_cols + col); }



    friend std::ostream& operator<< (std::ostream & out, const Matrix & m)
    {
        if(m.is_scalar()){
            out << "[" << m.front() << "];";
            return out;
        }

        if(m.is_vector()) {
            std::string cat = m.rows() > m.cols() ? "; " : ", ";
            out << "[";
            auto it = m.begin();
            for(auto it_end = m.end()-1; it != it_end; ++it)
                out << *it << cat;
            out << *it << "];";
            return out;
        }

        out << "[..." << std::endl;
        for(size_t row = 0, row_end = m.rows(); row < row_end; ++row)
        {
            size_t col = 0;
            for(size_t col_end = m.cols()-1; col < col_end; ++col)
                out << m.at(row,col) << " ";
            out << m.at(row,col) << ";..." << std::endl;
        }
        out << "];" << std::endl;
        return out;
    }
private:

    Matrix(Base &&base) :
        Base(base)
    {
    }
};
}


template<class T>
bool operator==(typename utl::Matrix<T,utl::row_major_tag>::const_reference value, const utl::Matrix<T,utl::row_major_tag>& matrix) {
	return matrix == value;
}

template<class T>
bool operator!=(typename utl::Matrix<T,utl::row_major_tag>::const_reference value, const utl::Matrix<T,utl::row_major_tag>& matrix) {
	return matrix != value;
}


#include <random>
#include <limits>

namespace utl{

///  Marking column_major matrices.
struct uniform_dist_tag { };

///  Marking row_major matrices.
struct normal_dist_tag  { };

template <class T, class F, class D>
class Rand : public Matrix<T,F>
{
    Rand() = delete;
};

template<class Engine, class Vector, bool>
struct uniform_int_distribution
{
    typedef typename Vector::value_type Type;
    static void run(Engine, Vector&, Type, Type){}
};

template<class Engine, class Vector>
struct uniform_int_distribution<Engine, Vector, true>
{
    typedef typename Vector::value_type Type;
    static void run(Engine engine, Vector& vector, Type min, Type max)
    {
        std::uniform_int_distribution<Type> dist(min, max);
        for(auto &v : vector)
            v = dist(engine);
    }
};

template<class Engine, class Vector, bool>
struct uniform_real_distribution
{
    typedef typename Vector::value_type Type;
    static void run(Engine, Vector&, Type, Type){}
};

template<class Engine, class Vector>
struct uniform_real_distribution<Engine, Vector, true>
{
    typedef typename Vector::value_type Type;
    static void run(Engine engine, Vector& vector, Type min, Type max)
    {
        std::uniform_real_distribution<Type> dist(min, max);
        for(auto &v : vector)
            v = dist(engine);
    }
};


template <class T, class F>
class Rand<T,F,uniform_dist_tag> : public Matrix<T,F>
{
public :
    typedef typename Matrix<T,F>::value_type        value_type;

    Rand(size_t rows, size_t cols, value_type min = value_type(0), value_type max = value_type(1)) :
        utl::Matrix<T,F>(rows, cols)
    {
        typedef std::mt19937 Engine;
        typedef std::vector<T> Vector;
        // Seed with a real random value, if available
        std::random_device device;

        Engine engine(device());

        constexpr bool is_integral = std::is_integral<T>::value;
        constexpr bool is_floating_point = std::is_floating_point<T>::value;

        uniform_int_distribution<Engine, Vector, is_integral>::run(engine, this->_vector , min, max);
        uniform_real_distribution<Engine, Vector, is_floating_point>::run(engine, this->_vector, min, max);
    }
};


template<class Engine, class Vector, bool isFloat>
struct normal_distribution
{
    static void run(Engine engine, Vector& vector, double mean, double dev)
    {
        std::normal_distribution<> dist(mean, dev);
        for(auto &v : vector)
            v = std::round(dist(engine));
    }
};

template<class Engine, class Vector>
struct normal_distribution<Engine, Vector, true>
{
    static void run(Engine engine, Vector& vector, double mean, double dev)
    {
        std::normal_distribution<> dist(mean, dev);
        for(auto &v : vector)
            v = dist(engine);
    }
};



template <class T, class F>
class Rand<T,F,normal_dist_tag> : public Matrix<T,F>
{
public :

    Rand(size_t rows, size_t cols, double mean = 0.0, double dev = 1.0) :
        utl::Matrix<T,F>(rows, cols)
    {
        typedef std::mt19937 Engine;
        typedef std::vector<T> Vector;

        // Seed with a real random value, if available
        std::random_device device;

        Engine engine(device());

        constexpr bool is_floating_point = std::is_floating_point<T>::value;

        normal_distribution<Engine, Vector, is_floating_point>::run(engine, this->_vector , mean, dev);

    }
};


template <class T, class F>
class Ones : public Matrix<T,F>
{
public :
    typedef typename Matrix<T,F>::value_type        value_type;
    Ones(size_t rows, size_t cols) :
        utl::Matrix<T,F>(rows, cols, value_type(1))
    {}
};

template <class T, class F>
class Zeros : public Matrix<T,F>
{
public :
    typedef typename Matrix<T,F>::value_type        value_type;
    Zeros(size_t rows, size_t cols) :
        utl::Matrix<T,F>(rows, cols, value_type(0))
    {}
};

template <class T, class F>
class Eye : public Matrix<T,F>
{
public :
	typedef typename Matrix<T,F>::value_type        value_type;
	Eye(size_t rows, size_t cols) :
		utl::Matrix<T,F>(rows, cols, value_type(0))
	{
		const auto M = rows, N = cols;
		for(auto n = 0u; n < N; ++n)
		{
			for(auto m = 0u; m < M; ++m)
			{
				if(m == n) this->at(m,n) = value_type(1);
			}
		}
	}
};


}
#endif
