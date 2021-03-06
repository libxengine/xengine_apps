#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/RfcComponents_EmailClient.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_EmailClient/EmailClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_RfcComponents/RfcComponents_EmailClient/EmailClient_Error.h"

//Linux::g++ -std=c++17 -Wall -g RfcComponents_APPEMail.cpp -o RfcComponents_APPEMail.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents -lXEngine_BaseLib -lRfcComponents_EmailClient -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_RfcComponents,--disable-new-dtags
//Macos::g++ -std=c++17 -Wall -g RfcComponents_APPEMail.cpp -o RfcComponents_APPEMail.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_RfcComponents -lXEngine_BaseLib -lRfcComponents_EmailClient

void CALLBACK Pop3_Callback(XNETHANDLE xhNet, BOOL bRet, LPCSTR lpszBuffer, int nLen, LPVOID lParam)
{
	printf("%lld-%d-   %s       -%d\n", xhNet, bRet, lpszBuffer, nLen);
	if (-1 == nLen)
	{
		printf(_T("接受完毕！\n"));
	}
}
int main()
{
	XNETHANDLE xhSmtp = 0;
	XNETHANDLE xhPop3 = 0;
	RFCCOMPONENTS_EMAILSMTP st_RfcSmtp;
	memset(&st_RfcSmtp, '\0', sizeof(RFCCOMPONENTS_EMAILSMTP));

	_tcscpy(st_RfcSmtp.tszServiceAddr, _T("smtps://smtp.exmail.qq.com:465"));
	_tcscpy(st_RfcSmtp.tszUserName, _T("qyt@xyry.org"));
	_tcscpy(st_RfcSmtp.tszPassWord, _T("11"));
	_tcscpy(st_RfcSmtp.tszFromAddr, _T("qyt@xyry.org"));

	st_RfcSmtp.bIsCall = FALSE;
	if (!RfcComponents_EMailClient_SmtpInit(&xhSmtp, &st_RfcSmtp))
	{
		printf(_T("初始化失败！\r\n"));
		return -1;
	}
	if (!RfcComponents_EMailClient_SmtpSend(xhSmtp, "<486179@qq.com>", _T("找回密码"), _T("你的密码")))
	{
		printf(_T("发送失败！\r\n"));
		return -3;
	}
	printf(_T("发送成功！\r\n"));
	RfcComponents_EMailClient_SmtpClose(xhSmtp);

	RFCCOMPONENTS_EMAILPOP3 st_EmailPop;
	memset(&st_EmailPop, '\0', sizeof(st_EmailPop));

	st_EmailPop.nIndex = 2;
	_tcscpy(st_EmailPop.tszUserName, _T("qyt@xyry.org:11"));
	_tcscpy(st_EmailPop.tszServiceAddr, _T("pop3://pop.exmail.qq.com"));

	RfcComponents_EMailClient_POP3SetCallBack(Pop3_Callback);
	if (!RfcComponents_EMailClient_POP3Init(&xhPop3, &st_EmailPop))
	{
		printf(_T("初始化失败！\r\n"));
	}
	if (!RfcComponents_EMailClient_POP3Recv(xhPop3))
	{
		printf(_T("接受邮件失败！\r\n"));
	}
	RfcComponents_EMailClient_POP3Close(xhPop3);
	return 0;
}