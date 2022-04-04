#ifndef _FORMATMEDIA_H
#define _FORMATMEDIA_H

#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "shlwapi.lib")

#include <Windows.h>
#include <windowsx.h>
#include <stdio.h>
#include<Commdlg.h> // 打开保存文件对话框
#include<Shlobj.h> // 选择文件夹对话框
#include <stdlib.h>
#include <shlwapi.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#ifdef _DEBUG
HANDLE g_hOutput = 0;	// 用于存储标准输出句柄
char pszText[1024] = { 0 };
#endif // 调试

struct // 储存屏幕 “宽” 和 “高”
{
	int ScreenW;
	int ScreenH;
} DeviceInfo = { 0 };

TCHAR workDir[MAX_PATH] = { 0 };// 存储程序当前工作目录（末尾带'\'）

#define ID_BUTTON_OPENFILE 0x0520
#define ID_BUTTON_SAVEFILE 0x0521
#define ID_EDIT_OPENFILE 0x0522
#define ID_EDIT_SAVEFILE 0x0523
#define ID_COMBOBOX_PIC 0x0524
#define ID_COMBOBOX_VID 0x0525
#define ID_COMBOBOX_AUD 0x0526
#define ID_EDIT_RESULT_OUTPUT 0x0527
#define ID_BUTTON_START 0x0528

HWND ofnButton = NULL;
HWND ofnEdit = NULL;
HWND sfnButton = NULL;
HWND sfnEdit = NULL;
HWND picFormatGroupBox = NULL;
HWND vidFormatGroupBox = NULL;
HWND audFormatGroupBox = NULL;
HWND picFormat_Combobox = NULL;
HWND vidFormat_Combobox = NULL;
HWND audFormat_Combobox = NULL;
HWND resultOutput_Edit = NULL;
HWND startButton = NULL;

// 图片支持格式
UINT picFormatC = 10;
TCHAR* picFormat[] = 
{ 
	__TEXT(".png"),
	__TEXT(".jpg"),
	__TEXT(".jpeg"),
	__TEXT(".bmp"),
	__TEXT(".tif"),
	__TEXT(".webp"),
	__TEXT(".gif"),
	__TEXT(".tga"),
	__TEXT(".jp2"),
	__TEXT(".ppm")
};

// 视频支持格式
UINT vidFormatC = 10;
TCHAR* vidFormat[] =
{
	__TEXT(".mp4"),
	__TEXT(".flv"),
	__TEXT(".avi"),
	__TEXT(".mov"),
	__TEXT(".wmv"),
	__TEXT(".m4v"),
	__TEXT(".f4v"),
	__TEXT(".ts"),
	__TEXT(".webm"),
	__TEXT(".mkv")
};

// 音频支持格式
UINT audFormatC = 7;
TCHAR* audFormat[] =
{
	__TEXT(".mp3"),
	__TEXT(".m4a"),
	__TEXT(".wma"),
	__TEXT(".ac3"),
	__TEXT(".flac"),
	__TEXT(".ogg"),
	__TEXT(".wav")
};

struct
{// 例：C:\A\B.C
	TCHAR filePath[MAX_PATH];// 文件原完整路径 [ C:\A\B.C ]
	TCHAR fileDrive[MAX_PATH];// 文件所在驱动器 [ C: ]
	TCHAR fileDir[MAX_PATH];// 文件所在目录（不带驱动器） [ \A\ ]
	TCHAR fileEnv[MAX_PATH];// 文件所在目录（带驱动器） [ C:\A\ ]
	TCHAR fileName[MAX_PATH];// 文件名（无后缀） [ B ]
	TCHAR fileExt[MAX_PATH];// 文件后缀（带'.'） [ .C ]
} filePathInfo = { 0 };

DWORD fileType = -1;// 用于确定目标文件类型


LRESULT CALLBACK mainMsgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam);

BOOL preHandle();	//	预处理函数

int createComponent(HWND hWnd, HINSTANCE hIns);

BOOL chooseFileDialog(HWND hWnd);

BOOL saveFileDialog(HWND hWnd);

BOOL startConversion(HWND hWnd);

#endif
