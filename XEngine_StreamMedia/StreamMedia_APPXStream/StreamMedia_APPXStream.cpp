#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XStream.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XStream/XStream_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XStream/XStream_Error.h"

//g++ -std=c++17 -Wall -g StreamMedia_APPXStream.cpp -o StreamMedia_APPXStream.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_XStream -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags


int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif

	XNETHANDLE xhNet = 111;
	if (!XStream_ClientPush_Init(&xhNet, "127.0.0.1", 1931))
	{
		printf("XStream_ClientPush_Init %lX\n", XStream_GetLastError());
		return -1;
	}
	XENGINE_AVPROTOCOL st_AVProtocol;
	memset(&st_AVProtocol, '\0', sizeof(XENGINE_AVPROTOCOL));
	if (!XStream_ClientPush_PushHdr(xhNet,&st_AVProtocol))
	{
		printf("XStream_ClientPush_PushHdr %lX\n", XStream_GetLastError());
		return -1;
	}

	FILE* pSt_File = fopen("1080P.264", "rb");

	int i = 0;
	while (1)
	{
		TCHAR tszMsgBuffer[10240];
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		if (XStream_ClientPush_PushPkg(xhNet, tszMsgBuffer, nRet))
		{
			printf("%d==%d\n", nRet, i++);
		}
		int nSleepTime = 0;
		XStream_ClientPush_GetSpeed(xhNet, &nSleepTime);
		std::this_thread::sleep_for(std::chrono::milliseconds(nSleepTime));
	}
	XStream_ClientPush_Close(xhNet);
	
#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}