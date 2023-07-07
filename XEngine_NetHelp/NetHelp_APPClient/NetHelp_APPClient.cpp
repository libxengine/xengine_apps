#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Define.h>
#include <XEngine_Include/XEngine_NetHelp/APIClient_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_NetHelp/NetHelp_APIClient.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIClient/APIClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIClient/APIClient_Error.h"
#ifdef _MSC_BUILD
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIClient.lib")
#endif
#endif

//Linux::g++ -std=c++17 -Wall -g NetHelp_APPClient.cpp -o NetHelp_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g NetHelp_APPClient.cpp -o NetHelp_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient

void CALLBACK NetHelp_APPClient_CBRecv(XHANDLE xhToken, LPCXSTR lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	printf("NetHelp_APPClient_CBRecv:%d,%s\n", nMsgLen, lpszMsgBuffer);
	if (-1 == nMsgLen)
	{
		printf(_X("接受完毕！\n"));
	}
}
void CALLBACK NetHelp_HttpGet_Chunked(XNETHANDLE xhToken, XPVOID lpszMsgBuffer, int nMsgLen, XPVOID lParam)
{
	printf("%d\n%s\n", nMsgLen, (LPCXSTR)lpszMsgBuffer);
}

int NetHelp_APPClient_EMailPop3()
{
	NETHELP_EMAILCLIENT st_EMailClient;
	memset(&st_EMailClient, '\0', sizeof(NETHELP_EMAILCLIENT));

	strcpy(st_EMailClient.tszServiceAddr, _X("pop3s://pop.exmail.qq.com:995"));
	strcpy(st_EMailClient.tszUserName, _X("qyt@xyry.org"));
	strcpy(st_EMailClient.tszPassWord, _X("111"));

	XHANDLE xhPop3 = APIClient_EMail_Init(&st_EMailClient);
	if (NULL == xhPop3)
	{
		printf(_X("初始化失败！\r\n"));
		return -1;
	}
	if (!APIClient_EMail_Recv(xhPop3, NetHelp_APPClient_CBRecv))
	{
		printf(_X("接受邮件失败！\r\n"));
		return -1;
	}
	APIClient_EMail_Close(xhPop3);
	return 0;
}
int NetHelp_APPClient_EMailSmtp()
{
	NETHELP_EMAILCLIENT st_EMailClient;
	memset(&st_EMailClient, '\0', sizeof(NETHELP_EMAILCLIENT));

	strcpy(st_EMailClient.tszServiceAddr, _X("smtps://smtp.exmail.qq.com:465"));
	strcpy(st_EMailClient.tszUserName, _X("qyt@xyry.org"));
	strcpy(st_EMailClient.tszPassWord, _X("111"));
	strcpy(st_EMailClient.tszFromAddr, _X("qyt@xyry.org"));

	XHANDLE xhSmtp = APIClient_EMail_Init(&st_EMailClient);
	if (NULL == xhSmtp)
	{
		printf(_X("初始化失败！\r\n"));
		return -1;
	}
	if (!APIClient_EMail_Send(xhSmtp, "<486179@qq.com>", _X("找回密码"), _X("你的密码")))
	{
		printf(_X("发送失败！\r\n"));
		return -3;
	}
	printf(_X("发送成功！\r\n"));
	APIClient_EMail_Close(xhSmtp);
	return 0;
}

