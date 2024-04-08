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
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Define.h>
#include <XEngine_Include/XEngine_StreamMedia/MP4Protocol_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_StreamMedia/StreamMedia_MP4Protocol.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_ManagePool/ManagePool_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_StreamMedia/StreamMedia_MP4Protocol/MP4Protocol_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/StreamMedia_MP4Protocol.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g StreamMedia_MP4App.cpp -o StreamMedia_MP4App.exe -lXEngine_BaseLib -lStreamMedia_MP4Protocol
int main()
{
	LPCXSTR lpszClientID = _X("127777");
	FILE* pSt_File = _xtfopen(_X("D:\\qyt.1711442415865.mp4"), _X("rb"));
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
		XENGINE_MP4HDR st_MP4Hdr = {};
		while (true)
		{
			if (!MP4Protocol_Parse_Recv(lpszClientID, &st_MP4Hdr))
			{
				break;
			}
			bDPos = true;
			nFCOunt += st_MP4Hdr.nHDRSize;
		}
		if (!bDPos)
		{
			nFCOunt += nRet;
		}
	}

	MP4Protocol_Parse_Delete(lpszClientID);
	MP4Protocol_Parse_Destory();
	return 0;
}
