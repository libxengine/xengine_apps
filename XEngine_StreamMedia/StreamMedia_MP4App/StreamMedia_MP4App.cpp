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
#include <XEngine_Include/XEngine_AVCodec/VideoCodec_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Define.h>
#include <XEngine_Include/XEngine_AVCodec/AVHelp_Error.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_AVCodec/XEngine_AVHelp.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_MP4Protocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_VideoCodec/VideoCodec_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AVCodec/XEngine_AVHelp/AVHelp_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AVHelp.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_MP4Protocol.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_MP4App.cpp -o StreamMedia_MP4App.exe -lXEngine_BaseLib -lStreamMedia_MP4Protocol -XEngine_AVHelp

int MP4_Parse()
{
	LPCXSTR lpszClientID = _X("127777");
	FILE* pSt_File = _xtfopen(_X("D:\\h264 file\\720x480.mp4"), _X("rb"));
	if (NULL == pSt_File)
	{
		return -1;
	}

	if (!MP4Protocol_Parse_Init(1))
	{
		printf("MP4Protocol_Parse_Init:%lX\n", MP4Protocol_GetLastError());
	}
	if (!MP4Protocol_Parse_Insert(lpszClientID))
	{
		printf("MP4Protocol_Parse_Insert:%lX\n", MP4Protocol_GetLastError());
	}

	__int64u nFCOunt = 0;
	while (true)
	{
		XCHAR tszMSGBuffer[1024];
		fseek(pSt_File, nFCOunt, SEEK_SET);
		int nRet = fread(tszMSGBuffer, 1, sizeof(tszMSGBuffer), pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		MP4Protocol_Parse_Send(lpszClientID, tszMSGBuffer, nRet);

		bool bDPos = false;
		bool bBreak = false;
		__int64u nFPos = 0;
		XENGINE_MP4HDR st_MP4Hdr = {};
		while (true)
		{
			if (!MP4Protocol_Parse_Recv(lpszClientID, &st_MP4Hdr, &nFPos))
			{
				break;
			}
			if (0 == st_MP4Hdr.nHDRSize)
			{
				bBreak = true;
				break;
			}
		}
		if (bBreak)
		{
			break;
		}
		if (st_MP4Hdr.enHType == ENUM_XENGINE_STREAMMEDIA_MP4PROTCOL_HTYPE_MDAT)
		{
			nFCOunt = nFPos;
		}
		else
		{
			nFCOunt += nRet;
		}
	}

	MP4Protocol_Parse_Delete(lpszClientID);
	MP4Protocol_Parse_Destory();
	return 0;
}
int MP4_Packet()
{
	XNETHANDLE xhToken = 0;
	LPCXSTR lpszClientID = _X("127777");

	FILE* pSt_RFile = _xtfopen(_X("D:\\h264 file\\720x480.264"), _X("rb"));
	if (NULL == pSt_RFile)
	{
		return -1;
	}
	FILE* pSt_WFile = _xtfopen(_X("D:\\h264 file\\720x480.mp4"), _X("wb"));
	if (NULL == pSt_WFile)
	{
		return -1;
	}
	
	MP4Protocol_Packet_Insert(lpszClientID);
	AVHelp_Parse_FrameInit(&xhToken, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264);
	
	int nRVLen = 0;
	int nSDLen = 0;
	__int64x nMDatPos = 0;
	__int64x nFilePos = 0;
	XCHAR tszRVBuffer[2048] = {};
	XCHAR *ptszSDBuffer = (XCHAR *)malloc(XENGINE_MEMORY_SIZE_MAX);
	MP4Protocol_Packet_FTyp(lpszClientID, tszRVBuffer, &nRVLen);
	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "ftyp", tszRVBuffer, nRVLen);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;

	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "free");
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;
	nMDatPos = nFilePos;

	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "mdat");
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	nFilePos += nSDLen;

	while (true)
	{
		nRVLen = 0;
		nSDLen = 0;
		memset(tszRVBuffer, '\0', sizeof(tszRVBuffer));
		memset(ptszSDBuffer, '\0', XENGINE_MEMORY_SIZE_MAX);
		nRVLen = fread(tszRVBuffer, 1, sizeof(tszRVBuffer), pSt_RFile);
		if (nRVLen <= 0)
		{
			break;
		}
		int nListCount = 0;
		AVHELP_FRAMEDATA** ppSt_Frame;
		AVHelp_Parse_FrameGet(xhToken, tszRVBuffer, nRVLen, &ppSt_Frame, &nListCount);
		for (int i = 0; i < nListCount; i++)
		{
			XENGINE_AVCODEC_VIDEOFRAMETYPE enVideoFrameType;
			AVHelp_Parse_NaluType((LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ENUM_XENGINE_AVCODEC_VIDEO_TYPE_H264, &enVideoFrameType);

			int nFrameType = 0;
			if ((ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_SPS == enVideoFrameType) || (ENUM_XENGINE_AVCODEC_VIDEO_FRAMETYPE_PPS == enVideoFrameType))
			{
				nFrameType = 1;
			}
			
			MP4Protocol_Packet_FrameVideo(lpszClientID, ptszSDBuffer, &nSDLen, (LPCXSTR)ppSt_Frame[i]->ptszMsgBuffer, ppSt_Frame[i]->nMsgLen, nFilePos, nFrameType, 40);
			fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
			nFilePos += nSDLen;
		}
		BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_Frame, nListCount);
	}
	MP4Protocol_Packet_HDRBox(lpszClientID, ptszSDBuffer, &nSDLen, "mdat", NULL, nFilePos - nMDatPos - 8);

	fseek(pSt_WFile, nMDatPos, SEEK_SET);
	fwrite(ptszSDBuffer, 1, nSDLen, pSt_WFile);
	fseek(pSt_WFile, nFilePos, SEEK_SET);

	AVHelp_Parse_FrameClose(xhToken);
	MP4Protocol_Packet_Delete(lpszClientID);
	fclose(pSt_WFile);
	fclose(pSt_RFile);
	return 0;
}
int main()
{
	MP4_Packet();
	MP4_Parse();

	return 0;
}
