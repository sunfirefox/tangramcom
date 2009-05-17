
############################################################################
#
#   Makefile for building : tgmwine.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -D__WINESRC__ -DMIDL_PASS
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../include -I../../include/winheaders -I../../utils/CommonUtilitiesLib -ITG3
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmwine_config.h -pipe  -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmwine_config.h -pipe  -g -Wall -W -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L../../bin/linux
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmwine.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libtgmwine.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/casemap.o \
	$(OBJECTS_DIR)/collation.o \
	$(OBJECTS_DIR)/compose.o \
	$(OBJECTS_DIR)/fold.o \
	$(OBJECTS_DIR)/mmap.o \
	$(OBJECTS_DIR)/port.o \
	$(OBJECTS_DIR)/string2.o \
	$(OBJECTS_DIR)/string3.o \
	$(OBJECTS_DIR)/wctype.o \
	$(OBJECTS_DIR)/critsection.o \
	$(OBJECTS_DIR)/heap.o \
	$(OBJECTS_DIR)/kernel_main.o \
	$(OBJECTS_DIR)/module.o \
	$(OBJECTS_DIR)/string.o \
	$(OBJECTS_DIR)/sync.o \
	$(OBJECTS_DIR)/thread.o \
	$(OBJECTS_DIR)/rtlstr.o \
	$(OBJECTS_DIR)/registery.o \
	$(OBJECTS_DIR)/xmlregistry.o \
	$(OBJECTS_DIR)/wsprintf.o

ADD_OBJECTS = 

$(OBJECTS_DIR) :
	$(MKDIR) $(OBJECTS_DIR)

$(DESTDIR) :
	$(MKDIR) $(DESTDIR)

all : $(OBJECTS_DIR) $(DESTDIR) $(TARGET)

$(TARGET) : $(OBJECTS)
	$(LINK) $(LINK_FLAGS) -o $(TARGET) $(OBJECTS) $(ADD_OBJECTS) $(LIBS)

clean : 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) $(TARGET)

$(OBJECTS_DIR)/casemap.o : ./casemap.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/casemap.o ./casemap.c

$(OBJECTS_DIR)/collation.o : ./collation.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/collation.o ./collation.c

$(OBJECTS_DIR)/compose.o : ./compose.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/compose.o ./compose.c

$(OBJECTS_DIR)/fold.o : ./fold.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/fold.o ./fold.c

$(OBJECTS_DIR)/mmap.o : ./mmap.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/mmap.o ./mmap.c

$(OBJECTS_DIR)/port.o : ./port.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/port.o ./port.c

$(OBJECTS_DIR)/string2.o : ./string2.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/string2.o ./string2.c

$(OBJECTS_DIR)/string3.o : ./string3.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/string3.o ./string3.c

$(OBJECTS_DIR)/wctype.o : ./wctype.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/wctype.o ./wctype.c

$(OBJECTS_DIR)/critsection.o : ./kernel32/critsection.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/critsection.o ./kernel32/critsection.cpp

$(OBJECTS_DIR)/heap.o : ./kernel32/heap.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/heap.o ./kernel32/heap.c

$(OBJECTS_DIR)/kernel_main.o : ./kernel32/kernel_main.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/kernel_main.o ./kernel32/kernel_main.c

$(OBJECTS_DIR)/module.o : ./kernel32/module.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/module.o ./kernel32/module.cpp

$(OBJECTS_DIR)/string.o : ./kernel32/string.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/string.o ./kernel32/string.c

$(OBJECTS_DIR)/sync.o : ./kernel32/sync.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/sync.o ./kernel32/sync.c

$(OBJECTS_DIR)/thread.o : ./kernel32/thread.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/thread.o ./kernel32/thread.c

$(OBJECTS_DIR)/rtlstr.o : ./ntdll/rtlstr.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rtlstr.o ./ntdll/rtlstr.c

$(OBJECTS_DIR)/registery.o : ./reg/registery.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/registery.o ./reg/registery.c

$(OBJECTS_DIR)/xmlregistry.o : ./reg/xmlregistry.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/xmlregistry.o ./reg/xmlregistry.cpp

$(OBJECTS_DIR)/wsprintf.o : ./user32/wsprintf.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/wsprintf.o ./user32/wsprintf.c

