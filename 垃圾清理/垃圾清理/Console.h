#pragma once
// 隐藏命令控制台的当前行光标
void fun_Conlose_HideCursor(void);



// 在命令控制台中指定位置打印字符串
void fun_Conlose_Printf_Pos(short sint_Columm,
	short sint_Row,
	const wchar_t *wchar_PrintfBuffer);



// 清空命令控制台里的内容
void fun_Conlose_ClearAll(void);



// 清空命令控制台里的指定行内容
void fun_Conlose_ClearRow(short short_param_Row,
	DWORD dword_param_Len);