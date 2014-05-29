OCL_WRAPPER_DIR := ../../Code

include ../libraries.mk 
INCS:="$(OCL_WRAPPER_INC) $(OCL_INC)"
LIBS:="$(OCL_WRAPPER_LIB) $(OCL_LIB) $(OGL_LIB)"

GCC_FLAGS:="-std=c++11 -Wall -O3"

all: platform context queue program buffer kernel events matrix minimum image profile

platform: 1.platform/platform.cpp
	$(MAKE) -C 1.platform LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

context: 2.context/context.cpp
	$(MAKE) -C 2.context  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
	
queue: 3.queue/queue.cpp
	$(MAKE) -C 3.queue    LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
	
program: 4.program/program.cpp
	$(MAKE) -C 4.program  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
	
buffer: 5.buffer/buffer.cpp
	$(MAKE) -C 5.buffer   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
	
kernel: 6.kernel/kernel.cpp
	$(MAKE) -C 6.kernel   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

events: 7.events/events.cpp
	$(MAKE) -C 7.events   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

matrix: 8.matrix/matrix.cpp
	$(MAKE) -C 8.matrix   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

minimum: 9.minimum/minimum.cpp
	$(MAKE) -C 9.minimum  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

image: 10.image/image.cpp
	$(MAKE) -C 10.image   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

profile: 11.profile/profile.cpp 11.profile/profile.h
	$(MAKE) -C 11.profile   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)

.PHONY : clean

clean :
	$(MAKE) clean -C 1.platform
	$(MAKE) clean -C 2.context
	$(MAKE) clean -C 3.queue
	$(MAKE) clean -C 4.program
	$(MAKE) clean -C 5.buffer
	$(MAKE) clean -C 6.kernel
	$(MAKE) clean -C 7.events
	$(MAKE) clean -C 8.matrix
	$(MAKE) clean -C 9.minimum
	$(MAKE) clean -C 10.image
	$(MAKE) clean -C 11.profile

