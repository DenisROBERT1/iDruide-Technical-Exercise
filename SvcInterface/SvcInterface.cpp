// SvcInterface.cpp : définit le point d'entrée pour l'application.
//

#include "stdafx.h"
#include "SvcInterface.h"
#include "TCltPipe.h"

#define MAX_LOADSTRING 100
#define MYWM_NOTIFY         (WM_USER + 1)

//--------------------------GLOBAL VARIABLES---------------------------------

HINSTANCE HInstance;								// current instance
HWND hWndTop;

// Pré-déclarations des fonctions incluses dans ce module de code :
INT_PTR CALLBACK	SvcInterfaceProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	AboutProc(HWND, UINT, WPARAM, LPARAM);
LRESULT FAR PASCAL SysTrayProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);

bool CreateSysTrayIcon(HWND hDlg);
bool DeleteSysTrayIcon(HWND hDlg);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HInstance = hInstance;

	DialogBox(HInstance, MAKEINTRESOURCE(IDD_SVCINTERFACE_DIALOG), GetDesktopWindow(), SvcInterfaceProc);

	return 0;
}

//---------------------------------------------------------------------------
// Message handler for main form box.
//---------------------------------------------------------------------------

INT_PTR CALLBACK SvcInterfaceProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);
	char szCommand[1024];
	char szReturn[1024];
	static TCltPipeIDruide *CltPipeIDruide;


	switch (message) {
	case WM_INITDIALOG:
	  hWndTop = hDlg;
		CreateSysTrayIcon(hDlg);
		CheckDlgButton(hDlg, IDD_CHECKBOX_CAMERA, MF_CHECKED);
		SetDlgItemText(hDlg, IDD_EDIT_LENGTH, TEXT("10"));
		CheckDlgButton(hDlg, IDD_RADIOBUTTON_ALLOWLETTERS, MF_CHECKED);
		CltPipeIDruide = new TCltPipeIDruide();
		if (CltPipeIDruide->CreateCltNamedPipe(TEXT("\\\\.\\pipe\\iDruide")))
		{
			CltPipeIDruide->SendCommand("INFO", szReturn, sizeof(szReturn));
			SetDlgItemTextA(hDlg, IDD_LABEL_INFORMATIONS, szReturn);
		}
		return (INT_PTR) TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDD_BUTTON_APPLY:
				sprintf_s(szCommand, sizeof(szCommand), "PARAM %i %02i %i",
					(SendDlgItemMessage(hDlg, IDD_CHECKBOX_CAMERA, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1: 0,
					GetDlgItemInt(hDlg, IDD_EDIT_LENGTH, FALSE, FALSE),
					(SendDlgItemMessage(hDlg, IDD_RADIOBUTTON_ALLOWLETTERS, BM_GETCHECK, 0, 0) == BST_CHECKED)? 1: 0);
				CltPipeIDruide->SendCommand(szCommand, szReturn, sizeof(szReturn));
				break;
			case IDD_BUTTON_RESTART:
				CltPipeIDruide->SendCommand("RESTART", szReturn, sizeof(szReturn));
				break;
			case IDM_EXIT:
			case IDD_BUTTON_QUIT:
			case IDCANCEL:
				EndDialog(hDlg, LOWORD(wParam));
				break;
			case IDM_ABOUT:
				DialogBox(HInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, AboutProc);
				break;
		}
		break;

	case WM_DESTROY:
		CltPipeIDruide->SendCommand("END", szReturn, sizeof(szReturn));
		delete CltPipeIDruide;
		DeleteSysTrayIcon(hDlg);
		return (INT_PTR) TRUE;

	}
	return (INT_PTR) FALSE;
}


//---------------------------------------------------------------------------
// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK AboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//---------------------------------------------------------------------------
LRESULT FAR PASCAL SysTrayProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(Handle, Message, wParam, lParam);
}

bool CreateSysTrayIcon(HWND hDlg)
{
  NOTIFYICONDATA nid;


  memset(&nid, 0, sizeof(NOTIFYICONDATA));
  nid.cbSize = sizeof(NOTIFYICONDATA);
  nid.hWnd = hDlg;
  nid.uID = IDC_TRAYICON;
  nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
  nid.uCallbackMessage = MYWM_NOTIFY;
  nid.hIcon = LoadIcon(HInstance, (LPTSTR) IDI_SMALL);
  lstrcpyn(nid.szTip, TEXT("iDruide service interface"), sizeof(nid.szTip));

  Shell_NotifyIcon(NIM_ADD, &nid);

	return true;
}

bool DeleteSysTrayIcon(HWND hDlg)
{
  NOTIFYICONDATA tnd;


  memset(&tnd, 0, sizeof(NOTIFYICONDATA));
  tnd.cbSize = sizeof(NOTIFYICONDATA);
  tnd.hWnd = hDlg;
  tnd.uID = IDC_TRAYICON;

  Shell_NotifyIcon(NIM_DELETE, &tnd);

	return true;
}
