#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_StreamClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_StreamClient/StreamClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_StreamClient/StreamClient_Error.h"

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPPull.cpp -o StreamMedia_APPPull.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_StreamClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_SystemSdk:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPPull.cpp -o StreamMedia_APPPull.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_StreamClient

void CBStream_Pull(uint8_t* puszMsgBuffer, int nSize, int nAVType, __int64x nPts, __int64x nDts, __int64x nDuration, double dlTime, XPVOID lParam)
{
	//printf("Size:%d,AV:%d,Time:%lf\n", nSize, nAVType, dlTime);
	return ;
}
int Test_RTMPPull()
{
	LPCXSTR lpszPullUrl = _X("http://app.xyry.org:8088/live/qyt.flv");
	LPCXSTR lpszPushUrl = _X("rtmp://app.xyry.org/live/123");
	int nStreamCount = 0;
	STREAMMEDIA_PULLSTREAM** ppSt_PullStream;

	XHANDLE xhStream = StreamClient_StreamPull_Init(lpszPullUrl, &ppSt_PullStream, &nStreamCount, CBStream_Pull);
	if (NULL == xhStream)
	{
		printf("XStream_FilePush_Push:%lX\n", StreamClient_GetLastError());
		return -1;
	}

	for (int i = 0; i < nStreamCount; i++)
	{
		if (ppSt_PullStream[i]->enStreamType == ENUM_STREAMMEIDA_STREAMCLIENT_STREAM_TYPE_VIDEO)
		{
			ppSt_PullStream[i]->bEnable = true;
		}
		else
		{
			ppSt_PullStream[i]->bEnable = false;
		}
	}
	if (!StreamClient_StreamPull_PushStream(xhStream, lpszPushUrl, &ppSt_PullStream, nStreamCount))
	{
		printf("XStream_StreamPull_PushStream:%lX\n", StreamClient_GetLastError());
		return -1;
	}
	XENGINE_PROTOCOL_AVINFO st_MediaStream;
	memset(&st_MediaStream, '\0', sizeof(XENGINE_PROTOCOL_AVINFO));

	StreamClient_StreamPull_Start(xhStream);
	bool bPull = true;

	while (bPull)
	{
		StreamClient_StreamPull_GetStatus(xhStream, &bPull);
	}
	StreamClient_StreamPull_Close(xhStream);
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