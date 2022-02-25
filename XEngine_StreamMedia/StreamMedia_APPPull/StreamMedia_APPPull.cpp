#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_XClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_XClient/XClient_Error.h"

//g++ -std=c++17 -Wall -g StreamMedia_APPPull.cpp -o StreamMedia_APPPull.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder -lXEngine_BaseLib -lStreamMedia_XClient -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCoder,--disable-new-dtags

void CBStream_Pull(uint8_t* puszMsgBuffer, int nSize, int nAVType, double dlTime, LPVOID lParam)
{
	printf("Size:%d,AV:%d,Time:%lf\n", nSize, nAVType, dlTime);
	return ;
}
int Test_RTMPPull()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszUrl = _T("http://app.xyry.org:8088/live/qyt.flv");

	if (!XClient_StreamPull_Init(&xhStream, lpszUrl, CBStream_Pull))
	{
		printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	XClient_StreamPull_GetInfo(xhStream, &st_MediaStream);
	XClient_StreamPull_Start(xhStream);
	BOOL bPull = TRUE;

	while (bPull)
	{
		XClient_StreamPull_GetStatus(xhStream, &bPull);
	}
	XClient_StreamPull_Close(xhStream);
	return 1;
}
int main()
{
#ifdef _MSC_BUILD
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	Test_RTMPPull();

#ifdef _MSC_BUILD
	WSACleanup();
#endif
	return 0;
}