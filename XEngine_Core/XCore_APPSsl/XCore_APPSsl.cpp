#include "../../XEngine_ExampleHdr.h"
#ifdef _MSC_BUILD
#include <Windows.h>
#include <tchar.h>
#else
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <thread>
using namespace std;

#if 1 == _XENGINE_USER_DIR_SYSTEM
#include <XEngine_Include/XEngine_CommHdr.h>
#include <XEngine_Include/XEngine_ProtocolHdr.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Define.h>
#include <XEngine_Include/XEngine_BaseLib/BaseLib_Error.h>
#include <XEngine_Include/XEngine_Core/Cryption_Define.h>
#include <XEngine_Include/XEngine_Core/Cryption_Error.h>
#ifdef _MSC_BUILD
#pragma comment(lib,"XEngine_BaseLib/XEngine_BaseLib.lib")
#pragma comment(lib,"XEngine_Core/XEngine_Cryption.lib")
#endif
#else
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_Cryption/Cryption_Error.h"
#ifdef _MSC_BUILD
#ifdef _WIN64
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/x64/Debug/XEngine_Cryption.lib")
#else
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_Cryption.lib")
#endif
#endif
#endif
//Linux::g++ -std=c++20 -Wall -g XCore_APPSsl.cpp -o XCore_APPSsl.exe -lXEngine_BaseLib -lXEngine_Cryption

void md5cal()
{
	XBYTE tszMD5Hex[MAX_PATH];
	XCHAR tszMD5Str[MAX_PATH];

	memset(tszMD5Hex, '\0', MAX_PATH);
	memset(tszMD5Str, '\0', MAX_PATH);

	LPCXSTR lpszFile = _X("D:\\XEngine_Storage\\XEngine_Source\\Debug\\XEngine_File2\\a.txt");
	int nLen = strlen(lpszFile);
	
	if (!Cryption_Api_Digest(lpszFile, tszMD5Hex, &nLen))
	{
		return;
	}
	BaseLib_String_StrToHex((LPCXSTR)tszMD5Hex, nLen, tszMD5Str);
	printf(_X("%s\n"), tszMD5Str);

	nLen = MAX_PATH;
	memset(tszMD5Hex, '\0', MAX_PATH);
	memset(tszMD5Str, '\0', MAX_PATH);

	if (!Cryption_Api_Digest("D:\\xengine_apps\\Debug\\XEngine_BaseLib.dll", tszMD5Hex, NULL, true, ENUM_XENGINE_CRYPTION_DIGEST_SHA1))
	{
		return;
	}

	BaseLib_String_StrToHex((char*)tszMD5Hex, 20, tszMD5Str);
	printf("%s\n", tszMD5Str);

	printf(_X("HASH END\n"));
}
void Cryptto()
{
	XCHAR tszSourceBuffer[4096];
	memset(tszSourceBuffer, '\0', sizeof(tszSourceBuffer));
	const char* lpszFileName = "E:\\netengineapp\\NetEngine_WINApps\\Debug\\NetEngine_MQService.dat1";

	strcpy(tszSourceBuffer, lpszFileName);

	LPCXSTR lpszKey = _X("123123");
	XBYTE tszOutString[1024];
	XCHAR tszDeString[1024];
	int nLen = strlen(lpszFileName);
	memset(tszDeString, '\0', 1024);
	memset(tszOutString, '\0', 1024);

	if (!Cryption_Api_CryptEncodec(tszSourceBuffer, tszOutString, &nLen, lpszKey, ENUM_XENGINE_CRYPTION_SYMMETRIC_3DES))
	{
		return;
	}
	printf(_X("加密后的数据长度：%d,数据为：%s\n"), nLen, tszOutString);

	if (!Cryption_Api_CryptDecodec(tszOutString, tszDeString, &nLen, lpszKey, ENUM_XENGINE_CRYPTION_SYMMETRIC_3DES))
	{
		return;
	}

	printf(_X("解密后的数据长度：%d,数据为：\n"), nLen);
	printf(_X("%s\n"), tszDeString);
}
void RsaSSL()
{
	LPCXSTR lpszPass = "123123";
	LPCXSTR lpszSource = "Hello World";
#ifdef _MSC_BUILD
	LPCXSTR lpszPrivateKey = _X("D:\\xengine_apps\\Debug\\test.Key");
	LPCXSTR lpszPublicKey = _X("D:\\xengine_apps\\Debug\\test_pub.Key");
#else
	LPCXSTR lpszPrivateKey = _X("test.Key");
	LPCXSTR lpszPublicKey = _X("test_pub.Key");
#endif
	int nSize = 0;
	XCHAR tszRSABuffer[4096] = {};
	Cryption_Api_RSAFileCreate(lpszPrivateKey);
	Cryption_Api_RSAMemoryCreate(tszRSABuffer, &nSize);

	Cryption_Api_RSAPubFile(lpszPrivateKey, lpszPublicKey);

	int nPLen = 4096;
	XCHAR tszPUBBuffer[4096] = {};
	Cryption_Api_RSAPubMemory(tszPUBBuffer, &nPLen, tszRSABuffer, nSize);

	int nRsaLen = strlen(lpszSource);
	XBYTE puszRsaEnString[1024];
	memset(puszRsaEnString, '\0', sizeof(puszRsaEnString));
	if (!Cryption_Api_RSAEncodec(lpszPublicKey, lpszSource, &nRsaLen, puszRsaEnString, true))
	{
		return;
	}
	printf("RSA:%s\n", puszRsaEnString);
	XCHAR tszDeString[1024];
	memset(tszDeString, '\0', sizeof(tszDeString));
	if (!Cryption_Api_RSADecodec(lpszPrivateKey, (LPCXSTR)puszRsaEnString, &nRsaLen, tszDeString, false))
	{
		return;
	}
	printf(_X("RSA:%s\n"), tszDeString);
}
void SignVer()
{
	LPCXSTR lpszSource = _X("Hello World");
	LPCXSTR lpszPass = _X("123123");

	int nLen = strlen(lpszSource);
	int nOLen = 1024;
	XCHAR tszSource[2048];

	memset(tszSource, '\0', sizeof(tszSource));

#ifdef _MSC_BUILD
	LPCXSTR lpszPrivateKey = _X("D:\\xengine_apps\\Debug\\test.Key");
	LPCXSTR lpszPublicKey = _X("D:\\xengine_apps\\Debug\\test_pub.Key");
#else
	LPCXSTR lpszPrivateKey = _X("test.Key");
	LPCXSTR lpszPublicKey = _X("test_pub.Key");
#endif

	if (!Cryption_Cert_SignEncoder(lpszSource,nLen, tszSource, &nOLen, lpszPrivateKey, lpszPass))
	{
		return;
	}
	if (!Cryption_Cert_SignVerifly(lpszSource, 11, tszSource, nOLen, lpszPublicKey, lpszPass))
	{
		return;
	}

	Cryption_Cert_Convert("D:\\xengine_apps\\Debug\\server.crt", "D:\\xengine_apps\\Debug\\server.der");

	return;
}

