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

#ifndef OCL_SAMPLER_H
#define OCL_SAMPLER_H


#include <vector>
#include <set>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#ifdef __OPENGL__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#else
#include <CL/opencl.h>
#ifdef __OPENGL__
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#endif

namespace ocl {

class Context;

class Sampler
{
public:
	/*!< specifies how out-of-range image coordinates are handled when reading from an image*/
	enum 	AdressingMode {
		MirrorRepeat = CL_ADDRESS_MIRRORED_REPEAT,
		Repeat = CL_ADDRESS_REPEAT,
		ClampToEdge  = CL_ADDRESS_CLAMP_TO_EDGE,
		Clamp = CL_ADDRESS_CLAMP,
		None = CL_ADDRESS_NONE
		};

	/*!< specifies the type of filter that must be applied when reading an image */
  enum 	FilterMode {
  	Nearest = CL_FILTER_NEAREST,
  	Linear = CL_FILTER_LINEAR
  };    

  explicit Sampler();
  ~Sampler();
  Sampler(Context&, AdressingMode amode = None, FilterMode fmode = Nearest, bool normalized = false);  
  void create(AdressingMode amode = None, FilterMode fmode = Nearest, bool normalized = false);
	 Sampler& operator =( Sampler const& ) = delete;
	 Sampler( Sampler const& ) = delete;  

	cl_sampler id() const;
	void release();
	const Context& context() const;    
  bool isNormalized() const;
  AdressingMode addressingMode() const;
  FilterMode filterMode() const;
  
private:
  Context *_context;
  cl_sampler _id;

};
}

#endif
