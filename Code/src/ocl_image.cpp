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

#include <ocl_memory.h>
#include <ocl_context.h>
#include <ocl_query.h>
#include <ocl_queue.h>
#include <ocl_platform.h>
#include <ocl_device.h>
#include <ocl_queue.h>
#include <ocl_image.h>

#include <utl_assert.h>

/**
 * \brief ocl::Image::Image Instantiates this Image without a Context
 *
 * No Image is created. Use Image::create for the creation of such an object.
 */
ocl::Image::Image()
    :Memory()
{

}

/**
 * \brief ocl::Image::Image Instantiates an 2D-Image within a context.
 *
 * No Memory is allocated but only an object created which can be used within
 * the active Context. Allocation takes place when data is transfered.
 * The existence of an active Context and an active Queue is assumed.
 *
 * \param ctxt Active Context.
 * \param width Width of the 2D-Image.
 * \param height Height of the 2D-Image.
 * \param type Channeltype of the image.
 * \param order Channelorder of the image.
 */
ocl::Image::Image(Context& ctxt, size_t width, size_t height, ChannelType type, ChannelOrder order, Access access)
    :Memory(ctxt)
{
    this->create(width, height, type, order, access);
}

/**
 * \brief ocl::Image::Image Instantiates an 3D-Image within a context.
 *
 * No Memory is allocated but only an object created which can be used within
 * the active Context. Allocation takes place when data is transfered.
 * The existence of an active Context and an active Queue is assumed.
 *
 * \param ctxt Active Context.
 * \param width Width of the 3D-Image.
 * \param height Height of the 3D-Image.
 * \param depth Depth of the 3D-Image.
 * \param type Channeltype of the image.
 * \param order Channelorder of the image.
 */
ocl::Image::Image(Context& ctxt, size_t width, size_t height, size_t depth, ChannelType type, ChannelOrder order, Access access)
    :Memory(ctxt)
{
    this->create(width, height, depth, type, order, access);
}

/**
 * \brief ocl::Image::Image Constructor. Initializes memory and creates the Image from an OpenGL texture.
 * \param ctxt OpenCL context.
 * \param texture OpenGL texture.
 * \param texture_target OpenGL texture target.
 * \param miplevel MipMapping level.
 */
ocl::Image::Image(Context &ctxt, unsigned int texture, unsigned long texture_target, long miplevel)
    :Memory(ctxt)
{
    this->create(texture, texture_target, miplevel);
}


/**
 * \brief ocl::Image::~Image Empty deconstructor.
 */
ocl::Image::~Image()
{

}

/**
 * \brief ocl::Image::create Creates cl_mem for this Image.
 *
 * Note that no Memory is allocated. Allocation takes place when data is transfered.
 * It is assumed that an active Queue exists.
 *
 * \param width Width of the image.
 * \param height Height of the image.
 * \param type Channeltype of the image.
 * \param order Channelorder of the image.
 */
void ocl::Image::create(size_t width, size_t height, ChannelType type, ChannelOrder order, Access access)
{
    TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Image");
    cl_mem_flags flags = access;
    cl_image_format format;
    format.image_channel_order = order;
    format.image_channel_data_type = type;
    cl_int status;

#if defined(OPENCL_V1_0) || defined(OPENCL_V1_1)
    this->_id = clCreateImage2D(this->_context->id(), flags, &format, width, height, 0, NULL, &status);
#else
    _cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE2D;
    desc.image_height = height;
    desc.image_width = width;
    desc.image_depth = 1;
    desc.image_array_size = 1;
    desc.image_row_pitch = 0;
    desc.image_slice_pitch = 0;
    desc.num_mip_levels = 0;
    desc.num_samples = 0;
    desc.buffer = NULL;

    this->_id = clCreateImage(this->_context->id(), flags, &format, &desc, NULL, &status);
#endif
    OPENCL_SAFE_CALL(status);
    TRUE_ASSERT(this->_id != 0, "Could not create 2D image.");
}


/**
 * \brief ocl::Image::create Creates cl_mem for this Image.
 *
 * Note that no Memory is allocated. Allocation takes place when data is transfered.
 * It is assumed that an active Queue exists.
 *
 * \param width Width of the image.
 * \param height Height of the image.
 * \param depth Depth of the image.
 * \param type Channeltype of the image.
 * \param order Channelorder of the image.
 */
