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

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <chrono>

#include <utl_assert.h>
#include <utl_stream.h>
#include <utl_storage.h>
#include <utl_dim.h>

/** \file util.cpp
 * \brief Implementierung der Schnittstelle zur Benutzung von Hilfsfunktionen wie z.B. zur Ein-/Ausgabe von Matrizen.
 */

/*! \brief Ausgabe einer Matrix in eine Datei
  * 
	* \param name    Dateiname
	* \param matrix  N-dimensionale Matrix, die linear in die Datei name geschrieben werden soll
	* \param mode    Wenn ios::out | ios::binary, dann wird binär rausgeschrieben, ansonsten ASCII
	*
	* Wird die Matrix als Text ausgegeben, so ist die Formatierung in Bezug auf die Dimensionen
    * nicht vorhanden. Benutze anstatt dessen die Funtion print
  */ 
template<class T>
void utl::write(const std::string &name, T *matrix, size_t size, std::ios::openmode mode){
	std::ofstream os;
	os.open (name.c_str(), mode );
	TRUE_ASSERT(!os.fail(), "Error opening file: " << name);
	os.write((const char*)matrix, size);
	TRUE_ASSERT(!os.bad(), "Error writing file: " << name);
	os.close();
}

template void utl::write<char> (const std::string &name, char *matrix, size_t size, std::ios::openmode mode);
template void utl::write<int> (const std::string &name, int *matrix, size_t size, std::ios::openmode mode);
template void utl::write<size_t> (const std::string &name, size_t *matrix, size_t size, std::ios::openmode mode);
template void utl::write<float> (const std::string &name, float *matrix, size_t size, std::ios::openmode mode);
template void utl::write<double> (const std::string &name, double *matrix, size_t size, std::ios::openmode mode);


/*! \brief Einlesen einer Matrix von einer Datei
	*
	* \param name    Dateiname
	* \param matrix  N-dimensionale Matrix, die linear in die Datei name geschrieben werden soll
	* \param mode    Wenn ios::in | ios::binary, dann wird binär eingelesen, ansonsten ASCII
	*
	* Wird die Matrix als Text eingelesen wird, so ist wird keine Formatierung vorgenommen. Den
	* Dimensionsvector muss man im nachhinein erstellen.
	*/
template<class T>
void utl::read(const std::string &name, T *matrix, size_t size, std::ios::openmode mode){
	std::ifstream is;
	is.open (name.c_str(), mode);
	TRUE_ASSERT(!is.fail(), "Error opening file: " << name);
    is.seekg (0, std::ios::end);
	auto length = is.tellg(); // number of characters, bytes
    is.seekg (0, std::ios::beg);
	TRUE_ASSERT(static_cast<std::streampos>(size) == length, size << " != " << length);
	is.read((char*)matrix, size);
	TRUE_ASSERT(!is.bad(), "Error reading file: " << name);
	is.close();
}

template void utl::read<char> (const std::string &name, char *matrix, size_t size, std::ios::openmode mode);
template void utl::read<int> (const std::string &name, int *matrix, size_t size, std::ios::openmode mode);
template void utl::read<size_t> (const std::string &name, size_t *matrix, size_t size, std::ios::openmode mode);
template void utl::read<float> (const std::string &name, float *matrix, size_t size, std::ios::openmode mode);
template void utl::read<double> (const std::string &name, double *matrix, size_t size, std::ios::openmode mode);

/*! \brief Initialisierung eines Feldes mit Pseudo-Zufallszahlen
  * 
	* \param data     Zeiger eine N-dimensionale Matrix
	* \param maxData  Bezeichnet das Maximum Element mit dem data initialisiert wird.
  */ 

template<class T>
void utl::randn(T *data, size_t num, T maxData)
{
	using namespace std::chrono;
	srand(static_cast< unsigned> (duration_cast<microseconds>(system_clock::now().time_since_epoch()).count()));


	if(maxData == (T)RAND_MAX){
		for (size_t i = 0; i < num; ++i){
				data[i] = (T)rand();
		}
		return;
	}
	for (size_t i = 0; i < num; ++i){
			data[i] = (T)( ( (double)rand() / (double)RAND_MAX ) * (double)maxData);
	}
}


