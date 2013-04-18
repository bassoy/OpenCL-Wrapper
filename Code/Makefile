CC := clang++

OPENCL_LIB := -L/usr/lib64 -lOpenCL
OPENCL_INC := -I/usr/include

INCS := $(OPENCL_INC) -Iinc
LIBS := $(OPENCL_LIB)

CFILES = $(wildcard src/*.cpp)
OBJS1  = $(notdir $(CFILES))
OBJS2  = $(patsubst %.cpp,%.o, $(OBJS1))
OBJS   = $(addprefix build/,$(OBJS2))	
	
TARGET := lib/libOclWrapper.a

GCC_FLAGS=-std=c++0x -stdlib=libc++ -Wall $(GCC_FLAGS1)

archive: $(OBJS)
	ar cr $(TARGET) build/*.o

build/%.o : src/%.cpp
	$(CC) -c $(INCS) $(GCC_FLAGS) $< -o $@

purge :
		rm -f build/* $(TARGET)
		$(MAKE) clean -C $(UTIL_DIR)

.PHONY : clean

clean:
	rm -f build/* $(TARGET)
	
