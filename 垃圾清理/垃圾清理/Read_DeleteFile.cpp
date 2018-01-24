#include "stdafx.h"
#include "Read_DeleteFile.h"

bool fun_AskUserIsDelete(void)
{
	wchar_t wchar_YesOrNo;
	fun_Conlose_Printf_Pos(0,
		ROW_JUNKINFO+6,
		L"\r\n是否需要删除Windows操作系统垃圾文件(按下y键进行扫描或者q键退出程序)？ ");
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

/*
参数1：输出删除状态命令行行坐标
参数2：文件开始读取位置（偏移量）
参数3：保存路径的文件句柄
参数4：命令行输出删除信息的回调函数
*/
void fun_ReadAndDeleteFile(	short short_param_CmdRow_ShowDeleteInfo,
	LPDWORD dword_param_BeginToRead,
	HANDLE handle_param_File,
	FUNCB_SHOWDELETEINFO funcb_param_Delete)
{

	DWORD dword_NumberOfBytesToRead = 2;//读取字节数
	DWORD dword_NumberOfBytesToRead_end;//实际读取字节数
	wchar_t wchar_DeletePath[MAX_PATH] = { '\0' };


	do
	{
		SetFilePointer(handle_param_File, (*dword_param_BeginToRead), 0, FILE_BEGIN);


		ReadFile(handle_param_File,
			wchar_DeletePath,
			dword_NumberOfBytesToRead,
			&dword_NumberOfBytesToRead_end,
			NULL);

		dword_NumberOfBytesToRead += 2;
	} while (wcsstr(wchar_DeletePath,L".tmp\r\n")== NULL||wcsstr(wchar_DeletePath,L".TMP\r\n")==NULL);//判断是否读取到完整文件路径

	wchar_DeletePath[(dword_NumberOfBytesToRead_end/2)-2] = '\0';//截断\r\n
	(*dword_param_BeginToRead) += (dword_NumberOfBytesToRead_end);

	if (DeleteFile(wchar_DeletePath))
	{
		funcb_param_Delete(DELETEINFO_TYPE_DELETEFILECOUNT, short_param_CmdRow_ShowDeleteInfo);
		return;
	}
	else
	{
		funcb_param_Delete(DELETEINFO_TYPE_FAILTODELETECOUNT, short_param_CmdRow_ShowDeleteInfo);
		return;
	}

}

/*
外部循环代码（线程）

DWORD dword_SizeOfFile;    （全文件字节数）
DWORD dword_BeginToRead = 2;   （把Unicode字符标识略过）
HANDLE handle_File;

handle_File = CreateFile(pstruct_DeleteParam->wchar_SaveFile,
GENERIC_READ,
0,
NULL,
OPEN_EXISTING,
FILE_ATTRIBUTE_NORMAL,
NULL);

dword_SizeOfFile = GetFileSize(handle_File, NULL);

do
{
fun_ReadAndDeleteFile(pstruct_DeleteParam->short_CmdRow_ShowDeleteInfo, &dword_BeginToRead,
handle_File, pstruct_DeleteParam->funcb_Show_Delete);
} while (dword_BeginToRead != dword_SizeOfFile);     （判断是否读完整个文件）


*/