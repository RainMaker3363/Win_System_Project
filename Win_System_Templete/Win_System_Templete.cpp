// Win_System_Templete.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <locale.h>
#include <Windows.h>


#define STR_LEN 256
#define CMD_TOKEN_NUM 10

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