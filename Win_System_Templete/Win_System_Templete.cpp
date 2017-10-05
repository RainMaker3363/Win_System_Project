// Win_System_Templete.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <locale.h>
#include <Windows.h>


#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[]
= _T("'%s'��(��) ������ �� �ִ� ���α׷��� �ƴմϴ�. \n");

TCHAR cmdSTring[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" , \t\n");

int CmdReadTokenSize(void);
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);

int tmain(int argc, TCHAR* argv[])
{
	// �ѱ� �Է��� �����ϰ� �ϱ� ����...
	_tsetlocale(LC_ALL, _T("Korean"));

	// �Ű����� �������ڰ� �ִ� ��� ó���� ���� ���̴�.
	// Start ��ɾ ���� ó���� �����ϸ� ���ڴ�.
	if (argc > 2)
	{
		for (int i = 1; i < argc; i++)
		{
			_tcscpy(cmdTokenList[i - 1], argv[i]);
		}

		CmdProcessing(argc - 1);
	}

	DWORD isExit;
	while (1)
	{
		int tokenNum = CmdReadTokenSize();

		// Enter �Է� ó���� ���� �κ�
		if (tokenNum == 0)
		{
			continue;
		}

		isExit = CmdProcessing(tokenNum);

		if (isExit == true)
		{
			_fputts(_T("��ɾ� ó���� �����մϴ�. \n"), stdout);
			break;
		}
	}

    return 0;
}

//==========================================================================
// �Լ� : int CmdReadTokenSize(void)
// ��� : ������� ������ �Է¹޴� ��ɰ� ���ÿ� ���� ��ɾ� ó�� ����� ���ÿ� ���ϰ� �ִ�.
//		  �̿� ������� ������ �Է¹޴� ����� CmdREadTokenSize �Լ��� �и����״�.
//		  ��ɾ main �Լ��� ���� ���޵Ǵ� ��쿡�� ����� �Է��� ���ʿ��ϱ� ����
//==========================================================================
int CmdReadTokenSize(void)
{
	TCHAR* token;

	_fputts(_T("Best Command Prompt >> "), stdout);
	_getts_s(cmdSTring);

	token = _tcstok(cmdSTring, seps);

	int tokenNum = 0;

	while (token != NULL)
	{
		_tcscpy(cmdTokenList[tokenNum++], StrLower(token));
		token = _tcstok(NULL, seps);
	}

	return tokenNum;
}

//==========================================================================
// �Լ� TCHAR int CmdProcessing(int)
// ��� : ��ɾ �Է� �޾Ƽ� �ش� ��� �����Ǿ� �ִ� ����� �����Ѵ�.
//			exit�� �ԷµǸ� TRUE�� ��ȯ�ϰ� �̴� ���α׷��� ����� �̾�����.
//==========================================================================
int CmdProcessing(int tokenNum)
{
	BOOL isRun;
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;

	TCHAR cmdStringWithOption[STR_LEN] = { 0, };
	TCHAR optString[STR_LEN] = { 0, };

	si.cb = sizeof(si);

	if (!_tcscmp(cmdTokenList[0], _T("exit")))
	{
		return true;
	}
	else if (!_tcscmp(cmdTokenList[0], _T("start")))
	{
		// "start echo test good" �̶�� ���ڿ��� �ԷµǾ��ٰ� ��������.
		// �� ���α׷��� �������� �̸��� CmdProject.exe��� �����Ѵ�

		// "start echo test good"�� ���� �Է� ó���� ����..
		if (tokenNum > 1)
		{
			// start�� ������ ������ ���ڿ��� �� �����Ѵ�.
			// ���ڿ� "echo test good"�� �����.
			for (int i = 1; i < tokenNum; i++)
			{
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
				_stprintf(optString, _T("%s %s"), _T("CmdProject.exe"), optString);
			}
		}
		// "start" �Է¸� �ϴ� ���..
		else
		{
			_stprintf(cmdStringWithOption, _T("%s"), _T("CmdProject.exe"));
		}

		isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL,
			TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		// �ڽ� ���μ����� �ڵ��� �ݾ� Usage(���� ī��Ʈ)�� �ٿ��ش�.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		// �Է��� ���ڿ��� �״�� ����ϴ� echo ��ɾ�
		for (int i = 1; i < tokenNum; i++)
		{
			_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);	
		}

		_tprintf(_T("echo Message : %s \n"), optString);
	}
	else
	{
		_tcscpy(cmdStringWithOption, cmdTokenList[0]);

		// Check Point 1 : ���ڿ� ó���� ���� for ��
		for (int i = 1; i < tokenNum; i++)
		{
			_stprintf(cmdStringWithOption, _T("%s %s"), cmdStringWithOption, cmdTokenList[i]);
		}

		
		isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL,
			TRUE, 0, NULL, NULL, &si, &pi);

		// �ڽ� ���μ����� �ڵ��� �ݾ� Usage(���� ī��Ʈ)�� �ٿ��ش�.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		if (isRun == FALSE)
		{
			_tprintf(ERROR_CMD, cmdTokenList[0]);
		}
		
	}

	return 0;
}

//==========================================================================
// �Լ� TCHAR* StrLower(TCHAR*);
// ��� : ���ڿ��� ���� �����ϴ� ��� �빮�ڸ� �ҹ��ڷ� �����Ѵ�
//			����� ���ڿ��� �����͸� ��ȯ�Ѵ�.
//==========================================================================
TCHAR* StrLower(TCHAR* pStr)
{
	TCHAR* ret = pStr;

	while (*pStr)
	{
		if (_istupper(*pStr))
		{
			*pStr = _totlower(*pStr);
			
			pStr++;
		}
	}

	return ret;
}