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
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFormat.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFormat.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux MacOS:g++ -std=c++20 -Wall -g AVCodec_APPFrame.cpp -o AVCodec_APPFrame.exe -lXEngine_BaseLib -lXEngine_AVFrame -lXEngine_AVFormat -lXEngine_AVHelp

int Test_BITStream()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("D:\\h264 file\\out.mp4");
#else
	LPCXSTR lpszFile = _X("out.mp4");
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
		XHANDLE** ppSt_AVBuffer;
		if (!AVFormat_UNPack_Read(xhPacket, &nAVIndex, &ppSt_AVBuffer))
		{
			break;
		}
		if (0 == nAVIndex)
		{
			int nListCount = 0;
			XHANDLE** ppSt_AVFrame;
			AVFrame_BITStream_Convert(xhToken, ppSt_AVBuffer[0], &ppSt_AVFrame, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				XENGINE_MSGBUFFER st_MSGBuffer = {};
				AVHelp_Memory_GetVideoBuffer(ppSt_AVFrame[0], &st_MSGBuffer, true);

				printf("bitstream:%d\n", st_MSGBuffer.nMSGLen[0]);
				BaseLib_Memory_MSGFree(&st_MSGBuffer);
			}
			AVHelp_Memory_FreeAVList(&ppSt_AVFrame, nListCount);
		}
		AVHelp_Memory_FreeAVList(&ppSt_AVBuffer, 1, true);
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
		XHANDLE** ppSt_AVFrame;
		AVFrame_Frame_ParseGet(xhParse, tszBuffer, nRet, &ppSt_AVFrame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_MSGBUFFER st_MSGBuffer = {};
			AVHelp_Memory_GetVideoBuffer(ppSt_AVFrame[0], &st_MSGBuffer, true);

			printf("Frame:%d\n", st_MSGBuffer.nMSGLen[0]);
			BaseLib_Memory_MSGFree(&st_MSGBuffer);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_AVFrame, nListCount);
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