void ocl::Image::create(size_t width, size_t height, size_t depth, ChannelType type, ChannelOrder order, Access access)
{
    TRUE_ASSERT(this->_context != 0, "Context not valid - cannot create Image");
    cl_mem_flags flags = access;

    cl_image_format format;
    format.image_channel_order = order;
    format.image_channel_data_type = type;

    cl_int status;

#if defined(OPENCL_V1_0) || defined(OPENCL_V1_1)
    this->_id = clCreateImage3D(this->_context->id(), flags, &format, width, height, depth, 0, 0, NULL, &status);
#else
    _cl_image_desc desc;
    desc.image_type = CL_MEM_OBJECT_IMAGE3D;
    desc.image_height = height;
    desc.image_width = width;
    desc.image_depth = depth;
    desc.image_array_size = 1;
    desc.image_row_pitch = 0;
    desc.image_slice_pitch = 0;
    desc.num_mip_levels = 0;
    desc.num_samples = 0;
    desc.buffer = NULL;
    this->_id = clCreateImage(this->_context->id(), flags, &format, &desc, NULL, &status);
#endif
    OPENCL_SAFE_CALL(status);
    TRUE_ASSERT(this->_id != 0, "Could not create 3D image.");
}


/**
 * \brief ocl::Image::create Creates an OpenCL Image from an OpenGL texture.
 * \param texture OpenGL-ID of the texture.
 * \param texture_target Target of the OpenGL texture.
 * \param miplevel MipMapping level.
 */
void ocl::Image::create(unsigned int texture, unsigned long texture_target, long miplevel)
{
    cl_mem_flags flags = ocl::Image::ReadWrite;
    if (this->_context->devices().size() == 1 &&
						this->_context->devices().front().type() == ocl::device_type::CPU){
        flags |= ocl::Image::AllocHost;
    }

    cl_int status;
#if defined(OPENCL_V1_0) || defined(OPENCL_V1_1)
    this->_id = clCreateFromGLTexture2D(this->_context->id(), flags, texture_target, miplevel, texture, &status);
#else
    this->_id = clCreateFromGLTexture(this->_context->id(), flags, texture_target, miplevel, texture, &status);
#endif
    OPENCL_SAFE_CALL(status);
    TRUE_ASSERT(this->_id != 0, "Could not create shared image.");
}

/**
 * \brief ocl::Image::recreate Creates cl_mem for this Image.
 *
 *  Note that no Memory is allocated. Allocation takes place when data is transfered.
 *
 * \param width Width of the image.
 * \param height Height of the image.
 * \param type Channeltype of the image.
 * \param order Channelorder of the image.
 */
void ocl::Image::recreate(size_t width, size_t height, ChannelType type, ChannelOrder order)
{
    this->release();
    this->create(width, height, type, order);
}


/**
 * \brief ocl::Image::copyTo Copies from this Image to the destination Image.
 *
 * The operation assumes that all data are valid and no synchronization is necessary (active Queue executes in-order).
 * The operation forces that all commands within the active Queue including this one are completed.
 *
 * \param src_origin is the 3D offset in bytes from which the Image is read.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param dest is the Image into which the data is going to be copied.
 * \param dest_origin is the 3D offset in bytes from which the destionation Image is read.
 */