void VerSign()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszPrivateKey = _X("D:\\xengine_apps\\Debug\\test.Key");
	LPCXSTR lpszReqFile = _X("D:\\xengine_apps\\Debug\\test.csr");
	LPCXSTR lpszCAFile = _X("D:\\xengine_apps\\Debug\\ca.crt");
	LPCXSTR lpszUserFile = _X("D:\\xengine_apps\\Debug\\test.crt");
#else
	LPCXSTR lpszPrivateKey = _X("test.Key");
	LPCXSTR lpszReqFile = _X("test.csr");
	LPCXSTR lpszCAFile = _X("ca.crt");
	LPCXSTR lpszUserFile = _X("test.crt");
#endif

	XCRYPTION_X509ATTR st_X509Info;
	memset(&st_X509Info, '\0', sizeof(XCRYPTION_X509ATTR));

	strcpy(st_X509Info.tszCommonName, "www.xyry.org");
	strcpy(st_X509Info.tszCountryName, "china");
	strcpy(st_X509Info.tszProvinceName, "sichuan");
	strcpy(st_X509Info.tszLocalityName, "chengdu");
	strcpy(st_X509Info.tszOrganizationName, "xyry");
	strcpy(st_X509Info.tszOrgUnitName, "soft");
	strcpy(st_X509Info.tszEmailAddress, "486179@qq.com");

	Cryption_Cert_MakeCACert(lpszCAFile, 120102, 60 * 60 * 24 * 10, &st_X509Info, lpszPrivateKey);
	Cryption_Cert_X509Create(lpszReqFile, &st_X509Info, lpszPrivateKey, NULL, false);
	Cryption_Cert_X509Sign(lpszCAFile, lpszReqFile, lpszUserFile, 60 * 60 * 24 * 10, lpszPrivateKey);
}
void CertVer()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszRootKey = _X("D:\\xengine_apps\\Debug\\ca.crt");
	LPCXSTR lpszUserKey = _X("D:\\xengine_apps\\Debug\\test.crt");
#else
	LPCXSTR lpszRootKey = _X("ca.crt");
	LPCXSTR lpszUserKey = _X("test.crt");
#endif

	if (!Cryption_Cert_X509Verifly(lpszRootKey, lpszUserKey))
	{
		return;
	}
}
void GetCert()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszKey = _X("D:\\xengine_apps\\Debug\\test.crt");
#else
	LPCXSTR lpszKey = _X("test.crt");
#endif
	XCRYPTION_X509INFO st_X509Info;
	memset(&st_X509Info, '\0', sizeof(XCRYPTION_X509INFO));

	if (!Cryption_Cert_GetCerInfomachine(lpszKey, &st_X509Info))
	{
		return;
	}
}
int XCrypto_Test()
{
#ifdef _MSC_BUILD
	LPCXSTR lpszFile = _X("K:\\netengineapp\\NetEngine_WINApps\\Debug\\test.Key");
#else
	LPCXSTR lpszFile = _X("test.Key");
#endif
	int nLen = strlen(lpszFile);
	XBYTE tszEncoder[2048];
	XCHAR tszDecoder[2048];

	memset(tszEncoder, '\0', sizeof(tszEncoder));
	memset(tszDecoder, '\0', sizeof(tszDecoder));

	if (!Cryption_XCrypto_Encoder(lpszFile, &nLen, tszEncoder, "123123"))
	{
		return -1;
	}
	for (int i = 0; i < nLen; i++)
	{
		printf("%02X ", tszEncoder[i]);
	}
	printf("\r\n%s\n", tszEncoder);
		
	Cryption_XCrypto_Decoder((LPCXSTR)tszEncoder, &nLen, tszDecoder, "123123");
	printf("%s\n", tszDecoder);
	return 0;
}

int main()
{
	XCrypto_Test();
	md5cal();
	Cryptto();
	RsaSSL();
	SignVer();
	VerSign();
	CertVer();
	GetCert();
	
	return 0;
}