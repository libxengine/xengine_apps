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

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPPull.cpp -o StreamMedia_APPPull.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_XClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPPull.cpp -o StreamMedia_APPPull.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_XClient

void CBStream_Pull(uint8_t* puszMsgBuffer, int nSize, int nAVType, double dlTime, LPVOID lParam)
{
	//printf("Size:%d,AV:%d,Time:%lf\n", nSize, nAVType, dlTime);
	return ;
}
int Test_RTMPPull()
{
	XNETHANDLE xhStream = 0;
	LPCTSTR lpszPullUrl = _T("rtsp://admin:sihan123@192.168.8.152:554/cam/realmonitor?channel=1?subtype=0");
	LPCTSTR lpszPushUrl = _T("rtmp://app.xyry.org:1935/live/123");
	int nStreamCount = 0;
	STREAMMEDIA_PULLSTREAM** ppSt_PullStream;

	if (!XClient_StreamPull_Init(&xhStream, lpszPullUrl, &ppSt_PullStream, &nStreamCount, CBStream_Pull))
	{
		printf("XClient_FilePush_Push:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	for (int i = 0; i < nStreamCount; i++)
	{
		if (ppSt_PullStream[i]->enStreamType == ENUM_STREAMMEIDA_XCLIENT_STREAM_TYPE_VIDEO)
		{
			ppSt_PullStream[i]->bEnable = TRUE;
		}
		else
		{
			ppSt_PullStream[i]->bEnable = FALSE;
		}
	}
	if (!XClient_StreamPull_PushStream(xhStream, lpszPushUrl, &ppSt_PullStream, nStreamCount))
	{
		printf("XClient_StreamPull_PushStream:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

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