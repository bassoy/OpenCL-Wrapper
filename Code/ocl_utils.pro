QT -= core  gui
QMAKESPEC = 
TARGET = main
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app


OGL_WRAPPER_DIR = $$(HOME)/Desktop/OpenGL-Wrapper/Code

INCLUDEPATH += inc/
INCLUDEPATH += $$OGL_WRAPPER_DIR/inc


SOURCES += \
	src/ocl_query.cpp \
	src/ocl_program.cpp \
	src/ocl_context.cpp \
	src/ocl_kernel.cpp \
	src/ocl_image.cpp \
	src/ocl_platform.cpp \
	src/ocl_device.cpp \
	src/ocl_device_type.cpp \        
	src/ocl_queue.cpp \
	src/ocl_buffer.cpp \
	src/ocl_memory.cpp \
	src/ocl_event.cpp \
	src/ocl_event_list.cpp
	
HEADERS += \
	inc/ocl_wrapper.h \
	inc/ocl_query.h \
	inc/ocl_program.h \
	inc/ocl_context.h \
	inc/ocl_kernel.h \
	inc/ocl_image.h \
	inc/ocl_platform.h \
	inc/ocl_device.h \
	inc/ocl_device_type.h \        
	inc/ocl_queue.h \
	inc/ocl_event.h \
	inc/ocl_buffer.h \
	inc/ocl_memory.h \
	inc/ocl_event_list.h


SOURCES += \
	src/utl_stream.cpp \
	src/utl_type.cpp \
	src/utl_storage.cpp \
	src/utl_args.cpp \
	src/utl_dim.cpp \
	src/utl_timer.cpp

HEADERS += \
	inc/utl_utils.h \
	inc/utl_stream.h \
	inc/utl_storage.h \
	inc/utl_args.h \
	inc/utl_dim.h \
	inc/utl_assert.h \
	inc/utl_type.h \
	inc/utl_profile_pass_manager.h \
	inc/utl_profile_pass.h \
	inc/utl_flags.h \
	inc/utl_matrix.h \
	inc/utl_timer.h
