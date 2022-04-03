#include "FormatMedia.h"
#include "resource.h"

int CALLBACK WinMain(_In_ HINSTANCE hIns, _In_opt_ HINSTANCE hPreIns, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();		// 召唤 调试DOS窗口
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);	// 获取标准输出句柄
#endif // DEBUG


	// 预处理操作
	if (preHandle() == FALSE)
	{
		DeviceInfo.ScreenW = 1920;
		DeviceInfo.ScreenH = 1080;
	}

	// 注册窗口类
	WNDCLASSEX mainWndClass = { 0 };
	mainWndClass.cbClsExtra = 0;
	mainWndClass.cbSize = sizeof(WNDCLASSEX);
	mainWndClass.lpfnWndProc = mainMsgProc;
	mainWndClass.lpszClassName = __TEXT("ForMeWnd");
	mainWndClass.hInstance = hIns;
	mainWndClass.cbWndExtra = 0;
	mainWndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// 窗口背景色
	mainWndClass.hCursor = NULL;// 窗口鼠标指针
	mainWndClass.hIcon = LoadIcon(hIns, (LPCSTR)IDI_ICON1);// 待定
	mainWndClass.hIconSm = LoadIcon(hIns, (LPCSTR)IDI_ICON1);// 待定
	mainWndClass.lpszMenuName = NULL;
	mainWndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &mainWndClass );

	// 创建窗口
#define mainWnd_W 520
#define mainWnd_H 326

	HWND mainWnd = CreateWindowEx(0,
		__TEXT("ForMeWnd"), __TEXT("FormatMedia"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(DeviceInfo.ScreenW - mainWnd_W) / 2,
		(DeviceInfo.ScreenH - mainWnd_H) / 2,	// 居中显示
		mainWnd_W, mainWnd_H,
		NULL, NULL, hIns, NULL);

	ShowWindow( mainWnd, SW_SHOW );
	UpdateWindow( mainWnd );

	MSG mainMSG = { 0 };

	while (TRUE)
	{
		if ( PeekMessage( &mainMSG, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if ( GetMessage( &mainMSG, NULL, 0, 0 ) )
			{
				// 有消息
				TranslateMessage( &mainMSG );
				DispatchMessage( &mainMSG );
			}
			else
			{
				return 0;
			}
		}
		else
		{
			// 空闲处理
		}
	}

	return 0;
}

LRESULT CALLBACK mainMsgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_CREATE:// 显示窗口之前
		MessageBox(hWnd, 
			__TEXT("欢迎使用本款软件！我是作者 SeaYJ！\n"
				"如果您在使用过程中出现任何问题或者对本软件有任何改进建议\n"
				"欢迎邮件联系我：seayj21@outlook.com ，\n我会认真阅读您的每一条信息！\n"
				"您的支持就是对我最大的鼓励，\n再次感谢使用！"),
			__TEXT("消息"),
			MB_OK | MB_DEFBUTTON1);
		CREATESTRUCT* pcs = (CREATESTRUCT*)lParam;
		createComponent(hWnd, (pcs->hInstance));
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_BUTTON_OPENFILE:
			chooseFileDialog(hWnd);
			break;

		case ID_BUTTON_SAVEFILE:
			saveFileDialog(hWnd);
			break;

		case ID_BUTTON_START:
			startConversion(hWnd);
			break;
		}
		break;

	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			int nRet = MessageBox(hWnd, __TEXT("确认要退出程序吗？"), __TEXT("消息"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION);

			if (nRet == IDYES)
			{
				return DefWindowProc(hWnd, msgID, wParam, lParam);
			}
			else
			{
				return 0;
			}
		}
		break;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		break;
	}

	return DefWindowProc(hWnd, msgID, wParam, lParam);
}

