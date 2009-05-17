
############################################################################
#
#   Makefile for building : scm_com.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DTGMSCM_COM_EXPORTS
INCLUDE_PATH = -I/usr/local/include \
               -I. -I..  -I../../../include/winheaders -I../../../include -I../../../tangram/ole32 -I../../../utils/CommonUtilitiesLib
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include ../tgmcom_config.h  -pipe  -g -Wall -W -fvisibility=hidden $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include ../tgmcom_config.h  -pipe  -g -Wall -W -fvisibility=hidden -fno-exceptions -fno-rtti $(DEFINES)
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmscm_com.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../../bin/linux
TARGET = $(DESTDIR)/libtgmscm_com.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/scm_client.o \
	$(OBJECTS_DIR)/svc.o

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

$(OBJECTS_DIR)/scm_client.o : ./scm_client.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/scm_client.o ./scm_client.cpp

$(OBJECTS_DIR)/svc.o : ./svc.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/svc.o ./svc.cpp

