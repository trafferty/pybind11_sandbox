
# CNT stuff
MII_COMMON=$(MII_HOME)/src/common
ifndef MII_WKSP
MII_WKSP=~/workspace
endif

#commands
CC=gcc
CPP=g++
LD=g++
RM = /bin/rm -f
MKDIR=/bin/mkdir -p
CPDIR=/bin/cp -r
CPFILE=/bin/cp
AR=/usr/bin/ar

#Compiler flags
DFLAGS = -O3
CFLAGS = $(DFLAGS) -W -Wall -Wextra -fpic -std=c++11

#Linker flags
LDFLAGS =-rdynamic -Wl,--export-dynamic,--no-as-needed -lX11

SOFLAGS =-W -WALL -pipe

PYTHON_CFLAGS=$(shell python3-config --cflags)
PYTHON_LDFLAGS=$(shell python3-config --ldflags --libs)

UTIL_DIR=/home/trafferty/dev/src/ipHost/util

AUXIL=./auxil
PYBIND11_BASE=../pybind11

PYBIND11_CFLAGS=-I$(PYBIND11_BASE)/include $(PYTHON_CFLAGS)
PYBIND11_LDFLAGS=$(PYTHON_LDFLAGS)

INCLUDES=\
	-I./include \
	-I$(UTIL_DIR)

UTIL_OBJS = \
	src/.obj/cJSON.o \
	src/.obj/CNT_JSON.o

DRIVERS = \
    src/.obj/ImgEngine.o \
    src/.obj/Sim_ImgEngine.o

all: \
	src/.obj \
	bin \
	$(UTIL_OBJS) \
	$(DRIVERS) \
	dif.so

clean:
	$(RM) -r src/.obj
	$(RM) -r bin
	$(RM) -r dif.so

src/.obj:
	$(MKDIR) src/.obj
bin:
	$(MKDIR) bin

# compile util objs
src/.obj/cJSON.o: $(UTIL_DIR)/cJSON.c $(UTIL_DIR)/cJSON.h
	$(CPP) -c $< -o $@ $(CFLAGS) $(INCLUDES)
src/.obj/CNT_JSON.o: $(UTIL_DIR)/CNT_JSON.c $(UTIL_DIR)/CNT_JSON.h
	$(CPP) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# compile Driver objs
src/.obj/ImgEngine.o: src/ImgEngine.cpp include/ImgEngine.h
	$(CPP) -c $< -o $@ $(CFLAGS) $(INCLUDES) $(PYBIND11_CFLAGS)

src/.obj/Sim_ImgEngine.o: src/Sim_ImgEngine.cpp include/Sim_ImgEngine.h
	$(CPP) -c $< -o $@ $(CFLAGS) $(INCLUDES) $(PYBIND11_CFLAGS)

dif.so: src/dif_bindings.cpp
	$(LD) $(CFLAGS) -shared $(INCLUDES) $(PYBIND11_CFLAGS) $(PYTHON_LDFLAGS) $(DRIVERS) $< -o $@

#
# g++ -O3 -shared -fpic -std=c++11 -I./include -I/home/trafferty/dev/src/ipHost/util  -I../pybind11/include -I/usr/include/python3.4m -I/usr/include/python3.4m  -Wno-unused-result -g -fstack-protector --param=ssp-buffer-size=4
# -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes -L/usr/lib/python3.4/config-3.4m-x86_64-linux-gnu -L/usr/lib -lpython3.4m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions
# -lpython3.4m -lpthread -ldl  -lutil -lm src/.obj/ImgEngine.o src/.obj/Sim_ImgEngine.o src/dif_bindings.cpp -o dif.so
