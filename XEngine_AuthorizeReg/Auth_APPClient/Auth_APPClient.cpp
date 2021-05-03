#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_AuthRegClient.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Client/XClient_Socket/XClient_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthGenerateSerial/AuthGenerateSerial_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthRegService/AuthRegService_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthRegClient/AuthRegClient_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_AuthorizeReg/XEngine_AuthRegClient/AuthRegClient_Error.h"

//g++ -std=c++17 -Wall -g Auth_APPClient.cpp -o Auth_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg -lXEngine_BaseLib -lXClient_Socket -lXEngine_AuthRegClient -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_AuthorizeReg,--disable-new-dtags

BOOL bRun = TRUE;
BOOL bRegiser = TRUE;
BOOL bPay = TRUE;
BOOL bLogin = TRUE;
BOOL bPass = TRUE;
BOOL bLeftTime = TRUE;
BOOL bDelete = TRUE;
BOOL bTry = TRUE;

SOCKET m_Socket = 0;
LPCTSTR lpszUser = _T("123123aa");
LPCTSTR lpszPass = _T("123123");
LPCTSTR lpszSerial = _T("XAUTH-XYRYM-AOUO5-VMN93-DUIFD-36702");
LPCTSTR lpszEmail = _T("486179@qq.com");
__int64x nQQNumber = 486179;
__int64x nIDNumber = 511025111111111111;

