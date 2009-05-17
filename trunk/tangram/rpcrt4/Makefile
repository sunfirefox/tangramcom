
############################################################################
#
#   Makefile for building : rpcrt4.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -D__WINESRC__
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../include -I../../include/winheaders -I../../Include/tangram/ole32 -I../../utils/CommonUtilitiesLib
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmcom_config.h -pipe  -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmcom_config.h -pipe  -g -Wall -W -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L../../bin/linux  -ltgmwine -ltgmrpc_com -lCommonUtilities
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmrpcrt4.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libtgmrpcrt4.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/cproxy.o \
	$(OBJECTS_DIR)/cpsf.o \
	$(OBJECTS_DIR)/cstub.o \
	$(OBJECTS_DIR)/epm_c.o \
	$(OBJECTS_DIR)/ndr_clientserver.o \
	$(OBJECTS_DIR)/ndr_contexthandle.o \
	$(OBJECTS_DIR)/ndr_es.o \
	$(OBJECTS_DIR)/ndr_fullpointer.o \
	$(OBJECTS_DIR)/ndr_marshall.o \
	$(OBJECTS_DIR)/ndr_ole.o \
	$(OBJECTS_DIR)/ndr_stubless.o \
	$(OBJECTS_DIR)/rpc_assoc.o \
	$(OBJECTS_DIR)/rpc_async.o \
	$(OBJECTS_DIR)/rpc_binding.o \
	$(OBJECTS_DIR)/rpc_epmap.o \
	$(OBJECTS_DIR)/rpc_message.o \
	$(OBJECTS_DIR)/rpc_packet_merger.o \
	$(OBJECTS_DIR)/rpc_server.o \
	$(OBJECTS_DIR)/rpc_transport.o \
	$(OBJECTS_DIR)/rpcrt4_main.o

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

$(OBJECTS_DIR)/cproxy.o : ./cproxy.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/cproxy.o ./cproxy.c

$(OBJECTS_DIR)/cpsf.o : ./cpsf.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/cpsf.o ./cpsf.c

$(OBJECTS_DIR)/cstub.o : ./cstub.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/cstub.o ./cstub.c

$(OBJECTS_DIR)/epm_c.o : ./epm_c.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/epm_c.o ./epm_c.c

$(OBJECTS_DIR)/ndr_clientserver.o : ./ndr_clientserver.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_clientserver.o ./ndr_clientserver.c

$(OBJECTS_DIR)/ndr_contexthandle.o : ./ndr_contexthandle.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_contexthandle.o ./ndr_contexthandle.cpp

$(OBJECTS_DIR)/ndr_es.o : ./ndr_es.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_es.o ./ndr_es.c

$(OBJECTS_DIR)/ndr_fullpointer.o : ./ndr_fullpointer.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_fullpointer.o ./ndr_fullpointer.c

$(OBJECTS_DIR)/ndr_marshall.o : ./ndr_marshall.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_marshall.o ./ndr_marshall.c

$(OBJECTS_DIR)/ndr_ole.o : ./ndr_ole.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_ole.o ./ndr_ole.c

$(OBJECTS_DIR)/ndr_stubless.o : ./ndr_stubless.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ndr_stubless.o ./ndr_stubless.c

$(OBJECTS_DIR)/rpc_assoc.o : ./rpc_assoc.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_assoc.o ./rpc_assoc.cpp

$(OBJECTS_DIR)/rpc_async.o : ./rpc_async.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_async.o ./rpc_async.c

$(OBJECTS_DIR)/rpc_binding.o : ./rpc_binding.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_binding.o ./rpc_binding.cpp

$(OBJECTS_DIR)/rpc_epmap.o : ./rpc_epmap.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_epmap.o ./rpc_epmap.cpp

$(OBJECTS_DIR)/rpc_message.o : ./rpc_message.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_message.o ./rpc_message.cpp

$(OBJECTS_DIR)/rpc_packet_merger.o : ./rpc_packet_merger.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_packet_merger.o ./rpc_packet_merger.cpp

$(OBJECTS_DIR)/rpc_server.o : ./rpc_server.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_server.o ./rpc_server.cpp

$(OBJECTS_DIR)/rpc_transport.o : ./rpc_transport.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_transport.o ./rpc_transport.cpp

$(OBJECTS_DIR)/rpcrt4_main.o : ./rpcrt4_main.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpcrt4_main.o ./rpcrt4_main.c

