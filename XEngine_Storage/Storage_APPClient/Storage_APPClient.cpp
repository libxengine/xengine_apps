#ifdef _WINDOWS
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XClient_Socket.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_XStorageProtocol.lib")
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_DataBase/DataBase_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Storage/XEngine_XStorageCore/XStorageCore_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Storage/XEngine_XStorageProtocol/XStorageProtocol_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Storage/XEngine_XStorageProtocol/XStorageProtocol_Error.h"

//g++ -std=c++17 -Wall -g Storage_APPClient.cpp -o Storage_APPClient.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Storage -lXEngine_BaseLib -lXClient_Socket -lXEngine_XStorageProtocol -lpthread -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Client:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Storage,--disable-new-dtags

BOOL bRun = TRUE;
BOOL bRegiser = TRUE;
BOOL bLogin = TRUE;
BOOL bCreateDir = TRUE;
BOOL bQueryDir = TRUE;
BOOL bDeleteDir = TRUE;
BOOL bUPFile = TRUE;
BOOL bQueryFile = TRUE;
BOOL bDeleteFile = TRUE;
BOOL bDLFile = TRUE;
BOOL bQueryUser = TRUE;

SOCKET m_Socket = 0;
XNETHANDLE xhToken = 0;

LPCTSTR lpszAddr = _T("192.168.1.10");
LPCTSTR lpszUser = _T("123123aa");
LPCTSTR lpszPass = _T("123123");

XHTHREAD StorageClient_Thread()
{
	while (bRun)
	{
		int nRcvLen = 2048;
		int nMsgLen = 2048;
		TCHAR tszRcvBuffer[2048];
		TCHAR tszMsgBuffer[2048];
		XENGINE_PROTOCOLHDR st_ProtocolHdr;

		memset(tszRcvBuffer, '\0', sizeof(tszRcvBuffer));
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		memset(&st_ProtocolHdr, '\0', sizeof(XENGINE_PROTOCOLHDR));

		if (XClient_TCPSelect_RecvMsg(m_Socket, tszRcvBuffer, &nRcvLen, TRUE))
		{
			XStorageProtocol_Comm_ParseClient(tszRcvBuffer, tszMsgBuffer, &nMsgLen, &st_ProtocolHdr);
			printf("%s\n", tszMsgBuffer);

			if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPLOGIN == st_ProtocolHdr.unOperatorCode)
			{
				xhToken = st_ProtocolHdr.xhToken;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER == st_ProtocolHdr.unOperatorCode)
			{
				bRegiser = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_AUTH_REPREGISTER == st_ProtocolHdr.unOperatorCode)
			{
				bLogin = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDIRCREATE == st_ProtocolHdr.unOperatorCode)
			{
				bCreateDir = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDIRQUERY == st_ProtocolHdr.unOperatorCode)
			{
				bQueryDir = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDIRDELETE == st_ProtocolHdr.unOperatorCode)
			{
				bDeleteDir = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPUPDATE == st_ProtocolHdr.unOperatorCode)
			{
				bUPFile = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERYFILE == st_ProtocolHdr.unOperatorCode)
			{
				bQueryFile = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDELETE == st_ProtocolHdr.unOperatorCode)
			{
				bDeleteFile = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPDOWN == st_ProtocolHdr.unOperatorCode)
			{
				bDLFile = FALSE;
			}
			else if (XENGINE_COMMUNICATION_PROTOCOL_OPERATOR_CODE_STORAGE_REPQUERYUSER == st_ProtocolHdr.unOperatorCode)
			{
				bQueryUser = FALSE;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}

int StorageClient_Register()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQRegister(tszMsgBuffer, &nMsgLen, lpszUser, lpszPass);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("注册协议发送失败！"));
		return -1;
	}
	while (bRegiser)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_Login()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	XENGINE_PROTOCOL_USERAUTH st_UserAuth;

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	memset(&st_UserAuth, '\0', sizeof(XENGINE_PROTOCOL_USERAUTH));

	strcpy(st_UserAuth.tszUserName, lpszUser);
	strcpy(st_UserAuth.tszPassword, lpszPass);
	st_UserAuth.enDeviceType = ENUM_PROTOCOL_FOR_DEVICE_TYPE_PC;
	st_UserAuth.enClientType = ENUM_PROTOCOL_FOR_SERVICE_TYPE_USER;

	XStorageProtocol_Client_REQLogin(tszMsgBuffer, &nMsgLen, &st_UserAuth);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("登录协议发送失败！"));
		return -1;
	}
	while (bLogin)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_CreateDir()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQDirOperator(tszMsgBuffer, &nMsgLen, "aa");
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("创建文件夹失败！"));
		return -1;
	}
	while (bCreateDir)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_QueryDir()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQDirQuery(tszMsgBuffer, &nMsgLen);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("查询文件夹失败！"));
		return -1;
	}
	while (bQueryDir)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_DeleteDir()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQDirOperator(tszMsgBuffer, &nMsgLen, "aa", FALSE);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("删除文件夹失败！"));
		return -1;
	}
	while (bDeleteDir)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}

