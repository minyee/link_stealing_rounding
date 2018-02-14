TARGET := link_stealing 
SRC := link_stealing.cc matrix_operations.cc 

CXX :=    g++
CC :=     gcc
CXXFLAGS :=
CPPFLAGS := -I. -I$(HOME)/src/alglib-3.12/src
#CPPFLAGS := -I. -I$(HOME)/install/sstcore-7.1.0/include
LIBDIR :=  
PREFIX := 
LDFLAGS :=  -Wl,-rpath,$(PREFIX)/lib

OBJ := $(SRC:.cc=.o) 
OBJ := $(OBJ:.cpp=.o)
OBJ := $(OBJ:.c=.o)

.PHONY: clean install 

all: $(TARGET)

$(TARGET): $(OBJ) 
	$(CXX) -std=c++11 -o $@ $+ $(LDFLAGS) $(LIBS)  $(CXXFLAGS)

%.o: %.cpp
	$(CXX) -std=c++11 $(CPPGLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.cc 
	$(CXX) -std=c++11 $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(TARGET) $(OBJ) 

install: $(TARGET)
	cp $< $(PREFIX)/bin

