#include "stdafx.h"
#include "ScanFile.h"


bool fun_AskUserIsScan(void)
{
	wchar_t wchar_YesOrNo;

	wprintf(L"是否需要扫描Windows操作系统垃圾文件(按下y键进行扫描或者q键退出程序)？ ");
	do
	{
		wscanf_s(L"%lc", &wchar_YesOrNo, 1);
		getwchar(); // 吸收回车换行
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
	wchar_t wchar_DirPath[MAX_PATH] = { '\0' }; // 目录路径缓冲区
	wchar_t wchar_SubDirPath[MAX_PATH] = { '\0' }; // 子目录路径缓冲区
	wchar_t wchar_TempFilePath[MAX_PATH] = { '\0' }; // 垃圾文件的全路径
	size_t size_Length_of_RootPath;                // 目录路径的实际长度

	WIN32_FIND_DATA strcut_Find; // 枚举文件时需要的数据结构体    
	HANDLE handle_FindFile = INVALID_HANDLE_VALUE; // 文件句柄

												   // 校验传递进来的参数是否合法
	if ((pwchar_param_RootPath == NULL) ||
		(wcslen(pwchar_param_RootPath) == 0))
	{
		wprintf_s(L"参数不合法");
		return;
	}

	// 检查传入的根目录路径是否过长
	// MAX_PATH-3 表示需要留出3个字符用来跟字符串"\*"进行拼接，
	// 这样才能进行枚举目录
	StringCchLength(pwchar_param_RootPath,
		MAX_PATH,
		&size_Length_of_RootPath);
	if (size_Length_of_RootPath > (MAX_PATH - 4))
	{
		wprintf_s(L"根目录路径太长");
		return;
	}

	// 保存路径并跟字符串"\*"进行拼接，形成如下格式
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


	// 开始执行枚举目标目录
	handle_FindFile = FindFirstFile(wchar_DirPath, &strcut_Find);
	if (handle_FindFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	// 利用do~while循环进行枚举操作
	do
	{
		if (strcut_Find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// 子目录(如果是"."和".."这2种特殊目录则不需要进行枚举)
			if ((wcscmp(strcut_Find.cFileName, L".") != 0) &&
				(wcscmp(strcut_Find.cFileName, L"..") != 0))
			{
				// 拼接新的子目录路径，形成如下格式
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


				// 通知命令控制台打印扫描信息(更新目录数目)
				(funcb_param_Show)(SCANINFO_TYPE_DIRCOUNT,
					short_param_CmdRow_ShowScanInfo);



				// 递归枚举当前新的子目录
				fun_EnumDirectory_listedTempFile(wchar_SubDirPath,
					pwchar_param_SaveFile,
					short_param_CmdRow_ShowScanInfo,
					funcb_param_Show);
			}
		}
		else
		{
			// 文件(判断是否是垃圾文件.tmp)
			if (fun_IsTempFiles(strcut_Find.cFileName) == true)
			{
				// 通知命令控制台打印扫描信息(更新垃圾文件数目)
				(funcb_param_Show)(SCANINFO_TYPE_TMPFILECOUNT,
					short_param_CmdRow_ShowScanInfo);

				// 拼接垃圾文件的全路径，并把路径信息保存到指定文件
				if (StringCchPrintf(wchar_TempFilePath,
					_countof(wchar_TempFilePath),
					L"%ls\\%ls\r\n", // 拼接全路径
					pwchar_param_RootPath,
					strcut_Find.cFileName) == S_OK)
				{
					fun_SaveTempFilesInfoToFile(pwchar_param_SaveFile,
						wchar_TempFilePath,
						wcslen(wchar_TempFilePath) * sizeof(wchar_t));
				}

			}


			// 通知命令控制台打印扫描信息(更新文件数目)
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

	// 判断是否是.tmp后缀名
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

	// 通过CreateFile()函数的OPEN_EXISTING参数标志位判断文件是否存在，
	// 如果不存在则创建
	handle_File = CreateFile(pwchar_param_SaveFile,
		GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (handle_File == INVALID_HANDLE_VALUE)
	{
		// 如果不存在,则创建新文件
		handle_File = CreateFile(pwchar_param_SaveFile,
			GENERIC_WRITE,
			0,
			NULL,
			CREATE_NEW,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (handle_File == INVALID_HANDLE_VALUE)
		{
			// 创建失败，则返回
			return false;
		}
		else
		{
			// 由于Windows系统下的记事本默认打开方式是ANSI编码，
			// 所以必须先设置Unicode编码方式打开
			// 写入0xfeff让记事本以Unicode方式打开，中文不会乱码
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
		// 设置写入文件内容的位置(为了追加新内容，必须设置到文件尾部)
		// 如果写入文件的总内容大小超过4G，可以考虑使用SetFilePointerEx
		SetFilePointer(handle_File, 0, NULL, FILE_END);
	}

	// 写入指定内容
	WriteFile(handle_File,
		pwchar_param_Buffer,
		dword_param_BufferSize,
		&dword_NumberOfBytesToWrite,
		NULL);

	FlushFileBuffers(handle_File);

	// 释放文件对象资源
	CloseHandle(handle_File);

	return true;

}
