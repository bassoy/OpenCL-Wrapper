OCL_LIB=
OCL_INC=

INTEL:=1

ifdef INTEL
	OCL_LIB += -L/usr/lib64 -lOpenCL
	OCL_INC += -I/usr/include
endif

ifdef INTEL_MIC
	INTEL_MIC_DIR = /opt/intel/opencl-1.2-3.2.1.16712
	OCL_LIB += -L$(INTEL_MIC_DIR)/lib64 -L$(INTEL_MIC_DIR)/libmic -lOpenCL
	OCL_INC += -I$(INTEL_MIC_DIR)/include
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


