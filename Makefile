
.SUFFIXES:
.PRECIOUS: %.o

# include CPP object files in CPP_OBJS variable, and set some 
# switches (TAU_PROFILE, NON_FT)
include cpp_objs_switches.mk

HDRS=Advect3D.h HaloArray3D.h ProcGrid3D.h Vec3D.h GridCombine3D.h \
	LinGrid3D.h Timer.h
OBJS=Advect3D.o Advect2D.o GridCombine3D.o
PROG=ex14
CCFLAGS=-O3 -fopenmp -std=gnu++0x

# MPI CPP compiler
ifndef MPICPP
  MPICPP := $(shell which mpic++)
endif

# MPI Fortran compiler
ifndef MPIFORTRAN
  MPIFORTRAN := $(shell which mpif90)
endif

# flags related to non-ft mpi
ifeq ($(NON_FT),yes)
  CPPFLAGS += -DNON_FT
  FFLAGS += -DNON_FT
endif

# CPP flags
CPPFLAGS += -I$(CPP_INCLUDE_DIR) -O3 -fopenmp -std=gnu++0x

# CPP link libs
CPP_LINK_LIBS = -L$(PETSC_DIR)/lib -lglpk -lmpi_cxx -lstdc++ -lgomp

CPP_INCLUDE_DIR = ${PETSC_DIR}/include

FORTRAN_OBJS = AppWrap.o

all: WRAP_CPP CPP LINK

WRAP_CPP:
	$(MPICPP) $(CPPFLAGS) -c -fPIC -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -I$(CPP_INCLUDE_DIR) -I$(CPP_INCLUDE_DIR)/openmpi/opal/mca/hwloc/hwloc132/hwloc/include -I$(CPP_INCLUDE_DIR)/openmpi/opal/mca/event/libevent2013/libevent -I$(CPP_INCLUDE_DIR)/openmpi/opal/mca/event/libevent2013/libevent/include -I$(CPP_INCLUDE_DIR)/openmpi    -o AppWrap.o AppWrap.cpp

CPP: $(CPP_OBJS)

%.o: %.cpp $(HDRS)
	$(MPICPP) $(CPPFLAGS) -c $*.cpp	

LINK:
	$(MPICPP) -fPIC -Wall -Wwrite-strings -Wno-strict-aliasing -Wno-unknown-pragmas -g3 -O0  -o app_raijin_cluster AppWrap.o $(CPP_OBJS) -Wl,-rpath,/home/mohsin/lib -L/home/mohsin/lib  -lpetsc -lflapack -lfblas -lssl -lcrypto -lX11 -lpthread -lhwloc -Wl,-rpath,/usr/lib/gcc/i686-linux-gnu/4.6 -L/usr/lib/gcc/i686-linux-gnu/4.6 -Wl,-rpath,/usr/lib/i386-linux-gnu -L/usr/lib/i386-linux-gnu -Wl,-rpath,/lib/i386-linux-gnu -L/lib/i386-linux-gnu -lmpi_f90 -lmpi_f77 -lgfortran -lm -lgfortran -lquadmath -lmpi_cxx -lstdc++ -Wl,-rpath,/usr/lib/gcc/i686-linux-gnu/4.6 -L/usr/lib/gcc/i686-linux-gnu/4.6  -L/lib/i386-linux-gnu -ldl -lmpi -lrt -lnsl -lutil -lgcc_s -lpthread -ldl $(CPP_LINK_LIBS)

clean:
	rm -f *.o $(PROG)

run3d:
	./run3dAdvectRaijin -v 0 -p 8 -q 4 -r 2 -l 4 8 8 8

include ${PETSC_DIR}/conf/test
