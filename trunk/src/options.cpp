/*
Elise Messages Plugin for Miranda IM

*/

#include "options.h"

HANDLE hHookOptionsChanged;
QUrl Options::qurlSkinPath;
QString Options::qstrSkinPath;

static INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);
static INT_PTR CALLBACK DlgProcInformation(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam);

typedef struct TabDefStruct {
	DLGPROC dlgProc;
	DWORD dlgId;
	TCHAR* tabName;
} TabDef;

static TabDef tabPages[] = {
	{DlgProcOptions, IDD_GENERAL_OPT, _T("General")},
	{DlgProcInformation, IDD_INFORMATION_OPT, _T("Information")}
};

//int Options::initialized = 0;
//int Options::changed = 0;
/*
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
}*/


INT_PTR CALLBACK DlgProcOptions(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wchar_t path[MAX_PATH] = L"";
	QString qstrPath;
	HWND hChild;
	switch (msg) {
	case WM_INITDIALOG:
		{
			//Options::MarkInitialized(1);
			TranslateDialogDefault(hwndDlg);
			qstrPath = Options::getRealTemplatePath();
			qstrPath.toWCharArray(path);			
			CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path);
			hChild = GetDlgItem(hwndDlg, IDC_TEMPLATE_PATH);
			SetWindowTextW(hChild, path);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
			case IDC_BROWSE_TEMPLATES:
				if (Options::BrowseFile(hwndDlg, L"Template (*.ivt)\0*.ivt\0All Files\0*.*\0\0", L"ivt", path, sizeof(path))) {
					//wchar_t path2[MAX_PATH];					
					CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path);							
					SetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH, path);
					SendMessage(GetParent(hwndDlg), PSM_CHANGED, 0, 0);
					//Options::MarkChanges(1, hwndDlg);
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
				//Options::ApplyChanges(1);
				NotifyEventHooks(hHookOptionsChanged, 0, 0);
				return TRUE;
			}
		}
		break;
	case WM_DESTROY:
		break;
	}
	//return DefWindowProc(hwndDlg, msg, wParam, lParam);
	return FALSE;
}

INT_PTR CALLBACK DlgProcInformation(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	wchar_t path[MAX_PATH] = L"";
	QString qstrPath;
	HWND hChild;
	switch (msg) {
	case WM_INITDIALOG:
		{
			//Options::MarkInitialized(1);
			TranslateDialogDefault(hwndDlg);
			//qstrPath = Options::getRealTemplatePath();
			//qstrPath.toWCharArray(path);			
			//CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path);
			//hChild = GetDlgItem(hwndDlg, IDC_TEMPLATE_PATH);
			//SetWindowTextW(hChild, path);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
			case IDC_BROWSE_TEMPLATES:
				//if (Options::BrowseFile(hwndDlg, L"Template (*.ivt)\0*.ivt\0All Files\0*.*\0\0", L"ivt", path, sizeof(path))) {
					//wchar_t path2[MAX_PATH];					
					//CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path);							
					//SetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH, path);
					//SendMessage(GetParent(hwndDlg), PSM_CHANGED, 0, 0);
					//Options::MarkChanges(1, hwndDlg);
				//}
				break;
			}			
		}
		break;
	case WM_NOTIFY:
		{
			switch (((LPNMHDR) lParam)->code) {
			case PSN_APPLY:
				Options::saveSettings(hwndDlg);
				//Options::ApplyChanges(1);
				NotifyEventHooks(hHookOptionsChanged, 0, 0);
				return TRUE;
			}
		}
		break;
	case WM_DESTROY:
		break;
	}
	//return DefWindowProc(hwndDlg, msg, wParam, lParam);
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class SingleOptions --/////////////////////////////////////////////////////////////////////////

