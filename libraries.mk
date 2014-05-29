OCL_LIB=
OCL_INC=

NVIDIA:=1

ifdef INTEL
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/usr/include
endif

ifdef NVIDIA
	#OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_LIB += -L/usr/lib64/nvidia -lOpenCL
	OCL_INC += -I/usr/local/cuda/include
endif

ifdef AMD
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/opt/AMDAPP/include
endif

OCL_WRAPPER_LIB:=-L$(OCL_WRAPPER_DIR)/lib -lOclWrapper
OCL_WRAPPER_INC:=-I$(OCL_WRAPPER_DIR)/inc

OGL_LIB := -lGL -lpthread #-lGLU 


