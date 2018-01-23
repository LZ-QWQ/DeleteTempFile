// 垃圾清理.cpp: 定义控制台应用程序的入口点。
//
// ScanFiles_MWaitThread.cpp : Defines the entry point for the console application.
//

/*
说明: 本程序演示WaitForMultipleObject()函数的用法，用于等待扫描垃圾文件
线程的执行完毕。

作者: 猪头三
网站: http://www.x86asm.com
版权: 本代码案例用于教学,如果未经批准请勿复制此代码用于其他商业教学。
如果用于免费教学，请保留版权的完整性。
*/

#include "stdafx.h"

//扫描
#include "ScanFile.h"

// 控制台
#include "Console.h"

//读文件和删除
#include "Read_DeleteFile.h"

static DWORD gs_dword_FilesCount;     // 已扫描的文件数量
static DWORD gs_dword_TempFilesCount; // 已扫描的垃圾文件数量
static DWORD gs_dword_DirsCount;       // 已扫描的目录数量

// 回调打印信息
void CALLBACK funcb_ShowScanInfo(DWORD dowrd_param_ShowType,
	short short_param_CmdRow_ShowScanInfo);

// 扫描垃圾文件（线程）
unsigned __stdcall ThreadFun_ScanTempFile(void *pvoid_param_Arg);

//删除垃圾文件（线程）
unsigned __stdcall ThreadFun_ReadAndDeleteTempFile(void *struct_delete_param);

//关键段
CRITICAL_SECTION g_strcut_CS_Printf; 

int _tmain(int argc, _TCHAR* argv[])
{
	_wsetlocale(LC_ALL, L"chs");

	fun_Conlose_ClearAll();
	fun_Conlose_HideCursor();


	// 初始化全局变量
	gs_dword_DirsCount = 0;
	gs_dword_FilesCount = 0;
	gs_dword_TempFilesCount = 0;

	// 咨询用户是否需要扫描垃圾文件
	if (fun_AskUserIsScan() == false)
	{
		// 退出扫描程序
		return 0;
	}

	InitializeCriticalSection(&g_strcut_CS_Printf);

	wprintf_s(L"正在对C:\\和D:\\分区下的垃圾文件进行扫描,请耐心等待...\r\n");

	HANDLE handle_ScanTempFile;
	STRUCT_THREAD_SCAN_PARAM struct_ScanParam = {
		L"C:",
		STR_SAVETEMPFILEINFO_FILENAME_C,
		ROW_JUNKINFO,
		ROW_JUNKINFO + 2,
		(FUNCB_SHOWSCANINFO)funcb_ShowScanInfo
	};
	handle_ScanTempFile = (HANDLE)_beginthreadex(NULL,
		0,
		&ThreadFun_ScanTempFile,
		&struct_ScanParam,
		0,
		NULL);

	HANDLE handle_ScanTempFile_Other;
	STRUCT_THREAD_SCAN_PARAM struct_ScanParam_Other = {
		L"D:",
		STR_SAVETEMPFILEINFO_FILENAME_D,
		ROW_JUNKINFO,
		ROW_JUNKINFO + 4,
		(FUNCB_SHOWSCANINFO)funcb_ShowScanInfo
	};
	handle_ScanTempFile_Other = (HANDLE)_beginthreadex(NULL,
		0,
		&ThreadFun_ScanTempFile,
		&struct_ScanParam_Other,
		0,
		NULL);



	// 等待多线程扫描完成(同步)
	HANDLE handle_MThread[2];
	handle_MThread[0] = handle_ScanTempFile;
	handle_MThread[1] = handle_ScanTempFile_Other;
	WaitForMultipleObjects(2,
		handle_MThread,
		TRUE,
		INFINITE);


	// 释放扫描垃圾文件的线程句柄
	CloseHandle(handle_ScanTempFile);
	CloseHandle(handle_ScanTempFile_Other);


	if (fun_AskUserIsScan() == false)
		return 0;
	HANDLE handle_DeleteTempFile_C;
	STRUCT_THREAD_DELETE_PARAM struct_delete_param_C
	{
		STR_SAVETEMPFILEINFO_FILENAME_C,
		2,
		GetFileSize(STR_SAVETEMPFILEINFO_FILENAME_C,0)
	};

	handle_DeleteTempFile_C = (HANDLE)_beginthreadex(NULL,
		0,
		&ThreadFun_ReadAndDeleteTempFile,
		&struct_delete_param_C,
		0,
		NULL);

	HANDLE handle_DeleteTempFile_D;
	STRUCT_THREAD_DELETE_PARAM struct_delete_param_D
	{
		STR_SAVETEMPFILEINFO_FILENAME_D,
		2,
		GetFileSize(STR_SAVETEMPFILEINFO_FILENAME_D,0),
	};

	handle_DeleteTempFile_D = (HANDLE)_beginthreadex(NULL,
		0,
		&ThreadFun_ReadAndDeleteTempFile,
		&struct_delete_param_D,
		0,
		NULL);

	HANDLE handle_MTDelete[2];
	handle_MThread[0] = handle_DeleteTempFile_C;
	handle_MThread[1] = handle_DeleteTempFile_D;
	WaitForMultipleObjects(2,
		handle_MTDelete,
		TRUE,
		INFINITE);

	CloseHandle(handle_DeleteTempFile_C);
	CloseHandle(handle_DeleteTempFile_D);

	// 释放关键段
	DeleteCriticalSection(&g_strcut_CS_Printf);

	return 0;

}// End tmain()


