#include "stdafx.h"
#include "Read_DeleteFile.h"

bool fun_AskUserIsDelete(void)
{
	wchar_t wchar_YesOrNo;

	wprintf(L"\r\n是否需要删除Windows操作系统垃圾文件(按下y键进行扫描或者q键退出程序)？ ");
	do
	{
		wscanf_s(L"%lc", &wchar_YesOrNo, 1);
		getwchar();
	} while ((wchar_YesOrNo != L'y') && (wchar_YesOrNo != L'q'));

	if (wchar_YesOrNo == L'q')
	{
		return false;
	}

	return true;

}


bool fun_ReadAndDeleteFile(const wchar_t *pwchar_param_SaveFile, 
	DWORD dword_PointOfBejinToRead, 
	DWORD dword_SizeOfFile)
{
	HANDLE handle_File;
	DWORD dword_NumberOfBytesToRead = 1;
	DWORD dword_NumberOfBytesToRead_end = 0;
	wchar_t wchar_DeletePath[MAX_PATH];

	handle_File = CreateFile(pwchar_param_SaveFile,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	do
	{
		if (dword_PointOfBejinToRead * sizeof(wchar_t) == dword_SizeOfFile)
			return false;

		SetFilePointer(handle_File, dword_PointOfBejinToRead * sizeof(wchar_t), 0, FILE_BEGIN);


		ReadFile(handle_File,
			wchar_DeletePath,
			dword_NumberOfBytesToRead,
			&dword_NumberOfBytesToRead_end,
			NULL);

		dword_NumberOfBytesToRead++;
	} while (wcsstr(wchar_DeletePath,L".tmp\r\n")== NULL);

	wchar_DeletePath[dword_NumberOfBytesToRead_end/2-2] = '\0';

	if (DeleteFile(wchar_DeletePath))
		return true;
	else
		return false;

}