template void utl::randn<char>(char *data, size_t num, char maxData);
template void utl::randn<int>(int *data, size_t num, int maxData);
template void utl::randn<size_t>(size_t *data, size_t num, size_t maxData);
template void utl::randn<float>(float *data, size_t num, float maxData);
template void utl::randn<double>(double *data, size_t num, double maxData);



template<class T>
inline void print(T a, std::ostream &out)
{
	if(std::is_same<T,char>())              { out << static_cast<int>(a);}
    else if(std::is_same<T,unsigned char>()){ out << static_cast<unsigned int>(a);}
	else                                    { out << a; }
}


template<class T>
inline void print1D(const T *A, const utl::Dim &dims, const int offset,  std::ostream &out)
{
	int maxDim = dims.at(1);
    std::string cat = ", ";

	if(dims.at(0) > 1){
		maxDim = dims.at(0);
		cat = ";";
	}

	out << "[";
    for(int i = 0; i < maxDim-1; ++i){
        print( A[i+offset], out);
        out << cat;
    }
    print(A[maxDim-1+offset],out);
    out  << "]";
}


template<class T>
inline void print2D(const T *A, const utl::Dim &dims, const int offset, const utl::Storage & s, std::ostream &out)
{
    out << "[..." << std::endl;
	for(int i = 0; i < dims.at(0); ++i){
		for(int j = 0; j < dims.at(1); ++j){
            if(s == utl::ColMajor) {print(A[j*dims.at(0)+i+offset],out); out << " ";}
            else              {print(A[i*dims.at(1)+j+offset],out); out << " ";}
		}
        out << ";..." << std::endl;
	}
	out << "]";
}

template<class T>
inline void print3D(const T *A, const utl::Dim &dims, const int offset, const utl::Storage & s, std::ostream &out)
{
    out << "cat(3,..." << std::endl;
	for(int k = 0; k < dims.at(2) ; ++k){ // numLayer = q
		int o = k * dims.at(0) * dims.at(1) + offset;
        print2D(A, dims, o, s, out);
		if(k < dims.at(2)-1) out << "," ;
        out << "..." << std::endl;
	}
	out << ")";
}

template<class T>
inline void print4D(const T *A, const utl::Dim &dims, const int offset, const utl::Storage & s, std::ostream &out)
{
    out << "cat(4,..." << std::endl;
	for(int k = 0; k < dims.at(3); ++k){ // numLayer = q
		int o = k * dims.at(0) * dims.at(1) * dims.at(2) + offset;
        print3D(A, dims, o, s, out);
        if(k < dims.at(3)-1) out << ",..." << std::endl;
	}
	out << ")";
}

template<class T>
inline void printND(const T* A, const utl::Dim &dims, std::vector<int> &offsets, int &size, const utl::Storage & s, std::ostream &out)
{
    out << "cat("<< size+1 <<",..." << std::endl;
	for(int d = 0; d < dims.at(size); ++d){
		int oldsize = size;
		if(size >= 2){
			int dimt = 1;
			for(int t = 0; t < size; ++t)
				dimt *= dims.at(t);
			offsets.at(size-2) = d*dimt + offsets.at(size-1);
		}
        if(size == 2)     print2D(A, dims, offsets[0], s, out);
        else if(size > 2) printND(A, dims, offsets, --size, s, out);
		size = oldsize;
		if(d < dims.at(size)-1) out << ",";
        out << "..." << std::endl;
	}
	out << ")";
}
//@}

/*! \brief Ausgabe einer ND Matrix
  *
  *
    * \param A    Matrix
  * \param dims Dimensionsvektor
    * \param f    Abspeicherungsform der Matrix : 'c' für column-major , 'r' für row-major
  * \param name Bezeichner für die Ausgabe
    * \param out
    */
template<class T>
void utl::print(const T *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out)
{
    TRUE_ASSERT(dims.size() > 1, dims.size() << " < 2");
    if(!name.empty()) out << name << " = ";
             if(dims.size() == 2 && ( dims(0) == 1 || dims(1) == 1 ) ) print1D(A, dims, 0, out);
    else if(dims.size() == 2)                                          print2D(A, dims, 0, s, out);
    else{
        std::vector<int> offsets(dims.size()-1, 0);
        int size = dims.size()-1;
        printND(A, dims,offsets, size, s , out);
    }
    out << ";" << std::endl;
}

