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

#ifndef OCL_TEXTURE_H
#define OCL_TEXTURE_H


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

#include <ocl_event.h>
#include <ocl_event_list.h>
#include <ocl_memory.h>

namespace ocl {

class Memory;
class Context;
class Queue;

class Image : public Memory
{
public:
    enum 	Access { ReadWrite = CL_MEM_READ_WRITE,     /*!< memory object will be read and written within a kernel.*/
                     WriteOnly = CL_MEM_WRITE_ONLY ,    /*!< memory object will only be written wihtin a kernel.*/
                     ReadOnly  = CL_MEM_READ_ONLY,      /*!< memory object will only be read within a kernel.*/
                     UseHost   = CL_MEM_USE_HOST_PTR,   /*!< memory object will provided by a host pointer and must not be allocated by any OpenCL API call.*/
                     AllocHost = CL_MEM_ALLOC_HOST_PTR, /*!< memory object in the host memory will be created by instantiating a Image object.*/
                     CopyHost  = CL_MEM_COPY_HOST_PTR   /*!< memory object will be created on the device and copied from the host memory.*/
                   };

    enum    ChannelType {
                Float         = CL_FLOAT,      /*!< 32-bit single precision floating-point */
                HalfFloat     = CL_HALF_FLOAT, /*!< 16-bit floating-point */
                SNormInt8  = CL_SNORM_INT8,
                SNormInt16 = CL_SNORM_INT16,
                UNormInt8  = CL_UNORM_INT8,
                UNormInt16 = CL_UNORM_INT16,
                UNormShort3 = CL_UNORM_SHORT_565,
                UNormShort4 = CL_UNORM_SHORT_555,
                UNormInt4   = CL_UNORM_INT_101010,
                SignedInt8  = CL_SIGNED_INT8,
                SignedInt16 = CL_SIGNED_INT16,
                SignedInt32 = CL_SIGNED_INT32,
                UnsignedInt8  = CL_UNSIGNED_INT8,
                UnsignedInt16 = CL_UNSIGNED_INT16,
                UnsignedInt32 = CL_UNSIGNED_INT32
    };
    
    enum  ChannelOrder {
                RGBA         = CL_RGBA,      /*!< 4-vector (r,g,b,a) */
                Intensity    = CL_INTENSITY, /*!< 4-vector (i,i,i,i) */
                Luminance    = CL_LUMINANCE, /*!< 4-vector (l,l,l,1) */
                ARGB         = CL_ARGB,      /*!< 4-vector (r,g,b,a) */
                R            = CL_R,         /*!< 4-vector (r,0,0,1) */
                RG           = CL_RG,        /*!< 4-vector (r,g,0,1) */
                RGB          = CL_RGB,       /*!< 4-vector (r,g,b,1) */
                A            = CL_A,         /*!< 4-vector (0,0,0,a) */
    };    


	enum  ImageType {
		Image1D       = CL_MEM_OBJECT_IMAGE1D ,
		Image1DBuffer = CL_MEM_OBJECT_IMAGE1D_BUFFER ,
		Image1DArray  = CL_MEM_OBJECT_IMAGE1D_ARRAY ,
		Image2D       = CL_MEM_OBJECT_IMAGE2D ,
		Image2DArray  = CL_MEM_OBJECT_IMAGE2D_ARRAY,
		Image3D       = CL_MEM_OBJECT_IMAGE3D,
	};

    explicit Image();
    ~Image();


	Image(Context&, size_t width, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
	Image(Context&, size_t width, size_t height, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
	Image(Context&, size_t width, size_t height, size_t depth, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
    Image(Context&, unsigned int texture, unsigned long texture_target, long miplevel);


	void create(size_t width, size_t height, size_t depth, ImageType image_type, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
	void create(size_t width, size_t height, size_t depth, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
	void create(size_t width, size_t height, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
	void create(size_t width, ChannelType channel_type = Float, ChannelOrder order = RGBA, Access access = ReadWrite);
    void create(unsigned int texture, unsigned long texture_target, long miplevel);

    void recreate(size_t width, size_t height, ChannelType, ChannelOrder);

    void copyTo(size_t *src_origin, const size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  ) const;
    Event copyToAsync(size_t *src_origin, const size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  );

    void copyTo(const Queue&, size_t *src_origin, const size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  ) const;
    Event copyToAsync(const Queue&, size_t *src_origin, const size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  );

    void * map ( size_t *origin, const size_t *region, Memory::Access access ) const;
    Event mapAsync ( void ** ptr, size_t *origin, const size_t *region, Memory::Access access, const EventList & list = EventList() ) const;


    void 	read ( size_t *origin, void *ptr_to_host_data,  const size_t *region, const EventList & list = EventList() ) const;
    void 	read ( void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    Event 	readAsync ( size_t *origin, void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;

    void 	read (const Queue&, const size_t *origin, void *ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    void 	read (const Queue&, void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    Event 	readAsync (const Queue&, size_t *origin, void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;

    void 	write ( const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    void 	write ( size_t *origin, const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    Event 	writeAsync ( size_t *origion, const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;

    void 	write (const Queue&, const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    void 	write (const Queue&, size_t *origin, const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;
    Event 	writeAsync (const Queue&, size_t *origion, const void * ptr_to_host_data, const size_t *region, const EventList & list = EventList() ) const;

    Image & 	operator= ( const Image & other );
    Image & 	operator= ( Image && other );

    void acquireAccess(Queue&);
    void releaseAccess(Queue&, const EventList& = EventList());
};
}

#endif // OCL_TEXTURE_H
