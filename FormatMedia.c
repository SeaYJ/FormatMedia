#include "FormatMedia.h"
#include "resource.h"

int CALLBACK WinMain(_In_ HINSTANCE hIns, _In_opt_ HINSTANCE hPreIns, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
#ifdef _DEBUG
	AllocConsole();		// �ٻ� ����DOS����
	g_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);	// ��ȡ��׼������
#endif // DEBUG


	// Ԥ�������
	if (preHandle() == FALSE)
	{
		DeviceInfo.ScreenW = 1920;
		DeviceInfo.ScreenH = 1080;
	}

	// ע�ᴰ����
	WNDCLASSEX mainWndClass = { 0 };
	mainWndClass.cbClsExtra = 0;
	mainWndClass.cbSize = sizeof(WNDCLASSEX);
	mainWndClass.lpfnWndProc = mainMsgProc;
	mainWndClass.lpszClassName = __TEXT("ForMeWnd");
	mainWndClass.hInstance = hIns;
	mainWndClass.cbWndExtra = 0;
	mainWndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);// ���ڱ���ɫ
	mainWndClass.hCursor = NULL;// �������ָ��
	mainWndClass.hIcon = LoadIcon(hIns, (LPCSTR)IDI_ICON1);// ����
	mainWndClass.hIconSm = LoadIcon(hIns, (LPCSTR)IDI_ICON1);// ����
	mainWndClass.lpszMenuName = NULL;
	mainWndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx( &mainWndClass );

	// ��������
#define mainWnd_W 520
#define mainWnd_H 326

	HWND mainWnd = CreateWindowEx(0,
		__TEXT("ForMeWnd"), __TEXT("FormatMedia"),
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		(DeviceInfo.ScreenW - mainWnd_W) / 2,
		(DeviceInfo.ScreenH - mainWnd_H) / 2,	// ������ʾ
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
				// ����Ϣ
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
			// ���д���
		}
	}

	return 0;
}

LRESULT CALLBACK mainMsgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM lParam)
{
	switch (msgID)
	{
	case WM_CREATE:// ��ʾ����֮ǰ
		MessageBox(hWnd, 
			__TEXT("��ӭʹ�ñ���������������� SeaYJ��\n"
				"�������ʹ�ù����г����κ�������߶Ա�������κθĽ�����\n"
				"��ӭ�ʼ���ϵ�ң�seayj21@outlook.com ��\n�һ������Ķ�����ÿһ����Ϣ��\n"
				"����֧�־��Ƕ������Ĺ�����\n�ٴθ�лʹ�ã�"),
			__TEXT("��Ϣ"),
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
			int nRet = MessageBox(hWnd, __TEXT("ȷ��Ҫ�˳�������"), __TEXT("��Ϣ"), MB_YESNO | MB_DEFBUTTON1 | MB_ICONQUESTION);

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
	// ��ȡ�������ڵ�Ŀ¼���� FormatMedia.exe ����Ŀ¼Ϊ׼��
	TCHAR tempDrive[MAX_PATH] = { 0 }, tempDir[MAX_PATH] = { 0 }, tempWorkDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, (LPSTR)tempWorkDir, MAX_PATH);
	_splitpath(tempWorkDir, tempDrive, tempDir, NULL, NULL);
	sprintf(workDir, __TEXT("%s%s"), tempDrive, tempDir);

	// ��ȡ��Ļ���
	DeviceInfo.ScreenW = GetSystemMetrics(SM_CXSCREEN);
	DeviceInfo.ScreenH = GetSystemMetrics(SM_CYSCREEN);

	if ((DeviceInfo.ScreenW <= 0) && (DeviceInfo.ScreenH <= 0))
		return FALSE;

	return TRUE;
}

int createComponent(HWND hWnd, HINSTANCE hIns)
{
	// ע��ؼ����ߺͼ�϶��������δ���壩
	UINT pos_X = 12;// �ؼ�λ�� X ֵ
	UINT pos_Y = 10;// �ؼ�λ�� Y ֵ
	UINT button_W = 100;
	UINT button_H = 25;
	UINT ctr_space = 10;// �ؼ���׼��϶
	UINT OS_Edit_W = 370;
	UINT OS_Edit_H = button_H;
	UINT format_Group_W = 153;
	UINT format_Group_H = 60;
	UINT format_ComboBox_W = 113;
	UINT format_ComboBox_H = 20;
	UINT resultOutput_Edit_W = 480;
	UINT resultOutput_Edit_H = 95;

	/******************************\
	*  ʵ�ⴰ�ڿͻ�����СΪ 504x282  *
	\******************************/

	// ѡ���ļ�����ť��
	ofnButton = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("ѡ���ļ�"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_OPENFILE, NULL, NULL);
	
	pos_X += ( button_W + ctr_space );

	// ѡ���ļ� - ·����ʾ���ı���
	ofnEdit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_OEMCONVERT | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY,
		pos_X, pos_Y,
		OS_Edit_W, OS_Edit_H,
		hWnd, (HMENU)ID_EDIT_OPENFILE, NULL, NULL);


	pos_X = 12;
	pos_Y += ( button_H + ctr_space );

	// �����ļ�����ť��
	sfnButton = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("����·��"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_SAVEFILE, NULL, NULL);

	pos_X += ( button_W + ctr_space );

	// �����ļ� - ��ʾ·�����ı���
	sfnEdit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_OEMCONVERT | ES_LEFT | ES_AUTOHSCROLL | ES_READONLY,
		pos_X, pos_Y,
		OS_Edit_W, OS_Edit_H,
		hWnd, (HMENU)ID_EDIT_SAVEFILE, NULL, NULL);

	pos_X = 12;
	pos_Y += ( button_H + ctr_space );

