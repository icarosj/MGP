#nvcc -ccbin=mpic++ -L/nfs/home/leejinho/dev/nccl_2018/build/lib -lnccl main.cu
#CXX = /home/leejinho/local/bin/mpiCC
#CXXFLAGS = -g -std=c++11 -Wall -Wextra -Wno-sign-compare -O3
CXX = g++
#CXX = /opt/anaconda3/pkgs/openmpi-3.1.3-495.g7c31c37/bin/mpiCC 
CXXFLAGS = -g -std=c++11 -Wall -Wno-sign-compare -O3 -DGIT_VERSION=\"$(GIT_VERSION)\" 

GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
DATE:= $(shell date)

SRCDIR = src
OBJDIR = obj
BINDIR = bin

INCS := $(wildcard $(SRCDIR)/*.h)
SRCS := $(wildcard $(SRCDIR)/*.cc)
OBJS := $(SRCS:$(SRCDIR)/%.cc=$(OBJDIR)/%.o)

all: dirs bin/matmul

dirs:
	mkdir -p bin
	mkdir -p obj 
	mkdir -p cuobj 


bin/matmul: $(OBJS) $(CUOBJS) #src/main.cc 
	@echo "OBJ: "$(OBJS)
	@echo "CUOBJ: "$(CUOBJS)
	$(CXX) $(CXXFLAGS) -o $@  $^ 
			    @echo "Compiled "$<" successfully!"

#bin/test: src/backlinks_compression_test.cc ${SRCS}  $(OBJS)
#	$(CXX) $(CXXFLAGS) -o $@ $^ $(OBJS)-I $(GTEST_ROOT)/include -L $(GTEST_ROOT)/lib $(LIB)
#	./$@

.PHONY:	test clean

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cc
	    @echo $(CXX) $(CXXFLAGS) "-Iinclude -I$(CUDAINC) -I$(NCCLINC)  -c" $< "-o" $@
	    @$(CXX) $(CXXFLAGS) -Iinclude -I$(CUDAINC) -I$(NCCLINC) -c $< -o $@
			    @echo "Compiled "$<" successfully!"

clean:
	rm -f $(OBJS) $(OBJS:%.o=%.d) 
	rm -f $(CUOBJS) $(CUOBJS:%.o=%.d) 
	rm -rf bin/*
