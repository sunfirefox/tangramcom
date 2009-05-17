
############################################################################
#
#   Makefile for building : scm_daemon.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DANGORA
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../../include -I../../../tangram/ole32 -I../../../utils/CommonUtilitiesLib -I../../../include/winheaders
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include ../tgmcom_config.h -pipe  -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include ../tgmcom_config.h -pipe  -g -Wall -W -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L/usr/local/lib -lpthread -ldl -lstdc++  -lm -lc -lgcc_s \
       -L$(DESTDIR) -lCommonUtilities -ltgmrpc_com -ltgmlog -ltgmwine  -ltgmole32 -ltgmrpcrt4 -luuid
LINK = g++

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../../bin/linux
TARGET = $(DESTDIR)/scm_daemon

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/scm_daemon.o \
	$(OBJECTS_DIR)/scm_service.o

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

$(OBJECTS_DIR)/scm_daemon.o : ./scm_daemon.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/scm_daemon.o ./scm_daemon.cpp

$(OBJECTS_DIR)/scm_service.o : ./scm_service.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/scm_service.o ./scm_service.cpp

