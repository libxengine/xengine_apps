#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/NetHelp_APIClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIClient/APIClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_NetHelp/NetHelp_APIClient/APIClient_Error.h"

//Linux::g++ -std=c++17 -Wall -g NetHelp_APPClient.cpp -o NetHelp_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_NetHelp -lXEngine_BaseLib -lNetHelp_APIClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/NetHelp_APIClient,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g NetHelp_APPClient.cpp -o NetHelp_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/NetHelp_APIClient -lXEngine_BaseLib -lNetHelp_APIClient

void CALLBACK NetHelp_APPClient_CBRecv(XHANDLE xhToken, LPCSTR lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
	printf("NetHelp_APPClient_CBRecv:%d,%s\n", nMsgLen, lpszMsgBuffer);
	if (-1 == nMsgLen)
	{
		printf(_T("接受完毕！\n"));
	}
}
void CALLBACK NetHelp_HttpGet_Chunked(XNETHANDLE xhToken, LPVOID lpszMsgBuffer, int nMsgLen, LPVOID lParam)
{
	printf("%d\n%s\n", nMsgLen, (LPCTSTR)lpszMsgBuffer);
}

int NetHelp_APPClient_EMailPop3()
{
	NETHELP_EMAILCLIENT st_EMailClient;
	memset(&st_EMailClient, '\0', sizeof(NETHELP_EMAILCLIENT));

	strcpy(st_EMailClient.tszServiceAddr, _T("pop3s://pop.exmail.qq.com:995"));
	strcpy(st_EMailClient.tszUserName, _T("qyt@xyry.org"));
	strcpy(st_EMailClient.tszPassWord, _T("123123Qa"));

	XHANDLE xhPop3 = APIClient_EMail_Init(&st_EMailClient);
	if (NULL == xhPop3)
	{
		printf(_T("初始化失败！\r\n"));
		return -1;
	}
	if (!APIClient_EMail_Recv(xhPop3, NetHelp_APPClient_CBRecv))
	{
		printf(_T("接受邮件失败！\r\n"));
		return -1;
	}
	APIClient_EMail_Close(xhPop3);
	return 0;
}
int NetHelp_APPClient_EMailSmtp()
{
	NETHELP_EMAILCLIENT st_EMailClient;
	memset(&st_EMailClient, '\0', sizeof(NETHELP_EMAILCLIENT));

	strcpy(st_EMailClient.tszServiceAddr, _T("smtps://smtp.exmail.qq.com:465"));
	strcpy(st_EMailClient.tszUserName, _T("qyt@xyry.org"));
	strcpy(st_EMailClient.tszPassWord, _T("123123Qa"));
	strcpy(st_EMailClient.tszFromAddr, _T("qyt@xyry.org"));

	XHANDLE xhSmtp = APIClient_EMail_Init(&st_EMailClient);
	if (NULL == xhSmtp)
	{
		printf(_T("初始化失败！\r\n"));
		return -1;
	}
	if (!APIClient_EMail_Send(xhSmtp, "<486179@qq.com>", _T("找回密码"), _T("你的密码")))
	{
		printf(_T("发送失败！\r\n"));
		return -3;
	}
	printf(_T("发送成功！\r\n"));
	APIClient_EMail_Close(xhSmtp);
	return 0;
}

int Test_Http2Request()
{
	LPCTSTR lpszUrl = _T("http://www.nghttp2.org/");
	CHAR* ptszMsgBuffer = NULL;
	int nLen = 0;
	NETHELP_HTTPCLIENT st_HTTPParam;
	memset(&st_HTTPParam, '\0', sizeof(NETHELP_HTTPCLIENT));

	st_HTTPParam.bHTTP2Enable = TRUE;

	APIClient_Http_Request(_T("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer, &nLen, NULL, NULL, &st_HTTPParam);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_HttpRequest()
{
	LPCTSTR lpszUrl = _T("http://www.baidu.com");
	CHAR* ptszMsgBuffer = NULL;

	APIClient_Http_Request(_T("GET"), lpszUrl, NULL, NULL, &ptszMsgBuffer);
	printf("%s\n", ptszMsgBuffer);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}
int Test_HttpCreate()
{
	XNETHANDLE xhToken = 0;
	CHAR* ptszMsgBuffer = NULL;

	APIClient_Http_Create(&xhToken, NetHelp_HttpGet_Chunked);
	APIClient_Http_SetUrl(xhToken, "www.xyry.org", "GET");

	APIClient_Http_Excute(xhToken, &ptszMsgBuffer);
	APIClient_Http_Close(xhToken);
	BaseLib_OperatorMemory_FreeCStyle((XPPMEM)&ptszMsgBuffer);
	return 0;
}

int main()
{
	Test_Http2Request();
	Test_HttpRequest();
	Test_HttpCreate();

	NetHelp_APPClient_EMailPop3();
	NetHelp_APPClient_EMailSmtp();
	
	return 0;
}