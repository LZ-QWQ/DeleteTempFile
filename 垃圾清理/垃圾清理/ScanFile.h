#pragma once
// ��ʾ�û��Ƿ���Ҫִ��ɨ�蹦��
bool fun_AskUserIsScan(void);



// ö��ָ��Ŀ¼�µ����������ļ�(.tmp)������������Ϣ���浽ָ���ռ��ļ�
void fun_EnumDirectory_listedTempFile(const wchar_t *pwchar_param_RootPath,
	const wchar_t *pwchar_param_SaveFile,
	short short_param_CmdRow_ShowScanInfo,
	FUNCB_SHOWSCANINFO funcb_param_Show);



// �����ļ���׺���жϵ�ǰ�ļ��Ƿ��������ļ�(.tmp)
bool fun_IsTempFiles(const wchar_t *pwchar_param_FileName);



// ��ָ����Ϣ���浽ָ���ļ�
bool fun_SaveTempFilesInfoToFile(const wchar_t *pwchar_param_SaveFile,
	const wchar_t *pwchar_param_Buffer,
	DWORD dword_param_BufferSize);