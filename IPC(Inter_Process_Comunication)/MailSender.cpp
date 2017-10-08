#include "stdafx.h"

// 메인 스롯 Sender
#include <Windows.h>

// *을 사용하면 모든 Receiver에게 브로드캐스트 방식으로 보낸다.
#define SLOT_NAME _T("\\\\*\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;	// mailslot 핸들
	TCHAR message[50];
	DWORD byteRead;	// 읽어온 바이트의 수

					// MailSlot 생성
	hMailSlot = CreateFile(
		SLOT_NAME,		// 메일 슬롯의 이름 (주소)
		GENERIC_WRITE,	// 사용되는 용도
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,	// 생성 방식
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hMailSlot == INVALID_HANDLE_VALUE)
	{
		_fputts(_T("Unable to create MailSlot!\n"), stdout);
		return 1;
	}


	while (1)
	{

		
		_fputts(_T("MY CMD >>"), stdout);
		_fgetts(message, sizeof(message) / sizeof(TCHAR), stdin);


		if (!WriteFile(hMailSlot, message, sizeof(TCHAR) * _tcslen(message), &byteRead, NULL))
		{
			_fputts(_T("Unable to Write!\n"), stdout);
			CloseHandle(hMailSlot);
			return 1;
		}

		if (!_tcsncmp(message, _T("exit"), 4))
		{
			_fputts(_T("Good Bye!\n"), stdout);
			break;
		}
	}

	CloseHandle(hMailSlot);
	return 0;
}