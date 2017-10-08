#include "stdafx.h"

// 메일슬롯 Receiver

//#include <Windows.h>
//
// '.'을 사용한 이유는 로컬 컴퓨터에 작동시키기 위함이다.
//#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")
//
//int _tmain(int argc, TCHAR* argv[])
//{
//	HANDLE hMailSlot;	// mailslot 핸들
//	TCHAR messageBox[50];
//	DWORD byteRead;	// 읽어온 바이트의 수
//
//	// MailSlot 생성
//	hMailSlot = CreateMailslot(SLOT_NAME,
//		0,
//		MAILSLOT_WAIT_FOREVER,
//		NULL);
//
//	if (hMailSlot == INVALID_HANDLE_VALUE)
//	{
//		_fputts(_T("Unable to create MailSlot!\n"), stdout);
//		return 1;
//	}
//
//	// Message 수신
//	_fputts(_T("****** Message ******\n"), stdout);
//
//	while (1)
//	{
//		if (!ReadFile(hMailSlot, messageBox, sizeof(TCHAR) * 50, &byteRead, NULL))
//		{
//			_fputts(_T("Unable to Read!\n"), stdout);
//			CloseHandle(hMailSlot);
//			return 1;
//		}
//		
//		if (!_tcsncmp(messageBox, _T("exit"), 4))
//		{
//			_fputts(_T("Good Bye!\n"), stdout);
//			break;
//		}
//		
//		// NULL 문자 삽입
//		messageBox[byteRead / sizeof(TCHAR)] = 0;
//		_fputts(messageBox, stdout);
//	}
//	
//	CloseHandle(hMailSlot);
//	return 0;
//}