
############################################################################
#
#   Makefile for building : tgmrpc.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DARGRPC_MAKEDLL -DDLLSVC
INCLUDE_PATH =  -I../../include -I../../include/winheaders -I. -I.. -ISources -I../../utils/CommonUtilitiesLib -I/usr/local/include
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1  -pipe  -g -Wall -W -fvisibility=hidden $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1  -pipe  -g -Wall -W -fvisibility=hidden -fno-exceptions -fno-rtti $(DEFINES)
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmrpc_com.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libtgmrpc_com.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/RpcFactory.o \
	$(OBJECTS_DIR)/svc.o \
	$(OBJECTS_DIR)/tgmrpccom.o \
	$(OBJECTS_DIR)/RpcClientRequest.o \
	$(OBJECTS_DIR)/RpcComCommon.o \
	$(OBJECTS_DIR)/RpcComProtocol.o \
	$(OBJECTS_DIR)/RpcTcpClient.o \
	$(OBJECTS_DIR)/RpcTcpConnection.o \
	$(OBJECTS_DIR)/RpcTcpServer.o \
	$(OBJECTS_DIR)/RpcUnixClient.o \
	$(OBJECTS_DIR)/RpcUnixConnection.o \
	$(OBJECTS_DIR)/RpcUnixServer.o

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

$(OBJECTS_DIR)/RpcFactory.o : ./RpcFactory.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcFactory.o ./RpcFactory.cpp

$(OBJECTS_DIR)/svc.o : ./svc.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/svc.o ./svc.cpp

$(OBJECTS_DIR)/tgmrpccom.o : ./tgmrpccom.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/tgmrpccom.o ./tgmrpccom.cpp

$(OBJECTS_DIR)/RpcClientRequest.o : ./Sources/RpcClientRequest.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcClientRequest.o ./Sources/RpcClientRequest.cpp

$(OBJECTS_DIR)/RpcComCommon.o : ./Sources/RpcComCommon.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcComCommon.o ./Sources/RpcComCommon.cpp

$(OBJECTS_DIR)/RpcComProtocol.o : ./Sources/RpcComProtocol.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcComProtocol.o ./Sources/RpcComProtocol.cpp

$(OBJECTS_DIR)/RpcTcpClient.o : ./Sources/RpcTcpClient.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcTcpClient.o ./Sources/RpcTcpClient.cpp

$(OBJECTS_DIR)/RpcTcpConnection.o : ./Sources/RpcTcpConnection.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcTcpConnection.o ./Sources/RpcTcpConnection.cpp

$(OBJECTS_DIR)/RpcTcpServer.o : ./Sources/RpcTcpServer.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcTcpServer.o ./Sources/RpcTcpServer.cpp

$(OBJECTS_DIR)/RpcUnixClient.o : ./Sources/RpcUnixClient.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcUnixClient.o ./Sources/RpcUnixClient.cpp

$(OBJECTS_DIR)/RpcUnixConnection.o : ./Sources/RpcUnixConnection.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcUnixConnection.o ./Sources/RpcUnixConnection.cpp

$(OBJECTS_DIR)/RpcUnixServer.o : ./Sources/RpcUnixServer.cpp
	$(CXX) -c $(CXX_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/RpcUnixServer.o ./Sources/RpcUnixServer.cpp