int StorageClient_UPFile()
{
	SOCKET hSocket;
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_TCPSelect_Create(lpszAddr, 10332, &hSocket))
	{
		printf(_T("连接失败!"));
		return -1;
	}
	LPCTSTR lpszFile = _T("G:\\netengineapp\\NetEngine_WINApps\\Debug\\123.exe");
	XStorageProtocol_Client_REQFile(tszMsgBuffer, &nMsgLen, lpszFile, "aa/", xhToken);
	XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nMsgLen);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));

	FILE* pSt_File = fopen(lpszFile, "rb");
	while (1)
	{
		nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
		int nRet = fread(tszMsgBuffer, 1, 2048, pSt_File);
		if (nRet <= 0)
		{
			break;
		}
		while (1)
		{
			if (XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nMsgLen))
			{
				break;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	XClient_TCPSelect_Close(hSocket);
	fclose(pSt_File);

	while (bUPFile)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_QueryFile()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQQueryFile(tszMsgBuffer, &nMsgLen, "20190701", "20300101", "TestFor_AuthRegClient.exe");
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("查询文件失败！"));
		return -1;
	}
	while (bQueryFile)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_DLFile()
{
	SOCKET hSocket;
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	if (!XClient_TCPSelect_Create(lpszAddr, 10332, &hSocket))
	{
		printf(_T("连接失败!"));
		return -1;
	}
	LPCTSTR lpszFile = _T("G:\\netengineapp\\NetEngine_WINApps\\Debug\\123.exe");
	XStorageProtocol_Client_REQFile(tszMsgBuffer, &nMsgLen, "TestFor_AuthRegClient.exe", "aa/", xhToken, FALSE);
	XClient_TCPSelect_SendMsg(hSocket, tszMsgBuffer, nMsgLen);

	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));
	XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, FALSE);

	XENGINE_PROTOCOLHDR st_ProtocolHdr;
	memcpy(&st_ProtocolHdr, tszMsgBuffer, sizeof(XENGINE_PROTOCOLHDR));

	FILE* pSt_File = fopen(lpszFile, "wb");
	int nRecvCount = 0;
	int nWriteCount = 0;
	while (1)
	{
		nMsgLen = 2048;
		memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

		if (XClient_TCPSelect_RecvMsg(hSocket, tszMsgBuffer, &nMsgLen, FALSE))
		{
			nRecvCount += nMsgLen;
			int nRet = fwrite(tszMsgBuffer, 1, nMsgLen, pSt_File);
			nWriteCount += nRet;
			printf("nRecvCount:%d nWriteCount:%d\n", nRecvCount, nWriteCount);
			if (nRecvCount >= 233472)
			{
				break;
			}
		}
	}
	XClient_TCPSelect_Close(hSocket);
	fclose(pSt_File);
	while (bDLFile)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_DeleteFile()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQDelete(tszMsgBuffer, &nMsgLen, "TestFor_AuthRegClient.exe");
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("删除文件失败！"));
		return -1;
	}
	while (bDeleteFile)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return 0;
}
int StorageClient_QueryUser()
{
	int nMsgLen = 2048;
	TCHAR tszMsgBuffer[2048];
	memset(tszMsgBuffer, '\0', sizeof(tszMsgBuffer));

	XStorageProtocol_Client_REQQueryUser(tszMsgBuffer, &nMsgLen, "123123aa", TRUE);
	if (!XClient_TCPSelect_SendMsg(m_Socket, tszMsgBuffer, nMsgLen))
	{
		printf(_T("查询用户失败！"));
		return -1;
	}
	while (bQueryUser)
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

	if (!XClient_TCPSelect_Create(lpszAddr, 10331, &m_Socket))
	{
		printf("XClient_TCPSelect_Create:%lX", XClient_GetLastError());
		return -1;
	}
	std::thread pSTDThread(StorageClient_Thread);

	StorageClient_Register();
	StorageClient_Login();

	bRun = FALSE;
	XClient_TCPSelect_Close(m_Socket);
	pSTDThread.join();

#ifdef _WINDOWS
	WSACleanup();
#endif
	return 0;
}