XHTHREAD AuthClient_Thread()
{
	while (bRun)
	{
		int nMsgLen = 2048;
		int nDTLen = 0;
		BYTE byCrypto = 0;
		TCHAR tszMsgBuffer[2048];

		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XClient_TCPSelect_RecvMsg(m_Socket, tszMsgBuffer, &nMsgLen, TRUE))
		{
			UINT unProtoCode = 0;
			int nError = 0;
			if (!AuthRegClient_Protocol_Recv(tszMsgBuffer, &unProtoCode, &nDTLen, &byCrypto, &nError))
			{
				printf(_T("解析协议失败"));
				continue;
			}
			if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPDEL == unProtoCode)
			{
				bDelete = FALSE;
				if (0 == nError)
				{
					printf(_T("删除用户成功\n"));
				}
				else
				{
					printf(_T("删除用户失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER == unProtoCode)
			{
				if (0 == nError)
				{
					bRegiser = FALSE;
					printf(_T("注册成功\n"));
				}
				else
				{
					printf(_T("注册失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN == unProtoCode)
			{
				if (0 == nError)
				{
					bLogin = FALSE;
					printf(_T("登录成功\n"));
				}
				else
				{
					printf(_T("登录失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPPAY == unProtoCode)
			{
				if (0 == nError)
				{
					bPay = FALSE;
					printf(_T("充值成功\n"));
				}
				else
				{
					printf(_T("充值失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETPASS == unProtoCode)
			{
				if (0 == nError)
				{
					XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;
					memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
					memcpy(&st_AuthProtocol, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), sizeof(XENGINE_PROTOCOL_USERAUTH));
					
					bPass = FALSE;
					printf(_T("找回密码成功,账号:%s,密码:%s\n"), st_AuthProtocol.tszUserName, st_AuthProtocol.tszUserPass);
				}
				else
				{
					printf(_T("找回密码失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPGETTIME == unProtoCode)
			{
				if (0 == nError)
				{
					AUTHREG_PROTOCOL_TIME st_AuthTime;
					memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
					memcpy(&st_AuthTime, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), sizeof(AUTHREG_PROTOCOL_TIME));

					bLeftTime = FALSE;
					printf(_T("获取剩余时间成功,账号:%s,地址:%s,剩余时间:%lld 分钟,在线时间:%lld,过期日期:%s,类型:%d\n"), st_AuthTime.tszUserName, st_AuthTime.tszUserAddr, st_AuthTime.nTimeLeft, st_AuthTime.nTimeONLine, st_AuthTime.tszLeftTime, st_AuthTime.enSerialType);
				}
				else
				{
					printf(_T("获取时间失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_NOTIFYMSG == unProtoCode)
			{
				if (0 == nError)
				{
					TCHAR tszMsgInfo[2048];
					memset(tszMsgInfo, '\0', sizeof(tszMsgInfo));
					memcpy(tszMsgInfo, tszMsgBuffer + sizeof(XENGINE_PROTOCOLHDR), nDTLen);

					printf(_T("读取到公告:%s\n"), tszMsgInfo);
				}
				else
				{
					printf(_T("获取数据失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPTRYVER == unProtoCode)
			{
				if (0 == nError)
				{
					bTry = FALSE;
					printf(_T("临时验证成功\n"));
				}
				else
				{
					printf(_T("临时验证失败,错误:%d\n"), nError);
				}
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_TIMEDOUT == unProtoCode)
			{
				printf(_T("用户过期\n"));
			}
			else
			{
				printf(_T("不明白的类型\n"));
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}

int AuthClient_Register()
{
	AUTHREG_USERTABLE st_UserTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserTable, '\0', sizeof(st_UserTable));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserTable.st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserTable.st_UserInfo.tszUserPass, lpszPass);
	strcpy(st_UserTable.st_UserInfo.tszEMailAddr, lpszEmail);
	st_UserTable.st_UserInfo.nQQNumber = nQQNumber;
	st_UserTable.st_UserInfo.nIDNumber = nIDNumber;
	strcpy(st_UserTable.tszHardCode, "2FDWAD02JD2091");

	int nMsgLen = sizeof(AUTHREG_USERTABLE);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQREGISTER);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("注册协议发送失败！\n"));
		return -1;
	}
	while (bRegiser)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Pay()
{
	AUTHREG_PROTOCOL_USERPAY st_SerialTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_SerialTable, '\0', sizeof(AUTHREG_PROTOCOL_USERPAY));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_SerialTable.tszSerialNumber, lpszSerial);
	strcpy(st_SerialTable.tszUserName, lpszUser);

	int nMsgLen = sizeof(AUTHREG_PROTOCOL_USERPAY);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_SerialTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQPAY);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("充值协议发送失败！\n"));
		return -1;
	}
	while (bPay)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Login()
{
	XENGINE_PROTOCOL_USERAUTH st_AuthUser;
	TCHAR tszMsgBuffer[2048];

	memset(&st_AuthUser, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_AuthUser.tszUserName, lpszUser);
	strcpy(st_AuthUser.tszUserPass, lpszPass);

	int nMsgLen = sizeof(XENGINE_PROTOCOL_USERAUTH);

	/*使用加密发送
	CHAR tszEnBuffer[2048];
		memset(tszEnBuffer, '\0', sizeof(tszEnBuffer));

		OPenSsl_XCrypto_Encoder((LPCTSTR)&st_AuthUser, &nMsgLen, (UCHAR*)tszMsgBuffer, "123123");
		AuthRegClient_Protocol_Send(tszMsgBuffer, tszEnBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN, 4);
	*/
	AuthRegClient_Protocol_Send((LPCTSTR)&st_AuthUser, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQLOGIN);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("登录协议发送失败！\n"));
		return -1;
	}
	while (bLogin)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_GetPass()
{
	AUTHREG_USERTABLE st_UserTable;
	XENGINE_PROTOCOL_USERAUTH st_AuthProtocol;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserTable, '\0', sizeof(AUTHREG_USERTABLE));
	memset(&st_AuthProtocol, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserTable.st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserTable.st_UserInfo.tszEMailAddr, lpszEmail);
	st_UserTable.st_UserInfo.nQQNumber = nQQNumber;
	st_UserTable.st_UserInfo.nIDNumber = nIDNumber;

	int nMsgLen = sizeof(AUTHREG_USERTABLE);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETPASS);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("找回密码协议发送失败！\n"));
		return -1;
	}
	while (bPass)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_GetTime()
{
	AUTHREG_PROTOCOL_TIME st_AuthTime;
	TCHAR tszMsgBuffer[2048];

	memset(&st_AuthTime, '\0', sizeof(AUTHREG_PROTOCOL_TIME));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_AuthTime.tszUserName, lpszUser);
	int nMsgLen = sizeof(AUTHREG_PROTOCOL_TIME);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_AuthTime, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQGETTIME);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("获取剩余时间协议发送失败！\n"));
		return -1;
	}
	while (bLeftTime)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int AuthClient_Delete()
{
	AUTHREG_USERTABLE st_UserTable;
	TCHAR tszMsgBuffer[2048];

	memset(&st_UserTable, '\0', sizeof(st_UserTable));
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	strcpy(st_UserTable.st_UserInfo.tszUserName, lpszUser);
	strcpy(st_UserTable.st_UserInfo.tszUserPass, lpszPass);
	strcpy(st_UserTable.st_UserInfo.tszEMailAddr, lpszEmail);
	st_UserTable.st_UserInfo.nQQNumber = nQQNumber;
	st_UserTable.st_UserInfo.nIDNumber = nIDNumber;

	int nMsgLen = sizeof(AUTHREG_USERTABLE);
	AuthRegClient_Protocol_Send((LPCTSTR)&st_UserTable, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQDEL);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("注销协议发送失败！\n"));
		return -1;
	}
	while (bDelete)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int AuthClient_Try()
{
	TCHAR tszMsgBuffer[2048];
	LPCTSTR lpszSerialNet = _T("cpuid:112233"); //通过此可以做临时网络验证,安全性比本地临时验证高

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	int nMsgLen = strlen(lpszSerialNet);

	AuthRegClient_Protocol_Send(lpszSerialNet, tszMsgBuffer, &nMsgLen, XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REQTRYVER);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("登录协议发送失败！\n"));
		return -1;
	}
	while (bTry)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int main()
{
#ifdef _WINDOWS
	WSADATA st_WSAData;
	WSAStartup(MAKEWORD(2, 2), &st_WSAData);
#endif
	int nPort = 5555;
	LPCTSTR lpszAddr = _T("127.0.0.1");
	
	if (!XClient_TCPSelect_Create(lpszAddr, nPort, &m_Socket))
	{
		printf("XClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}
	std::thread pSTDThread(AuthClient_Thread);

	AuthClient_Register();
	AuthClient_Pay();
	AuthClient_Login();
	AuthClient_GetPass();
	AuthClient_GetTime();
	AuthClient_Delete();
	AuthClient_Try();

	bRun = FALSE;
	XClient_TCPSelect_Close(m_Socket);
	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}