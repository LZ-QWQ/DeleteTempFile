#pragma once
// �����������̨�ĵ�ǰ�й��
void fun_Conlose_HideCursor(void);



// ���������̨��ָ��λ�ô�ӡ�ַ���
void fun_Conlose_Printf_Pos(short sint_Columm,
	short sint_Row,
	const wchar_t *wchar_PrintfBuffer);



// ����������̨�������
void fun_Conlose_ClearAll(void);



// ����������̨���ָ��������
void fun_Conlose_ClearRow(short short_param_Row,
	DWORD dword_param_Len);