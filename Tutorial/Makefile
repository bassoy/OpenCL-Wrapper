OCL_WRAPPER_DIR := ../../Code

include ../libraries.mk 
INCS:="$(OCL_WRAPPER_INC) $(OCL_INC)"
LIBS:="$(OCL_WRAPPER_LIB) $(OCL_LIB) $(OGL_LIB)"

GCC_FLAGS:="-std=c++11 -Wall -g -DDEBUG"

TARGET:
		$(MAKE) -C 1.platform LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 2.context  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 3.queue    LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 4.program  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 5.buffer   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 6.kernel   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 7.events   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 8.matrix   LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)
		$(MAKE) -C 9.minimum  LIBS=$(LIBS) INCS=$(INCS) GCC_FLAGS=$(GCC_FLAGS)


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

