SRC_PATH = src/
BIN_PATH = bin/
INCLUDE_PATH = include/

all: $(BIN_PATH)main

IFLAGS			= -I$(INCLUDE_PATH)
DEBUG_MODULES	=
COPTFLAGS		= -O3 -march=native -mtune=native
CPPFLAGS		= -DVERSION=${PETSC_VERSION_NUM}
CXXFLAGS		= $(DEBUG_MODULES) $(COPTFLAGS) $(IFLAGS)
CLINKER			= g++


include ${PETSC_DIR}/lib/petsc/conf/variables
include ${PETSC_DIR}/lib/petsc/conf/rules
include ${PETSC_DIR}/lib/petsc/conf/test

$(BIN_PATH)main: $(SRC_PATH)main.o
	mkdir -p $(BIN_PATH)
	-${CLINKER} $^ -o $@ ${PETSC_SYS_LIB}
	mv $(SRC_PATH)*.o $(BIN_PATH)
	
#.PHONY : clean
clean::
	-rm -f $(BIN_PATH)*.o $(BIN_PATH)main