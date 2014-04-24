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

INSTALLATION_PATH = /usr/local/lib
INSTALLATION_PATH_HEADER = /usr/local/include

ifdef NVIDIA
	OCL_VERSION=-DOPENCL_V1_1
else
	OCL_VERSION=-DOPENCL_V1_2
endif

GCC_FLAGS=-std=c++11 -Wall -fPIC -shared $(OCL_VERSION) #-D__OPENGL__ #


archive: $(OBJS)
	ar cr $(TARGET) build/*.o

build/%.o : src/%.cpp
	$(CC) -c $(INCS) $(GCC_FLAGS) $< -o $@

purge :
	rm -f build/* $(TARGET)

install: purge archive
	@echo "Installing library..."
	@mkdir -p $(INSTALLATION_PATH)
	@cp -v $(TARGET) $(INSTALLATION_PATH)/$(OUTPUT_FULLNAME)
	@echo "Installing header..."
	@mkdir -p $(INSTALLATION_PATH_HEADER)
	@cp -v ./inc/*.h $(INSTALLATION_PATH_HEADER)


.PHONY : clean

clean:
	rm -f build/* $(TARGET)
	