void SingleOptions::setBBcodes(unsigned char val)
{
	bBBcodes = val;
}
void SingleOptions::setURLParse(unsigned char val)
{
	bURLParse = val;
}
void SingleOptions::setMessageGrouping(unsigned char val)
{
	bMessageGrouping = val;
}
void SingleOptions::setShowNick(unsigned char val)
{
	bShowNick = val;
}
void SingleOptions::setShowTime(unsigned char val)
{
	bShowTime = val;
}
void SingleOptions::setShowSeconds(unsigned char val)
{
	bShowSeconds = val;
}
void SingleOptions::setShowDate(unsigned char val)
{
	bShowDate = val;
}
void SingleOptions::setWordDate(unsigned char val)
{
	bWordDate = val;
}
void SingleOptions::setRelativeTime(unsigned char val)
{
	bRelativeTime = val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

QString* SingleOptions::getTemplatePath() {	
	return &qstrSkinPath;
}

QString SingleOptions::getRealTemplatePath() {	
	return qstrSkinPath;
}

QUrl SingleOptions::getTemplateUrl() {
	return qurlSkinPath;
}

void SingleOptions::setTemplatePath(wchar_t pszPath[MAX_PATH])
{
	CallService(MS_UTILS_PATHTOABSOLUTEW, (WPARAM)pszPath, (LPARAM)pszPath);
	qstrSkinPath = QString::fromWCharArray(pszPath);
	qurlSkinPath = QUrl::fromLocalFile(qstrSkinPath);
}


//-- class SingleOptions --/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class Options --///////////////////////////////////////////////////////////////////////////////

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
	if(GetOpenFileNameW(&ofn)) {
		//wchar_t path2[MAX_PATH] = L"";
		//CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path2);
		//MessageBoxW(NULL, path2, path, MB_OK);
		//SetWindowTextW(hwndDlg, path2);
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
	//MessageBoxW(NULL, L"Saving settings", L"Debug", MB_OK);
	DBWriteContactSettingTString(NULL, eliseModuleName, "templatePath", pszPath);
	CallService(MS_UTILS_PATHTOABSOLUTEW, (WPARAM)pszPath, (LPARAM)pszPath);
	qstrSkinPath = QString::fromWCharArray(pszPath);
	qurlSkinPath = QUrl::fromLocalFile(qstrSkinPath);
	TemplateMap::loadTemplate(qstrSkinPath);	
}

QString* Options::getTemplatePath() {	
	return &qstrSkinPath;
}

QString Options::getRealTemplatePath() {	
	return qstrSkinPath;
}

QUrl Options::getTemplateUrl() {
	return qurlSkinPath;
}

bool Options::initOptions() {	
	DBVARIANT dbv={0};	
	wchar_t pszPath[MAX_PATH];

	DBGetContactSettingTString(NULL,  eliseModuleName, "templatePath", &dbv);
	if (lstrcmp(dbv.ptszVal, NULL) == 0) {
		DBFreeVariant(&dbv);
	}
	else {
		CallService(MS_UTILS_PATHTOABSOLUTEW, (WPARAM)dbv.ptszVal, (LPARAM)pszPath);
		qstrSkinPath = QString::fromWCharArray(pszPath);
		qurlSkinPath = QUrl::fromLocalFile(qstrSkinPath);
	}	
	
	if (qstrSkinPath.isEmpty()) {
		return false;
	}
	else {
		return true;
	}
}

//--Options init
int Options::initOptionsPage(WPARAM wParam, LPARAM lParam)
{
	DWORD i;
	OPTIONSDIALOGPAGE odp = { 0 };
	odp.cbSize = sizeof(odp);
	odp.position = 0;
	odp.hInstance = hEliseInst;
	odp.ptszGroup = _T("Message Sessions");
	odp.ptszTitle = _T("Elise Messages");
	odp.flags = ODPF_BOLDGROUPS | ODPF_TCHAR;
	odp.nIDBottomSimpleControl = 0;
	for (i = 0; i < SIZEOF(tabPages); i++) {
		odp.pszTemplate = MAKEINTRESOURCEA(tabPages[i].dlgId);
		odp.pfnDlgProc = tabPages[i].dlgProc;
		odp.ptszTab = tabPages[i].tabName;
		CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);
	}
	return 0;
}

//-- class Options --///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
