#include "Menu.h"

#include "Client.h"
#include "Renderer.h"
#include "ImageMgr.h"
#include "Image.h"
#include "ColorPicker.h"
#include "FileBrowser.h"
#include "Settings.h"


HWND CMenu::s_hWnd;

CMenu::CMenu()
{
	s_hWnd = CreateDialogParam(CClient::Get()->GetHInstance(), MAKEINTRESOURCE(IDD_MENU), CClient::Get()->GetRenderer()->GetHWnd(), DialogProc, 0);

	printf("CMenu initialized!\n");
}

CMenu::~CMenu()
{
}

void CMenu::ShowWindow()
{
	CreateThread(0, 0, &Window, 0, 0, 0);
}

DWORD WINAPI CMenu::Window(LPVOID)
{
	MSG msg;
	BOOL ret;

	::ShowWindow(s_hWnd, SW_SHOW);

	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1)
			return 1;

		if (!IsDialogMessage(s_hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}

int CALLBACK CMenu::DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		DestroyWindow(CClient::Get()->GetRenderer()->GetHWnd());
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
	{
		switch (wParam)
		{
			case IDC_MCOLOR:
			{
				CColorPicker cpicker;

				if (cpicker.Accepted())
				{
					CClient::Get()->GetImgMgr()->GetImage()->SetMColor(cpicker.GetRGB());
				}
			} break;
			case IDC_BGCOLOR:
			{
				CColorPicker cpicker;

				if (cpicker.Accepted())
				{
					CClient::Get()->GetImgMgr()->GetImage()->SetBgColor(cpicker.GetRGB());
				}
			} break;
			case IDC_SETTINGS:
			{
				CClient::Get()->GetSettings()->ShowWindow();
			} break;
			case IDC_ISAVE:
			{
				CFileBrowser browser(DIALOG_SAVE, L"PK Image (*.pk)\0*.pk\0");

				if (browser.Accepted())
				{
					wchar_t _fileName[MAX_PATH] = { 0 };
					wcscpy(_fileName, browser.GetFileName());
					int _lenFN = wcslen(_fileName);

					if (_fileName[_lenFN - 1] != 'k' && _fileName[_lenFN - 2] != 'p' && _fileName[_lenFN - 3] != '.')
					{

						wcscat(_fileName, L".pk");
					}

					CClient::Get()->GetImgMgr()->Save(_fileName);
				}
			} break;
			case IDC_ILOAD:
			{
				CFileBrowser browser(DIALOG_OPEN, L"PK Image (*.pk)\0*.pk\0");

				if (browser.Accepted())
				{
					wchar_t _fileName[MAX_PATH] = { 0 };
					wcscpy(_fileName, browser.GetFileName());
					int _lenFN = wcslen(_fileName);

					if (_fileName[_lenFN - 1] != 'k' && _fileName[_lenFN - 2] != 'p' && _fileName[_lenFN - 3] != '.')
					{
						wcscat(_fileName, L".pk");
					}

					CClient::Get()->GetImgMgr()->Load(_fileName);
				}
			} break;
			case IDC_ICLEAR:
			{
				CClient::Get()->GetImgMgr()->GetImage()->ClearScreen();
			} break;
		}
	} break;
	}

	return 0;
}
