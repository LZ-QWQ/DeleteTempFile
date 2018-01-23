
#pragma once


/************************************************************************/
/* 控制打印信息的输出位置                                               */
/************************************************************************/

#define ROW_JUNKINFO 3 // 命令控制台的第3行，用于输出扫描状态

// 输出扫描状态的描述
#define ROW_SCANINFO_STATUS L"扫描状态: %d个目录 %d个文件 %d个垃圾文件\r\n"

// 输出等待扫描的描述
#define ROW_SCANINFO_WAIT_STATUS L"\r\n正在扫描%ls分区..."

// 输出扫描完成状态的描述
#define ROW_SCANINFO_COMPLETE_STATUS L"\r\n%ls分区下的垃圾文件扫描操作已结束(垃圾文件信息保存在%ls)...\r\n"


/************************************************************************/
/* 命令控制台更新扫描信息的类型标识                                     */
/************************************************************************/
#define SCANINFO_TYPE_DIRCOUNT     0 // 目录数目
#define SCANINFO_TYPE_FILECOUNT    1 // 文件数目
#define SCANINFO_TYPE_TMPFILECOUNT 2 // 垃圾文件数目


/************************************************************************/
/* 相关路径                                                             */
/************************************************************************/

// 用于存储垃圾文件信息的文件
#define STR_SAVETEMPFILEINFO_FILENAME_C L"D:\\alltemp_C.txt"
#define STR_SAVETEMPFILEINFO_FILENAME_D L"D:\\alltemp_D.txt"

/************************************************************************/
/* 扫描函数的回调函数指针类型                                           */
/************************************************************************/
typedef void (CALLBACK *FUNCB_SHOWSCANINFO)(DWORD, short);


/************************************************************************/
/* Unicode编码的文件格式头                                              */
/************************************************************************/
#define UNICODE_HEAD_FLAG 0xFEFF


/************************************************************************/
/* 常用ASCII编码字符                                                    */
/************************************************************************/
#define ASCII_SPACE 32 // 空格字符


/************************************************************************/
/* 相关结构体定义                                                       */
/************************************************************************/

// 创建扫描线程所用的结构体
struct STRUCT_THREAD_SCAN_PARAM
{
	wchar_t wchar_RootPath[MAX_PATH];
	wchar_t wchar_SaveFile[MAX_PATH];
	short short_CmdRow_ShowScanInfo;
	// 指定命令提示符下的目标行，输出扫描线程完成状态信息
	short short_CmdRow_ShowScanCompleteInfo;
	FUNCB_SHOWSCANINFO funcb_Show;
};

struct STRUCT_THREAD_DELETE_PARAM
{
	const wchar_t *pwchar_param_SaveFile;
	DWORD dword_PointOfBejinToRead;
	DWORD dword_SizeOfFile;
};
