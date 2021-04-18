CC = g++ -std=c++17 -Wall
RELEASE = 0
UNICODE = 0  
FILECENTOS = /etc/redhat-release
LOADBIN = -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_UPData
LIB = -lXEngine_BaseLib -lHelpComponents_XLog -lUPData_FileParser -lUPData_Downloads -lUPData_HelpModule
LIBEX = -static-libgcc -ldl -lrt -lpthread
LOADSO = -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_DownLoad:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_UPData,--disable-new-dtags
LIBINCLUDE = UPDater_Net.o UPDater_Config.o UPDater_APPTest.o

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

ifeq ($(FILECENTOS), $(wildcard $(FILECENTOS)))
	UNICODE = -D _CENTOS
else
	UNICODE = -D _UBUNTU
endif

all:$(LIBINCLUDE)
	$(CC) $(DEBUG) $(LIBINCLUDE) -o XEngine_UPDaterApp.exe $(LOADBIN) $(LIB) $(LIBEX) $(LOADSO)
  
UPDater_Net.o:./UPDater_Net.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./UPDater_Net.cpp
UPDater_Config.o:./UPDater_Config.cpp
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./UPDater_Config.cpp
UPDater_APPTest.o:./UPDater_APPTest.cpp             
	$(CC) $(DEBUG) $(FLAGS) $(UNICODE) ./UPDater_APPTest.cpp   


CleanAll:CleanObj CleanMk
CleanObj:                                              
	rm *.o
CleanMk:                                  
	rm XEngine_UPDaterApp.exe     
	       
