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
#include <string>
#include <vector>
#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/ManagePool_Define.h>
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVFrame/AVFrame_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_AVHelp.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVFrame.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g StreamMedia_APPRTPProtocol.cpp -o StreamMedia_APPRTPProtocol.exe -lXEngine_BaseLib -lStreamMedia_RTPProtocol -lXEngine_AVFrame -lXEngine_AVHelp

void TestPacket_RTP264_TCP()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID, false))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	RTPProtocol_Packet_SetTime(lpszClientID, 96, 25);
	RTPProtocol_Packet_SetLink(lpszClientID, 96, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\480p.264");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
#else
	LPCXSTR lpsz264File = _X("480p.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
#endif
	FILE* pSt_264File = fopen(lpsz264File, _X("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("wb"));

	int nCount = 0;
	int nIndex = 0;
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
		XENGINE_MSGBUFFER** ppSt_Frame;
		XENGINE_MSGBUFFER** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, 96, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer + 4, ppSt_Frame[i]->nMSGLen[0] - 4, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", nIndex++, ppSt_RTPPacket[j]->nMSGLen[0]);
				nCount += ppSt_RTPPacket[j]->nMSGLen[0];
				fwrite(ppSt_RTPPacket[j]->unData.tszMSGBuffer, 1, ppSt_RTPPacket[j]->nMSGLen[0], pSt_RTPFile);
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	AVFrame_Frame_ParseClose(xhFrame);
	RTPProtocol_Packet_Delete(lpszClientID);
}
void TestParse_RTP264_TCP()
{
	RTPProtocol_Parse_Init(1);
	LPCXSTR lpszClientID = _X("xhSsrc");
	if (!RTPProtocol_Parse_Insert(lpszClientID, false))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Parse_SetLink(lpszClientID, 96, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);
#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\480p_tcp.264");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
#else
	LPCXSTR lpsz264File = _X("480p_tcp.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
#endif

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[8192];
	int nCount = 0;
	bool bKEYFrame = false;

	while (true)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_RTPFile);
		if (nRet <= 0)
		{
			break;
		}
		if (!RTPProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet))
		{
			printf("error\n");
			continue;
		}
		while (1)
		{
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;
			st_RTPHdr.nPayID = 96;
			if (!RTPProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
				break;
			}
			if (!bKEYFrame)
			{
				XENGINE_AVCODEC_VIDEOFRAMETYPE enFrameType;
				AVHelp_Parse_NaluType(ptszMsgBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enFrameType);
				if ((ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enFrameType))
				{
					bKEYFrame = true;
				}
			}

			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			fflush(pSt_264File);
			printf("%d = %d\n", nCount++, nMsgLen);
			BaseLib_Memory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Parse_Delete(lpszClientID);
	RTPProtocol_Parse_Destory();
}

void TestPacket_RTP264_UDP()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	RTPProtocol_Packet_SetTime(lpszClientID, 96, 25);
	RTPProtocol_Packet_SetLink(lpszClientID, 96, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\480p.264");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
	FILE* pSt_File = fopen("D:\\h264 file\\1.txt", _X("wb"));
#else
	LPCXSTR lpsz264File = _X("480p.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
	FILE* pSt_File = fopen("1.txt", _X("wb"));
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
		XENGINE_MSGBUFFER** ppSt_Frame;
		XENGINE_MSGBUFFER** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, 96, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer + 4, ppSt_Frame[i]->nMSGLen[0] - 4, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", j, ppSt_RTPPacket[j]->nMSGLen[0]);

				char tszFSize[64] = {};
				int nFSize = sprintf(tszFSize, "%d\r\n", ppSt_RTPPacket[j]->nMSGLen[0]);
				fwrite(tszFSize, 1, nFSize, pSt_File);
				fwrite(ppSt_RTPPacket[j]->unData.tszMSGBuffer, 1, ppSt_RTPPacket[j]->nMSGLen[0], pSt_RTPFile);
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	fclose(pSt_File);
	AVFrame_Frame_ParseClose(xhFrame);
	RTPProtocol_Packet_Delete(lpszClientID);
}

void TestParse_RTP264_UDP()
{
	RTPProtocol_Parse_Init(1);
	LPCXSTR lpszClientID = _X("xhSsrc");
	if (!RTPProtocol_Parse_Insert(lpszClientID))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Parse_SetLink(lpszClientID, 96, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264);
	RTPProtocol_Parse_SetLink(lpszClientID, 111, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_OPUS);
#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\XEngine_StreamMedia\\XEngine_Source\\Debug\\480p_1.264");
	LPCXSTR lpszOPUSFile = _X("D:\\XEngine_StreamMedia\\XEngine_Source\\Debug\\opus_1.opus");
	LPCXSTR lpszRTPFile = _X("D:\\XEngine_StreamMedia\\XEngine_Source\\Debug\\1.h264");
	LPCXSTR lpszFile = _X("D:\\XEngine_StreamMedia\\XEngine_Source\\Debug\\1.opus");

	//LPCXSTR lpsz264File = _X("D:\\h264 file\\480p.264");
	//LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
	//LPCXSTR lpszFile = "D:\\h264 file\\1.txt";
#else
	LPCXSTR lpsz264File = _X("480p_1.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
	LPCXSTR lpszFile = _X("1.txt");
#endif

	std::vector<int> stl_ListFSize;
	
	FILE* pSt_File = fopen(lpszFile, _X("rb"));

	char *ptszFileBuffer = (char *)malloc(XENGINE_MEMORY_SIZE_MAX);
	memset(ptszFileBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	int nFLen = fread(ptszFileBuffer, 1, XENGINE_MEMORY_SIZE_MAX, pSt_File);
	
	char* ptszTokStr = strtok(ptszFileBuffer, "\r\n");
	while (NULL != ptszTokStr)
	{
		int nSize = atoi(ptszTokStr);
		stl_ListFSize.push_back(nSize);
		ptszTokStr = strtok(NULL, "\r\n");
	}
	fclose(pSt_File);

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_OPUSFile = fopen(lpszOPUSFile, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[8192];
	int nCount = 0;
	bool bKEYFrame = false;

	for (int i = 0; i < stl_ListFSize.size(); i++)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, stl_ListFSize[i], pSt_RTPFile);
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
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;
			st_RTPHdr.nPayID = 96;
			if (!RTPProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
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
				}
			}
			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			printf("%d %d\n", i, nMsgLen);
			fflush(pSt_264File);
			free(ptszMsgBuffer);
			ptszMsgBuffer = NULL;
		}
		while (1)
		{
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;
			st_RTPHdr.nPayID = 111;
			if (!RTPProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				free(ptszMsgBuffer);
				ptszMsgBuffer = NULL;
				break;
			}
			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_OPUSFile);
			printf("%d %d\n", i, nMsgLen);
			fflush(pSt_OPUSFile);
			free(ptszMsgBuffer);
			ptszMsgBuffer = NULL;
		}
	}
	fclose(pSt_264File);
	fclose(pSt_OPUSFile);
	fclose(pSt_RTPFile);
	RTPProtocol_Parse_Delete(lpszClientID);
	RTPProtocol_Parse_Destory();
}
void TestPacket_RTP265()
{
	LPCXSTR lpszClientID = _X("xhSsrc");
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Insert(lpszClientID))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265);
	RTPProtocol_Packet_SetTime(lpszClientID, 97, 25);
	RTPProtocol_Packet_SetLink(lpszClientID, 97, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\2.hevc");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\hevc.RTP");
	FILE* pSt_File = fopen("D:\\h264 file\\2.txt", _X("wb"));
#else
	LPCXSTR lpsz264File = _X("2.hevc");
	LPCXSTR lpszRTPFile = _X("hevc.RTP");
	FILE* pSt_File = fopen("2.txt", _X("wb"));
#endif
	int nIndex = 0;
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
		XENGINE_MSGBUFFER** ppSt_Frame;
		XENGINE_MSGBUFFER** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			//printf("%d\n", ppSt_Frame[i]->nMsgLen);
			RTPProtocol_Packet_Packet(lpszClientID, 97, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer + 4, ppSt_Frame[i]->nMSGLen[0] - 4, &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d = %d,%d\n", nIndex++, j, ppSt_RTPPacket[j]->nMSGLen[0]);
				char tszFSize[64] = {};
				int nFSize = sprintf(tszFSize, "%d\r\n", ppSt_RTPPacket[j]->nMSGLen[0]);
				fwrite(tszFSize, 1, nFSize, pSt_File);

				fwrite(ppSt_RTPPacket[j]->unData.tszMSGBuffer, 1, ppSt_RTPPacket[j]->nMSGLen[0], pSt_RTPFile);
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
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
	if (!RTPProtocol_Parse_Insert(lpszClientID))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Parse_SetLink(lpszClientID, 97, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\2_1.hevc");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\hevc.RTP");
	LPCXSTR lpszTextFile = "D:\\h264 file\\2.txt";
#else
	LPCXSTR lpsz264File = _X("2_1.hevc");
	LPCXSTR lpszRTPFile = _X("hevc.RTP");
	LPCXSTR lpszTextFile = _X("2.txt");
#endif
	std::vector<int> stl_ListFSize;
	FILE* pSt_File = fopen(lpszTextFile, _X("rb"));

	char* ptszFileBuffer = (char*)malloc(XENGINE_MEMORY_SIZE_MAX);
	memset(ptszFileBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
	int nFLen = fread(ptszFileBuffer, 1, XENGINE_MEMORY_SIZE_MAX, pSt_File);

	char* ptszTokStr = strtok(ptszFileBuffer, "\r\n");
	while (NULL != ptszTokStr)
	{
		int nSize = atoi(ptszTokStr);
		stl_ListFSize.push_back(nSize);
		ptszTokStr = strtok(NULL, "\r\n");
	}
	fclose(pSt_File);

	FILE* pSt_264File = fopen(lpsz264File, _X("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _X("rb"));
	XCHAR tszMsgBuffer[10240];
	int i = 0;

	for (int i = 0; i < stl_ListFSize.size(); i++)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, stl_ListFSize[i], pSt_RTPFile);
		if (nRet <= 0)
		{
			break;
		}
		
		RTPProtocol_Parse_Send(lpszClientID, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer  = NULL;
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr = {};

			memset(ptszMsgBuffer, '\0', nMsgLen);
			st_RTPHdr.nPayID = 97;
			if (!RTPProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				free(ptszMsgBuffer);
				ptszMsgBuffer = NULL;
				break;
			}
			//printf("%d=%d\n", i++, nMsgLen);

			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			free(ptszMsgBuffer);
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
	if (!RTPProtocol_Packet_Insert(lpszClientID, false))
	{
		printf("errrno");
		return;
	}
	AVFrame_Frame_ParseInit(&xhFrame, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	RTPProtocol_Packet_SetTime(lpszClientID, 95, 44100);
	RTPProtocol_Packet_SetLink(lpszClientID, 95, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC);
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
		XENGINE_MSGBUFFER** ppSt_Frame;
		XENGINE_MSGBUFFER** ppSt_RTPPacket;

		AVFrame_Frame_ParseGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(lpszClientID, 95, (LPCXSTR)ppSt_Frame[i]->unData.ptszMSGBuffer, ppSt_Frame[i]->nMSGLen[0], &ppSt_RTPPacket, &nPacketCount);
			for (int j = 0; j < nPacketCount; j++)
			{
				printf("%d=%d\n", j, ppSt_RTPPacket[j]->nMSGLen[0]);
				fwrite(ppSt_RTPPacket[j]->unData.tszMSGBuffer, 1, ppSt_RTPPacket[j]->nMSGLen[0], pSt_RTPFile);
			}
			BaseLib_Memory_Free((XPPPMEM)&ppSt_RTPPacket, nPacketCount);
		}
		BaseLib_Memory_Free((XPPPMEM)&ppSt_Frame, nFrameCount);
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
	if (!RTPProtocol_Parse_Insert(lpszClientID, false))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Parse_SetLink(lpszClientID, 95, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC);
	RTPProtocol_Parse_SetAAC(lpszClientID, 2, 44100);
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
			int nMsgLen = 0;
			XCHAR* ptszMsgBuffer = NULL;
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr = {};

			memset(ptszMsgBuffer, '\0', nMsgLen);
			st_RTPHdr.nPayID = 95;
			if (!RTPProtocol_Parse_Recv(lpszClientID, &ptszMsgBuffer, &nMsgLen, &st_RTPHdr))
			{
				free(ptszMsgBuffer);
				ptszMsgBuffer = NULL;
				break;
			}
			printf("%d\n", i++);
			fwrite(ptszMsgBuffer, 1, nMsgLen, pSt_264File);
			free(ptszMsgBuffer);
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
	//TestPacket_RTP264_TCP();
	//TestParse_RTP264_TCP();

	//TestPacket_RTP264_UDP();
	TestParse_RTP264_UDP();

	//TestPacket_RTP265();
	//TestParse_RTP265();

	TestPacket_RTPAAC();
	TestParse_RTPAAC();
	return 0;
}