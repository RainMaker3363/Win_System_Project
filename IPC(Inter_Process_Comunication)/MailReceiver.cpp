#include "stdafx.h"

// ���Ͻ��� Receiver

//#include <Windows.h>
//
// '.'�� ����� ������ ���� ��ǻ�Ϳ� �۵���Ű�� �����̴�.
//#define SLOT_NAME _T("\\\\.\\mailslot\\mailbox")
//
//int _tmain(int argc, TCHAR* argv[])
//{
//	HANDLE hMailSlot;	// mailslot �ڵ�
//	TCHAR messageBox[50];
//	DWORD byteRead;	// �о�� ����Ʈ�� ��
//
//	// MailSlot ����
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
//	// Message ����
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
//		// NULL ���� ����
//		messageBox[byteRead / sizeof(TCHAR)] = 0;
//		_fputts(messageBox, stdout);
//	}
//	
//	CloseHandle(hMailSlot);
//	return 0;
//}