void ocl::Image::copyTo(size_t *src_origin, const size_t *region, const Image & dest, size_t *dest_origin, const EventList & list ) const
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(this->id() != dest.id(), "Images must not be equal this->id() " << this->id() << "; other.id " << dest.id());
    OPENCL_SAFE_CALL( clEnqueueCopyImage(this->activeQueue().id(), this->id(), dest.id(), src_origin, dest_origin, region, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::copyToAsync Copies asynchronously from this Image to the destination Image.
 *
 * \param src_origin is the 3D offset in bytes from which the Image is read.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param dest is the Image into which the data is going to be copied.
 * \param dest_origin is the 3D offset in bytes from which the destionation Image is read.
 * \param list contains all events for which this command has to wait.
 * \return event which can be integrated into other EventList.
 */
ocl::Event ocl::Image::copyToAsync(size_t *src_origin, const size_t *region, const Image &dest, size_t *dest_origin, const EventList &list)
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(this->id() != dest.id(), "Images must not be equal this->id() " << this->id() << "; other.id " << dest.id());
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueCopyImage(this->activeQueue().id(), this->id(), dest.id(),
                                         src_origin, dest_origin, region, list.size(),
                                         list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}


/**
 * \brief ocl::Image::copyTo Copies from this Image to the destination Image.
 *
 * The operation assumes that all data are valid and no synchronization is necessary (active Queue executes in-order).
 * The operation forces that all commands within the active Queue including this one are completed.
 *
 * \param queue is a command queue on which the command is executed.
 * \param src_origin is the 3D offset in bytes from which the Image is read.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param dest is the Image into which the data is going to be copied.
 * \param dest_origin is the 3D offset in bytes from which the destionation Image is read.
 */
void ocl::Image::copyTo(const Queue &queue, size_t *src_origin, const size_t *region, const Image &dest, size_t *dest_origin, const EventList &list) const
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(this->id() != dest.id(), "Image must not be equal this->id() " << this->id() << "; other.id " << dest.id());
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL( clEnqueueCopyImage(queue.id(), this->id(), dest.id(), src_origin, dest_origin, region, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::copyToAsync Copies asynchronously from this Image to the destination Image.
 *
 * \param queue is a command queue on which the command is executed.
 * \param src_origin is the 3D offset in bytes from which the Image is read.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param dest is the Image into which the data is going to be copied.
 * \param dest_origin is the 3D offset in bytes from which the destionation Image is read.
 * \param list contains all events for which this command has to wait.
 * \return event which can be integrated into other EventList.
 */
ocl::Event ocl::Image::copyToAsync(const Queue &queue, size_t *src_origin, const size_t *region, const Image &dest, size_t *dest_origin, const EventList &list)
{
    TRUE_ASSERT(this->context() == dest.context(), "Context of this and dest must be equal");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueCopyImage(queue.id(), this->id(), dest.id(),
                                         src_origin, dest_origin, region, list.size(),
                                         list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}

/**
 * \brief ocl::Image::map Maps the Image into the host memory.
 *
 *  No data transfer is performed. Note that in order to map data of the Image the active queue must be a cpu and must have been allocated
 *  with the Image access mode AllocHost. You cannot modify the Image with OpenCL until unmap.
 * \param origin is the 3D offset in bytes from which the image is read.
 * \param region is the 3D region in bytes to be mapped.
 * \param access specifies in what way the host_mem is used.
 * \return a void pointer to the mapped host memory location.
 */
void * ocl::Image::map(size_t *origin, const size_t *region, Memory::Access access) const
{
    TRUE_ASSERT(this->activeQueue().device().isCpu(), "Device " << this->activeQueue().device().name() << " is not a cpu!");
    cl_int status;
    cl_map_flags flags = access;
    void *pointer = clEnqueueMapImage(this->activeQueue().id(), this->id(), CL_TRUE, flags,
                                      origin, region, 0, 0, 0, NULL, NULL, &status);
    OPENCL_SAFE_CALL (status ) ;
    TRUE_ASSERT(pointer != NULL, "Could not map image!");
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
    return pointer;
}

/**
 * \brief ocl::Image::map Maps the Image into the host memory.
 *
 *  No data transfer is performed. Note that in order to map data of the Image the active queue must be a cpu and must have been allocated
 *  with the Image access mode AllocHost. You cannot modify the Image with OpenCL until unmap.
 * \param ptr is returned and contains the address of a pointer to the host memory.
 * \param origin is the 3D offset in bytes from which the image is read.
 * \param region is the 3D region in bytes to be mapped.
 * \param access specifies in what way the host_mem is used.
 * \param list contains all events for which this command has to wait.
 * \return event which can be integrated into other EventList
 */
ocl::Event ocl::Image::mapAsync(void **ptr, size_t *origin, const size_t *region, Memory::Access access, const EventList &list) const
{
    TRUE_ASSERT(this->activeQueue().device().isCpu(), "Device " << this->activeQueue().device().name() << " is not a cpu!");
    cl_int status;
    cl_event event_id;
    cl_map_flags flags = access;
    *ptr = clEnqueueMapImage(this->activeQueue().id(), this->id(), CL_TRUE, flags,
                                      origin, region, 0, 0, list.size(), list.events().data(), &event_id, &status);
    OPENCL_SAFE_CALL (status ) ;
    TRUE_ASSERT(ptr != NULL, "Could not map image!");
    return ocl::Event(event_id, this->context());
}


/**
 * \brief ocl::Image::read Transfers data from this Image to the host memory.
 *
 * You can be sure that the data is read.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::read(size_t *origin,  void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    OPENCL_SAFE_CALL( clEnqueueReadImage(this->activeQueue().id(), this->id(), CL_TRUE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::read Transfers data from this Image to the host memory.
 *
 * You can be sure that the data is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::read(void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    std::vector<size_t> origin = {0, 0, 0};
    OPENCL_SAFE_CALL( clEnqueueReadImage(this->activeQueue().id(), this->id(), CL_TRUE, origin.data(), region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::readAsync Transfers data from this Image to the host memory.
 *
 * Waits until the event list is completed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param list contains all events for which this command has to wait.
 * \return an event which can be further put into an event list for synchronization.
 */
ocl::Event ocl::Image::readAsync(size_t *origin, void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueReadImage(this->activeQueue().id(), this->id(), CL_FALSE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}


/**
 * \brief ocl::Image::read Transfers data from this Image to the host memory.
 *
 * You can be sure that the data is read. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::read(const Queue& queue, const size_t *origin, void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL( clEnqueueReadImage(queue.id(), this->id(), CL_TRUE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/**
 * \brief ocl::Image::read Transfers data from this Image to the host memory.
 *
 * You can be sure that the data is read. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::read(const Queue& queue, void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    const size_t origin[3] = {0, 0, 0};
    OPENCL_SAFE_CALL( clEnqueueReadImage(queue.id(), this->id(), CL_TRUE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/**
 * \brief ocl::Image::readAsync Transfers data from this Image to the host memory.
 *
 * Waits until the event list is completed. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param list contains all events for which this command has to wait.
 * \return an event which can be further put into an event list for synchronization.
 */
ocl::Event ocl::Image::readAsync(const Queue &queue, size_t *origin, void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueReadImage(queue.id(), this->id(), CL_FALSE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}

/**
 * \brief ocl::Image::write Transfers data from host memory to this Image.
 *
 * You can be sure that the data is read.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::write(size_t *origin, const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    OPENCL_SAFE_CALL( clEnqueueWriteImage(this->activeQueue().id(), this->id(), CL_TRUE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::write Transfers data from host memory to this Image.
 *
 * You can be sure that the data is write.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::write(const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    std::vector<size_t> origin = {0, 0, 0};
    OPENCL_SAFE_CALL( clEnqueueWriteImage(this->activeQueue().id(), this->id(), CL_TRUE, origin.data(), region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(this->activeQueue().id()) );
}

/**
 * \brief ocl::Image::writeAsync Transfers data from host memory to this Image.
 *
 * Waits until the event list is completed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param list contains all events for which this command has to wait.
 * \return an event which can be further put into an event list for synchronization.
 */
ocl::Event ocl::Image::writeAsync(size_t *origin, const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueWriteImage(this->activeQueue().id(), this->id(), CL_FALSE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}


/**
 * \brief ocl::Image::write Transfers data from host memory to this Image.
 *
 * You can be sure that the data is read. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::write(const Queue& queue, size_t *origin, const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    OPENCL_SAFE_CALL( clEnqueueWriteImage(queue.id(), this->id(), CL_TRUE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/**
 * \brief ocl::Image::write Transfers data from host memory to this Image.
 *
 * You can be sure that the data is read. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 */
void ocl::Image::write(const Queue& queue, const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    std::vector<size_t> origin = {0, 0, 0};
    OPENCL_SAFE_CALL( clEnqueueWriteImage(queue.id(), this->id(), CL_TRUE, origin.data(), region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), NULL) );
    OPENCL_SAFE_CALL( clFinish(queue.id()) );
}

/**
 * \brief ocl::Image::writeAsync Transfers data from host memory to this Image.
 *
 * Waits until the event list is completed. Be sure that the queue
 * and this Image are in the same context.
 * \param queue is a command queue on which the command is executed.
 * \param origin is the 3D offset in bytes from which the Image is read.
 * \param ptr_to_host_data must point to a memory location whith region bytes available.
 * \param region is the 3D region of the data. It is given with {image_width, image_height, image_depth}.
 * \param list contains all events for which this command has to wait.
 * \return an event which can be further put into an event list for synchronization.
 */
ocl::Event ocl::Image::writeAsync(const Queue &queue, size_t *origin, const void *ptr_to_host_data, const size_t *region, const EventList &list) const
{
    TRUE_ASSERT(ptr_to_host_data != NULL, "data == 0");
    TRUE_ASSERT(queue.context() == *this->context(), "Context of queue and this must be equal");
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueWriteImage(queue.id(), this->id(), CL_FALSE, origin, region, 0, 0, ptr_to_host_data, list.size(), list.events().data(), &event_id) );
    return ocl::Event(event_id, this->context());
}

/*! \brief Acquires access to this Image.
  *
  * Access is aquired to this Image. Only OpenCL can access this Image now.
  * \param q is the active OpenCL queue.
  * \returns whether acquiring was successful or not.
  */
#ifdef __OPENGL__
void ocl::Image::acquireAccess(Queue &q) {
    glFinish();
    OPENCL_SAFE_CALL( clEnqueueAcquireGLObjects(q.id(), 1, &this->_id, NULL, NULL, NULL) );
}
#endif


/*! \brief Releases access to this Image.
  *
  * Access is released to this Image.
  * \param q is the active OpenCL queue.
  * \returns whether releasing was successful or not.
  */
void ocl::Image::releaseAccess(Queue &q, const EventList& list) {
    cl_event event_id;
    OPENCL_SAFE_CALL( clEnqueueReleaseGLObjects(q.id(), 1, &this->_id, list.size(), list.events().data(), &event_id) );
}
