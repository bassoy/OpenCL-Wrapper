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

    enum    DataType {
                Float       = CL_FLOAT,
                UnsignedInt = CL_UNSIGNED_INT8 // 8 Bit signed integer

    };

    explicit Image();
    ~Image();
    Image(Context&, size_t width, DataType data_type, Access access);
    Image(Context&, size_t width, size_t height, DataType data_type, Access access);
    Image(Context&, size_t width, size_t height, size_t depth, DataType data_type, Access access);
    Image(Context&, unsigned int, unsigned long, long);


    void create(size_t, size_t, DataType, Access access);
    void create(size_t, size_t, size_t, DataType, Access access);
    void create(unsigned int, unsigned long, long);

    void recreate(size_t, size_t, DataType);

    void copyTo(size_t *src_origin, size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  ) const;
    Event copyToAsync(size_t *src_origin, size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  );

    void copyTo(const Queue&, size_t *src_origin, size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  ) const;
    Event copyToAsync(const Queue&, size_t *src_origin, size_t *region, const Image & dest, size_t *dest_origin, const EventList & list = EventList()  );

    void * map ( size_t *origin, size_t *region, Memory::Access access ) const;
    Event mapAsync ( void ** ptr, size_t *origin, size_t *region, Memory::Access access, const EventList & list = EventList() ) const;


    void 	read ( size_t *origin, void *ptr_to_host_data,  size_t *region, const EventList & list = EventList() ) const;
    void 	read ( void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    Event 	readAsync ( size_t *origin, void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;

    void 	read (const Queue&, size_t *origin, void *ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    void 	read (const Queue&, void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    Event 	readAsync (const Queue&, size_t *origin, void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;

    void 	write ( const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    void 	write ( size_t *origin, const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    Event 	writeAsync ( size_t *origion, const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;

    void 	write (const Queue&, const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    void 	write (const Queue&, size_t *origin, const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;
    Event 	writeAsync (const Queue&, size_t *origion, const void * ptr_to_host_data, size_t *region, const EventList & list = EventList() ) const;

    Image & 	operator= ( const Image & other );
    Image & 	operator= ( Image && other );

    void acquireAccess(Queue&);
    void releaseAccess(Queue&, const EventList& = EventList());
};
}

#endif // OCL_TEXTURE_H
