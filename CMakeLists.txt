cmake_minimum_required(VERSION 2.8)

# Name the project.
project(OclWrapper)

# Find library offering thread support.
set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

# Find OpenGL library and include directory.
find_package(OpenGL REQUIRED)
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})

# Find OpenCL library and include directory.
find_library(OPENCL_LIBRARIES OpenCL)
find_path(OPENCL_INCLUDE_DIR CL/opencl.h 
 HINTS /usr/local/cuda/include/ /opt/AMDAPP/include/
 /usr/include/)
include_directories(SYSTEM ${OPENCL_INCLUDE_DIR})

# Set compiler flags.
if(CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-Wall -Wextra -Werror -pedantic -pedantic-errors -Wno-unused-local-typedefs)
endif(CMAKE_COMPILER_IS_GNUC OR CMAKE_COMPILER_IS_GNUCXX)

if(CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(-std=c++0x -Weffc++)
endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(Code/inc)

# Create OclWrapper library.
set(OCL_WRAPPER_HDRS 
  Code/inc/ocl_buffer.h
  Code/inc/ocl_device_type.h
  Code/inc/ocl_image.h
  Code/inc/ocl_platform.h
  Code/inc/ocl_queue.h
  Code/inc/utl_assert.h
  Code/inc/utl_matrix.h
  Code/inc/utl_storage.h
  Code/inc/utl_type.h
  Code/inc/ocl_context.h
  Code/inc/ocl_event.h
  Code/inc/ocl_kernel.h
  Code/inc/ocl_program.h
  Code/inc/ocl_wrapper.h
  Code/inc/utl_dim.h
  Code/inc/utl_profile_pass.h
  Code/inc/utl_stream.h
  Code/inc/utl_utils.h
  Code/inc/ocl_device.h
  Code/inc/ocl_event_list.h
  Code/inc/ocl_memory.h
  Code/inc/ocl_query.h
  Code/inc/utl_args.h
  Code/inc/utl_flags.h
  Code/inc/utl_profile_pass_manager.h
  Code/inc/utl_timer.h)
set(OCL_WRAPPER_SRCS 
  Code/src/ocl_buffer.cpp 
  Code/src/ocl_device.cpp 
  Code/src/ocl_event.cpp 
  Code/src/ocl_image.cpp 
  Code/src/ocl_memory.cpp 
  Code/src/ocl_program.cpp 
  Code/src/ocl_queue.cpp 
  Code/src/utl_dim.cpp 
  Code/src/utl_stream.cpp 
  Code/src/utl_type.cpp 
  Code/src/ocl_context.cpp 
  Code/src/ocl_device_type.cpp 
  Code/src/ocl_event_list.cpp 
  Code/src/ocl_kernel.cpp 
  Code/src/ocl_platform.cpp 
  Code/src/ocl_query.cpp 
  Code/src/utl_args.cpp 
  Code/src/utl_storage.cpp 
  Code/src/utl_timer.cpp)
add_library(OclWrapper ${OCL_WRAPPER_HDRS} ${OCL_WRAPPER_SRCS})
target_link_libraries(OclWrapper ${OPENCL_LIBRARIES}
  ${OPENGL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

add_executable(platform Tutorial/1.platform/platform.cpp)
target_link_libraries(platform OclWrapper)

add_executable(context Tutorial/2.context/context.cpp)
target_link_libraries(context OclWrapper)

add_executable(queue Tutorial/3.queue/queue.cpp)
target_link_libraries(queue OclWrapper)

add_executable(program Tutorial/4.program/program.cpp)
target_link_libraries(program OclWrapper)

add_executable(buffer Tutorial/5.buffer/buffer.cpp)
target_link_libraries(buffer OclWrapper)

add_executable(kernel Tutorial/6.kernel/kernel.cpp)
target_link_libraries(kernel OclWrapper)

add_executable(events Tutorial/7.events/events.cpp)
target_link_libraries(events OclWrapper)

add_executable(matrix Tutorial/8.matrix/matrix.cpp)
target_link_libraries(matrix OclWrapper)
