#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_RTPProtocol.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_RTPProtocol/RTPProtocol_Error.h"

//g++ -std=c++17 -Wall -g StreamMedia_APPRTPProtocol.cpp -o StreamMedia_APPRTPProtocol.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia -lXEngine_BaseLib -lStreamMedia_RTPProtocol -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_StreamMedia,--disable-new-dtags

void TestPacket_RTP264()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264, FALSE))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Packet_SetInfo(xhSsrc);

	LPCTSTR lpsz264File = _T("480p.264");
	LPCTSTR lpszRTPFile = _T("480p.RTP");

	FILE* pSt_264File = fopen(lpsz264File, _T("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("wb"));
	TCHAR tszMsgBuffer[10240];

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		RTPProtocol_Packet_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nListCount = 0;
			STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;
			if (!RTPProtocol_Packet_Recv(xhSsrc, &ppSt_RTPPacket, &nListCount))
			{
				break;
			}
			for (int i = 0; i < nListCount; i++)
			{
				printf("%d=%d\n", i, ppSt_RTPPacket[i]->nMsgLen);
				fwrite(ppSt_RTPPacket[i]->tszMsgBuffer, 1, ppSt_RTPPacket[i]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nListCount);
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTP264()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H264, FALSE))
	{
		printf("errrno");
		return;
	}
	LPCTSTR lpsz264File = _T("480p_1.264");
	LPCTSTR lpszRTPFile = _T("480p.RTP");

	FILE* pSt_264File = fopen(lpsz264File, _T("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("rb"));
	TCHAR tszMsgBuffer[10240];
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
			TCHAR* ptszMsgBuffer = new TCHAR[nMsgLen];
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
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, FALSE))
	{
		printf("errrno");
		return;
	}

	RTPProtocol_Packet_SetInfo(xhSsrc);
	LPCTSTR lpsz264File = _T("2.hevc");
	LPCTSTR lpszRTPFile = _T("hevc.RTP");

	FILE* pSt_264File = fopen(lpsz264File, _T("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("wb"));
	TCHAR tszMsgBuffer[10240];

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		RTPProtocol_Packet_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nListCount = 0;
			STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;
			if (!RTPProtocol_Packet_Recv(xhSsrc, &ppSt_RTPPacket, &nListCount))
			{
				break;
			}
			for (int i = 0; i < nListCount; i++)
			{
				printf("%d=%d\n", i, ppSt_RTPPacket[i]->nMsgLen);
				fwrite(ppSt_RTPPacket[i]->tszMsgBuffer, 1, ppSt_RTPPacket[i]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nListCount);
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTP265()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_H265, FALSE))
	{
		printf("errrno");
		return;
	}
	LPCTSTR lpsz264File = _T("2_1.hevc");
	LPCTSTR lpszRTPFile = _T("hevc.RTP");

	FILE* pSt_264File = fopen(lpsz264File, _T("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("rb"));
	TCHAR tszMsgBuffer[10240];
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
			TCHAR* ptszMsgBuffer = new TCHAR[nMsgLen];
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
	if (!RTPProtocol_Packet_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, FALSE))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Packet_SetInfo(xhSsrc, 1024, 44100);

	LPCTSTR lpszAACFile = _T("test.aac");
	LPCTSTR lpszRTPFile = _T("test.RTP");

	FILE* pSt_264File = fopen(lpszAACFile, _T("rb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("wb"));
	TCHAR tszMsgBuffer[10240];

	while (1)
	{
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, sizeof(tszMsgBuffer), pSt_264File);
		if (nRet <= 0)
		{
			break;
		}
		RTPProtocol_Packet_Send(xhSsrc, tszMsgBuffer, nRet);
		while (1)
		{
			int nListCount = 0;
			STREAMMEDIA_RTPPROTOCOL_PACKET** ppSt_RTPPacket;
			if (!RTPProtocol_Packet_Recv(xhSsrc, &ppSt_RTPPacket, &nListCount))
			{
				break;
			}
			for (int i = 0; i < nListCount; i++)
			{
				printf("%d=%d\n", i, ppSt_RTPPacket[i]->nMsgLen);
				fwrite(ppSt_RTPPacket[i]->tszMsgBuffer, 1, ppSt_RTPPacket[i]->nMsgLen, pSt_RTPFile);
			}
			BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_RTPPacket, nListCount);
		}
	}
	fclose(pSt_264File);
	fclose(pSt_RTPFile);
	RTPProtocol_Packet_Destory(xhSsrc);
}
void TestParse_RTPAAC()
{
	XNETHANDLE xhSsrc = 0;
	if (!RTPProtocol_Parse_Init(&xhSsrc, ENUM_STREAMMEDIA_RTPPROTOCOL_PAYLOAD_TYPE_AAC, FALSE))
	{
		printf("errrno");
		return;
	}
	RTPProtocol_Packet_SetInfo(xhSsrc, 1024, 44100);
	LPCTSTR lpsz264File = _T("test_1.aac");
	LPCTSTR lpszRTPFile = _T("test.RTP");

	FILE* pSt_264File = fopen(lpsz264File, _T("wb"));
	FILE* pSt_RTPFile = fopen(lpszRTPFile, _T("rb"));
	TCHAR tszMsgBuffer[10240];
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
			TCHAR* ptszMsgBuffer = new TCHAR[nMsgLen];
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