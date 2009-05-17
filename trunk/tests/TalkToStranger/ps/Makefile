
############################################################################
#
#   Makefile for building : talk_ps.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DREGISTER_PROXY_DLL=1 -D_EXPORT_STANDARD_PROXY_DLLROUTINES_
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../../include -I../../../include/winheaders -I../../../Include/CommonUtilitiesLib
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fvisibility=hidden $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fvisibility=hidden -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L../../../bin/linux -lCommonUtilities  -ltgmwine -ltgmrpcrt4 -luuid
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtest_chat_com.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../../bin/linux
TARGET = $(DESTDIR)/libtalk_ps.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/dlldata.o \
	$(OBJECTS_DIR)/talk_i.o \
	$(OBJECTS_DIR)/talk_p.o

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

$(OBJECTS_DIR)/dlldata.o : ./dlldata.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/dlldata.o ./dlldata.c

$(OBJECTS_DIR)/talk_i.o : ./talk_i.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/talk_i.o ./talk_i.c

$(OBJECTS_DIR)/talk_p.o : ./talk_p.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/talk_p.o ./talk_p.c

