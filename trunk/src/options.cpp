/*
Elise Messages Plugin for Miranda IM

*/

#include "options.h"


INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG:
            {
                return TRUE;
            }
            break;
        case WM_COMMAND:
            {

            }
            break;
        case WM_NOTIFY:
            {

            }
            break;
        case WM_DESTROY:
            break;
    }
    return FALSE;
}

// Options init
int EliseOptInit(WPARAM wParam, LPARAM lParam)
{     
	OPTIONSDIALOGPAGE odp = { 0 };

	ZeroMemory(&odp, sizeof(odp));
	odp.cbSize = sizeof(odp);
	odp.position = 100000000;
	odp.hInstance = hEliseInst;
	odp.pszTemplate = MAKEINTRESOURCEA(IDD_GENERAL_OPT);
	odp.ptszGroup = TranslateT("Message Sessions");
	odp.ptszTab = TranslateT("eliseTab");
	odp.ptszTitle = TranslateT("Elise Messages");
	odp.pfnDlgProc = DlgProcOptions;
	odp.flags = ODPF_BOLDGROUPS | ODPF_TCHAR;
	CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);

/*
	DWORD i;
	OPTIONSDIALOGPAGE odp = { 0 };
	odp.cbSize = sizeof(odp);
	odp.position = 0;
	odp.hInstance = hEliseInst;
	odp.ptszGroup = TranslateT("Message Sessions");
	odp.ptszTitle = TranslateT("Elise Messages");
	odp.flags = ODPF_BOLDGROUPS | ODPF_TCHAR;
	odp.nIDBottomSimpleControl = 0;
	//for (i = 0; i < SIZEOF(tabPages); i++) {
		odp.pszTemplate = MAKEINTRESOURCEA(IDD_GENERAL_OPT);
		odp.pfnDlgProc = DlgProcOptions;
		odp.ptszTab = _T("General");
                CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);*
	//}

	OPTIONSDIALOGPAGE odp = {0};

	ZeroMemory(&odp, sizeof(odp));
	odp.cbSize = sizeof(odp);
	odp.position = 100000000;
	odp.hInstance = hEliseInst;
	odp.pszTemplate = MAKEINTRESOURCEA(IDD_GENERAL_OPT);
	odp.ptszGroup = _T("Message Sessions");
	odp.ptszTab = _T("eliseTab");
	odp.ptszTitle = _T("Elise Messages");
	odp.pfnDlgProc = DlgProcOptions;
	odp.flags = ODPF_BOLDGROUPS | ODPF_TCHAR;
	CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);
*/
        return 0;
}
