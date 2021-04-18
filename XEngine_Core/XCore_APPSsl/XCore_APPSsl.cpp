#ifdef _WINDOWS
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
#include "../../../XEngine/XEngine_SourceCode/XEngine_Types.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_ProtocolHdr.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine_Lib/XEngine_BaseLib/BaseLib_Error.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Define.h"
#include "../../../XEngine/XEngine_SourceCode/XEngine/XEngine_OPenSsl/OPenSsl_Error.h"

//g++ -std=gnu++17 -Wall -g XCore_APPSsl.cpp -o XCore_APPSsl.exe -L ../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib -lXEngine_BaseLib -lXEngine_OPenSsl -Wl,-rpath=../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_BaseLib:../../../XEngine/XEngine_Release/XEngine_Linux/Ubuntu/XEngine_Core,--disable-new-dtags

void md5cal()
{
	UCHAR tszMD5[20];
	memset(tszMD5, '\0', 20);
	LPCTSTR lpszFile = _T("123123");
	int nLen = 6;

	if (!OPenSsl_Api_Digest(lpszFile, tszMD5, &nLen))
	{
		return;
	}

	for (int i = 0; i < 16; i++)
	{
		printf(_T("%X"), tszMD5[i]);
	}
	printf(_T("MD5 END\n"));
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

	if (!OPenSsl_Api_CryptEncodec(tszSourceBuffer, tszOutString, &nLen, lpszKey, NETENGINE_OPENSSL_API_CRYPT_SMCBC))
	{
		return;
	}
	printf(_T("加密后的数据长度：%d,数据为：%s\n"), nLen, tszOutString);

	if (!OPenSsl_Api_CryptDecodec(tszOutString, tszDeString, &nLen, lpszKey, NETENGINE_OPENSSL_API_CRYPT_SMCBC))
	{
		return;
	}

	printf(_T("解密后的数据长度：%d,数据为：\n"), nLen);
	printf(_T("%s\n"), tszDeString);
}
void RsaSSL()
{
	LPCTSTR lpszSource = _T("Hello World");
#ifdef _WINDOWS
	LPCTSTR lpszPrivateKey = _T("H:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("H:\\XEngine_Apps\\Debug\\test_pub.Key");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
#endif

	if (!OPenSsl_Api_RsaGenerater(lpszPublicKey, lpszPrivateKey, "123123"))
	{
		return;
	}
	int nRsaLen = strlen(lpszSource);
	UCHAR puszRsaEnString[1024];
	memset(puszRsaEnString, '\0', 1024);
	if (!OPenSsl_Api_RsaEnCodec(lpszPrivateKey, lpszSource, &nRsaLen, puszRsaEnString, FALSE, "123123"))
	{
		return;
	}
	printf("RSA:%s\n", puszRsaEnString);
	TCHAR tszDeString[1024];
	memset(tszDeString, '\0', sizeof(tszDeString));
	if (!OPenSsl_Api_RsaDeCodec(lpszPublicKey, puszRsaEnString, &nRsaLen, tszDeString, TRUE))
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
	TCHAR tszSource[2048];

	memset(tszSource, '\0', sizeof(tszSource));

#ifdef _WINDOWS
	LPCTSTR lpszPrivateKey = _T("H:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("H:\\XEngine_Apps\\Debug\\test_pub.Key");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
#endif

	if (!OPenSsl_Cert_SignEncoder(lpszPrivateKey, lpszPass, lpszSource, tszSource, &nLen, NETENGINE_OPENSSL_API_DIGEST_MD5))
	{
		return;
	}
	if (!OPenSsl_Cert_SignVerifly(lpszPublicKey, lpszPass, tszSource, nLen, lpszSource, 11, NETENGINE_OPENSSL_API_DIGEST_MD5))
	{
		return;
	}
}

void VerSign()
{
#ifdef _WINDOWS
	LPCTSTR lpszPrivateKey = _T("H:\\XEngine_Apps\\Debug\\test.Key");
	LPCTSTR lpszPublicKey = _T("H:\\XEngine_Apps\\Debug\\test_pub.Key");
	LPCTSTR lpszReqFile = _T("H:\\XEngine_Apps\\Debug\\test.csr");
	LPCTSTR lpszCAFile = _T("H:\\XEngine_Apps\\Debug\\ca.crt");
	LPCTSTR lpszCAKey = _T("H:\\XEngine_Apps\\Debug\\ca.Key");
	LPCTSTR lpszUserFile = _T("H:\\XEngine_Apps\\Debug\\test.crt");
#else
	LPCTSTR lpszPrivateKey = _T("test.Key");
	LPCTSTR lpszPublicKey = _T("test_pub.Key");
	LPCTSTR lpszReqFile = _T("test.csr");
	LPCTSTR lpszCAFile = _T("ca.crt");
	LPCTSTR lpszCAKey = _T("ca.Key");
	LPCTSTR lpszUserFile = _T("test.crt");
#endif
	LPCTSTR lpszPass = _T("12345");
	
	_tremove(lpszPrivateKey);
	_tremove(lpszPublicKey);

	if (!OPenSsl_Api_RsaGenerater(lpszPublicKey, lpszPrivateKey, lpszPass))
	{
		return;
	}

	OPENSSL_X509CCINL st_X509Info;
	memset(&st_X509Info, '\0', sizeof(OPENSSL_X509CCINL));

	strcpy(st_X509Info.tszCommonName, "www.xyry.org");
	strcpy(st_X509Info.tszCountryName, "china");
	strcpy(st_X509Info.tszProvinceName, "sichuan");
	strcpy(st_X509Info.tszLocalityName, "chengdu");
	strcpy(st_X509Info.tszOrganizationName, "xyry");
	strcpy(st_X509Info.tszOrgUnitName, "soft");
	strcpy(st_X509Info.tszEmailAddress, "486179@qq.com");

	OPenSsl_Cert_MakeCACert(lpszCAFile, lpszCAKey, "333", 2048, 120102, 60 * 60 * 24 * 10, &st_X509Info);

	OPenSsl_Cert_X509GenRequest(lpszReqFile, &st_X509Info, lpszPrivateKey, lpszPass);
	OPenSsl_Cert_X509SignVer(lpszCAFile, lpszCAKey, "333", lpszReqFile, lpszPass, lpszUserFile, 1100220011, 60 * 60 * 24 * 10, NULL);
}
void CertVer()
{
#ifdef _WINDOWS
	LPCTSTR lpszRootKey = _T("H:\\XEngine_Apps\\Debug\\ca.crt");
	LPCTSTR lpszUserKey = _T("H:\\XEngine_Apps\\Debug\\Debug\\test.crt");
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
#ifdef _WINDOWS
	LPCTSTR lpszKey = _T("H:\\XEngine_Apps\\Debug\\test.crt");
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

	if (!OPenSsl_XCrypto_Encoder(lpszFile, &nLen, tszEncoder, "123"))
	{
		return -1;
	}
	for (int i = 0; i < nLen; i++)
	{
		printf("%02X ", tszEncoder[i]);
	}
	printf("\r\n%s\n", tszEncoder);
		
	OPenSsl_XCrypto_Decoder((LPCSTR)tszEncoder, &nLen, tszDecoder, "123");
	printf("%s\n", tszDecoder);
	return 0;
}

int main()
{
	md5cal();
	Cryptto();
	RsaSSL();
	SignVer();
	VerSign();
	CertVer();
	GetCert();
	XCrypto_Test();
	return 0;
}