cc	=	g++
CFLAGS	=	-std=c++11	-Wall	-g

CLIENT_SOURCES	=	client_1.cpp
CLIENT_OBJECTS	=	$(CLIENT_SOURCES:.cpp=.o)
CLIENT_EXECUTABLE	=	client_1

SERVER_SOURCES	=	server_1.cpp
SERVER_OBJECTS	=	$(SERVER_SOURCES:.cpp=.o)
SERVER_EXECUTABLE	=	server_1

HEADER_FILE	=	linkedList.h

.PHONY:	all	clean 

all:	$(CLIENT_EXECUTABLE)	$(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE):	$(CLIENT_OBJECTS)
	$(cc)	$(CFLAGS)	$^	-o	$@

$(SERVER_EXECUTABLE):	$(SERVER_OBJECTS)
	$(cc)	$(CFLAGS)	$^	-o	$@

$(CLIENT_OBJECTS):	%.o:	%.cpp	$(HEADER_FILE)
	$(cc)	$(CFLAGS)	-c	$<	-o	$@

$(SERVER_OBJECTS):	%.o:	%.cpp	$(HEADER_FILE)
	$(cc)	$(CFLAGS)	-c	$<	-o	$@

clean:
	rm	-f	$(CLIENT_OBJECTS)	$(SERVER_OBJECTS)	$(CLIENT_EXECUTABLE)	$(SERVER_EXECUTABLE)