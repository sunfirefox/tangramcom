
############################################################################
#
#   Makefile for building : uuid.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -D__WINESRC__ -DMIDL_PASS
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../include -I../../include/winheaders
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fvisibility=default $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W  -fvisibility=default -fno-exceptions -fno-rtti $(DEFINES)
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libuuid.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libuuid.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/uuid.o

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

$(OBJECTS_DIR)/uuid.o : ./uuid.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/uuid.o ./uuid.c

