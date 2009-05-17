
############################################################################
#
#   Makefile for building : widl.TMK3
#   Created by TMK3_V2, please do not modify.
#
#############################################################################

CC = gcc
CXX = g++
DEFINES = -DANGORA
INCLUDE_PATH = -I/usr/local/include \
               -I. -I.. -I../include -Iwpp -I../../../include/winheaders
CC_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W $(DEFINES)
CXX_FLAGS = -D_REENTRANT -D__USE_POSIX -D__linux__=1 -pipe  -g -Wall -W -fno-exceptions -fno-rtti -fshort-wchar $(DEFINES)
LINK = g++

OBJECTS_DIR = ./DebugLinux
DESTDIR = ../../../bin/linux
TARGET = $(DESTDIR)/widl

DEL_FILE = rm -f
MKDIR = mkdir -p

first: all

OBJECTS = \
	$(OBJECTS_DIR)/client.o \
	$(OBJECTS_DIR)/expr.o \
	$(OBJECTS_DIR)/getopt.o \
	$(OBJECTS_DIR)/getopt1.o \
	$(OBJECTS_DIR)/hash.o \
	$(OBJECTS_DIR)/header.o \
	$(OBJECTS_DIR)/list.o \
	$(OBJECTS_DIR)/mkstemps.o \
	$(OBJECTS_DIR)/parser.tab.o \
	$(OBJECTS_DIR)/parser.yy.o \
	$(OBJECTS_DIR)/proxy.o \
	$(OBJECTS_DIR)/server.o \
	$(OBJECTS_DIR)/typegen.o \
	$(OBJECTS_DIR)/typelib.o \
	$(OBJECTS_DIR)/utils.o \
	$(OBJECTS_DIR)/widl.o \
	$(OBJECTS_DIR)/write_msft.o \
	$(OBJECTS_DIR)/ppl.yy.o \
	$(OBJECTS_DIR)/ppy.tab.o \
	$(OBJECTS_DIR)/preproc.o \
	$(OBJECTS_DIR)/wpp.o

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

$(OBJECTS_DIR)/client.o : ./client.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/client.o ./client.c

$(OBJECTS_DIR)/expr.o : ./expr.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/expr.o ./expr.c

$(OBJECTS_DIR)/getopt.o : ./getopt.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/getopt.o ./getopt.c

$(OBJECTS_DIR)/getopt1.o : ./getopt1.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/getopt1.o ./getopt1.c

$(OBJECTS_DIR)/hash.o : ./hash.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/hash.o ./hash.c

$(OBJECTS_DIR)/header.o : ./header.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/header.o ./header.c

$(OBJECTS_DIR)/list.o : ./list.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/list.o ./list.c

$(OBJECTS_DIR)/mkstemps.o : ./mkstemps.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/mkstemps.o ./mkstemps.c

$(OBJECTS_DIR)/parser.tab.o : ./parser.tab.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/parser.tab.o ./parser.tab.c

$(OBJECTS_DIR)/parser.yy.o : ./parser.yy.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/parser.yy.o ./parser.yy.c

$(OBJECTS_DIR)/proxy.o : ./proxy.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/proxy.o ./proxy.c

$(OBJECTS_DIR)/server.o : ./server.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/server.o ./server.c

$(OBJECTS_DIR)/typegen.o : ./typegen.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/typegen.o ./typegen.c

$(OBJECTS_DIR)/typelib.o : ./typelib.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/typelib.o ./typelib.c

$(OBJECTS_DIR)/utils.o : ./utils.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/utils.o ./utils.c

$(OBJECTS_DIR)/widl.o : ./widl.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/widl.o ./widl.c

$(OBJECTS_DIR)/write_msft.o : ./write_msft.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/write_msft.o ./write_msft.c

$(OBJECTS_DIR)/ppl.yy.o : ./wpp/ppl.yy.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ppl.yy.o ./wpp/ppl.yy.c

$(OBJECTS_DIR)/ppy.tab.o : ./wpp/ppy.tab.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/ppy.tab.o ./wpp/ppy.tab.c

$(OBJECTS_DIR)/preproc.o : ./wpp/preproc.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/preproc.o ./wpp/preproc.c

$(OBJECTS_DIR)/wpp.o : ./wpp/wpp.c
	$(CC) -c $(CC_FLAGS) $(INCLUDE_PATH) -o $(OBJECTS_DIR)/wpp.o ./wpp/wpp.c

