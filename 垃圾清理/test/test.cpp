// test.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <Locale.h>
#include <Windows.h>
#define UNICODE_HEAD_FLAG 0xFEFF

int _tmain()
{
	_wsetlocale(LC_ALL, L"chs");
	HANDLE test;
	test = CreateFile(L"D:\\alltemp_C.txt",
		GENERIC_WRITE|GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (test == INVALID_HANDLE_VALUE)
	{
		test = CreateFile(L"D:\\alltemp_C.txt",
			GENERIC_WRITE|GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
	}
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);
	wprintf_s(L"%d %ls", dw, lpMsgBuf);
	DWORD x=2;
	DWORD dword_NumberOfBytesToRead = 2;
	DWORD dword_NumberOfBytesToRead_end;
	DWORD z = GetFileSize(test, NULL);
	wchar_t wchar_DeletePath[MAX_PATH] = { '\0' };
	do
	{

		do
		{
			SetFilePointer(test, x, 0, FILE_BEGIN);


			ReadFile(test,
				wchar_DeletePath,
				dword_NumberOfBytesToRead,
				&dword_NumberOfBytesToRead_end,
				NULL);

			dword_NumberOfBytesToRead += 2;
		} while (wcsstr(wchar_DeletePath, L".tmp") == NULL);
		wprintf_s(L"\r\n%ls", wchar_DeletePath);
		wchar_DeletePath[dword_NumberOfBytesToRead_end / 2] = '\0';
		DeleteFile(wchar_DeletePath);
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);
		wprintf_s(L"%d %ls", dw,lpMsgBuf);
		x += dword_NumberOfBytesToRead_end+4;
	} while (x !=z);
	CloseHandle(test);

	getwchar();

    return 0;
}

