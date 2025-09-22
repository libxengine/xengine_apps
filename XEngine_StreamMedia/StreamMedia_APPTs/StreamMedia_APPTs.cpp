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
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_HLSProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/StreamMedia_HLSProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_HLSProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g StreamMedia_APPTs.cpp -o StreamMedia_APPTs.exe -lXEngine_BaseLib -lNetHelp_APIHelp -lStreamMedia_HLSProtocol -lXEngine_AVFrame

int M3U8File_Packet()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszRootFile = _X("D:\\xengine_apps\\Debug\\live\\root.m3u8");
	LPCXSTR lpszLowFile = _X("D:\\xengine_apps\\Debug\\live\\low\\live.m3u8");
	LPCXSTR lpszNormalFile = _X("D:\\xengine_apps\\Debug\\live\\normal\\live.m3u8");
#else
	LPCXSTR lpszRootFile = _X("./live/root.m3u8");
	LPCXSTR lpszLowFile = _X("./live/low/live.m3u8");
	LPCXSTR lpszNormalFile = _X("./live/normal/live.m3u8");
#endif

	XNETHANDLE xhRoot = 0;
	XNETHANDLE xhLow = 0;
	XNETHANDLE xhNormal = 0;

	if (!HLSProtocol_M3u8Packet_Create(&xhRoot, lpszRootFile))
	{
		printf("HLSProtocol_M3u8Packet_Create:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}

	if (!HLSProtocol_M3u8Packet_AddStream(xhRoot, &xhLow, lpszLowFile, true, 15, 100, 40000, 1, "www.xyry.org/live/low/live.m3u8"))
	{
		printf("HLSProtocol_M3u8Packet_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	if (!HLSProtocol_M3u8Packet_AddStream(xhRoot, &xhNormal, lpszNormalFile, true, 15, 100, 40000, 1, "www.xyry.org/live/normal/live.m3u8"))
	{
		printf("HLSProtocol_M3u8Packet_AddStream:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	HLSProtocol_M3u8Packet_AddFile(xhRoot, xhNormal, "1.ts", 233, false);
	HLSProtocol_M3u8Packet_AddFile(xhRoot, xhNormal, "2.ts", 233, false);
	HLSProtocol_M3u8Packet_AddFile(xhRoot, xhNormal);

	//HLSProtocol_M3u8Packet_Delete(xhRoot, xhNormal);
	//HLSProtocol_M3u8Packet_Delete(xhRoot);
	printf("wandan\n");
	return 0;
}
int M3U8File_Parse()
{
	XNETHANDLE xhToken = 0;
	if (!HLSProtocol_M3U8Parse_Create(&xhToken))
	{
		printf("HLSProtocol_M3U8Parse_Create:%lX\n", HLSProtocol_GetLastError());
		return -1;
	}
	bool bStream = false;
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\xengine_apps\\Debug\\live\\root.m3u8");
	LPCXSTR lpszFileInfo = _X("D:\\xengine_apps\\Debug\\live\\normal\\live.m3u8");
#else
	LPCXSTR lpszFile = _X("./live/root.m3u8");
	LPCXSTR lpszFileInfo = _X("./live/normal/live.m3u8");
#endif
	HLSProtocol_M3U8Parse_ReadStream(xhToken, &bStream, lpszFile);

	XBYTE byVersion = 0;
	int nListCount = 0;
	HLSPROTOCOL_M3U8HDR** ppSt_ListFile;
	HLSProtocol_M3U8Parse_GetStream(xhToken, &byVersion, &ppSt_ListFile, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d:%s\n", ppSt_ListFile[i]->nBandwidth, ppSt_ListFile[i]->tszStreamAddr);
	}
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListFile, nListCount);

	HLSPROTOCOL_M3U8INFO st_M3u8Info = {};
	HLSProtocol_M3U8Parse_ReadLive(xhToken, &st_M3u8Info, lpszFileInfo);

	while (true)
	{
		double dlTime = 0;
		XCHAR tszStreamAddr[XPATH_MAX] = {};
		if (HLSProtocol_M3U8Parse_GetLive(xhToken, tszStreamAddr, &dlTime))
		{
			printf("%s %lf\n", tszStreamAddr, dlTime);
		}
		else
		{
			break;
		}
	}
	return 0;
}

bool TSFile_Parse()
{
	LPCXSTR lpszClientID = _X("client");
#ifdef _MSC_BUILD
	FILE* pSt_RFile = fopen("D:\\h264 file\\480p.ts", "rb");
	FILE* pSt_VFile = fopen("D:\\h264 fileD:\\2.h264", "wb");
	FILE* pSt_AFile = fopen("D:\\h264 fileD:\\2.aac", "wb");
#else
	FILE* pSt_RFile = fopen("480p.ts", "rb");
	FILE* pSt_VFile = fopen("2.h264", "wb");
	FILE* pSt_AFile = fopen("2.aac", "wb");
#endif

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
									printf("Write Video:%d %d\n", nCount, nMSGLen);
                                }
                                else if (0x0f == byAVType)
                                {
									fwrite(ptszMsgBuffer, 1, nMSGLen, pSt_AFile);
									nCount += nMSGLen;
									printf("Write Audio:%d %d\n", nCount, nMSGLen);
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

#ifdef _MSC_BUILD
	LPCXSTR lpszVideoFile = _X("D:\\h264 file\\480p.264");
	FILE* pSt_WFile = fopen("D:\\h264 file\\480p.ts", "wb");
#else
	LPCXSTR lpszVideoFile = _X("./480p.264");
	FILE* pSt_WFile = fopen("./480p.ts", "wb");
#endif

	LPCXSTR lpszClientID = _X("client");
	FILE* pSt_RVideo = fopen(lpszVideoFile, "rb");

	AVFrame_Frame_ParseInit(&xhVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	HLSProtocol_TSPacket_Insert(lpszClientID, 0x1000, 0x1B, 0x0F, 0x101, 0);
	HLSProtocol_TSPacket_SetTime(lpszClientID, 24, 0);
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
		XENGINE_MSGBUFFER** ppSt_Frame;

		AVFrame_Frame_ParseGet(xhVideo, tszVBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			int nMSGCount = 0;
			XBYTE** ptszMsgBuffer;

			HLSProtocol_TSPacket_AVPacketTS(lpszClientID, &ptszMsgBuffer, &nMSGCount, 0x101, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer, ppSt_Frame[i]->nMSGLen);
			for (int j = 0; j < nMSGCount; j++)
			{
				fwrite(ptszMsgBuffer[j], 1, 188, pSt_WFile);
			}
			BaseLib_Memory_Free((XPPPMEM)&ptszMsgBuffer, nMSGCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}

	fclose(pSt_WFile);
	AVFrame_Frame_ParseClose(xhVideo);
	return true;
}

int main()
{
	M3U8File_Packet();
	M3U8File_Parse();

	TSFile_Packet();
	TSFile_Parse();
	return 0;
}