BOOL preHandle()
{
	// 获取程序所在的目录（以 FormatMedia.exe 所在目录为准）
	TCHAR tempDrive[MAX_PATH] = { 0 }, tempDir[MAX_PATH] = { 0 }, tempWorkDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, (LPSTR)tempWorkDir, MAX_PATH);
	_splitpath(tempWorkDir, tempDrive, tempDir, NULL, NULL);
	sprintf(workDir, __TEXT("%s%s"), tempDrive, tempDir);

	// 获取屏幕宽高
	DeviceInfo.ScreenW = GetSystemMetrics(SM_CXSCREEN);
	DeviceInfo.ScreenH = GetSystemMetrics(SM_CYSCREEN);

	if ((DeviceInfo.ScreenW <= 0) && (DeviceInfo.ScreenH <= 0))
		return FALSE;

	return TRUE;
}

int createComponent(HWND hWnd, HINSTANCE hIns)
{
	// 注册控件宽、高和间隙（极少数未定义）
	UINT pos_X = 12;// 控件位置 X 值
	UINT pos_Y = 10;// 控件位置 Y 值
	UINT button_W = 100;
	UINT button_H = 25;
	UINT ctr_space = 10;// 控件标准间隙
	UINT OS_Edit_W = 370;
	UINT OS_Edit_H = button_H;
	UINT format_Group_W = 153;
	UINT format_Group_H = 60;
	UINT format_ComboBox_W = 113;
	UINT format_ComboBox_H = 20;
	UINT resultOutput_Edit_W = 480;
	UINT resultOutput_Edit_H = 95;

	/******************************\
	*  实测窗口客户区大小为 504x282  *
	\******************************/

	// 选择文件（按钮）
	ofnButton = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("选择文件"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_OPENFILE, NULL, NULL);
	
	pos_X += ( button_W + ctr_space );

	// 选择文件 - 路径显示（文本框）
	ofnEdit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_OEMCONVERT | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY,
		pos_X, pos_Y,
		OS_Edit_W, OS_Edit_H,
		hWnd, (HMENU)ID_EDIT_OPENFILE, NULL, NULL);


	pos_X = 12;
	pos_Y += ( button_H + ctr_space );

	// 保存文件（按钮）
	sfnButton = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("保存路径"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_SAVEFILE, NULL, NULL);

	pos_X += ( button_W + ctr_space );

	// 保存文件 - 显示路径（文本框）
	sfnEdit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_OEMCONVERT | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY,
		pos_X, pos_Y,
		OS_Edit_W, OS_Edit_H,
		hWnd, (HMENU)ID_EDIT_SAVEFILE, NULL, NULL);

	pos_X = 12;
	pos_Y += ( button_H + ctr_space );

