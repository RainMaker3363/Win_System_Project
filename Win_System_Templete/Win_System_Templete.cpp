// Win_System_Templete.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <locale.h>
#include <Windows.h>
#include <TlHelp32.h>


#define STR_LEN 256
#define CMD_TOKEN_NUM 10

#define MAX_STRING_NUM 100
#define MAX_STRING_LEN 256

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

	SortString();

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
// ���μ������� �������� ����ϱ� ���� �Լ�
// ������ ����� �̿��Ͽ� ���� ���μ����� ������ �� ���� ���μ����� ��������
// �̸��� ID�� ��½�Ų��.
//==========================================================================
void ListProcessInfo(void)
{
	// ���� ���μ����� ������ ĸ���Ѵ�.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHeal32Sanpshot error! \n"));
		return;
	}

	// ���μ����� ������ ��� ���� ����ü ����
	PROCESSENTRY32 pe32;

	// ����ü�� �ʱ�ȭ�Ѵ�.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// ù��° ���μ��� ���� ����
	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	do
	{
		// ���μ��� �̸�, ID ���� ���
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID);

	} while (Process32Next(hProcessSnap, &pe32));
	
	CloseHandle(hProcessSnap);
}

//==========================================================================
// ���μ����� ���� ��Ű�� �Լ�
//==========================================================================
void KillProcess(void)
{
	// ���� ���μ����� ������ ĸ���Ѵ�.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHeal32Sanpshot error! \n"));
		return;
	}

	// ���μ����� ������ ��� ���� ����ü ����
	PROCESSENTRY32 pe32;

	// ����ü�� �ʱ�ȭ�Ѵ�.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// ù��° ���μ��� ���� ����
	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}
	
	HANDLE hProcess;
	BOOL isKill = FALSE;

	do
	{
		// �Էµ� ���μ��� �̸��� ���Ѵ�.
		if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			// ���μ��� ID�� �ڵ�� ��ȯ�ϴ� ����
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			if (hProcess != NULL)
			{
				// ���μ����� ������ �����Ų��.
				TerminateProcess(hProcess, -1);
				isKill = TRUE;
			}

			CloseHandle(hProcess);
			break;
		}
	} while (Process32Next(hProcessSnap, &pe32));
	
	CloseHandle(hProcessSnap);

	if (isKill == FALSE)
	{
		_tprintf(_T("Kill Process Fail, Try Agian! \n"));
	}
}

//==========================================================================
// �ַܼ� ���� ���ڿ��� ���� �� �����̷����� ��Ű�� �Լ�
//==========================================================================
void SortString(void)
{
	TCHAR stringArr[MAX_STRING_NUM][MAX_STRING_LEN];

	// �ַܼκ��� ���ڿ��� �о� ���δ�.
	int nStr;

	for (nStr = 0; nStr < MAX_STRING_NUM; nStr++)
	{
		TCHAR* isEOF = _fgetts(stringArr[nStr], MAX_STRING_LEN, stdin);

		// EOF�� Ctrl + z
		if (isEOF == NULL)
		{
			break;
		}
	}

	// String Bubble Sort
	// ���� ������� �ʰ� ���ڿ� ���� ����
	TCHAR strTemp[MAX_STRING_LEN];

	for (int i = 0; i < nStr; i++)
	{
		for (int j = nStr - 1; j > i; j--)
		{
			if (_tcscmp(stringArr[j - 1], stringArr[j]) > 0)
			{
				_tcscpy(strTemp, stringArr[j - 1]);
				_tcscpy(stringArr[j - 1], stringArr[j]);
				_tcscpy(stringArr[j], strTemp);
			}
		}
	}

	for (int i = 0; i < nStr; i++)
	{
		_fputts(stringArr[i], stdout);
	}
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
	else if (!_tcscmp(cmdTokenList[0], _T("lp")))
	{
		ListProcessInfo();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kp")))
	{
		if (tokenNum < 2)
		{
			_tprintf(_T("usage : kp <process Name> \n"));
			return 0;
		}

		KillProcess();
	}
	else if (!_tcscmp(cmdTokenList[0], _T("kp")))
	{
		if (!_tcscmp(cmdTokenList[1], _T(">")))
		{
			SECURITY_ATTRIBUTES fileSec = {
				sizeof(SECURITY_ATTRIBUTES), NULL, TRUE
			};

			// ����� �����ؾ� ���� �����ϴ�
			HANDLE hFile = CreateFile(
				cmdTokenList[2], GENERIC_WRITE, FILE_SHARE_READ,
				&fileSec, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL
			);

			si.hStdOutput = hFile;
			si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
			si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
			si.dwFlags |= STARTF_USESTDHANDLES;

			isRun = CreateProcess(
				NULL, cmdTokenList[0], NULL, NULL,
				TRUE, 0, NULL, NULL, &si, &pi
			);

			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(hFile);
		}
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