int Test_Http2Request()
{
	LPCXSTR lpszUrl = _X("http://www.nghttp2.org/");
	XCHAR* ptszMsgBuffer = NULL;
	int nLen = 0;
	NETHELP_HTTPCLIENT st_HTTPParam;
	memset(&st_HTTPParam, '\0', sizeof(NETHELP_HTTPCLIENT));

	st_HTTPParam.bHTTP2Enable = true;

	APIClient_Http_Request(_X("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_HttpRequest()
{
	LPCXSTR lpszUrl = _X("http://www.baidu.com");
	XCHAR* ptszMsgBuffer = NULL;

	APIClient_Http_Request(_X("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_HttpCreate()
{
	XNETHANDLE xhToken = 0;
	XCHAR* ptszMsgBuffer = NULL;

	APIClient_Http_Create(&xhToken, NetHelp_HttpGet_Chunked);
	APIClient_Http_SetUrl(xhToken, "www.xyry.org", "GET");

	APIClient_Http_Excute(xhToken);

	while (true)
	{
		bool bComplete = false;
		APIClient_Http_GetResult(xhToken, &bComplete);

		if (bComplete)
		{
			break;
		}
	}
	APIClient_Http_Close(xhToken);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}

bool bRun = false;
void CALLBACK Download_Progress(XHANDLE xhToken, double dlTotal, double dlNow, double ulTotal, double ulNow, ENUM_NETHELP_APICLIENT_FILE_STATUS en_DownHttpStatus, XPVOID lParam)
{
	printf("下载任务：%p,总大小：%lf，已经下载大小：%lf，下载标识符：%d\n", xhToken, dlTotal, dlNow, en_DownHttpStatus);

	if (ENUM_NETHELP_APICLIENT_FILE_STATUS_COMPLETE == en_DownHttpStatus)
	{
		bRun = false;
	}
}
int download_http()
{
	LPCXSTR lpszHttpAddr = _X("https://webcdn.m.qq.com/spcmgr/download/QQ9.7.1.28940.exe");
	//LPCXSTR lpszHttpAddr = _X("http://192.168.1.7:5101/QQ.exe");
#ifdef _MSC_BUILD
	LPCXSTR lpszFileAddr = _X("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCXSTR lpszFileAddr = _X("QQ.exe");
#endif

	XHANDLE xhDownCall = APIClient_File_Create(lpszHttpAddr, lpszFileAddr, true, NULL, Download_Progress);
	if (NULL == xhDownCall)
	{
		printf("下载失败！");
		return -1;
	}
	bRun = true;
	APIClient_File_Start(xhDownCall);

	while (bRun)
	{
		NETHELP_FILEINFO st_TaskInfo;
		memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

		if (!APIClient_File_Query(xhDownCall, &st_TaskInfo))
		{
			break;
		}
		if (ENUM_NETHELP_APICLIENT_FILE_STATUS_COMPLETE == st_TaskInfo.en_DownStatus)
		{
			break;
		}
	}
	APIClient_File_Delete(xhDownCall);
	return 0;
}
int upload_http()
{
	LPCXSTR lpszHttpAddr = _X("http://192.168.1.7:5102/QQ.exe");
#ifdef _MSC_BUILD
	LPCXSTR lpszFileAddr = _X("D:\\xengine_apps\\Debug\\QQ.exe");
#else
	LPCXSTR lpszFileAddr = _X("QQ.exe");
#endif

	XHANDLE xhUPLoad = APIClient_File_Create(lpszHttpAddr, lpszFileAddr, false);
	if (NULL == xhUPLoad)
	{
		printf("下载失败！");
		return -1;
	}
	APIClient_File_Start(xhUPLoad, false, "PUT");
	while (1)
	{
		NETHELP_FILEINFO st_TaskInfo;
		memset(&st_TaskInfo, '\0', sizeof(NETHELP_FILEINFO));

		if (!APIClient_File_Query(xhUPLoad, &st_TaskInfo))
		{
			break;
		}
		if (ENUM_NETHELP_APICLIENT_FILE_STATUS_COMPLETE == st_TaskInfo.en_DownStatus)
		{
			break;
		}
		printf("上传任务：%p,总大小：%lf，已经上传大小：%lf，标识符：%d\n", xhUPLoad, st_TaskInfo.ulTotal, st_TaskInfo.ulNow, st_TaskInfo.en_DownStatus);
	}
	APIClient_File_Delete(xhUPLoad);
	return 0;
}

int main()
{
	//upload_http();
	//download_http();

	//Test_Http2Request();
	//Test_HttpRequest();
	Test_HttpCreate();

	NetHelp_APPClient_EMailPop3();
	NetHelp_APPClient_EMailSmtp();
	return 0;
}