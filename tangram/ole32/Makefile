
############################################################################
#
#   Makefile for building : ole32.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -D__WINESRC__
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../../include -I../../include/winheaders -I../../utils/CommonUtilitiesLib
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmcom_config.h -pipe  -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -include tgmcom_config.h -pipe  -g -Wall -W  -fno-exceptions -fno-rtti $(DEFINES)
LIBS = -L../../bin/linux -ltgmrpc_com -lCommonUtilities  -ltgmwine -ltgmrpcrt4 -luuid
LINK = g++
LINK_FLAGS = -shared -Wl,-soname,libtgmole32.so

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../bin/linux
TARGET = $(DESTDIR)/libtgmole32.so

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/compobj.o \
	$(OBJECTS_DIR)/dcom_p.o \
	$(OBJECTS_DIR)/errorinfo.o \
	$(OBJECTS_DIR)/ftmarshal.o \
	$(OBJECTS_DIR)/git.o \
	$(OBJECTS_DIR)/hglobalstream.o \
	$(OBJECTS_DIR)/ifs.o \
	$(OBJECTS_DIR)/irot_c.o \
	$(OBJECTS_DIR)/marshal.o \
	$(OBJECTS_DIR)/moniker.o \
	$(OBJECTS_DIR)/ole32_objidl_p.o \
	$(OBJECTS_DIR)/ole32_unknwn_p.o \
	$(OBJECTS_DIR)/oleproxy.o \
	$(OBJECTS_DIR)/regsvr.o \
	$(OBJECTS_DIR)/rpc.o \
	$(OBJECTS_DIR)/rpc_scm.o \
	$(OBJECTS_DIR)/stubmanager.o \
	$(OBJECTS_DIR)/usrmarshal.o

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

$(OBJECTS_DIR)/compobj.o : ./compobj.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/compobj.o ./compobj.c

$(OBJECTS_DIR)/dcom_p.o : ./dcom_p.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/dcom_p.o ./dcom_p.c

$(OBJECTS_DIR)/errorinfo.o : ./errorinfo.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/errorinfo.o ./errorinfo.c

$(OBJECTS_DIR)/ftmarshal.o : ./ftmarshal.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ftmarshal.o ./ftmarshal.c

$(OBJECTS_DIR)/git.o : ./git.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/git.o ./git.c

$(OBJECTS_DIR)/hglobalstream.o : ./hglobalstream.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/hglobalstream.o ./hglobalstream.c

$(OBJECTS_DIR)/ifs.o : ./ifs.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ifs.o ./ifs.c

$(OBJECTS_DIR)/irot_c.o : ./irot_c.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/irot_c.o ./irot_c.c

$(OBJECTS_DIR)/marshal.o : ./marshal.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/marshal.o ./marshal.c

$(OBJECTS_DIR)/moniker.o : ./moniker.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/moniker.o ./moniker.c

$(OBJECTS_DIR)/ole32_objidl_p.o : ./ole32_objidl_p.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ole32_objidl_p.o ./ole32_objidl_p.c

$(OBJECTS_DIR)/ole32_unknwn_p.o : ./ole32_unknwn_p.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ole32_unknwn_p.o ./ole32_unknwn_p.c

$(OBJECTS_DIR)/oleproxy.o : ./oleproxy.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/oleproxy.o ./oleproxy.c

$(OBJECTS_DIR)/regsvr.o : ./regsvr.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/regsvr.o ./regsvr.c

$(OBJECTS_DIR)/rpc.o : ./rpc.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc.o ./rpc.c

$(OBJECTS_DIR)/rpc_scm.o : ./rpc_scm.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/rpc_scm.o ./rpc_scm.c

$(OBJECTS_DIR)/stubmanager.o : ./stubmanager.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/stubmanager.o ./stubmanager.c

$(OBJECTS_DIR)/usrmarshal.o : ./usrmarshal.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/usrmarshal.o ./usrmarshal.c

