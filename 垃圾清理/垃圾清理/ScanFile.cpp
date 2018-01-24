#include "stdafx.h"
#include "ScanFile.h"


bool fun_AskUserIsScan(void)
{
	wchar_t wchar_YesOrNo;

	wprintf(L"�Ƿ���Ҫɨ��Windows����ϵͳ�����ļ�(����y������ɨ�����q���˳�����)�� ");
	do
	{
		wscanf_s(L"%lc", &wchar_YesOrNo, 1);
		getwchar(); // ���ջس�����
	} while ((wchar_YesOrNo != L'y') && (wchar_YesOrNo != L'q'));

	if (wchar_YesOrNo == L'q')
	{
		return false;
	}

	return true;

}



void fun_EnumDirectory_listedTempFile(const wchar_t *pwchar_param_RootPath,
	const wchar_t *pwchar_param_SaveFile,
	short short_param_CmdRow_ShowScanInfo,
	FUNCB_SHOWSCANINFO funcb_param_Show)
{
	wchar_t wchar_DirPath[MAX_PATH] = { '\0' }; // Ŀ¼·��������
	wchar_t wchar_SubDirPath[MAX_PATH] = { '\0' }; // ��Ŀ¼·��������
	wchar_t wchar_TempFilePath[MAX_PATH] = { '\0' }; // �����ļ���ȫ·��
	size_t size_Length_of_RootPath;                // Ŀ¼·����ʵ�ʳ���

	WIN32_FIND_DATA strcut_Find; // ö���ļ�ʱ��Ҫ�����ݽṹ��    
	HANDLE handle_FindFile = INVALID_HANDLE_VALUE; // �ļ����

												   // У�鴫�ݽ����Ĳ����Ƿ�Ϸ�
	if ((pwchar_param_RootPath == NULL) ||
		(wcslen(pwchar_param_RootPath) == 0))
	{
		wprintf_s(L"�������Ϸ�");
		return;
	}

	// ��鴫��ĸ�Ŀ¼·���Ƿ����
	// MAX_PATH-3 ��ʾ��Ҫ����3���ַ��������ַ���"\*"����ƴ�ӣ�
	// �������ܽ���ö��Ŀ¼
	StringCchLength(pwchar_param_RootPath,
		MAX_PATH,
		&size_Length_of_RootPath);
	if (size_Length_of_RootPath > (MAX_PATH - 4))
	{
		wprintf_s(L"��Ŀ¼·��̫��");
		return;
	}

	// ����·�������ַ���"\*"����ƴ�ӣ��γ����¸�ʽ
	// x:\xxxx\*
	StringCchCopy(wchar_DirPath, MAX_PATH, pwchar_param_RootPath);
	if (pwchar_param_RootPath[wcslen(pwchar_param_RootPath) - 1] == '\\')
	{
		StringCchCat(wchar_DirPath, MAX_PATH, L"*.*");
	}
	else
	{
		StringCchCat(wchar_DirPath, MAX_PATH, L"\\*.*");
	}


	// ��ʼִ��ö��Ŀ��Ŀ¼
	handle_FindFile = FindFirstFile(wchar_DirPath, &strcut_Find);
	if (handle_FindFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	// ����do~whileѭ������ö�ٲ���
	do
	{
		if (strcut_Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// ��Ŀ¼(�����"."��".."��2������Ŀ¼����Ҫ����ö��)
			if ((wcscmp(strcut_Find.cFileName, L".") != 0) &&
				(wcscmp(strcut_Find.cFileName, L"..") != 0))
			{
				// ƴ���µ���Ŀ¼·�����γ����¸�ʽ
				// x:\xxxx\xxx
				StringCchCopy(wchar_SubDirPath,
					MAX_PATH,
					pwchar_param_RootPath);
				if (wchar_SubDirPath[wcslen(wchar_SubDirPath) - 1] != '\\')
				{
					StringCchCat(wchar_SubDirPath, MAX_PATH, L"\\");
				}
				StringCchCat(wchar_SubDirPath,
					MAX_PATH,
					strcut_Find.cFileName);


				// ֪ͨ�������̨��ӡɨ����Ϣ(����Ŀ¼��Ŀ)
				(funcb_param_Show)(SCANINFO_TYPE_DIRCOUNT,
					short_param_CmdRow_ShowScanInfo);



				// �ݹ�ö�ٵ�ǰ�µ���Ŀ¼
				fun_EnumDirectory_listedTempFile(wchar_SubDirPath,
					pwchar_param_SaveFile,
					short_param_CmdRow_ShowScanInfo,
					funcb_param_Show);
			}
		}
		else
		{
			// �ļ�(�ж��Ƿ��������ļ�.tmp)
			if (fun_IsTempFiles(strcut_Find.cFileName) == true)
			{
				// ֪ͨ�������̨��ӡɨ����Ϣ(���������ļ���Ŀ)
				(funcb_param_Show)(SCANINFO_TYPE_TMPFILECOUNT,
					short_param_CmdRow_ShowScanInfo);

				// ƴ�������ļ���ȫ·��������·����Ϣ���浽ָ���ļ�
				if (StringCchPrintf(wchar_TempFilePath,
					_countof(wchar_TempFilePath),
					L"%ls\\%ls\r\n", // ƴ��ȫ·��
					pwchar_param_RootPath,
					strcut_Find.cFileName) == S_OK)
				{
					fun_SaveTempFilesInfoToFile(pwchar_param_SaveFile,
						wchar_TempFilePath,
						wcslen(wchar_TempFilePath) * sizeof(wchar_t));
				}

			}


			// ֪ͨ�������̨��ӡɨ����Ϣ(�����ļ���Ŀ)
			(funcb_param_Show)(SCANINFO_TYPE_FILECOUNT,
				short_param_CmdRow_ShowScanInfo);

		}

	} while (FindNextFile(handle_FindFile, &strcut_Find) != 0);

	FindClose(handle_FindFile);

	return;

}



bool fun_IsTempFiles(const wchar_t *pwchar_param_FileName)
{
	wchar_t *pwchar_Ext = PathFindExtension(pwchar_param_FileName);

	if ((pwchar_Ext == NULL) ||
		wcslen(pwchar_Ext) == 0)
	{
		return false;
	}

	// �ж��Ƿ���.tmp��׺��
	if (_wcsicmp(pwchar_Ext, L".tmp") == 0)
	{
		return true;
	}

	return  false;

}



bool fun_SaveTempFilesInfoToFile(const wchar_t *pwchar_param_SaveFile,
	const wchar_t *pwchar_param_Buffer,
	DWORD dword_param_BufferSize)
{
	HANDLE handle_File = NULL;

	DWORD dword_NumberOfBytesToWrite = 0;

	// ͨ��CreateFile()������OPEN_EXISTING������־λ�ж��ļ��Ƿ���ڣ�
	// ����������򴴽�
	handle_File = CreateFile(pwchar_param_SaveFile,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (handle_File == INVALID_HANDLE_VALUE)
	{
		// ���������,�򴴽����ļ�
		handle_File = CreateFile(pwchar_param_SaveFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (handle_File == INVALID_HANDLE_VALUE)
		{
			// ����ʧ�ܣ��򷵻�
			return false;
		}
		else
		{
			// ����Windowsϵͳ�µļ��±�Ĭ�ϴ򿪷�ʽ��ANSI���룬
			// ���Ա���������Unicode���뷽ʽ��
			// д��0xfeff�ü��±���Unicode��ʽ�򿪣����Ĳ�������
			WORD dword_UnicodeFlag = UNICODE_HEAD_FLAG;
			WriteFile(handle_File,
				&dword_UnicodeFlag,
				sizeof(WORD),
				&dword_NumberOfBytesToWrite,
				NULL);

		}
	}
	else
	{
		// ����д���ļ����ݵ�λ��(Ϊ��׷�������ݣ��������õ��ļ�β��)
		// ���д���ļ��������ݴ�С����4G�����Կ���ʹ��SetFilePointerEx
		SetFilePointer(handle_File, 0, NULL, FILE_END);
	}

	// д��ָ������
	WriteFile(handle_File,
		pwchar_param_Buffer,
		dword_param_BufferSize,
		&dword_NumberOfBytesToWrite,
		NULL);

	FlushFileBuffers(handle_File);

	// �ͷ��ļ�������Դ
	CloseHandle(handle_File);

	return true;

}
