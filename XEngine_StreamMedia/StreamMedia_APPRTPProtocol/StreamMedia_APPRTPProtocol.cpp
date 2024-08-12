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
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVFrame_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVFrame.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRTPProtocol.cpp -o StreamMedia_APPRTPProtocol.exe -lXEngine_BaseLib -lStreamMedia_RTPProtocol -lXEngine_AVFrame -lXEngine_AVHelp

void TestPacket_RTP264()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	RTPProtocol_Packet_SetTime(lpszClientID, 25);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\720x480.264");
	LPCXSTR lpszRTPFile = _X("D:\\xengine_apps\\Debug\\480p.RTP");
#else
	LPCXSTR lpsz264File = _X("480p.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
#endif
	FILE* pSt_264File = fopen(lpsz264File, _X("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("wb"));

	FILE* pSt_File = fopen("D:\\xengine_apps\\Debug\\1.txt", _X("wb"));

	XCHAR tszMsgBuffer[10240];
	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		int nFrameCount = 0;
		int nPacketCount = 0;
		AVFRAME_PARSEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer + 4, ppSt_Frame[i]->nMsgLen - 4, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", j, ppSt_RTPPacket[j]->nMsgLen);

				char tszFSize[64] = {};
				int nFSize = sprintf(tszFSize, "%d\r\n", ppSt_RTPPacket[j]->nMsgLen);
				fwrite(tszFSize, 1, nFSize, pSt_File);
				fwrite(ppSt_RTPPacket[j]->tszMsgBuffer, 1, ppSt_RTPPacket[j]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	fclose(pSt_File);
	AVFrame_Frame_ParseClose(xhFrame);
	RTPProtocol_Packet_Delete(lpszClientID);
}

#include <string>
#include <vector>
void TestParse_RTP264()
{
	RTPProtocol_Parse_Init(1);
	LPCXSTR lpszClientID = _X("xhSsrc");
	if (!RTPProtocol_Parse_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264))
	{
		printf("errrno");
		return;
	}
	
#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\xengine_apps\\Debug\\480p_1.264");
	LPCXSTR lpszRTPFile = _X("D:\\xengine_apps\\Debug\\recv.rtp");
#else
	LPCXSTR lpsz264File = _X("480p_1.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
#endif

	std::vector<int> stl_ListFSize;
	LPCXSTR lpszFile = _X("D:\\xengine_apps\\Debug\\1.txt");
	FILE* pSt_File = fopen(lpszFile, _X("rb"));

	char *ptszFileBuffer = (char *)malloc(XENGINE_MEMORY_SIZE_MAX);
	memset(ptszFileBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	int nFLen = fread(ptszFileBuffer, 1, XENGINE_MEMORY_SIZE_MAX, pSt_File);
	
	char* ptszTokStr = strtok(ptszFileBuffer, "\r\n");
	while (NULL != ptszTokStr)
	{
		int nSize = _ttoi(ptszTokStr);
		stl_ListFSize.push_back(nSize);
		ptszTokStr = strtok(NULL, "\r\n");
	}
	fclose(pSt_File);

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[8192];
	int i = 0;
	int nCount = 0;
	bool bKEYFrame = false;

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, stl_ListFSize[i++], pSt_RTPFile);
		if (nRet <= 0)
		{
			break;
		}
		nCount += nRet;
		if (!RTPProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet))
		{
			printf("error\n");
			continue;
		}
		while (1)
		{
			int nMsgLen = XENGINE_MEMORY_SIZE_MAX;
			XCHAR* ptszMsgBuffer = (XCHAR*)malloc(nMsgLen);
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			if (!RTPProtocol_Parse_Recv(lpszClientID, ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				free(ptszMsgBuffer);
				ptszMsgBuffer = NULL;
				break;
			}
			if (!bKEYFrame)
			{
				XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;
				AVHelp_Parse_NaluType(ptszMsgBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
				if ((ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enFrameType))
				{
					bKEYFrame = true;
					printf("ok\n");
				}
			}

			if (bKEYFrame)
			{
				fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
				//printf("%d %d\n", i, nMsgLen);
				fflush(pSt_264File);
			}
			
			free(ptszMsgBuffer);
			ptszMsgBuffer = NULL;
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Parse_Delete(lpszClientID);
	RTPProtocol_Parse_Destory();
}
void TestPacket_RTP265()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, false))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265);
	RTPProtocol_Packet_SetTime(lpszClientID, 25);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\2.hevc");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\hevc.RTP");
#else
	LPCXSTR lpsz264File = _X("2.hevc");
	LPCXSTR lpszRTPFile = _X("hevc.RTP");
#endif

	FILE* pSt_264File = fopen(lpsz264File, _X("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("wb"));
	XCHAR tszMsgBuffer[10240];

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		int nFrameCount = 0;
		int nPacketCount = 0;
		AVFRAME_PARSEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", j, ppSt_RTPPacket[j]->nMsgLen);
				fwrite(ppSt_RTPPacket[j]->tszMsgBuffer, 1, ppSt_RTPPacket[j]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	AVFrame_Frame_ParseClose(xhFrame);
	RTPProtocol_Packet_Delete(lpszClientID);
}
void TestParse_RTP265()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	RTPProtocol_Parse_Init(1);
	if (!RTPProtocol_Parse_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, false))
	{
		printf("errrno");
		return;
	}

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\2_1.hevc");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\hevc.RTP");
#else
	LPCXSTR lpsz264File = _X("2_1.hevc");
	LPCXSTR lpszRTPFile = _X("hevc.RTP");
#endif

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[10240];
	int i = 0;

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_RTPFile);
		if (nRet <= 0)
		{
			break;
		}
		RTPProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 1024000;
			XCHAR* ptszMsgBuffer = new XCHAR[nMsgLen];
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			memset(ptszMsgBuffer, '\0', nMsgLen);
			memset(&st_RTPHdr, '\0', sizeof(STREAMMEDIA_RTPPROTOCOL_HDR));

			if (!RTPProtocol_Parse_Recv(lpszClientID, ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				delete[] ptszMsgBuffer;
				ptszMsgBuffer = NULL;
				break;
			}
			printf("%d=%d\n", i++, nMsgLen);
			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			delete[] ptszMsgBuffer;
			ptszMsgBuffer = NULL;
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Parse_Delete(lpszClientID);
	RTPProtocol_Parse_Destory();
}
void TestPacket_RTPAAC()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, false))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	RTPProtocol_Packet_SetTime(lpszClientID, 44100);

