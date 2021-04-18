CC = g++ -Wall -std=c++17 -fPIC
RELEASE = 0
UNICODE = 0
INCLUDE = 
FILECENTOS = 
LOADBIN = 
LIB = 
LIBEX = -lrt -ldl
LOADSO = 
LIBINCLUDE = pch.o

ifeq ($(RELEASE),1)
FLAGS = -c -O2 
DEBUG = 
else
FLAGS = -c -lc_p
DEBUG = -g -pg 
endif

ifeq ($(UNICODE),1)
UNICODE = -D _UNICODE
else
UNICODE = 
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o libPlugin_Module.xpl -shared -fPIC $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)


pch.o:pch.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) pch.cpp


CleanAll:CleanObj CleanMk
CleanObj:
	rm *.o                                        
CleanMk:
	rm *.so                                   
