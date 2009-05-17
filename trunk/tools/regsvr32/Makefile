
############################################################################
#
#   Makefile for building : regsvr32.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DHAHAH
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../include -I../../include/winheaders
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe -g -Wall -W -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L/usr/local/lib -lpthread -ldl -lstdc++  -lm -lc -lgcc_s \
       -L../../bin/linux -ltgmlog -lCommonUtilities -ltgmwine  -ltgmole32 -ltgmrpcrt4 -luuid -ltgmrpc_com
LINK = g++

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/tgmregsvr32

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/regsvr32.o

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

$(OBJECTS_DIR)/regsvr32.o : ./regsvr32.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/regsvr32.o ./regsvr32.c