#ifdef _MSC_BUILD
	LPCXSTR lpszAACFile = _X("D:\\h264 file\\test.aac");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\test.RTP");
#else
	LPCXSTR lpszAACFile = _X("test.aac");
	LPCXSTR lpszRTPFile = _X("test.RTP");
#endif

	FILE* pSt_264File = fopen(lpszAACFile, _X("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("wb"));
	XCHAR tszMsgBuffer[10240];

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		int nFrameCount = 0;
		int nPacketCount = 0;
		AVFRAME_PARSEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", j, ppSt_RTPPacket[j]->nMsgLen);
				fwrite(ppSt_RTPPacket[j]->tszMsgBuffer, 1, ppSt_RTPPacket[j]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	AVFrame_Frame_ParseClose(xhFrame);
	RTPProtocol_Packet_Delete(lpszClientID);
}
void TestParse_RTPAAC()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	RTPProtocol_Parse_Init(1);
	if (!RTPProtocol_Parse_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, false))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Packet_SetTime(lpszClientID, 44100);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\test_1.aac");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\test.RTP");
#else
	LPCXSTR lpsz264File = _X("test_1.aac");
	LPCXSTR lpszRTPFile = _X("test.RTP");
#endif

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[10240];
	int i = 0;

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_RTPFile);
		if (nRet <= 0)
		{
			break;
		}
		RTPProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 1024000;
			XCHAR* ptszMsgBuffer = new XCHAR[nMsgLen];
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			memset(ptszMsgBuffer, '\0', nMsgLen);
			memset(&st_RTPHdr, '\0', sizeof(STREAMMEDIA_RTPPROTOCOL_HDR));

			if (!RTPProtocol_Parse_Recv(lpszClientID, ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				delete[] ptszMsgBuffer;
				ptszMsgBuffer = NULL;
				break;
			}
			printf("%d\n", i++);
			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			delete[] ptszMsgBuffer;
			ptszMsgBuffer = NULL;
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Parse_Delete(lpszClientID);
	RTPProtocol_Parse_Destory();
}
int main()
{
	//TestPacket_RTP264();
	TestParse_RTP264();

	TestPacket_RTP265();
	TestParse_RTP265();

	TestPacket_RTPAAC();
	TestParse_RTPAAC();
	return 0;
}