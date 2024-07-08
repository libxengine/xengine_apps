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
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_VideoCodec.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_VideoCodec.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#endif
#endif

//Linux MacOS:g++ -std=c++17 -Wall -g AVCodec_APPVideo.cpp -o AVCodec_APPVideo.exe -lXEngine_BaseLib -lXEngine_VideoCodec 

XNETHANDLE xhDeVideo;
XNETHANDLE xhEnVideo;
XNETHANDLE xhParse;
FILE* pSt_YUVFile;
FILE* pSt_264File;

int Test_H265Hevc()
{
#ifdef _MSC_BUILD
	FILE* pSt_File = _tfopen("d:\\1.hevc", "rb");
	pSt_YUVFile = _tfopen("d:\\ds.yuv", "wb");
#else
	FILE* pSt_File = fopen("1.hevc", "rb");
	pSt_YUVFile = fopen("ds.yuv", "wb");
#endif
	AVFrame_Frame_ParseInit(&xhParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265);
	if (!VideoCodec_Stream_DeInit(&xhDeVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265))
	{
		printf("VideoCodec_Stream_DeInit\n");
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
			int nVideoCount = 0;
			AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
			if (VideoCodec_Stream_DeCodec(xhDeVideo, ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_MSGBuffer, &nVideoCount))
			{
				for (int j = 0; j < nVideoCount; j++)
				{
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_MSGBuffer[j]->ptszYBuffer);
				}
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nVideoCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	VideoCodec_Stream_Destroy(xhDeVideo);
	AVFrame_Frame_ParseClose(xhParse);
	return 0;
}
int Test_Codech264()
{
#ifdef _MSC_BUILD
	FILE* pSt_File = _tfopen("D:\\h264 file\\720x480.264", "rb");
	pSt_264File = fopen("D:\\h264 file\\en.h264", "wb");
	pSt_YUVFile = _tfopen("D:\\h264 file\\ds.yuv", "wb");
#else
	FILE* pSt_File = fopen("480p.264", "rb");
	pSt_264File = fopen("en.h264", "wb");
	pSt_YUVFile = fopen("ds.yuv", "wb");
#endif
	AVCODEC_VIDEO_INFO st_VideoInfo;
	memset(&st_VideoInfo, '\0', sizeof(AVCODEC_VIDEO_INFO));

	st_VideoInfo.nWidth = 720;
	st_VideoInfo.nHeight = 480;
	st_VideoInfo.nBitRate = 400000;
	st_VideoInfo.nFrameRate = 24;
	st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264;

	AVFrame_Frame_ParseInit(&xhParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	if (!VideoCodec_Stream_EnInit(&xhEnVideo, &st_VideoInfo))
	{
		printf("VideoCodec_Stream_EnInit\n");
		return -1;
	}
	if (!VideoCodec_Stream_DeInit(&xhDeVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264))
	{
		printf("VideoCodec_Stream_DeInit\n");
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
			int nDecodecCount = 0;
			AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
			VideoCodec_Stream_DeCodec(xhDeVideo, ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_MSGBuffer, &nDecodecCount);
			for (int j = 0; j < nDecodecCount; j++)
			{
				fwrite(ppSt_MSGBuffer[j]->ptszYBuffer, 1, ppSt_MSGBuffer[j]->nYLen, pSt_YUVFile);

				int nEncoedcCount = 0;
				AVCODEC_VIDEO_MSGBUFFER** ppSt_EncoedcBuffer;
				VideoCodec_Stream_EnCodec(xhEnVideo, ppSt_MSGBuffer[j]->ptszYBuffer, NULL, NULL, ppSt_MSGBuffer[j]->nYLen, 0, 0, &ppSt_EncoedcBuffer, &nEncoedcCount);
				for (int k = 0; k < nEncoedcCount; k++)
				{
					fwrite(ppSt_EncoedcBuffer[k]->ptszYBuffer, 1, ppSt_EncoedcBuffer[k]->nYLen, pSt_264File);
					BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_MSGBuffer[j]->ptszYBuffer);
				}
				BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_EncoedcBuffer[j]->ptszYBuffer);
				BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_EncoedcBuffer, nEncoedcCount);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_MSGBuffer, nDecodecCount);
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	fclose(pSt_File);
	fclose(pSt_264File);
	VideoCodec_Stream_Destroy(xhDeVideo);
	VideoCodec_Stream_Destroy(xhEnVideo);
	AVFrame_Frame_ParseClose(xhParse);
	return 0;
}
int Test_CodechAVS()
{
#ifdef _MSC_BUILD
	FILE* pSt_File = _tfopen("D:\\windows-ffmpeg\\x86\\hunan_avs.es", "rb");
	pSt_YUVFile = _tfopen("d:\\ds.yuv", "wb");
#else
	FILE* pSt_File = fopen("output.avs", "rb");
	pSt_YUVFile = fopen("ds.yuv", "wb");
#endif
	AVCODEC_VIDEO_INFO st_VideoInfo;
	memset(&st_VideoInfo, '\0', sizeof(AVCODEC_VIDEO_INFO));
	st_VideoInfo.nWidth = 1920;
	st_VideoInfo.nHeight = 1080;
	st_VideoInfo.nBitRate = 400000;
	st_VideoInfo.nFrameRate = 24;
	st_VideoInfo.enAVCodec = ENUM_XENGINE_AVCODEC_VIDEO_TYPE_AVS2;

	if (!VideoCodec_Stream_EnInit(&xhEnVideo, &st_VideoInfo))
	{
		printf("VideoCodec_Stream_EnInit\n");
		//return -1;
	}

	AVFrame_Frame_ParseInit(&xhParse, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_AVS);
	if (!VideoCodec_Stream_DeInit(&xhDeVideo, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_AVS))
	{
		printf("VideoCodec_Stream_DeInit\n");
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
			int nListCount = 0;
			AVCODEC_VIDEO_MSGBUFFER** ppSt_MSGBuffer;
			VideoCodec_Stream_DeCodec(xhDeVideo, ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_MSGBuffer, &nListCount);
			for (int i = 0; i < nListCount; i++)
			{
				fwrite(ppSt_MSGBuffer[i]->ptszYBuffer, 1, ppSt_MSGBuffer[i]->nYLen, pSt_YUVFile);
				fwrite(ppSt_MSGBuffer[i]->ptszUBuffer, 1, ppSt_MSGBuffer[i]->nULen, pSt_YUVFile);
				fwrite(ppSt_MSGBuffer[i]->ptszVBuffer, 1, ppSt_MSGBuffer[i]->nVLen, pSt_YUVFile);
			}
			BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ppSt_Frame[i]->ptszMsgBuffer);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	VideoCodec_Stream_Destroy(xhDeVideo);
	AVFrame_Frame_ParseClose(xhParse);
	return 0;
}
int main()
{
	Test_Codech264();
	Test_CodechAVS();
	Test_H265Hevc();
	int nListCount = 0;
	AVCODEC_VIDEO_HWCODEC** ppSt_ListHWCodec;
	VideoCodec_Help_GetHWCodec(&ppSt_ListHWCodec, &nListCount);
	for (int i = 0; i < nListCount; i++)
	{
		printf("%d = %s\n", ppSt_ListHWCodec[i]->enHWDevice, ppSt_ListHWCodec[i]->tszHWName);
	}
	
	return 0;
}