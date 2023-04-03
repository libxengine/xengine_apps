#ifdef _MSC_BUILD
#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <locale.h>
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_BaseLib.lib")
#pragma comment(lib,"../../../XEngine/XEngine_SourceCode/Debug/XEngine_OPenSsl.lib")
#else
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#endif
#include <thread>
using namespace std;
#include "../../../XEngine/XEngine_SourceCode/XEngine_CommHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_BaseLib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Core/XEngine_OPenSsl/OPenSsl_Error.h"

//Linux::g++ -std=gnu++17 -Wall -g XCore_APPSsl.cpp -o XCore_APPSsl.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core -lXEngine_BaseLib -lXEngine_OPenSsl -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags
//Macos::g++ -std=gnu++17 -Wall -g XCore_APPSsl.cpp -o XCore_APPSsl.exe -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_BaseLib -L ../../../XEngine/XEngine_Release/XEngine_Mac/XEngine_Core -lXEngine_BaseLib -lXEngine_OPenSsl

void md5cal()
{
	UCHAR tszMD5Hex[MAX_PATH];
	TCHAR tszMD5Str[MAX_PATH];

	memset(tszMD5Hex, '\0', MAX_PATH);
	memset(tszMD5Str, '\0', MAX_PATH);

	int nLen = 512;
	LPCTSTR lpszFile = _T("D:\\XEngine_Storage\\XEngine_Source\\Debug\\XEngine_File2\\a.txt");

	if (!OPenSsl_Api_Digest(lpszFile, tszMD5Hex, &nLen, TRUE, XENGINE_OPENSSL_API_DIGEST_MD5))
	{
		return;
	}
	BaseLib_OperatorString_StrToHex((char*)tszMD5Hex, nLen, tszMD5Str);
	printf(_T("%s\n"), tszMD5Str);

	nLen = MAX_PATH;
	memset(tszMD5Hex, '\0', MAX_PATH);
	memset(tszMD5Str, '\0', MAX_PATH);

	if (!OPenSsl_Api_Digest("D:\\xengine_apps\\Debug\\XEngine_BaseLib.dll", tszMD5Hex, NULL, TRUE, XENGINE_OPENSSL_API_DIGEST_SHA1))
	{
		return;
	}

	BaseLib_OperatorString_StrToHex((char*)tszMD5Hex, 20, tszMD5Str);
	printf("%s\n", tszMD5Str);

	printf(_T("HASH END\n"));
}
void Cryptto()
{
	TCHAR tszSourceBuffer[4096];
	memset(tszSourceBuffer, '\0', sizeof(tszSourceBuffer));
	const char* lpszFileName = "E:\\netengineapp\\NetEngine_WINApps\\Debug\\NetEngine_MQService.dat1";

	strcpy(tszSourceBuffer, lpszFileName);

	LPCTSTR lpszKey = _T("123123");
	UCHAR tszOutString[1024];
	TCHAR tszDeString[1024];
	int nLen = strlen(lpszFileName);
	memset(tszDeString, '\0', 1024);
	memset(tszOutString, '\0', 1024);

	if (!OPenSsl_Api_CryptEncodec(tszSourceBuffer, tszOutString, &nLen, lpszKey, XENGINE_OPENSSL_API_CRYPT_SMCBC))
	{
		return;
	}
	printf(_T("加密后的数据长度：%d,数据为：%s\n"), nLen, tszOutString);

	if (!OPenSsl_Api_CryptDecodec(tszOutString, tszDeString, &nLen, lpszKey, XENGINE_OPENSSL_API_CRYPT_SMCBC))
	{
		return;
	}

	printf(_T("解密后的数据长度：%d,数据为：\n"), nLen);
	printf(_T("%s\n"), tszDeString);
}
void RsaSSL()
{
	LPCTSTR lpszPass = "123123";
	LPCTSTR lpszSource = "Hello World";
#ifdef _MSC_BUILD
	LPCTSTR lpszPrivateKey = _T("D:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("D:\\XEngine_Apps\\Debug\\test_pub.Key");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
#endif

	if (!OPenSsl_Api_RSACreate(lpszPublicKey, lpszPrivateKey, lpszPass))
	{
		return;
	}
	int nRsaLen = _tcslen(lpszSource);
	UCHAR puszRsaEnString[1024];
	memset(puszRsaEnString, '\0', 1024);
	if (!OPenSsl_Api_RSAEncodec(lpszPublicKey, lpszSource, &nRsaLen, puszRsaEnString, TRUE, lpszPass))
	{
		return;
	}
	printf("RSA:%s\n", puszRsaEnString);
	TCHAR tszDeString[1024];
	memset(tszDeString, '\0', sizeof(tszDeString));
	if (!OPenSsl_Api_RSADecodec(lpszPrivateKey, puszRsaEnString, &nRsaLen, tszDeString, FALSE, lpszPass))
	{
		return;
	}
	printf(_T("RSA:%s\n"), tszDeString);
}
void SignVer()
{
	LPCTSTR lpszSource = _T("Hello World");
	LPCTSTR lpszPass = _T("123123");

	int nLen = strlen(lpszSource);
	int nOLen = 1024;
	TCHAR tszSource[2048];

	memset(tszSource, '\0', sizeof(tszSource));

#ifdef _MSC_BUILD
	LPCTSTR lpszPrivateKey = _T("D:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("D:\\XEngine_Apps\\Debug\\test_pub.Key");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
#endif

	if (!OPenSsl_Cert_SignEncoder(lpszSource,nLen, tszSource, &nOLen, lpszPrivateKey, lpszPass))
	{
		return;
	}
	if (!OPenSsl_Cert_SignVerifly(lpszSource, 11, tszSource, nOLen, lpszPublicKey, lpszPass))
	{
		return;
	}
}

void VerSign()
{
#ifdef _MSC_BUILD
	LPCTSTR lpszPrivateKey = _T("D:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("D:\\XEngine_Apps\\Debug\\test_pub.Key");
	LPCTSTR lpszReqFile = _T("D:\\XEngine_Apps\\Debug\\test.csr");
	LPCTSTR lpszCAFile = _T("D:\\XEngine_Apps\\Debug\\ca.crt");
	LPCTSTR lpszUserFile = _T("D:\\XEngine_Apps\\Debug\\test.crt");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
	LPCTSTR lpszReqFile = _T("test.csr");
	LPCTSTR lpszCAFile = _T("ca.crt");
	LPCTSTR lpszUserFile = _T("test.crt");
#endif
	LPCTSTR lpszPass = _T("123123");

	OPENSSL_X509CCINL st_X509Info;
	memset(&st_X509Info, '\0', sizeof(OPENSSL_X509CCINL));

	strcpy(st_X509Info.tszCommonName, "www.xyry.org");
	strcpy(st_X509Info.tszCountryName, "china");
	strcpy(st_X509Info.tszProvinceName, "sichuan");
	strcpy(st_X509Info.tszLocalityName, "chengdu");
	strcpy(st_X509Info.tszOrganizationName, "xyry");
	strcpy(st_X509Info.tszOrgUnitName, "soft");
	strcpy(st_X509Info.tszEmailAddress, "486179@qq.com");

	OPenSsl_Cert_MakeCACert(lpszCAFile, 120102, 60 * 60 * 24 * 10, &st_X509Info, lpszPrivateKey, lpszPass);
	OPenSsl_Cert_X509GenRequest(lpszReqFile, &st_X509Info, lpszPrivateKey, lpszPass);
	OPenSsl_Cert_X509SignVer(lpszCAFile, lpszReqFile, lpszPass, lpszUserFile, 1100220011, 60 * 60 * 24 * 10, lpszPrivateKey, lpszPass);
}
void CertVer()
{
#ifdef _MSC_BUILD
	LPCTSTR lpszRootKey = _T("D:\\XEngine_Apps\\Debug\\ca.crt");
	LPCTSTR lpszUserKey = _T("D:\\XEngine_Apps\\Debug\\test.crt");
#else
	LPCTSTR lpszRootKey = _T("ca.crt");
	LPCTSTR lpszUserKey = _T("test.crt");
#endif

	if (!OPenSsl_Cert_X509Verifly(lpszRootKey, lpszUserKey))
	{
		return;
	}
}
void GetCert()
{
#ifdef _MSC_BUILD
	LPCTSTR lpszKey = _T("D:\\XEngine_Apps\\Debug\\test.crt");
#else
	LPCTSTR lpszKey = _T("test.crt");
#endif
	OPENSSL_X509CCINFO st_X509Info;
	memset(&st_X509Info, '\0', sizeof(OPENSSL_X509CCINFO));

	if (!OPenSsl_Cert_GetCerInfomachine(lpszKey, &st_X509Info))
	{
		return;
	}
}
int XCrypto_Test()
{
	LPCTSTR lpszFile = _T("K:\\netengineapp\\NetEngine_WINApps\\Debug\\test.Key");
	int nLen = strlen(lpszFile);
	UCHAR tszEncoder[2048];
	CHAR tszDecoder[2048];

	memset(tszEncoder, '\0', sizeof(tszEncoder));
	memset(tszDecoder, '\0', sizeof(tszDecoder));

	if (!OPenSsl_XCrypto_Encoder(lpszFile, &nLen, tszEncoder, "123123"))
	{
		return -1;
	}
	for (int i = 0; i < nLen; i++)
	{
		printf("%02X ", tszEncoder[i]);
	}
	printf("\r\n%s\n", tszEncoder);
		
	OPenSsl_XCrypto_Decoder((LPCXSTR)tszEncoder, &nLen, tszDecoder, "123123");
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