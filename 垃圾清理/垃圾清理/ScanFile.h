#pragma once
// 提示用户是否需要执行扫描功能
bool fun_AskUserIsScan(void);



// 枚举指定目录下的所有垃圾文件(.tmp)，并把垃圾信息保存到指定日记文件
void fun_EnumDirectory_listedTempFile(const wchar_t *pwchar_param_RootPath,
	const wchar_t *pwchar_param_SaveFile,
	short short_param_CmdRow_ShowScanInfo,
	FUNCB_SHOWSCANINFO funcb_param_Show);



// 根据文件后缀名判断当前文件是否是垃圾文件(.tmp)
bool fun_IsTempFiles(const wchar_t *pwchar_param_FileName);



// 把指定信息保存到指定文件
bool fun_SaveTempFilesInfoToFile(const wchar_t *pwchar_param_SaveFile,
	const wchar_t *pwchar_param_Buffer,
	DWORD dword_param_BufferSize);