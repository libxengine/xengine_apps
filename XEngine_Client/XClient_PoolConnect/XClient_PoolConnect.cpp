#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_ManagePool.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#endif
#include <list>
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_ManagePool/ManagePool_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"

//g++ -std=gnu++17 -Wall -g XClient_PoolConnect.cpp -o XClient_PoolConnect.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -lXEngine_BaseLib -lXClient_Pool -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client,--disable-new-dtags

typedef struct
{
	TCHAR tszServiceAddr[64];
	SOCKET hSocket;
	int nPort;
	int nIndex;
	BOOL bTCP;
	BOOL bRun;
}XCLIENT_CONNECTINFO;
typedef struct
{
	XNETHANDLE xhPool;
	list<XCLIENT_CONNECTINFO*> stl_ConnectInfo;
}XCLIENT_POOLCONNECT;

XHTHREAD CALLBACK XClient_PoolConnect_Thread(LPVOID lParam)
{
	XCLIENT_CONNECTINFO* pSt_ConnectInfo = (XCLIENT_CONNECTINFO*)lParam;

	TCHAR tszMsgBuffer[8096];
	TCHAR tszClientAddr[128];
	while (pSt_ConnectInfo->bRun)
	{
		int nMsgLen = 8096;
		memset(tszMsgBuffer, '\0', nMsgLen);
		memset(tszClientAddr, '\0', sizeof(tszClientAddr));
		//判断连接类型
		if (pSt_ConnectInfo->bTCP)
		{
			//接受TCP数据
			if (!XClient_TCPSelect_RecvMsg(pSt_ConnectInfo->hSocket, tszMsgBuffer, &nMsgLen))
			{
				continue;
			}
		}
		else
		{
			if (!XClient_UDPSelect_RecvMsg(pSt_ConnectInfo->hSocket, tszMsgBuffer, &nMsgLen))
			{
				continue;
			}
		}
        printf("%d,%s\n", nMsgLen, tszMsgBuffer);
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return 0;
}
XCLIENT_POOLCONNECT* XClient_PoolConnect_OPen(LPCTSTR lpszServiceAddr, int nPort, int nCount, BOOL bTCP, int nIPVer)
{
    //申请内存
    XCLIENT_POOLCONNECT* pSt_PoolConnect = new XCLIENT_POOLCONNECT;
    if (NULL == pSt_PoolConnect)
    {
        return FALSE;
    }
    //处理个数
    THREADPOOL_PARAMENT** ppSt_ThreadPool;
    BaseLib_OperatorMemory_Malloc((XPPPMEM)&ppSt_ThreadPool, nCount, sizeof(THREADPOOL_PARAMENT));
    for (int i = 0; i < nCount; i++)
    {
        XCLIENT_CONNECTINFO* pSt_ConnectInfo = new XCLIENT_CONNECTINFO;
        if (NULL == pSt_ConnectInfo)
        {
            delete pSt_ConnectInfo;
            pSt_ConnectInfo = NULL;
            return FALSE;
        }
        //是什么连接类型类型
        if (bTCP)
        {
            if (!XClient_TCPSelect_Create(&pSt_ConnectInfo->hSocket, lpszServiceAddr, nPort, nIPVer))
            {
                delete pSt_ConnectInfo;
                pSt_ConnectInfo = NULL;
                return FALSE;
            }
        }
        else
        {
            if (!XClient_UDPSelect_Create(&pSt_ConnectInfo->hSocket, lpszServiceAddr, nPort, nIPVer))
            {
                delete pSt_ConnectInfo;
                pSt_ConnectInfo = NULL;
                return FALSE;
            }
        }
        //赋值属性
        pSt_ConnectInfo->nIndex = i;
        pSt_ConnectInfo->bTCP = bTCP;
        pSt_ConnectInfo->nPort = nPort;
        _tcscpy(pSt_ConnectInfo->tszServiceAddr, lpszServiceAddr);
        //线程池
        ppSt_ThreadPool[i]->lParam = pSt_ConnectInfo;
        ppSt_ThreadPool[i]->fpCall_ThreadsTask = XClient_PoolConnect_Thread;
        pSt_PoolConnect->stl_ConnectInfo.push_back(pSt_ConnectInfo);
    }

    if (!ManagePool_Thread_NQCreate(&pSt_PoolConnect->xhPool, &ppSt_ThreadPool, nCount))
    {
        return FALSE;
    }
    return pSt_PoolConnect;
}
BOOL XClient_PoolConnect_Send(XCLIENT_POOLCONNECT* pSt_PoolConnect, LPCTSTR lpszMsgBuffer, int nMsgLen, int nIndex /* = -1 */, LPCTSTR lpszClientAddr /* = NULL */)
{
    if (-1 == nIndex)
    {
        //如果索引为-1,那么需要随机选择
        srand((unsigned int)time(NULL));
        nIndex = rand() % pSt_PoolConnect->stl_ConnectInfo.size();
    }
    //查找到指定索引后操作
    list<XCLIENT_CONNECTINFO*>::const_iterator stl_ListIterator = pSt_PoolConnect->stl_ConnectInfo.begin();
    for (; stl_ListIterator != pSt_PoolConnect->stl_ConnectInfo.end(); stl_ListIterator++)
    {
        if (nIndex == (*stl_ListIterator)->nIndex)
        {
            //根据协议类型发送数据
            if ((*stl_ListIterator)->bTCP)
            {
                if (!XClient_TCPSelect_SendMsg((*stl_ListIterator)->hSocket, lpszMsgBuffer, nMsgLen))
                {
                    return FALSE;
                }
            }
            else
            {
                int nPort = 0;
                TCHAR tszClientAddr[64];
                memset(tszClientAddr, '\0', sizeof(tszClientAddr));
                _tcscpy(tszClientAddr, lpszClientAddr);

                BaseLib_OperatorIPAddr_SegAddr(tszClientAddr, &nPort);
                if (!XClient_UDPSelect_SendMsg((*stl_ListIterator)->hSocket, lpszMsgBuffer, nMsgLen, tszClientAddr, nPort))
                {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}
BOOL XClient_PoolConnect_Close(XCLIENT_POOLCONNECT* pSt_PoolConnect)
{
	list<XCLIENT_CONNECTINFO*>::iterator stl_ListIterator = pSt_PoolConnect->stl_ConnectInfo.begin();
	for (; stl_ListIterator != pSt_PoolConnect->stl_ConnectInfo.end(); stl_ListIterator++)
	{
		(*stl_ListIterator)->bRun = FALSE;
		//判断连接类型
		if ((*stl_ListIterator)->bTCP)
		{
			XClient_TCPSelect_Close((*stl_ListIterator)->hSocket);
		}
		else
		{
			XClient_UDPSelect_Close((*stl_ListIterator)->hSocket);
		}

		delete (*stl_ListIterator);
		(*stl_ListIterator) = NULL;
	}
    pSt_PoolConnect->stl_ConnectInfo.clear();

	ManagePool_Thread_NQDestroy(pSt_PoolConnect->xhPool);
	delete pSt_PoolConnect;
    pSt_PoolConnect = NULL;
    return TRUE;
}

int main()
{
    WSADATA st_WSAData;
    WSAStartup(MAKEWORD(2, 2), &st_WSAData);
    XCLIENT_POOLCONNECT* pSt_PoolConnect = XClient_PoolConnect_OPen("192.168.1.12", 5103, 10, TRUE, 2);
	if (NULL == pSt_PoolConnect)
	{
		printf("XClient_PoolConnect_OPen:%X\n", XClient_GetLastError());
		return -1;
	}

	while (1)
	{
		Sleep(1000);
	}
	XClient_PoolConnect_Close(pSt_PoolConnect);
    WSACleanup();
	return 0;
}