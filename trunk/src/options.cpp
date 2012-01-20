/*
Elise Messages Plugin for Miranda IM

*/

#include "options.h"

HANDLE hHookOptionsChanged;

int Options::initialized = 0;
int Options::changed = 0;

void Options::MarkInitialized(int i) {
    if (initialized == 0) {
        //Options::resetProtocolSettings();
        //RefreshProtoIcons();
        //RefreshIcons();
    }
    initialized |= i;
}

void Options::ApplyChanges(int i) {
    changed &= ~i;
    initialized &= ~i;
    if (changed == 0) {
        Options::saveSettings();
        NotifyEventHooks(hHookOptionsChanged, 0, 0);
    }
}

void Options::MarkChanges(int i, HWND hWnd) {
    SendMessage(GetParent(hWnd), PSM_CHANGED, 0, 0);
    changed |= i;
}

INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG:
            {
                Options::MarkInitialized(1);
                TranslateDialogDefault(hwndDlg);
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

void Options::saveSettings() {
    //char dbsName[256];
    //sprintf(dbsName, "%s.%s", proto->getProtocolName(), DBS_SRMM_ENABLE);
    //DBWriteContactSettingByte(NULL, eliseModuleName, dbsName, proto->isSRMMEnable());
    DBWriteContactSettingWString(NULL, eliseModuleName, "templatePath", Options::getTemplatePath());
}

wchar_t* Options::getTemplatePath() {
    wchar_t* path = _T("test");
    return path;
}

// Options init
int Options::InitOptions(WPARAM wParam, LPARAM lParam)
{     
	OPTIONSDIALOGPAGE odp = { 0 };

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
