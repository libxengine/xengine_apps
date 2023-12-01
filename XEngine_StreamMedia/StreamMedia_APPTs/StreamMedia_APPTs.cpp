#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <winsock2.h>
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
using namespace std;
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_Types.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/HLSProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/HLSProtocol_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_HLSProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_HLSProtocol/HLSProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_HLSProtocol/HLSProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_HLSProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPTs.cpp -o StreamMedia_APPTs.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lNetHelp_APIHelp -lStreamMedia_HLSProtocol -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPRtmp.cpp -o StreamMedia_APPRtmp.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib-lNetHelp_APIHelp -lStreamMedia_HLSProtocol -lXEngine_AVHelp

int M3U8File_Packet()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = _X("D:\\h264 file\\480p.264");
#else
	LPCXSTR lpszVideoFile = _X("480p.264");
#endif
	LPCXSTR lpszDstFile = _X("./live/normal/480p-");
	LPCXSTR lpszRootFile = _X("./live/live.m3u8");
	LPCXSTR lpszLowFile = _X("./live/low/live.m3u8");
	LPCXSTR lpszNormalFile = _X("./live/normal/live.m3u8");
	LPCXSTR lpszHighFile = _X("./live/high/live.m3u8");
	XNETHANDLE xhRoot = 0;
	XNETHANDLE xhLow = 0;
	XNETHANDLE xhNormal = 0;
	XNETHANDLE xhHigh = 0;
	if (!HLSProtocol_M3u8File_Create(&xhRoot, lpszRootFile))
	{
		printf("HLSProtocol_M3u8File_Create:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhLow, lpszLowFile, "www.xyry.org/live/low/live.m3u8", 150000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhNormal, lpszNormalFile, "www.xyry.org/live/normal/live.m3u8", 500000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_M3u8File_AddStream(xhRoot, &xhHigh, lpszHighFile, "www.xyry.org/live/high/live.m3u8", 2000000))
	{
		printf("HLSProtocol_M3u8File_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	HLSProtocol_M3u8File_Delete(xhRoot);
	printf("wandan\n");
	return 0;
}
bool TSFile_Parse()
{
	LPCXSTR lpszClientID = _X("client");
    FILE* pSt_RFile = fopen("D:\\XEngine_StreamMedia\\XEngine_APPClient\\Debug\\1.ts", "rb");
    FILE* pSt_VFile = fopen("D:\\2.h264", "wb");
    FILE* pSt_AFile = fopen("D:\\2.aac", "wb");

	if (NULL == pSt_RFile)
	{
		return false;
	}
	HLSProtocol_TSParse_Init(1);
	HLSProtocol_TSParse_Insert(lpszClientID);

    int nCount = 0;
	XCHAR tszMSGBuffer[2048];
	
	while (true)
	{
		memset(tszMSGBuffer, '\0', sizeof(tszMSGBuffer));

		int nRet = fread(tszMSGBuffer, 1, sizeof(tszMSGBuffer), pSt_RFile);
		if (nRet <= 0)
		{
			break;
		}
		HLSProtocol_TSParse_Send(lpszClientID, tszMSGBuffer, nRet);

        if (HLSProtocol_TSParse_WaitEvent(1, 0))
        {
			int nListCount = 0;
			XENGINE_MANAGEPOOL_TASKEVENT** ppSt_TaskEvent;
			if (HLSProtocol_TSParse_GetPool(1, &ppSt_TaskEvent, &nListCount))
			{
				for (int i = 0; i < nListCount; i++)
				{
                    for (int j = 0; j < ppSt_TaskEvent[i]->nPktCount; j++)
					{
                        XBYTE byAVType = 0;
						int nMSGLen = 0;
						XCHAR* ptszMsgBuffer = (XCHAR *)malloc(XENGINE_MEMORY_SIZE_MAX);
                        if (HLSProtocol_TSParse_Recv(ppSt_TaskEvent[i]->tszClientAddr, ptszMsgBuffer, &nMSGLen, &byAVType))
						{
							if (nMSGLen > 0)
							{
                                if (0x1b == byAVType)
                                {
									fwrite(ptszMsgBuffer, 1, nMSGLen, pSt_VFile);
									nCount += nMSGLen;
									printf("Write:%d %d\n", nCount, nMSGLen);
                                }
                                else if (0x0f == byAVType)
                                {
									fwrite(ptszMsgBuffer, 1, nMSGLen, pSt_AFile);
									nCount += nMSGLen;
									printf("Write:%d %d\n", nCount, nMSGLen);
                                }
							}
						}
                        free(ptszMsgBuffer);
					}
				}
			}
        }
	}
	fclose(pSt_RFile);
    fclose(pSt_VFile);
    fclose(pSt_AFile);
	HLSProtocol_TSParse_Destory();
	return true;
}

bool TSFile_Packet()
{
	XNETHANDLE xhVideo = 0;
	LPCXSTR lpszVideoFile = _X("D:\\h264 file\\1080P.264");
	LPCXSTR lpszClientID = _X("client");
	FILE* pSt_WFile = fopen("D:\\windows-ffmpeg\\x64\\1.ts", "wb");
	FILE* pSt_RVideo = fopen(lpszVideoFile, "rb");

	AVHelp_Parse_FrameInit(&xhVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	HLSProtocol_TSPacket_Insert(lpszClientID, 0x100, 0x1B, 0x0F, 0x101, 0);

	int nMsgLen = 0;
	XCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	HLSProtocol_TSPacket_PATInfo(lpszClientID, (XBYTE *)tszMsgBuffer, &nMsgLen);
	fwrite(tszMsgBuffer, 1, nMsgLen, pSt_WFile);

	nMsgLen = 0;
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	HLSProtocol_TSPacket_PMTInfo(lpszClientID, (XBYTE*)tszMsgBuffer, &nMsgLen);
	fwrite(tszMsgBuffer, 1, nMsgLen, pSt_WFile);

	while (true)
	{
		XCHAR tszVBuffer[2048];
		memset(tszVBuffer, '\0', sizeof(tszVBuffer));

		int nRet = fread(tszVBuffer, 1, sizeof(tszVBuffer), pSt_RVideo);
		if (nRet <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;

		AVHelp_Parse_FrameGet(xhVideo, tszVBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nWLen = 0;
			int nMSGCount = 0;
			XBYTE** ptszMsgBuffer;

			HLSProtocol_TSPacket_AVPacket(lpszClientID, &ptszMsgBuffer, &nMSGCount, 0x101, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen);
			for (int j = 0; j < nMSGCount; j++)
			{
				fwrite(ptszMsgBuffer[j], 1, 188, pSt_WFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ptszMsgBuffer, nMSGCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}

	fclose(pSt_WFile);
	AVHelp_Parse_FrameClose(xhVideo);
	return true;
}

int main()
{
	bool bServer = true;
	if (bServer)
	{
		TSFile_Parse();
	}
	else
	{
		TSFile_Packet();
	}

	return 0;
}
