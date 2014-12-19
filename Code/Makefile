include ../libraries.mk 

CC := g++
#CC := icpc

INCS := $(OCL_INC) -Iinc
LIBS := $(OCL_LIB)

CFILES := $(wildcard src/*.cpp)
OBJS1  := $(notdir $(CFILES))
OBJS2  := $(patsubst %.cpp,%.o, $(OBJS1))
OBJS   := $(addprefix build/,$(OBJS2))	
	
TARGET := lib/libOclWrapper.a

ifdef NVIDIA
	OCL_VERSION=-DOPENCL_V1_1
else
	OCL_VERSION=-DOPENCL_V1_2
endif

GCC_FLAGS=-std=c++11 -Wall $(OCL_VERSION) -O3 #-D__OPENGL__ #


archive: $(OBJS)
	ar cr $(TARGET) build/*.o

build/%.o : src/%.cpp
	$(CC) -c $(INCS) $(GCC_FLAGS) $< -o $@

purge :
	rm -f build/* $(TARGET)

.PHONY : clean

clean:
	rm -f build/* $(TARGET)
	
