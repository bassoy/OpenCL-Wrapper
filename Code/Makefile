include ../libraries.mk 

CC := g++

INCS := $(OCL_INC) -Iinc
LIBS := $(OCL_LIB)

CFILES = $(wildcard src/*.cpp)
OBJS1  = $(notdir $(CFILES))
OBJS2  = $(patsubst %.cpp,%.o, $(OBJS1))
OBJS   = $(addprefix build/,$(OBJS2))	
	
TARGET := lib/libOclWrapper.a

GCC_FLAGS=-std=c++0x -Wall $(GCC_FLAGS1)

archive: $(OBJS)
	ar cr $(TARGET) build/*.o

build/%.o : src/%.cpp
	$(CC) -c $(INCS) $(GCC_FLAGS) $< -o $@

purge :
		rm -f build/* $(TARGET)

.PHONY : clean

clean:
	rm -f build/* $(TARGET)
	