template void utl::print<char>(const char *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out);
template void utl::print<int>(const int *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out);
template void utl::print<size_t>(const size_t *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out);
template void utl::print<float>(const float *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out);
template void utl::print<double>(const double *A, const utl::Dim &dims, const std::string &name, const utl::Storage & s, std::ostream &out);


/*! \brief Ausgabe einer ND Matrix
	*
	*
	* \param A    Matrix
	* \param dims Dimensionsvektor
	* \param f    Abspeicherungsform der Matrix : 'c' für column-major , 'r' für row-major
	* \param name Bezeichner für die Ausgabe
	* \param out
	*/
template<class T>
std::string utl::toString(const T *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s)
{
    std::ostringstream out;

	TRUE_ASSERT(dims.size() > 1, dims.size() << " < 2");
	if(!name.empty()) out << name << " = ";
             if(dims.size() == 2 && ( dims.at(0) == 1 || dims.at(1) == 1 ) ) print1D(A, dims, 0, out);
    else if(dims.size() == 2)                                                print2D(A, dims, 0, s, out);
	else{
		std::vector<int> offsets(dims.size()-1, 0);
		int size = dims.size()-1;
        printND(A, dims,offsets, size, s , out);
	}
	out << ";";
	return out.str();
}

template std::string utl::toString<char>(const char *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s);
template std::string utl::toString<int>(const int *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s);
template std::string utl::toString<size_t>(const size_t *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s);
template std::string utl::toString<float>(const float *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s);
template std::string utl::toString<double>(const double *A, const utl::Dim &dims, const std::string &name, const utl::Storage &s);

#include <type_traits>
template<class T>
void utl::octaveErrorPrint(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max)
{
	if(std::is_same<float, T>() ) {
		out << v1 << " = single("<< v1 <<");" << std::endl;
		out << v2 << " = single("<< v2 <<");" << std::endl;
	}
	out << "abs_err = abs("<<v1<< "-"<< v2<<");" << std::endl;
    out << "max_val = max(  max(  abs("<<v1<<"),abs("<<v2<<")  )  );" << std::endl;
    out << "if(max_val == 0), max_val = 1; end; " << std::endl;
    if(max.empty()) out << "rel_err = abs_err / max_val;" << std::endl;
	else            out << "rel_err = abs_err / " << max << ";" << std::endl;
	out << "mean_err = mean(rel_err); " << std::endl;
	out << "max_err = max(rel_err); " << std::endl;
    if(std::is_same<float, T>())      { out << "if(max_err > eps('single')),"; }
    else if(std::is_same<double, T>()){ out << "if(max_err > eps('double')),"; }
    else                              { out << "if(max_err > 0),"; }
    out << "disp(max_err); exit(1); end; " << std::endl;

}

template void utl::octaveErrorPrint<char>(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max);
template void utl::octaveErrorPrint<int>(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max);
template void utl::octaveErrorPrint<size_t>(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max);
template void utl::octaveErrorPrint<float>(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max);
template void utl::octaveErrorPrint<double>(const std::string &v1, const std::string &v2, std::ostream &out, const std::string &max);


/*! \brief Prints the selected platform of this */
template<class T>
std::string utl::typeToString()
{
	if(std::is_same<float,T>())        return "float";
	if(std::is_same<double,T>())       return "double";
	if(std::is_same<int,T>())          return "int";
	if(std::is_same<unsigned int,T>()) return "unsigned int";
	if(std::is_same<size_t,T>())       return "size_t";
	if(std::is_same<char,T>())         return "char";
	if(std::is_same<short int,T>())    return "short int";
    if(std::is_same<std::string,T>())       return "string";
	TRUE_ASSERT(0,"Cannot find type " << typeid(T).name());

	return "";
}

template std::string utl::typeToString<char>();
template std::string utl::typeToString<int>();
template std::string utl::typeToString<size_t>();
template std::string utl::typeToString<float>();
template std::string utl::typeToString<double>();


