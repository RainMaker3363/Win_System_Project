#include "stdafx.h"

// ���� ���� Sender
#include <Windows.h>

// *�� ����ϸ� ��� Receiver���� ��ε�ĳ��Ʈ ������� ������.
#define SLOT_NAME _T("\\\\*\\mailslot\\mailbox")

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hMailSlot;	// mailslot �ڵ�
	TCHAR message[50];
	DWORD byteRead;	// �о�� ����Ʈ�� ��

					// MailSlot ����
	hMailSlot = CreateFile(
		SLOT_NAME,		// ���� ������ �̸� (�ּ�)
		GENERIC_WRITE,	// ���Ǵ� �뵵
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,	// ���� ���
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