// 输出文件格式选择
	// 图片边界框
	picFormatGroupBox = CreateWindowEx(0, 
		__TEXT("BUTTON"), __TEXT("转为图片格式"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X += (format_Group_W + ctr_space );

	// 视频边界框
	vidFormatGroupBox = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("转为视频格式"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X += (format_Group_W + ctr_space);

	// 音频边界框
	audFormatGroupBox = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("转为音频格式"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X = ( 12 + 2 * ctr_space );
	pos_Y += 22;

	// 图片下拉列表
	picFormat_Combobox = CreateWindowEx(0,
		__TEXT("COMBOBOX"), __TEXT("Picture"),
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST ,
		pos_X, pos_Y,
		format_ComboBox_W, format_ComboBox_H,
		hWnd, (HMENU)ID_COMBOBOX_PIC, NULL, NULL);

	for (unsigned int i=0; i<picFormatC; i++)
	{
		SendMessage(picFormat_Combobox, CB_ADDSTRING, 0, (LPARAM)picFormat[i]);
	}

	pos_X += ( format_ComboBox_W + 5 * ctr_space );

	// 视频下拉列表
	vidFormat_Combobox = CreateWindowEx(0,
		__TEXT("COMBOBOX"), __TEXT("Video"),
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		pos_X, pos_Y,
		format_ComboBox_W, format_ComboBox_H,
		hWnd, (HMENU)ID_COMBOBOX_VID, NULL, NULL);

	for (unsigned int i = 0; i < vidFormatC; i++)
	{
		SendMessage(vidFormat_Combobox, CB_ADDSTRING, 0, (LPARAM)vidFormat[i]);
	}

	pos_X += (format_ComboBox_W + 2 * ctr_space + ctr_space + 2 * ctr_space );

	// 视频下拉列表
	audFormat_Combobox = CreateWindowEx(0,
		__TEXT("COMBOBOX"), __TEXT("Audio"),
		WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
		pos_X, pos_Y,
		format_ComboBox_W, format_ComboBox_H,
		hWnd, (HMENU)ID_COMBOBOX_AUD, NULL, NULL);

	for (unsigned int i = 0; i < audFormatC; i++)
	{
		SendMessage(audFormat_Combobox, CB_ADDSTRING, 0, (LPARAM)audFormat[i]);
	}

	pos_X = 12;
	pos_Y -= 22;
	
	pos_Y += ( format_Group_H + ctr_space );

	// 结果输出（文本框）
	resultOutput_Edit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
		pos_X, pos_Y,
		resultOutput_Edit_W, resultOutput_Edit_H,
		hWnd, (HMENU)ID_EDIT_RESULT_OUTPUT, NULL, NULL);
	
	pos_X += 190;
	pos_Y += ( resultOutput_Edit_H + ctr_space );

	// 开始转换（按钮）
	startButton = CreateWindowEx(0, 
		__TEXT("BUTTON"), __TEXT("开始转换"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_START, NULL, NULL);

	// 设置字体
	HFONT hFontDef = (HFONT)GetStockObject(DEFAULT_GUI_FONT);// 默认字体（偏小）
	LOGFONT font;
	font.lfHeight = 16;
	font.lfWidth = 0;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfWeight = FW_LIGHT;
	font.lfItalic = FALSE;
	font.lfUnderline = FALSE;
	font.lfStrikeOut = FALSE;
	font.lfEscapement = 0;
	font.lfOrientation = 0;
	font.lfOutPrecision = OUT_DEFAULT_PRECIS;
	font.lfClipPrecision = CLIP_STROKE_PRECIS | CLIP_MASK | CLIP_TT_ALWAYS | CLIP_LH_ANGLES;
	font.lfQuality = ANTIALIASED_QUALITY;
	font.lfPitchAndFamily = VARIABLE_PITCH | FF_DONTCARE;
	HFONT hFont = CreateFontIndirect(&font);// 程序主要使用的字体

	SendMessage(ofnButton, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(sfnButton, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(ofnEdit, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(sfnEdit, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(picFormatGroupBox, WM_SETFONT, (WPARAM)hFontDef, 0);
	SendMessage(vidFormatGroupBox, WM_SETFONT, (WPARAM)hFontDef, 0);
	SendMessage(audFormatGroupBox, WM_SETFONT, (WPARAM)hFontDef, 0);
	SendMessage(picFormat_Combobox, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(vidFormat_Combobox, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(audFormat_Combobox, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(resultOutput_Edit, WM_SETFONT, (WPARAM)hFont, 0);
	SendMessage(startButton, WM_SETFONT, (WPARAM)hFont, 0);

	return 0;
}

BOOL chooseFileDialog(HWND hWnd) 
{
// 选择文件对话框
	// 注册选择文件对话框信息
	OPENFILENAME ofn = { 0 };
	TCHAR strFilePath[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.dwReserved = 0;
		// 可选择文件类型筛选器
	ofn.lpstrFilter = __TEXT("图片("
		"*.png;*.jpg;*.bmp;*.webp;*.tif;*.jp2;*.jpeg;*.ppm;*.tga;*.gif)\0"
		"*.png;*.jpg;*.bmp;*.webp;*.tif;*.jp2;*.jpeg;*.ppm;*.tga;*.gif\0"
		
		"视频("
		"*.mp4,*.flv;*.avi;*.mov;*.wmv;*.m4v;*.f4v;*.ts;*.webm)\0"
		"*.mp4;*.flv;*.avi;*.mov;*.wmv;*.m4v;*.f4v;*.ts;*.webm\0"

		"音频("
		"*.mp3;*.m4a;*.wma;*.ac3;*.flac;*.ogg;*.wav)\0"
		"*.mp3;*.m4a;*.wma;*.ac3;*.flac;*.ogg;*.wav\0"
		"\0");
	ofn.nFilterIndex = 1;// 文件类型筛选器索引起始（别改！千万别改！很麻烦！）
	ofn.lpstrFile = strFilePath;
	ofn.nMaxFile = sizeof(strFilePath);
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = __TEXT("请选择一个需要转换格式的文件");// 对话框标题
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn))
	{
		// 对获取的文件路径进行分割，并存储在结构体变量 filePathInfo 中
		_splitpath(strFilePath, filePathInfo.fileDrive, filePathInfo.fileDir, filePathInfo.fileName, filePathInfo.fileExt);
		sprintf(filePathInfo.fileEnv, __TEXT("%s%s"), filePathInfo.fileDrive, filePathInfo.fileDir);
		strcpy(filePathInfo.filePath, strFilePath);

#define INDEX_PicFilterIndex 1
#define INDEX_VidFilterIndex 2
#define INDEX_AudFilterIndex 3

		// 根据选文件时使用的筛选器索引 “禁用/启用相关（目标文件类型选择列表）控件”
		switch (ofn.nFilterIndex)
		{
		case INDEX_PicFilterIndex:// 启用 “图片类型” 列表，并禁用其他
			EnableWindow(picFormat_Combobox, TRUE);
			EnableWindow(vidFormat_Combobox, FALSE);
			EnableWindow(audFormat_Combobox, FALSE);
			fileType = 1;
			break;

		case INDEX_VidFilterIndex:// 启用 “视频类型” 列表，并禁用其他
			EnableWindow(picFormat_Combobox, FALSE);
			EnableWindow(vidFormat_Combobox, TRUE);
			EnableWindow(audFormat_Combobox, FALSE);
			fileType = 2;
			break;

		case INDEX_AudFilterIndex:// 启用 “音频类型” 列表，并禁用其他
			EnableWindow(picFormat_Combobox, FALSE);
			EnableWindow(vidFormat_Combobox, FALSE);
			EnableWindow(audFormat_Combobox, TRUE);
			fileType = 3;
			break;

		default:
			break;
		}

		// 根据选择的文件路径 “在路径显示控件（OPEN/SAVE - EDIT）中显示相关路径信息”
		SendMessage(ofnEdit, WM_SETTEXT, 0, (LPARAM)strFilePath);
		SendMessage(sfnEdit, WM_SETTEXT, 0, (LPARAM)filePathInfo.fileEnv);
	}
	else {
		
		if (CommDlgExtendedError() == FALSE) // 用户取消了选择
			return FALSE;

	}

	return TRUE;
}

BOOL saveFileDialog(HWND hWnd)
{
	// 保存文件位置对话框
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = hWnd;//拥有着窗口句柄，为NULL表示对话框是非模态的，实际应用中一般都要有这个句柄
	bi.pszDisplayName = szBuffer;//接收文件夹的缓冲区
	bi.lpszTitle = TEXT("请选择文件保存位置");//标题
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
RECHOOSE: {}
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);

	// 用户在对话框中选择"取消"按钮
	if (idl == NULL) return TRUE;

	if (SHGetPathFromIDList(idl, szBuffer)) 
	{	
		// 显示用户选择的保存路径（目标路径）
		PathAddBackslash((LPSTR)szBuffer);
		SendMessage(sfnEdit, WM_SETTEXT, 0, (LPARAM)szBuffer);
	}
	else {
		MessageBox(NULL, TEXT("请选择一个文件夹\n用于保存目标文件！"), NULL, MB_ICONERROR);
		goto RECHOOSE;
	}
	
	return TRUE;
}

BOOL startConversion(HWND hWnd)
{
	// 显示状态信息
	SetDlgItemText(hWnd, ID_EDIT_RESULT_OUTPUT, __TEXT("FormatMedia->转换开始：\r\n"));
	SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
	SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("消息： 读取相关信息中...\r\n"));
	
	// 读取保存目录
	TCHAR targetDir[MAX_PATH] = { 0 };
	GetDlgItemText(hWnd, ID_EDIT_SAVEFILE, (LPSTR)targetDir, MAX_PATH);

	

#define TYPEID_PIC 1
#define TYPEID_VID 2
#define TYPEID_AUD 3

	// 根据选文件时使用的筛选器索引 “读取目标格式”
	TCHAR targetExt[MAX_PATH] = { 0 };

	switch (fileType)
	{
	case TYPEID_PIC:// 在“图片类型列表”中读取
		GetDlgItemText(hWnd, ID_COMBOBOX_PIC, (LPSTR)targetExt, MAX_PATH);
		break;

	case TYPEID_VID:// 在“视频类型列表”中读取
		GetDlgItemText(hWnd, ID_COMBOBOX_VID, (LPSTR)targetExt, MAX_PATH);
		break;

	case TYPEID_AUD:// 在“音频类型列表”中读取
		GetDlgItemText(hWnd, ID_COMBOBOX_AUD, (LPSTR)targetExt, MAX_PATH);
		break;

	default:
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("警告： 请选择一个文件，并指定待转换目标类型!\r\n"));
		return FALSE;
		break;
	}

	// 检查文件后缀知否读取成功
	if (targetExt[0] == __TEXT('\0') || strlen(targetExt) <= 0) 
	{
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("错误： 未找到目标文件类型！\r\n"));
		return FALSE;
	}
	
	// 生成文件标识符(-xxxxYxxMxxDxxHxxMxxS)
	SYSTEMTIME time;
	GetLocalTime(&time);
	TCHAR dateTimeStr[100] = { 0 };
	sprintf(dateTimeStr, "-%dY%02dM%02dD%02dH%02dM%02dS", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// 生成目标文件名
	TCHAR targetFile[MAX_PATH] = { 0 };
	sprintf(targetFile, __TEXT("%s%s%s"), filePathInfo.fileName, dateTimeStr, targetExt);

	// 生成目标文件完整路径(带目标文件名)
	TCHAR targetPath[MAX_PATH] = { 0 };
	sprintf(targetPath, __TEXT("%s%s"), targetDir, targetFile);

// 开启进程，调用FFmpeg
	// 生成 ffmpeg.exe 所在绝对路径
	TCHAR ffmpegPath[MAX_PATH] = { 0 };
	sprintf(ffmpegPath, __TEXT("%sbin\\ffmpeg.exe"), workDir);
	PathQuoteSpaces(ffmpegPath);

	// 生成命令行参数（最终参数）
	TCHAR cmdParam[2048] = { 0 };
	sprintf(cmdParam, __TEXT("-i \"%s\" \"%s\""), filePathInfo.filePath, targetPath);

	// 显示状态信息
	SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
	SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("消息：已准备就绪！正在拼命转换中...\r\n"));

	// 启动 ffmpeg.exe 程序并传递相关参数
	HINSTANCE nRet = ShellExecute(resultOutput_Edit, __TEXT("open"), ffmpegPath, cmdParam, NULL, SW_HIDE);

	// 反馈输出结果
	if ( nRet > 32 ) 
	{
		// 转换成功
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("消息：转换成功！\r\n"));
	}
	else 
	{
		// 转换失败
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("消息：转换失败！\r\n请联系作者反馈该情况：seayj21@outlook.com\r\n或者去https://github.com/SeaYJ/FormatMedia留言！\r\n"));
		return FALSE;
	}

	return TRUE;
}