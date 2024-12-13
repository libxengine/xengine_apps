#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Define.h>
#include <XEngine_Include/XEngine_HelpComponents/DataBase_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_HelpComponents/HelpComponents_DataBase.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_DataBase/DataBase_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_DataBase/DataBase_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/HelpComponents_DataBase.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_DataBase.lib")
#endif
#endif
#endif

//Linux::g++ -std=c++20 -Wall -g HelpComponents_APPDatabase.cpp -o HelpComponents_APPDatabase.exe -lXEngine_BaseLib -lHelpComponents_DataBase 

int Test_SQLite()
{
	XNETHANDLE xhData = 0;
	if (DataBase_SQLite_Create(_X("SQLUser.db")))
	{
		if (!DataBase_SQLite_Open(&xhData, _X("SQLUser.db")))
		{
			return false;
		}
		LPCXSTR lpszSQLTable = _X("create table AuthReg_User(ID INTEGER PRIMARY KEY,UserName TEXT,Password TEXT,RegData TEXT,LeftTime TEXT,HardCode TEXT,QQNumber integer,IDCard integer)");
		LPCXSTR lpszSQLSerial = _X("create table AuthReg_Serial(ID INTEGER PRIMARY KEY,UserName TEXT,SerialNumber TEXT,UsedTime TEXT,nMaxTime integer,bIsUsed boolean)");
		LPCXSTR lpszSQLInsert = _X("INSERT INTO AuthReg_User values('0','123123aa','123123','2011/1/2-12:33:22','0','FF01X0F012','486179','511025198807018792')");
		LPCXSTR lpszSQLInsertO = _X("INSERT INTO AuthReg_User values(NULL,'aaadddzxc','123123','2012/3/2-01:44:22','0','AFAFAWFAFAF4124AA','22222','515325325235325235')");
		if (!DataBase_SQLite_Exec(xhData, lpszSQLTable))
		{
			return false;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLSerial))
		{
			return false;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLInsert))
		{
			return false;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLInsertO))
		{
			return false;
		}
	}
	LPCXSTR lpszSQLQuery = _X("select * from AuthReg_User where UserName='123123aa'");
	char** pszResult;
	int nRow = 0;
	int nColume = 0;
	int nIndex = 0;
	if (!DataBase_SQLite_GetTable(xhData, lpszSQLQuery, &pszResult, &nRow, &nColume))
	{
		return false;
	}
	nIndex = nColume;
	for (int i = 0; i < nRow; i++)
	{
		for (int j = 0; j < nColume; j++)
		{
			printf("字段名:%s > 字段值:%s\n", pszResult[j], pszResult[nIndex]);
			++nIndex;
		}
		printf("-------\n");
	}
	DataBase_SQLite_FreeTable(pszResult);
	DataBase_SQLite_Close(xhData);
	return 0;
}
int mysql_test()
{
	XNETHANDLE xhDBDay;
	DATABASE_MYSQL_CONNECTINFO st_ConnectInfo;
	memset(&st_ConnectInfo, '\0', sizeof(DATABASE_MYSQL_CONNECTINFO));

	//连接数据库
	strcpy(st_ConnectInfo.tszDBName, _X("XEngine_ListInfo"));
	strcpy(st_ConnectInfo.tszSQLAddr, _X("192.168.1.12"));
	strcpy(st_ConnectInfo.tszSQLName, _X("ruiyue"));
	strcpy(st_ConnectInfo.tszSQLPass, _X("123123Ruiyue"));

	if (!DataBase_MySQL_Connect(&xhDBDay, &st_ConnectInfo, 5, true, _X("utf8")))
	{
		return false;
	}

	__int64u dwLine = 0;
	__int64u dwField = 0;
	XNETHANDLE xhTableResult;
	XCHAR tszSQLQuery[2048];

	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
	sprintf(tszSQLQuery, _X("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='XEngine_ListInfo' AND TABLE_TYPE='BASE TABLE'"));
	if (DataBase_MySQL_ExecuteQuery(xhDBDay, &xhTableResult, tszSQLQuery, &dwLine, &dwField))
	{
		for (__int64u i = 0; i < dwLine; i++)
		{
			XCHAR** pptszResult = DataBase_MySQL_GetResult(xhDBDay, xhTableResult);
			if (NULL == pptszResult[0])
			{
				continue;
			}
			printf("%s\n", pptszResult[0]);
		}
	}
	DataBase_MySQL_FreeResult(xhDBDay, xhTableResult);
	return 1;
}

int mongodb_test()
{
	XNETHANDLE xhToken;

	LPCXSTR lpszConnectName = _X("mongodb://root:11@192.168.1.110/?authSource=admin");
	if (!DataBase_Mongo_Connect(&xhToken, lpszConnectName))
	{
		return -1;
	}

	int nListCount = 0;
	XCHAR** ppszResult = NULL;
	if (!DataBase_Mongo_FindJson(xhToken, _X("xyry"), _X("test"), NULL, NULL, &ppszResult, &nListCount))
	{
		return -2;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", ppszResult[i]);
	}
	BaseLib_Memory_Free((void***)&ppszResult, nListCount);
	DataBase_Mongo_Close(xhToken);
	return 0;
}

int postgredb_test()
{
	XNETHANDLE xhToken;
	XNETHANDLE xhTable;
	DATABASE_MYSQL_CONNECTINFO st_ConnectInfo;
	memset(&st_ConnectInfo, '\0', sizeof(DATABASE_MYSQL_CONNECTINFO));

	//连接数据库
	st_ConnectInfo.nPort = 5432;
	strcpy(st_ConnectInfo.tszDBName, _X("xengine"));
	strcpy(st_ConnectInfo.tszSQLAddr, _X("192.168.1.9"));
	strcpy(st_ConnectInfo.tszSQLName, _X("xyry"));
	strcpy(st_ConnectInfo.tszSQLPass, _X("123123"));

	if (!DataBase_Postgre_ConnectWithStruct(&xhToken, &st_ConnectInfo))
	{
		return -1;
	}

	int nRecordCount = 0;
	int nFieldCount = 0;
	
	if (!DataBase_Postgre_QueryResult(xhToken, &xhTable, _X("SELECT * FROM \"List\""), &nRecordCount, &nFieldCount))
	{
		return -2;
	}
	int nListCount = 0;
	DATABASE_POSTGRE_DATAINFO** ppSt_ListData;
	if (!DataBase_Postgre_GetResult(xhToken, xhTable, &ppSt_ListData, &nListCount, nRecordCount, nFieldCount))
	{
		return -3;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("Name:%s Value:%s Len:%d Type:%d\n", ppSt_ListData[i]->ptszName, ppSt_ListData[i]->ptszValue, ppSt_ListData[i]->unLen, ppSt_ListData[i]->unOidType);
	}
	DataBase_Postgre_FreeResult(xhToken, xhTable);
	BaseLib_Memory_Free((XPPPMEM)&ppSt_ListData, nListCount);
	DataBase_Postgre_Close(xhToken);
	return 0;
}


int main()
{
	Test_SQLite();
	//postgredb_test();
	//mysql_test();
	//mongodb_test();

	return true;
}