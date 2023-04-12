#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/HelpComponents_DataBase.lib")
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_DataBase/DataBase_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_HelpComponents/HelpComponents_DataBase/DataBase_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g HelpComponents_APPDatabase.cpp -o HelpComponents_APPDatabase.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_DataBase -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_HelpComponents,--disable-new-dtags
//Macos::clang++ -std=gnu++17 -Wall -g HelpComponents_APPDatabase.cpp -o HelpComponents_APPDatabase.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_HelpComponents -lXEngine_BaseLib -lHelpComponents_DataBase 

int Test_SQLite()
{
	XNETHANDLE xhData = 0;
	if (DataBase_SQLite_Create(_T("SQLUser.db")))
	{
		if (!DataBase_SQLite_Open(&xhData, _T("SQLUser.db")))
		{
			return XFALSE;
		}
		LPCXSTR lpszSQLTable = _T("create table AuthReg_User(ID INTEGER PRIMARY KEY,UserName TEXT,Password TEXT,RegData TEXT,LeftTime TEXT,HardCode TEXT,QQNumber integer,IDCard integer)");
		LPCXSTR lpszSQLSerial = _T("create table AuthReg_Serial(ID INTEGER PRIMARY KEY,UserName TEXT,SerialNumber TEXT,UsedTime TEXT,nMaxTime integer,bIsUsed boolean)");
		LPCXSTR lpszSQLInsert = _T("INSERT INTO AuthReg_User values('0','123123aa','123123','2011/1/2-12:33:22','0','FF01X0F012','486179','511025198807018792')");
		LPCXSTR lpszSQLInsertO = _T("INSERT INTO AuthReg_User values(NULL,'aaadddzxc','123123','2012/3/2-01:44:22','0','AFAFAWFAFAF4124AA','22222','515325325235325235')");
		if (!DataBase_SQLite_Exec(xhData, lpszSQLTable))
		{
			return XFALSE;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLSerial))
		{
			return XFALSE;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLInsert))
		{
			return XFALSE;
		}
		if (!DataBase_SQLite_Exec(xhData, lpszSQLInsertO))
		{
			return XFALSE;
		}
	}
	LPCXSTR lpszSQLQuery = _T("select * from AuthReg_User where UserName='123123aa'");
	char** pszResult;
	int nRow = 0;
	int nColume = 0;
	int nIndex = 0;
	if (!DataBase_SQLite_GetTable(xhData, lpszSQLQuery, &pszResult, &nRow, &nColume))
	{
		return XFALSE;
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
	strcpy(st_ConnectInfo.tszDBName, _T("XEngine_ListInfo"));
	strcpy(st_ConnectInfo.tszSQLAddr, _T("192.168.1.12"));
	strcpy(st_ConnectInfo.tszSQLName, _T("ruiyue"));
	strcpy(st_ConnectInfo.tszSQLPass, _T("123123Ruiyue"));

	if (!DataBase_MySQL_Connect(&xhDBDay, &st_ConnectInfo, 5, XTRUE, _T("utf8")))
	{
		return XFALSE;
	}

	__int64u dwLine = 0;
	__int64u dwField = 0;
	XNETHANDLE xhTableResult;
	XCHAR tszSQLQuery[2048];

	memset(tszSQLQuery, '\0', sizeof(tszSQLQuery));
	sprintf(tszSQLQuery, _T("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA='XEngine_ListInfo' AND TABLE_TYPE='BASE TABLE'"));
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

	LPCXSTR lpszConnectName = _T("mongodb://root:11@192.168.1.110/?authSource=admin");
	if (!DataBase_Mongo_Connect(&xhToken, lpszConnectName))
	{
		return -1;
	}

	int nListCount = 0;
	XCHAR** ppszResult = NULL;
	if (!DataBase_Mongo_FindJson(xhToken, _T("xyry"), _T("test"), NULL, NULL, &ppszResult, &nListCount))
	{
		return -2;
	}
	for (int i = 0; i < nListCount; i++)
	{
		printf("%s\n", ppszResult[i]);
	}
	BaseLib_OperatorMemory_Free((void***)&ppszResult, nListCount);
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
	strcpy(st_ConnectInfo.tszDBName, _T("xengine"));
	strcpy(st_ConnectInfo.tszSQLAddr, _T("192.168.1.9"));
	strcpy(st_ConnectInfo.tszSQLName, _T("xyry"));
	strcpy(st_ConnectInfo.tszSQLPass, _T("123123"));

	if (!DataBase_Postgre_ConnectWithStruct(&xhToken, &st_ConnectInfo))
	{
		return -1;
	}

	int nRecordCount = 0;
	int nFieldCount = 0;
	
	if (!DataBase_Postgre_QueryResult(xhToken, &xhTable, _T("SELECT * FROM \"List\""), &nRecordCount, &nFieldCount))
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
	BaseLib_OperatorMemory_Free((XPPPMEM)&ppSt_ListData, nListCount);
	DataBase_Postgre_Close(xhToken);
	return 0;
}


int main()
{
	Test_SQLite();
	//postgredb_test();
	//mysql_test();
	//mongodb_test();

	return XTRUE;
}