// ����ļ���ʽѡ��
	// ͼƬ�߽��
	picFormatGroupBox = CreateWindowEx(0, 
		__TEXT("BUTTON"), __TEXT("תΪͼƬ��ʽ"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X += (format_Group_W + ctr_space );

	// ��Ƶ�߽��
	vidFormatGroupBox = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("תΪ��Ƶ��ʽ"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X += (format_Group_W + ctr_space);

	// ��Ƶ�߽��
	audFormatGroupBox = CreateWindowEx(0,
		__TEXT("BUTTON"), __TEXT("תΪ��Ƶ��ʽ"),
		WS_VISIBLE | WS_CHILD | BS_GROUPBOX,
		pos_X, pos_Y,
		format_Group_W, format_Group_H,
		hWnd, NULL, NULL, NULL);

	pos_X = ( 12 + 2 * ctr_space );
	pos_Y += 22;

	// ͼƬ�����б�
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

	// ��Ƶ�����б�
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

	// ��Ƶ�����б�
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

	// ���������ı���
	resultOutput_Edit = CreateWindowEx(0,
		__TEXT("EDIT"), __TEXT(""),
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
		pos_X, pos_Y,
		resultOutput_Edit_W, resultOutput_Edit_H,
		hWnd, (HMENU)ID_EDIT_RESULT_OUTPUT, NULL, NULL);
	
	pos_X += 190;
	pos_Y += ( resultOutput_Edit_H + ctr_space );

	// ��ʼת������ť��
	startButton = CreateWindowEx(0, 
		__TEXT("BUTTON"), __TEXT("��ʼת��"),
		WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
		pos_X, pos_Y,
		button_W, button_H,
		hWnd, (HMENU)ID_BUTTON_START, NULL, NULL);

	// ��������
	HFONT hFontDef = (HFONT)GetStockObject(DEFAULT_GUI_FONT);// Ĭ�����壨ƫС��
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
	HFONT hFont = CreateFontIndirect(&font);// ������Ҫʹ�õ�����

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
// ѡ���ļ��Ի���
	// ע��ѡ���ļ��Ի�����Ϣ
	OPENFILENAME ofn = { 0 };
	TCHAR strFilePath[MAX_PATH] = { 0 };
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.dwReserved = 0;
		// ��ѡ���ļ�����ɸѡ��
	ofn.lpstrFilter = __TEXT("ͼƬ("
		"*.png;*.jpg;*.bmp;*.webp;*.tif;*.jp2;*.jpeg;*.ppm;*.tga;*.gif)\0"
		"*.png;*.jpg;*.bmp;*.webp;*.tif;*.jp2;*.jpeg;*.ppm;*.tga;*.gif\0"
		
		"��Ƶ("
		"*.mp4,*.flv;*.avi;*.mov;*.wmv;*.m4v;*.f4v;*.ts;*.webm)\0"
		"*.mp4;*.flv;*.avi;*.mov;*.wmv;*.m4v;*.f4v;*.ts;*.webm\0"

		"��Ƶ("
		"*.mp3;*.m4a;*.wma;*.ac3;*.flac;*.ogg;*.wav)\0"
		"*.mp3;*.m4a;*.wma;*.ac3;*.flac;*.ogg;*.wav\0"
		"\0");
	ofn.nFilterIndex = 1;// �ļ�����ɸѡ��������ʼ����ģ�ǧ���ģ����鷳����
	ofn.lpstrFile = strFilePath;
	ofn.nMaxFile = sizeof(strFilePath);
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = __TEXT("��ѡ��һ����Ҫת����ʽ���ļ�");// �Ի������
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

	if (GetOpenFileName(&ofn))
	{
		// �Ի�ȡ���ļ�·�����зָ���洢�ڽṹ����� filePathInfo ��
		_splitpath(strFilePath, filePathInfo.fileDrive, filePathInfo.fileDir, filePathInfo.fileName, filePathInfo.fileExt);
		sprintf(filePathInfo.fileEnv, __TEXT("%s%s"), filePathInfo.fileDrive, filePathInfo.fileDir);
		strcpy(filePathInfo.filePath, strFilePath);

#define INDEX_PicFilterIndex 1
#define INDEX_VidFilterIndex 2
#define INDEX_AudFilterIndex 3

		// ����ѡ�ļ�ʱʹ�õ�ɸѡ������ ������/������أ�Ŀ���ļ�����ѡ���б��ؼ���
		switch (ofn.nFilterIndex)
		{
		case INDEX_PicFilterIndex:// ���� ��ͼƬ���͡� �б�����������
			EnableWindow(picFormat_Combobox, TRUE);
			EnableWindow(vidFormat_Combobox, FALSE);
			EnableWindow(audFormat_Combobox, FALSE);
			fileType = 1;
			break;

		case INDEX_VidFilterIndex:// ���� ����Ƶ���͡� �б�����������
			EnableWindow(picFormat_Combobox, FALSE);
			EnableWindow(vidFormat_Combobox, TRUE);
			EnableWindow(audFormat_Combobox, FALSE);
			fileType = 2;
			break;

		case INDEX_AudFilterIndex:// ���� ����Ƶ���͡� �б�����������
			EnableWindow(picFormat_Combobox, FALSE);
			EnableWindow(vidFormat_Combobox, FALSE);
			EnableWindow(audFormat_Combobox, TRUE);
			fileType = 3;
			break;

		default:
			break;
		}

		// ����ѡ����ļ�·�� ����·����ʾ�ؼ���OPEN/SAVE - EDIT������ʾ���·����Ϣ��
		SendMessage(ofnEdit, WM_SETTEXT, 0, (LPARAM)strFilePath);
		SendMessage(sfnEdit, WM_SETTEXT, 0, (LPARAM)filePathInfo.fileEnv);
	}
	else {
		
		if (CommDlgExtendedError() == FALSE) // �û�ȡ����ѡ��
			return FALSE;

	}

	return TRUE;
}

BOOL saveFileDialog(HWND hWnd)
{
	// �����ļ�λ�öԻ���
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi = { 0 };
	bi.hwndOwner = hWnd;//ӵ���Ŵ��ھ����ΪNULL��ʾ�Ի����Ƿ�ģ̬�ģ�ʵ��Ӧ����һ�㶼Ҫ��������
	bi.pszDisplayName = szBuffer;//�����ļ��еĻ�����
	bi.lpszTitle = TEXT("��ѡ���ļ�����λ��");//����
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
RECHOOSE: {}
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);

	// �û��ڶԻ�����ѡ��"ȡ��"��ť
	if (idl == NULL) return TRUE;

	if (SHGetPathFromIDList(idl, szBuffer)) 
	{	
		// ��ʾ�û�ѡ��ı���·����Ŀ��·����
		PathAddBackslash((LPSTR)szBuffer);
		SendMessage(sfnEdit, WM_SETTEXT, 0, (LPARAM)szBuffer);
	}
	else {
		MessageBox(NULL, TEXT("��ѡ��һ���ļ���\n���ڱ���Ŀ���ļ���"), NULL, MB_ICONERROR);
		goto RECHOOSE;
	}
	
	return TRUE;
}

BOOL startConversion(HWND hWnd)
{
	// ��ʾ״̬��Ϣ
	SetDlgItemText(hWnd, ID_EDIT_RESULT_OUTPUT, __TEXT("FormatMedia->ת����ʼ��\r\n"));
	SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
	SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("��Ϣ�� ��ȡ�����Ϣ��...\r\n"));
	
	// ��ȡ����Ŀ¼
	TCHAR targetDir[MAX_PATH] = { 0 };
	GetDlgItemText(hWnd, ID_EDIT_SAVEFILE, (LPSTR)targetDir, MAX_PATH);

	

#define TYPEID_PIC 1
#define TYPEID_VID 2
#define TYPEID_AUD 3

	// ����ѡ�ļ�ʱʹ�õ�ɸѡ������ ����ȡĿ���ʽ��
	TCHAR targetExt[MAX_PATH] = { 0 };

	switch (fileType)
	{
	case TYPEID_PIC:// �ڡ�ͼƬ�����б��ж�ȡ
		GetDlgItemText(hWnd, ID_COMBOBOX_PIC, (LPSTR)targetExt, MAX_PATH);
		break;

	case TYPEID_VID:// �ڡ���Ƶ�����б��ж�ȡ
		GetDlgItemText(hWnd, ID_COMBOBOX_VID, (LPSTR)targetExt, MAX_PATH);
		break;

	case TYPEID_AUD:// �ڡ���Ƶ�����б��ж�ȡ
		GetDlgItemText(hWnd, ID_COMBOBOX_AUD, (LPSTR)targetExt, MAX_PATH);
		break;

	default:
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("���棺 ��ѡ��һ���ļ�����ָ����ת��Ŀ������!\r\n"));
		return FALSE;
		break;
	}

	// ����ļ���׺֪���ȡ�ɹ�
	if (targetExt[0] == __TEXT('\0') || strlen(targetExt) <= 0) 
	{
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("���� δ�ҵ�Ŀ���ļ����ͣ�\r\n"));
		return FALSE;
	}
	
	// �����ļ���ʶ��(-xxxxYxxMxxDxxHxxMxxS)
	SYSTEMTIME time;
	GetLocalTime(&time);
	TCHAR dateTimeStr[100] = { 0 };
	sprintf(dateTimeStr, "-%dY%02dM%02dD%02dH%02dM%02dS", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	// ����Ŀ���ļ���
	TCHAR targetFile[MAX_PATH] = { 0 };
	sprintf(targetFile, __TEXT("%s%s%s"), filePathInfo.fileName, dateTimeStr, targetExt);

	// ����Ŀ���ļ�����·��(��Ŀ���ļ���)
	TCHAR targetPath[MAX_PATH] = { 0 };
	sprintf(targetPath, __TEXT("%s%s"), targetDir, targetFile);

// �������̣�����FFmpeg
	// ���� ffmpeg.exe ���ھ���·��
	TCHAR ffmpegPath[MAX_PATH] = { 0 };
	sprintf(ffmpegPath, __TEXT("%sbin\\ffmpeg.exe"), workDir);
	PathQuoteSpaces(ffmpegPath);

	// ���������в��������ղ�����
	TCHAR cmdParam[2048] = { 0 };
	sprintf(cmdParam, __TEXT("-i \"%s\" \"%s\""), filePathInfo.filePath, targetPath);

	// ��ʾ״̬��Ϣ
	SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
	SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("��Ϣ����׼������������ƴ��ת����...\r\n"));

	// ���� ffmpeg.exe ���򲢴�����ز���
	HINSTANCE nRet = ShellExecute(resultOutput_Edit, __TEXT("open"), ffmpegPath, cmdParam, NULL, SW_HIDE);

	// ����������
	if ( nRet > 32 ) 
	{
		// ת���ɹ�
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("��Ϣ��ת���ɹ���\r\n"));
	}
	else 
	{
		// ת��ʧ��
		SendMessage(resultOutput_Edit, EM_SETSEL, -2, -1);
		SendMessage(resultOutput_Edit, EM_REPLACESEL, TRUE, (LPARAM)__TEXT("��Ϣ��ת��ʧ�ܣ�\r\n����ϵ���߷����������seayj21@outlook.com\r\n����ȥhttps://github.com/SeaYJ/FormatMedia���ԣ�\r\n"));
		return FALSE;
	}

	return TRUE;
}