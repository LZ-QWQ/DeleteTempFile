// test.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <Locale.h>
#include <Windows.h>
#define UNICODE_HEAD_FLAG 0xFEFF

int _tmain()
{
	_wsetlocale(LC_ALL, L"chs");
	bool why;
	wchar_t a[] = L"D:\\alltemp.txt\r\n";
	wchar_t b[MAX_PATH];
	PVOID d;
	DWORD temp=-1;
	DWORD c;
	HANDLE test;

	test = CreateFile(L"D:\\test.txt",
		GENERIC_WRITE|GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);


	if (test == INVALID_HANDLE_VALUE)
	{
		test = CreateFile(L"D:\\test.txt",
			GENERIC_WRITE|GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);

		WORD dword_UnicodeFlag = UNICODE_HEAD_FLAG;
		WriteFile(test,
			&dword_UnicodeFlag,
			sizeof(WORD),
			&temp,
			NULL);
	}

    SetFilePointer(test, 2,NULL, FILE_BEGIN);


	WriteFile(
		test,
		a,
		wcslen(a) * sizeof(wchar_t),
		&temp,
		NULL);

	wprintf_s(L"%d \r\n", temp);
	DWORD temp_;
	temp_=GetFileSize(test,NULL);
	wprintf_s(L"%d \r\n", temp_);

	SetFilePointer(test, 2, NULL, FILE_BEGIN);
	
	ReadFile(test,b,wcslen(a)*sizeof(wchar_t), &temp, NULL);
	b[temp/2] = L'\0';
	wprintf_s(L"%ls", a);
	wprintf_s(L"%ls has %d %d\n", b, temp,wcsstr(b,L".txt\r\n"));
	CloseHandle(test);
	DeleteFile(b);
	c = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		c,
		LANG_NEUTRAL,//MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)
		(LPWSTR)&d,
		0,
		NULL);

	wprintf_s(L"%s\r\n", d);

	getwchar();

    return 0;
}

