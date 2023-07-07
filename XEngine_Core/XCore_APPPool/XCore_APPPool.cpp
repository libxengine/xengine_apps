#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <tchar.h>
#include <Windows.h>
#pragma comment(lib,"Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;

#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_Core/XEngine_ManagePool.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_ManagePool.lib")
#endif
#endif
//linux:g++ -std=gnu++17 -Wall -g XCore_APPPool.cpp -o XCore_APPPool.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//macos:g++ -std=gnu++17 -Wall -g XCore_APPPool.cpp -o XCore_APPPool.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_Algorithm -lXEngine_ManagePool

XHTHREAD CALLBACK ManagePool_ThreadPool(XPVOID lParam)
{
	int *pInt_Number = (int*)lParam;
	printf(_X("%d\n"), *pInt_Number);

	free(pInt_Number);
	pInt_Number = NULL;
	return 0;
}
void ThreadPool_Test()
{
	XHANDLE xhPool = ManagePool_Thread_DTCreate(2);
	if (NULL == xhPool)
	{
		return;
	}
	for (int i = 0; i < 100; i++)
	{
		int* pInt = (int*)malloc(sizeof(int));
		*pInt = i;
		ManagePool_Thread_DTPostTask(xhPool, ManagePool_ThreadPool, pInt);
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	ManagePool_Thread_DTDestroy(xhPool);

	ManagePool_Thread_CTCreate(2);
	for (int i = 0; i < 100; i++)
	{
		int* pInt = (int*)malloc(sizeof(int));
		*pInt = i;
		ManagePool_Thread_CTPostTask(ManagePool_ThreadPool, pInt);
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
	ManagePool_Thread_CTDestroy();
}

void SocketPool_Test()
{
	XNETHANDLE xhPool = 0;
	if (!ManagePool_Socket_Create(&xhPool, 100))
	{
		printf("ManagePool_Socket_Create!@\n");
		return;
	}
	XSOCKET hSocket = INVALID_SOCKET;
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

void CALLBACK fun(void* p)
{
	printf("call fun\n");
}
int MemoryPool_Test()
{
	XCHAR* p1, * p2, * p3, * p4;

	XHANDLE xmPool = ManagePool_Memory_Create();
	p1 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 416); /*分配内存，在可分配内*/
	memcpy(p1, "p1", 2);
	printf("%s\n", p1);

	p2 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 200);/*分配内存，在可分配内，但pool中没有足够的内存空间*/
	memcpy(p2, "p2", 2);
	printf("%s\n", p2);

	ManagePool_Memory_Free(xmPool, p1);
	ManagePool_Memory_Free(xmPool, p2);

	p3 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 624);//分配大内存
	memcpy(p3, "p3", 2);
	printf("%s\n", p3);

	p4 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 100);//分配内存
	memcpy(p4, "p4", 2);
	printf("%s\n", p4);

	XCHAR* p5 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 4000);//分配内存
	memcpy(p5, "p5", 2);
	printf("%s,%s\n", p5, p4);

	XCHAR* p6 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 5000);//分配大内存
	memcpy(p6, "p6", 2);
	printf("%s,%s\n", p6, p4);

	ManagePool_Memory_Free(xmPool, p6);
	XCHAR* p7 = (XCHAR*)ManagePool_Memory_Alloc(xmPool, 5000);//分配大内存
	memcpy(p7, "p7", 2);
	printf("%s,%s,%s\n", p7, p4, p5);

	ManagePool_Memory_CleanupAdd(xmPool, fun);/*注册销毁回调函数*/

	ManagePool_Memory_Destory(xmPool);/*pool池注销*/
	printf("\n");
	return 0;
}
int main()
{
#ifdef _MSC_BUILD
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
#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}

