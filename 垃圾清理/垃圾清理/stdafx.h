// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <Windows.h>
#include <strsafe.h>
#include <stdlib.h>
#include <Locale.h>
#include <Shlwapi.h>
#include <process.h>

// 相关的全局宏定义
#include "GDef.h"

//扫描
#include "ScanFile.h"

// 控制台
#include "Console.h"

//读文件和删除
#include "Read_DeleteFile.h"

// 使用Shlwapi.lib库
#pragma comment(lib, "Shlwapi")