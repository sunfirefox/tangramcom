
############################################################################
#
#   Makefile for building : tgmlog.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DTGMLOG_MAKEDLL
INCLUDE_PATH = -I. -I.. -I/usr/local/include -I../../include/winheaders
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fvisibility=hidden $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fvisibility=hidden -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L/usr/local/lib -lpthread
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmlog.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libtgmlog.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/tgmlog.o \
	$(OBJECTS_DIR)/tgmlog_common.o \
	$(OBJECTS_DIR)/tgmlog_fun.o \
	$(OBJECTS_DIR)/tgmlog_func_trace.o \
	$(OBJECTS_DIR)/tgmlog_util.o

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

$(OBJECTS_DIR)/tgmlog.o : ./Sources/tgmlog.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmlog.o ./Sources/tgmlog.c

$(OBJECTS_DIR)/tgmlog_common.o : ./Sources/tgmlog_common.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmlog_common.o ./Sources/tgmlog_common.c

$(OBJECTS_DIR)/tgmlog_fun.o : ./Sources/tgmlog_fun.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmlog_fun.o ./Sources/tgmlog_fun.c

$(OBJECTS_DIR)/tgmlog_func_trace.o : ./Sources/tgmlog_func_trace.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmlog_func_trace.o ./Sources/tgmlog_func_trace.c

$(OBJECTS_DIR)/tgmlog_util.o : ./Sources/tgmlog_util.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmlog_util.o ./Sources/tgmlog_util.c

