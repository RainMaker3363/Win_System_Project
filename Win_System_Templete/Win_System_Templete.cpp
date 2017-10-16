// Win_System_Templete.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
= _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

TCHAR cmdSTring[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" , \t\n");

int CmdReadTokenSize(void);
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);

int tmain(int argc, TCHAR* argv[])
{
	// 한글 입력이 가능하게 하기 위해...
	_tsetlocale(LC_ALL, _T("Korean"));

	SortString();

	// 매개변수 전달인자가 있는 경우 처리를 위한 것이다.
	// Start 명령어를 위한 처리로 생각하면 좋겠다.
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

		// Enter 입력 처리를 위한 부분
		if (tokenNum == 0)
		{
			continue;
		}

		isExit = CmdProcessing(tokenNum);

		if (isExit == true)
		{
			_fputts(_T("명령어 처리를 종료합니다. \n"), stdout);
			break;
		}
	}

    return 0;
}

//==========================================================================
// 함수 : int CmdReadTokenSize(void)
// 기능 : 사용자의 선택을 입력받는 기능과 선택에 따른 명령어 처리 기능을 동시에 지니고 있다.
//		  이에 사용자의 선택을 입력받는 기능을 CmdREadTokenSize 함수로 분리시켰다.
//		  명령어가 main 함수를 통해 전달되는 경우에는 사용자 입력이 불필요하기 때문
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
// 프로세스들의 정보들을 출력하기 위한 함수
// 스냅샷 기능을 이용하여 현재 프로세스의 정보와 그 뒤의 프로세스의 정보들을
// 이름과 ID를 출력시킨다.
//==========================================================================
void ListProcessInfo(void)
{
	// 현재 프로세스의 정보를 캡쳐한다.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHeal32Sanpshot error! \n"));
		return;
	}

	// 프로세스의 정보를 얻기 위한 구조체 변수
	PROCESSENTRY32 pe32;

	// 구조체를 초기화한다.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 첫번째 프로세스 정보 얻음
	if (!Process32First(hProcessSnap, &pe32))
	{
		_tprintf(_T("Process32First error! \n"));
		CloseHandle(hProcessSnap);
		return;
	}

	do
	{
		// 프로세스 이름, ID 정보 출력
		_tprintf(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ProcessID);

	} while (Process32Next(hProcessSnap, &pe32));
	
	CloseHandle(hProcessSnap);
}

//==========================================================================
// 프로세스를 종료 시키는 함수
//==========================================================================
void KillProcess(void)
{
	// 현재 프로세스의 정보를 캡쳐한다.
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolHeal32Sanpshot error! \n"));
		return;
	}

	// 프로세스의 정보를 얻기 위한 구조체 변수
	PROCESSENTRY32 pe32;

	// 구조체를 초기화한다.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// 첫번째 프로세스 정보 얻음
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
		// 입력된 프로세스 이름과 비교한다.
		if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
		{
			// 프로세스 ID를 핸들로 반환하는 과정
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

			if (hProcess != NULL)
			{
				// 프로세스를 강제로 종료시킨다.
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
// 콘솔로 부터 문자열을 읽은 후 리다이렉션을 시키는 함수
//==========================================================================
void SortString(void)
{
	TCHAR stringArr[MAX_STRING_NUM][MAX_STRING_LEN];

	// 콘솔로부터 문자열을 읽어 들인다.
	int nStr;

	for (nStr = 0; nStr < MAX_STRING_NUM; nStr++)
	{
		TCHAR* isEOF = _fgetts(stringArr[nStr], MAX_STRING_LEN, stdin);

		// EOF는 Ctrl + z
		if (isEOF == NULL)
		{
			break;
		}
	}

	// String Bubble Sort
	// 성능 고려하지 않고 문자열 단위 연산
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
// 함수 TCHAR int CmdProcessing(int)
// 기능 : 명령어를 입력 받아서 해당 명렁어에 지정되어 있는 기능을 수행한다.
//			exit가 입력되면 TRUE를 반환하고 이는 프로그램의 종료로 이어진다.
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
		// "start echo test good" 이라는 문자열이 입력되었다고 가정하자.
		// 이 프로그램의 실행파일 이름이 CmdProject.exe라고 가정한다

		// "start echo test good"과 같은 입력 처리를 위해..
		if (tokenNum > 1)
		{
			// start를 제외한 나머지 문자열을 재 구성한다.
			// 문자열 "echo test good"을 만든다.
			for (int i = 1; i < tokenNum; i++)
			{
				_stprintf(optString, _T("%s %s"), optString, cmdTokenList[i]);
				_stprintf(optString, _T("%s %s"), _T("CmdProject.exe"), optString);
			}
		}
		// "start" 입력만 하는 경우..
		else
		{
			_stprintf(cmdStringWithOption, _T("%s"), _T("CmdProject.exe"));
		}

		isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL,
			TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

		// 자식 프로세스의 핸들을 닫아 Usage(참조 카운트)를 줄여준다.
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	else if (!_tcscmp(cmdTokenList[0], _T("echo")))
	{
		// 입력한 문자열을 그대로 출력하는 echo 명령어
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

			// 상속이 가능해야 지정 가능하다
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

		// Check Point 1 : 문자열 처리를 위한 for 문
		for (int i = 1; i < tokenNum; i++)
		{
			_stprintf(cmdStringWithOption, _T("%s %s"), cmdStringWithOption, cmdTokenList[i]);
		}

		
		isRun = CreateProcess(NULL, cmdStringWithOption, NULL, NULL,
			TRUE, 0, NULL, NULL, &si, &pi);

		// 자식 프로세스의 핸들을 닫아 Usage(참조 카운트)를 줄여준다.
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
// 함수 TCHAR* StrLower(TCHAR*);
// 기능 : 문자열의 내에 존재하는 모든 대문자를 소문자로 변경한다
//			변경된 문자열의 포인터를 반환한다.
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