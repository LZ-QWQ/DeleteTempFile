#pragma once

/************************************************************************/
/* �������̨���ɨ����ɾ��״̬                                    */
/************************************************************************/
#define ROW_JUNKINFO 3 

#define ROW_SCANINFO_STATUS L"ɨ��״̬: %d��Ŀ¼ %d���ļ� %d�������ļ�\r\n"

#define ROW_SCANINFO_WAIT_STATUS L"\r\n����ɨ��%ls����..."

#define ROW_SCANINFO_COMPLETE_STATUS L"\r\n%ls�����µ������ļ�ɨ������ѽ���(�����ļ���Ϣ������%ls)...\r\n"

#define ROW_DELETEINFO_STATUS L"ɾ��״̬: ����%d���ļ� %d����ɾ�� %d��ɾ��ʧ��\r\n"

#define ROW_DELETEINFO_WAIT_STATUS L"\r\n����ɾ��%ls����..."

#define ROW_DELETEINFO_COMPLETE_STATUS L"\r\n%ls�����µ������ļ�ɾ�������ѽ���...\r\n"

/************************************************************************/
/* �������̨����ɨ����Ϣ�����ͱ�ʶ                                     */
/************************************************************************/
#define SCANINFO_TYPE_DIRCOUNT     0 // Ŀ¼��Ŀ
#define SCANINFO_TYPE_FILECOUNT    1 // �ļ���Ŀ
#define SCANINFO_TYPE_TMPFILECOUNT 2 // �����ļ���Ŀ

#define DELETEINFO_TYPE_DELETEFILECOUNT 0  //ɾ���ļ���Ŀ	
#define DELETEINFO_TYPE_FAILTODELETECOUNT 1 //ɾ��ʧ����Ŀ
/************************************************************************/
/* ���ڴ洢�����ļ���Ϣ���ļ�·��                                                         */
/************************************************************************/
#define STR_SAVETEMPFILEINFO_FILENAME_C L"D:\\alltemp_C.txt"

#define STR_SAVETEMPFILEINFO_FILENAME_D L"D:\\alltemp_D.txt"

/************************************************************************/
/* ɨ�躯���Ļص�����ָ������                                           */
/************************************************************************/
typedef void (CALLBACK *FUNCB_SHOWSCANINFO)(DWORD, short);

typedef void (CALLBACK *FUNCB_SHOWDELETEINFO)(DWORD, short);

/************************************************************************/
/* Unicode������ļ���ʽͷ                                              */
/************************************************************************/
#define UNICODE_HEAD_FLAG 0xFEFF

/************************************************************************/
/* ����ASCII�����ַ�                                                    */
/************************************************************************/
#define ASCII_SPACE 32 // �ո��ַ�

/************************************************************************/
/* ��ؽṹ�嶨��                                                       */
/************************************************************************/

// ����ɨ���߳����õĽṹ��
struct STRUCT_THREAD_SCAN_PARAM
{
	wchar_t wchar_RootPath[MAX_PATH];
	wchar_t wchar_SaveFile[MAX_PATH];
	short short_CmdRow_ShowScanInfo;
	// ָ��������ʾ���µ�Ŀ���У����ɨ���߳����״̬��Ϣ
	short short_CmdRow_ShowScanCompleteInfo;
	FUNCB_SHOWSCANINFO funcb_Show_Scan;
};

struct STRUCT_THREAD_DELETE_PARAM
{
	wchar_t wchar_RootPath[MAX_PATH];
	wchar_t wchar_SaveFile[MAX_PATH];
	short short_CmdRow_ShowDeleteInfo;
	short short_CmdRow_ShowDeleteCompleteInfo;
	FUNCB_SHOWDELETEINFO funcb_Show_Delete;
};
