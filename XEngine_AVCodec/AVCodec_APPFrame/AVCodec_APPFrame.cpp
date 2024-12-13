#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFormat_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFormat.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFormat/AVFormat_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFormat.lib")
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g AVCodec_APPFrame.cpp -o AVCodec_APPFrame.exe -lXEngine_BaseLib -lXEngine_AVFrame -lXEngine_AVFormat

int Test_BITStream()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\h264 file\\output.mp4");
#else
	LPCXSTR lpszFile = _X("output.mp4");
#endif

	XNETHANDLE xhToken = 0;
	XHANDLE xhPacket = AVFormat_UNPack_Init();
	if (!AVFrame_BITStream_Init(&xhToken, "h264_mp4toannexb"))
	{
		printf("AVFrame_BITStream_Init:%lX\n", AVFrame_GetLastError());
		return -1;
	}
	if (!AVFormat_UNPack_Input(xhPacket, lpszFile))
	{
		printf("AVFormat_UNPack_Input:%lX\n", AVFormat_GetLastError());
		return -1;
	}
	XHANDLE pSt_AVCodecParameter = NULL;
	AVFormat_UNPack_GetAVCodec(xhPacket, 0, &pSt_AVCodecParameter);
	AVFrame_BITStream_Parameter(xhToken, pSt_AVCodecParameter, pSt_AVCodecParameter);

	while (1)
	{
		int nAVIndex = 0;
		int nMSGLen = 0;
		XBYTE* ptszMSGBuffer = (XBYTE *)malloc(XENGINE_MEMORY_SIZE_MAX);

		if (!AVFormat_UNPack_Read(xhPacket, &nAVIndex, ptszMSGBuffer, &nMSGLen))
		{
			break;
		}
		if (0 == nAVIndex)
		{
			printf("BIT:%d\n", nMSGLen);
			AVFrame_BITStream_Convert(xhToken, ptszMSGBuffer, nMSGLen);
		}
		free(ptszMSGBuffer);
	}

	AVFormat_UNPack_Stop(xhPacket);
	AVFrame_BITStream_Stop(xhToken);
	return 0;
}
int Test_Frame()
{
#ifdef _MSC_BUILD
	FILE* pSt_File = fopen("D:\\h264 file\\480p.264", "rb");
#else
	FILE* pSt_File = fopen("480p.264", "rb");
#endif
	XNETHANDLE xhParse = 0;
	if (!AVFrame_Frame_ParseInit(&xhParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264))
	{
		printf("AVFrame_Frame_ParseInit:%lX\n", AVFrame_GetLastError());
		return -1;
	}
	
	while (1)
	{
		XCHAR tszBuffer[10240];
		memset(tszBuffer, '\0', sizeof(tszBuffer));
		int nRet = fread(tszBuffer, 1, 10240, pSt_File);
		if (nRet <= 0)
		{
			break;
		}

		int nListCount = 0;
		AVFRAME_PARSEDATA** ppSt_Frame;
		AVFrame_Frame_ParseGet(xhParse, tszBuffer, nRet, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			printf("Frame:%d\n", ppSt_Frame[i]->nMsgLen);
			BaseLib_Memory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	AVFrame_Frame_ParseClose(xhParse);
	return 0;
}
int main()
{
	Test_BITStream();
	Test_Frame();
	
	return 0;
}