void CALLBACK funcb_ShowScanInfo(DWORD dowrd_param_ShowType,
	short short_param_CmdRow_ShowScanInfo)
{
	// 用于显示扫描状态的缓冲区
	wchar_t wchar_PrintfBuffer[MAX_PATH] = { '\0' };


	// 关键段保护
	EnterCriticalSection(&g_strcut_CS_Printf);

	// 统计数目
	switch (dowrd_param_ShowType)
	{
	case SCANINFO_TYPE_DIRCOUNT:
		gs_dword_DirsCount++;
		break;
	case SCANINFO_TYPE_FILECOUNT:
		gs_dword_FilesCount++;
		break;
	case SCANINFO_TYPE_TMPFILECOUNT:
		gs_dword_TempFilesCount++;
		break;
	default:
		return;
	}

	StringCchPrintf(wchar_PrintfBuffer,
		_countof(wchar_PrintfBuffer),
		ROW_SCANINFO_STATUS,
		gs_dword_DirsCount,
		gs_dword_FilesCount,
		gs_dword_TempFilesCount);

	// 显示新信息之前清空旧信息
	fun_Conlose_ClearRow(short_param_CmdRow_ShowScanInfo,
		wcslen(wchar_PrintfBuffer));
	fun_Conlose_Printf_Pos(0,
		short_param_CmdRow_ShowScanInfo,
		wchar_PrintfBuffer);

	LeaveCriticalSection(&g_strcut_CS_Printf);

	return;

}

unsigned __stdcall ThreadFun_ScanTempFile(void *pvoid_param_Arg)
{
	// 用于显示扫描完成状态的缓冲区
	wchar_t wchar_PrintfBuffer[MAX_PATH] = { '\0' };

	// 获取扫描参数
	STRUCT_THREAD_SCAN_PARAM *pstruct_ScanParam = (STRUCT_THREAD_SCAN_PARAM*)pvoid_param_Arg;

	// 关键段保护
	EnterCriticalSection(&g_strcut_CS_Printf);

	// 打印扫描信息
	StringCchPrintf(wchar_PrintfBuffer,
		_countof(wchar_PrintfBuffer),
		ROW_SCANINFO_WAIT_STATUS,
		pstruct_ScanParam->wchar_RootPath);
	fun_Conlose_Printf_Pos(0,
		pstruct_ScanParam->short_CmdRow_ShowScanCompleteInfo,
		wchar_PrintfBuffer);

	LeaveCriticalSection(&g_strcut_CS_Printf);

	fun_EnumDirectory_listedTempFile(pstruct_ScanParam->wchar_RootPath,
		pstruct_ScanParam->wchar_SaveFile,
		pstruct_ScanParam->short_CmdRow_ShowScanInfo,
		pstruct_ScanParam->funcb_Show);

	// 关键段保护
	EnterCriticalSection(&g_strcut_CS_Printf);

	// 打印扫描结束信息
	StringCchPrintf(wchar_PrintfBuffer,
		_countof(wchar_PrintfBuffer),
		ROW_SCANINFO_COMPLETE_STATUS,
		pstruct_ScanParam->wchar_RootPath,
		pstruct_ScanParam->wchar_SaveFile);
	// 显示新信息之前清空旧信息
	fun_Conlose_ClearRow(pstruct_ScanParam->short_CmdRow_ShowScanCompleteInfo,
		wcslen(wchar_PrintfBuffer));
	fun_Conlose_Printf_Pos(0,
		pstruct_ScanParam->short_CmdRow_ShowScanCompleteInfo,
		wchar_PrintfBuffer);

	LeaveCriticalSection(&g_strcut_CS_Printf);

	// 释放线程资源
	_endthreadex(0);

	return 0;

}// End ThreadFun_ScanTempFile()

void CALLBACK funcd_ShowDeleteInfo()
{

}

unsigned __stdcall ThreadFun_ReadAndDeleteTempFile(void *struct_delete_param)
{
}
