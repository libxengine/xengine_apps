#ifdef _WINDOWS
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_ManagePool.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_ManagePool/ManagePool_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPPool.cpp -o XCore_APPPool.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

XHTHREAD CALLBACK ManagePool_ThreadPool(LPVOID lParam)
{
	int *pInt_Number = (int*)lParam;
	printf(_T("%d\n"), *pInt_Number);

	free(pInt_Number);
	pInt_Number = NULL;
	return 0;
}
void ThreadPool_Test()
{
	XNETHANDLE xhPool = 0;
	if (!ManagePool_Thread_Create(&xhPool, 2))
	{
		return;
	}
	for (int i = 0; i < 100; i++)
	{
		int* pInt = (int*)malloc(sizeof(int));
		*pInt = i;
		ManagePool_Thread_PostTask(xhPool, ManagePool_ThreadPool, pInt);
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	ManagePool_Thread_Destroy(xhPool);
}

void SocketPool_Test()
{
	XNETHANDLE xhPool = 0;
	if (!ManagePool_Socket_Create(&xhPool, 100))
	{
		printf("ManagePool_Socket_Create!@\n");
		return;
	}
	SOCKET hSocket = INVALID_SOCKET;
	if (!ManagePool_Socket_GetIdleSocket(xhPool, &hSocket))
	{
		printf("ManagePool_Socket_GetIdleSocket!@\n");
		return;
	}
	sockaddr_in st_SockProxy;

	st_SockProxy.sin_addr.s_addr = inet_addr("192.168.1.4");
	st_SockProxy.sin_port = htons(5002);
	st_SockProxy.sin_family = AF_INET;

	if (SOCKET_ERROR == connect(hSocket, (sockaddr*)&st_SockProxy, sizeof(st_SockProxy)))
	{
	}
	send(hSocket, "hello", 5, 0);
	if (!ManagePool_Socket_RetIdleSocket(xhPool, hSocket))
	{
		printf("ManagePool_Socket_GetIdleSocket!@\n");
		return;
	}
	ManagePool_Socket_Destroy(xhPool);
}

void __stdcall fun(void* p)
{
	printf("call fun\n");
}
int MemoryPool_Test()
{
	XMPOOL xmPool;
	MANAGEPOOL_MEMORY_CLEANUP st_Cleanup;

	CHAR* p1, * p2, * p3, * p4;

	xmPool = ManagePool_Memory_Create();
	p1 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 416); /*分配内存，在可分配内*/
	memcpy(p1, "p1", 2);
	printf("%s\n", p1);

	p2 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 200);/*分配内存，在可分配内，但pool中没有足够的内存空间*/
	memcpy(p2, "p2", 2);
	printf("%s\n", p2);

	ManagePool_Memory_Free(xmPool, p1);
	ManagePool_Memory_Free(xmPool, p2);

	p3 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 624);//分配大内存
	memcpy(p3, "p3", 2);
	printf("%s\n", p3);

	p4 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 100);//分配内存
	memcpy(p4, "p4", 2);
	printf("%s\n", p4);

	CHAR* p5 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 4000);//分配内存
	memcpy(p5, "p5", 2);
	printf("%s,%s\n", p5, p4);

	CHAR* p6 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 5000);//分配大内存
	memcpy(p6, "p6", 2);
	printf("%s,%s\n", p6, p4);

	ManagePool_Memory_Free(xmPool, p6);
	CHAR* p7 = (CHAR*)ManagePool_Memory_Alloc(xmPool, 5000);//分配大内存
	memcpy(p7, "p7", 2);
	printf("%s,%s,%s\n", p7, p4, p5);

	st_Cleanup.fpCall_Cleanup = &fun;
	ManagePool_Memory_CleanupAdd(xmPool, &st_Cleanup);/*注册销毁回调函数*/

	ManagePool_Memory_Destory(xmPool);/*pool池注销*/
	printf("\n");
	return 0;
}
int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	MemoryPool_Test();
	ThreadPool_Test();
	SocketPool_Test();
	while (1)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}

