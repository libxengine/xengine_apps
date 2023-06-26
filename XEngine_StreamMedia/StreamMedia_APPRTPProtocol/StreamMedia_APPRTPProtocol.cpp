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

#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_AVCodec/AVCollect_Define.h>
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AudioCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/RTPProtocol_Error.h>
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVCollect/AVCollect_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AudioCodec/AudioCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTPProtocol.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_APPRTPProtocol.cpp -o StreamMedia_APPRTPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lStreamMedia_RTPProtocol -lXEngine_AVHelp -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g StreamMedia_APPRTPProtocol.cpp -o StreamMedia_APPRTPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_StreamMedia -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AVCodec -lXEngine_BaseLib -lStreamMedia_RTPProtocol -lXEngine_AVHelp

void TestPacket_RTP264()
{
	XNETHANDLE xhSsrc = 0;
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264, false))
	{
		printf("errrno");
		return;
	}
	AVHelp_Parse_FrameInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	RTPProtocol_Packet_SetInfo(xhSsrc);

#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\480p.264");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
#else
	LPCXSTR lpsz264File = _X("480p.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
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
		AVHELP_FRAMEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVHelp_Parse_FrameGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(xhSsrc, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_RTPPacket, &nPacketCount);
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
	AVHelp_Parse_FrameClose(xhFrame);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTP264()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264, false))
	{
		printf("errrno");
		return;
	}
	
#ifdef _MSC_BUILD
	LPCXSTR lpsz264File = _X("D:\\h264 file\\480p_1.264");
	LPCXSTR lpszRTPFile = _X("D:\\h264 file\\480p.RTP");
#else
	LPCXSTR lpsz264File = _X("480p_1.264");
	LPCXSTR lpszRTPFile = _X("480p.RTP");
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
		RTPProtocol_Parse_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 1024000;
			XCHAR* ptszMsgBuffer = new XCHAR[nMsgLen];
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			memset(ptszMsgBuffer, '\0', nMsgLen);
			memset(&st_RTPHdr, '\0', sizeof(STREAMMEDIA_RTPPROTOCOL_HDR));

			if (!RTPProtocol_Parse_Recv(xhSsrc, ptszMsgBuffer, &nMsgLen, &st_RTPHdr, NULL, NULL))
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
	RTPProtocol_Parse_Destory(xhSsrc);
}
void TestPacket_RTP265()
{
	XNETHANDLE xhSsrc = 0;
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, false))
	{
		printf("errrno");
		return;
	}
	AVHelp_Parse_FrameInit(&xhFrame, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H265);
	RTPProtocol_Packet_SetInfo(xhSsrc);

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
		AVHELP_FRAMEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVHelp_Parse_FrameGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(xhSsrc, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_RTPPacket, &nPacketCount);
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
	AVHelp_Parse_FrameClose(xhFrame);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTP265()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, false))
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
		RTPProtocol_Parse_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 1024000;
			XCHAR* ptszMsgBuffer = new XCHAR[nMsgLen];
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			memset(ptszMsgBuffer, '\0', nMsgLen);
			memset(&st_RTPHdr, '\0', sizeof(STREAMMEDIA_RTPPROTOCOL_HDR));

			if (!RTPProtocol_Parse_Recv(xhSsrc, ptszMsgBuffer, &nMsgLen, &st_RTPHdr, NULL, NULL))
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
	RTPProtocol_Parse_Destory(xhSsrc);
}
void TestPacket_RTPAAC()
{
	XNETHANDLE xhSsrc = 0;
	XNETHANDLE xhFrame = 0;
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, false))
	{
		printf("errrno");
		return;
	}
	AVHelp_Parse_FrameInit(&xhFrame, ENUM_XENGINE_AVCODEC_AUDIO_TYPE_AAC);
	RTPProtocol_Packet_SetInfo(xhSsrc, 1024, 44100);

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
		AVHELP_FRAMEDATA** ppSt_Frame;
		STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;

		AVHelp_Parse_FrameGet(xhFrame, tszMsgBuffer, nRet, &ppSt_Frame, &nFrameCount);
		for (int i = 0; i < nFrameCount; i++)
		{
			RTPProtocol_Packet_Packet(xhSsrc, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, &ppSt_RTPPacket, &nPacketCount);
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
	AVHelp_Parse_FrameClose(xhFrame);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTPAAC()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, false))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Packet_SetInfo(xhSsrc, 1024, 44100);

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
		RTPProtocol_Parse_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nMsgLen = 1024000;
			XCHAR* ptszMsgBuffer = new XCHAR[nMsgLen];
			STREAMMEDIA_RTPPROTOCOL_HDR st_RTPHdr;

			memset(ptszMsgBuffer, '\0', nMsgLen);
			memset(&st_RTPHdr, '\0', sizeof(STREAMMEDIA_RTPPROTOCOL_HDR));

			if (!RTPProtocol_Parse_Recv(xhSsrc, ptszMsgBuffer, &nMsgLen, &st_RTPHdr, NULL, NULL))
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
	RTPProtocol_Parse_Destory(xhSsrc);
}
int main()
{
	TestPacket_RTP264();
	TestParse_RTP264();

	TestPacket_RTP265();
	TestParse_RTP265();

	TestPacket_RTPAAC();
	TestParse_RTPAAC();
	return 0;
}