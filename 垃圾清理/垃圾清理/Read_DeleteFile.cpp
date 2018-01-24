#include "stdafx.h"
#include "Read_DeleteFile.h"

bool fun_AskUserIsDelete(void)
{
	wchar_t wchar_YesOrNo;
	fun_Conlose_Printf_Pos(0,
		ROW_JUNKINFO+6,
		L"\r\n�Ƿ���Ҫɾ��Windows����ϵͳ�����ļ�(����y������ɨ�����q���˳�����)�� ");
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
����1�����ɾ��״̬������������
����2���ļ���ʼ��ȡλ�ã�ƫ������
����3������·�����ļ����
����4�����������ɾ����Ϣ�Ļص�����
*/
void fun_ReadAndDeleteFile(	short short_param_CmdRow_ShowDeleteInfo,
	LPDWORD dword_param_BeginToRead,
	HANDLE handle_param_File,
	FUNCB_SHOWDELETEINFO funcb_param_Delete)
{

	DWORD dword_NumberOfBytesToRead = 2;//��ȡ�ֽ���
	DWORD dword_NumberOfBytesToRead_end;//ʵ�ʶ�ȡ�ֽ���
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
	} while (wcsstr(wchar_DeletePath,L".tmp\r\n")== NULL||wcsstr(wchar_DeletePath,L".TMP\r\n")==NULL);//�ж��Ƿ��ȡ�������ļ�·��

	wchar_DeletePath[(dword_NumberOfBytesToRead_end/2)-2] = '\0';//�ض�\r\n
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
�ⲿѭ�����루�̣߳�

DWORD dword_SizeOfFile;    ��ȫ�ļ��ֽ�����
DWORD dword_BeginToRead = 2;   ����Unicode�ַ���ʶ�Թ���
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
} while (dword_BeginToRead != dword_SizeOfFile);     ���ж��Ƿ���������ļ���


*/