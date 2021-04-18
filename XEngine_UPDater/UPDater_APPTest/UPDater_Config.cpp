#include "UPDater_Hdr.h"

BOOL UPDater_Parament(int argc, char **argv, UPDATERSERVICE_CONFIG *pSt_StartlParam)
{
	pSt_StartlParam->nLogType = 2;
	pSt_StartlParam->bIsDown = 0;
	pSt_StartlParam->bIsMake = FALSE;
	_tcscpy(pSt_StartlParam->tszUPUrl, "http://www.xyry.org/UPLoad/XEngine_Release.txt");
#ifdef _WINDOWS
	_tcscpy(pSt_StartlParam->tszLocalList, "H:\\XEngine_Apps\\Debug\\XEngine_Release.list");
	_tcscpy(pSt_StartlParam->tszMakePath, "H:\\XEngine\\XEngine_Release\\");
	_tcscpy(pSt_StartlParam->tszDownPath, "H:\\XEngine_Apps\\Debug\\UPFile\\");
#else
	_tcscpy(pSt_StartlParam->tszLocalList, "./XEngine_Release.list");
	_tcscpy(pSt_StartlParam->tszMakePath, "./Debug");
	_tcscpy(pSt_StartlParam->tszDownPath, "./UPFile/");
#endif

	for (int i = 0;i < argc;i++)
	{
		if ((0 == _tcscmp("-h", argv[i])) || (0 == _tcscmp("-H", argv[i])))
		{
			UPDater_ParamentHelp();
			return FALSE;
		}
		if ((0 == _tcscmp("-v", argv[i])) || (0 == _tcscmp("-V", argv[i])))
		{
			printf("Version��V1.1.0\n");
			return FALSE;
		}
		else if (0 == _tcscmp("-u", argv[i]))
		{
			memset(pSt_StartlParam->tszUPUrl,'\0',sizeof(pSt_StartlParam->tszUPUrl));
			_tcscpy_s(pSt_StartlParam->tszUPUrl, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-f", argv[i]))
		{
			memset(pSt_StartlParam->tszLocalList, '\0', sizeof(pSt_StartlParam->tszLocalList));
			_tcscpy_s(pSt_StartlParam->tszLocalList, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-d", argv[i]))
		{
			memset(pSt_StartlParam->tszDownPath, '\0', sizeof(pSt_StartlParam->tszDownPath));
			_tcscpy_s(pSt_StartlParam->tszDownPath, MAX_PATH, argv[i + 1]);
		}
		else if (0 == _tcscmp("-l", argv[i]))
		{
			pSt_StartlParam->nLogType = _ttoi(argv[i + 1]);
		}
		else if (0 == _tcscmp("-down", argv[i]))
		{
			pSt_StartlParam->bIsDown = _ttoi(argv[i + 1]);
		}
	}

	return TRUE;
}

void UPDater_ParamentHelp()
{
	printf(_T("--------------------------��������������ʼ--------------------------\n"));
	printf(_T("���·����������������� ���� ����ֵ�����������ִ�Сд��\n"));
	printf(_T("-h or -H����������������ʾ��Ϣ\n"));
	printf(_T("-v or -V���鿴ϵͳ�汾\n"));
	printf(_T("-u�����ü�������Ϣ��URL��ַ,Ĭ��:http://www.xyry.org/UPLoad/NetEngine_WINVer.txt\n"));
	printf(_T("-f�����ñ����ļ��б�Ŀ¼,Ĭ��:./LocalFile_List.txt\n"));
	printf(_T("-d�����ø����ļ����ص�Ŀ¼,Ĭ��:./UPFile/\n"));
	printf(_T("-m���������ذ汾�����б���Ҫ��������Ҫ��������汾��Ŀ¼,Ŀ¼Ĭ����Ҫ���/����.\n"));
	printf(_T("-l�����ñ�����־��¼��Ϣ,Ĭ��Ϊ2,ʹ���ļ��Ϳ���̨\n"));
	printf(_T("-d���������°汾�Ƿ������ļ�,Ĭ��0������,1λ����\n"));
	printf(_T("--------------------------����������������--------------------------\n"));
}