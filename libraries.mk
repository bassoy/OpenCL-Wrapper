OCL_LIB=
OCL_INC=

XEON:=1

ifdef INTEL
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/usr/include
endif

ifdef NVIDIA
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/usr/local/cuda/include
endif

ifdef AMD
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/opt/AMDAPP/include
endif

ifdef XEON
	OCL_LIB += -L/opt/intel/opencl-1.2-3.1.1.11385/lib64 -lOpenCL
        OCL_INC += -I/opt/intel/opencl-1.2-3.1.1.11385/include
endif 


OCL_WRAPPER_LIB:=-L$(OCL_WRAPPER_DIR)/lib -lOclWrapper
OCL_WRAPPER_INC:=-I$(OCL_WRAPPER_DIR)/inc

#OGL_LIB := -lGL -lGLU -lpthread
