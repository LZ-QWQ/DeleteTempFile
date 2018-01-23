#include "stdafx.h"
#include "Console.h"

/*
作用: 隐藏命令控制台的当前行光标
参数: 无
返回: 无
*/
void fun_Conlose_HideCursor(void)
{
	CONSOLE_CURSOR_INFO struct_CurosInfo;

	HANDLE handle_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	if (GetConsoleCursorInfo(handle_StdOut, &struct_CurosInfo))
	{
		struct_CurosInfo.bVisible = FALSE;

		SetConsoleCursorInfo(handle_StdOut, &struct_CurosInfo);
	}

	return;

}



 /*
 作用: 在命令控制台中指定位置打印字符串
 参数: 1> 列
 2> 行
 3> 需要显示的字符串
 返回: 无
 */
void fun_Conlose_Printf_Pos(short sint_Columm,
	short sint_Row,
	const wchar_t *wchar_PrintfBuffer)
{
	DWORD dword_Count;

	COORD strcut_Coord = { sint_Columm, sint_Row };

	HANDLE handle_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleCursorPosition(handle_StdOut, strcut_Coord);

	WriteConsole(handle_StdOut,
		wchar_PrintfBuffer,
		wcslen(wchar_PrintfBuffer),
		&dword_Count,
		NULL);

	return;

}



 /*
 作用: 清空命令控制台里的内容
 参数: 无
 返回：无
 */
void fun_Conlose_ClearAll(void)
{
	COORD strcut_Coord = { 0, 0 };

	DWORD dword_Count;

	HANDLE handle_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO struct_CSBufferInfo;

	if (GetConsoleScreenBufferInfo(handle_StdOut, &struct_CSBufferInfo))
	{

		FillConsoleOutputCharacter(handle_StdOut,
			(wchar_t)ASCII_SPACE,
			struct_CSBufferInfo.dwSize.X * struct_CSBufferInfo.dwSize.Y,
			strcut_Coord,
			&dword_Count);
		FillConsoleOutputAttribute(handle_StdOut,
			struct_CSBufferInfo.wAttributes,
			struct_CSBufferInfo.dwSize.X * struct_CSBufferInfo.dwSize.Y,
			strcut_Coord,
			&dword_Count);

		SetConsoleCursorPosition(handle_StdOut, strcut_Coord);
	}

	return;

}



 /*
 作用: 清空命令控制台里的指定行内容
 参数: 1> 行
 2> 清空的长度
 返回：无
 */
void fun_Conlose_ClearRow(short short_param_Row, DWORD dword_param_Len)
{
	COORD strcut_Coord = { 0, short_param_Row };

	DWORD dword_Count;

	HANDLE handle_StdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO struct_CSBufferInfo;

	if (GetConsoleScreenBufferInfo(handle_StdOut, &struct_CSBufferInfo))
	{

		FillConsoleOutputCharacter(handle_StdOut,
			(wchar_t)ASCII_SPACE,
			dword_param_Len,
			strcut_Coord,
			&dword_Count);
		FillConsoleOutputAttribute(handle_StdOut,
			struct_CSBufferInfo.wAttributes,
			dword_param_Len,
			strcut_Coord,
			&dword_Count);

		SetConsoleCursorPosition(handle_StdOut, strcut_Coord);
	}

	return;

}