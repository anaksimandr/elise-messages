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
		//Options::saveSettings();
		NotifyEventHooks(hHookOptionsChanged, 0, 0);
	}
}

void Options::MarkChanges(int i, HWND hWnd) {
	SendMessage(GetParent(hWnd), PSM_CHANGED, 0, 0);
	changed |= i;
}

INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wchar_t path[MAX_PATH];
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
			switch (LOWORD(wParam)) {
			case IDC_BROWSE_TEMPLATES:
				if (Options::BrowseFile(hwndDlg, L"Template (*.ivt)\0*.ivt\0All Files\0*.*\0\0", L"ivt", path, sizeof(path))) {
					SetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH, path);
					Options::MarkChanges(1, hwndDlg);
				}
				break;
			}			
		}
		break;
	case WM_NOTIFY:
		{
			switch (((LPNMHDR) lParam)->code) {
			case PSN_APPLY:
				Options::saveSettings(hwndDlg);
				Options::ApplyChanges(1);
				return TRUE;
			}
		}
		break;
	case WM_DESTROY:
		break;
	}
	DefWindowProc(hwndDlg, msg, wParam, lParam);
	return FALSE;
}

bool Options::BrowseFile(HWND hwndDlg, wchar_t* filter, wchar_t* defExt,  wchar_t* path, int maxLen) {
	OPENFILENAMEW ofn={0};
	//GetWindowTextW(hwndDlg, path, maxLen);
	ofn.lStructSize = sizeof(OPENFILENAME);//_SIZE_VERSION_400;
	ofn.hwndOwner = hwndDlg;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = filter;//"Templates (*.ivt)\0*.ivt\0All Files\0*.*\0\0";
	ofn.lpstrFile = path;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.nMaxFile = maxLen;
	ofn.nMaxFileTitle = maxLen;
	ofn.lpstrDefExt = defExt;//"ivt";
	ofn.lpstrFileTitle = L"test";
	if(GetOpenFileNameW(&ofn)) {
		SetWindowTextW(hwndDlg, path);
		return true;
	}
	return false;
}

void Options::saveSettings(HWND hwnd) {
	wchar_t pszPath[MAX_PATH];
	//char dbsName[256];
	//sprintf(dbsName, "%s.%s", proto->getProtocolName(), DBS_SRMM_ENABLE);
	//DBWriteContactSettingByte(NULL, eliseModuleName, dbsName, proto->isSRMMEnable());
	GetDlgItemTextW(hwnd,
		IDC_TEMPLATE_PATH,
		pszPath,
		MAX_PATH);
	//MessageBoxW(NULL, pszPath, L"Debug", MB_OK);
	DBWriteContactSettingWString(NULL, eliseModuleName, "templatePath", pszPath);
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
