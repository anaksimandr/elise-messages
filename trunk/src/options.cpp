/*
Elise Messages Plugin for Miranda IM

*/

#include "options.h"

//-- Defines of settings names
#define ELISE_DEFAULT_OPT		"_default_"
#define ELISE_DEFAULT_OPT_N		"Default"
#define ELISE_TEMPLATE_PATH		"templatePath"
//-- Flags
#define ELISE_BBCODES_ENABLE	"bbcodes"
#define ELISE_URL_PARSE			"urlsparse"
#define ELISE_MESSG_GROUP		"messg_group"
#define ELISE_SHOW_NICK			"nick"
#define ELISE_SHOW_AVATAR		"avatar"
#define ELISE_SHOW_TIME			"time"
#define ELISE_SHOW_SEC			"seconds"
#define ELISE_DATE_SHOW			"date"
#define ELISE_DATE_WORD			"date_word"
#define ELISE_DATE_RELAT		"date_relat"
#define ELISE_SHOW_SMILEYS		"smileys"


//-- Constants, which are means that the option is not set
const QString cqstrNotSet = "NOTSET";
const QUrl cqurlNotSet = "NOTSET";
//-- Note: change cNotSet in inline function Options::updateCheckBox because... and in comments of class SingleOptions
const unsigned char cNotSet = 2;

HANDLE hHookOptionsChanged;
QMap<QString, SingleOptions*>	Options::settings;
QString* Options::currentTab;
QMap<QString, SAVEOPTIONS*>*	Options::settingsToSave;

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
	wchar_t tmp[MAX_PATH] = L"";
	//QString qstrPath;
	HWND hChild;
	switch (msg) {
	case WM_INITDIALOG:
		{
			//Options::MarkInitialized(1);			
			TranslateDialogDefault(hwndDlg);

			//-- Initialisation of protocols list
			hChild = GetDlgItem(hwndDlg, IDC_COMBO_PROTO);
			Options::buildProtocolList(hChild);
			SendMessage(hChild, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			{
				wchar_t* tmp2 = Utils::convertToWCS(ELISE_DEFAULT_OPT);
				wcscpy(tmp, tmp2);
				Options::updateProtocolSettings(hwndDlg, tmp);
				delete tmp2;
			}

			return TRUE;
		}
		break;
	case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {			
				//-- List of checkboxes
			case IDC_CHECKBOX_BBCODES:
			case IDC_CHECKBOX_URL:
			case IDC_CHECKBOX_GROUP:
			case IDC_CHECKBOX_AVATARS:
			case IDC_CHECKBOX_TIME:
			case IDC_CHECKBOX_SECONDS:
			case IDC_CHECKBOX_DATE:
			case IDC_CHECKBOX_DATEWORD:
			case IDC_CHECKBOX_DATERELAT:
			case IDC_CHECKBOX_SMILEYS:
				{
					SendMessage(GetParent(hwndDlg), PSM_CHANGED, 0, 0);
				}
				break;
			case IDC_BROWSE_TEMPLATES:
				if (Options::BrowseFile(hwndDlg, L"Template (*.ivt)\0*.ivt\0All Files\0*.*\0\0", L"ivt", tmp, sizeof(tmp))) {
					wchar_t pszPath2[MAX_PATH];
					CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)tmp, (LPARAM)pszPath2);							
					SetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH, pszPath2);
					SendMessage(GetParent(hwndDlg), PSM_CHANGED, 0, 0);
					//Options::MarkChanges(1, hwndDlg);
				}
				break;
			case IDC_COMBO_PROTO:
				if(HIWORD(wParam) == CBN_SELCHANGE)
					// If the user makes a selection from the list:
					//   Send CB_GETCURSEL message to get the index of the selected list item.
					//   Send CB_GETLBTEXT message to get the item.
					//   Display the item in a messagebox.
				{ 
					int itemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
					if (itemIndex == 0) {
						wchar_t* tmp2 = Utils::convertToWCS(ELISE_DEFAULT_OPT);
						wcscpy(tmp, tmp2);
						delete tmp2;
					}
					else
						(wchar_t)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT, (WPARAM)itemIndex, (LPARAM)tmp);
					//MessageBox(NULL, path, _T("Item Selected"), MB_OK);
					//-- First, save settings from previous tab
					Options::prepareToSave(hwndDlg);
					Options::updateProtocolSettings(hwndDlg, tmp);                        
				}
				break;
			case IDC_BUTTON_DELPATH:
				{
					//wchar_t pszPath2[MAX_PATH];
					//cqstrNotSet.toWCharArray(pszPath2);
					SetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH, L"NOTSET");
					SendMessage(GetParent(hwndDlg), PSM_CHANGED, 0, 0);
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
			Options::destroyTempParts();
		break;
	}
	//return DefWindowProc(hwndDlg, msg, wParam, lParam);
	return FALSE;
}

INT_PTR CALLBACK DlgProcInformation(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//wchar_t path[MAX_PATH] = L"";
	//QString qstrPath;
	//HWND hChild;
	switch (msg) {
	case WM_INITDIALOG:
		{
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
			switch (((LPNMHDR) lParam)->code) {
			case PSN_APPLY:
				//Options::saveSettings(hwndDlg);
				
				NotifyEventHooks(hHookOptionsChanged, 0, 0);
				return TRUE;
			}
		}
		break;
	case WM_DESTROY:
		break;
	}
	return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class SingleOptions --/////////////////////////////////////////////////////////////////////////

SingleOptions::SingleOptions()
{
	currentTemplate = new TemplateMap();

	cBBCodes = cNotSet;
	cURLParse = cNotSet;
	cMessageGrouping = cNotSet;
	cShowAvatar = cNotSet;
	cShowTime = cNotSet;
	cShowSeconds = cNotSet;
	cShowDate = cNotSet;
	cWordDate = cNotSet;
	cRelativeTime = cNotSet;
	cShowSmileys = cNotSet;
//	bJQueryUse = false;
}

SingleOptions::SingleOptions(SingleOptions* other)
{
	currentTemplate = new TemplateMap(other->currentTemplate);

	cBBCodes = other->isBBCodes();
	cURLParse = other->isURLParse();
	cMessageGrouping = other->isMessageGrouping();
	cShowAvatar = other->isShowAvatar();
	cShowTime = other->isShowTime();
	cShowSeconds = other->isShowSeconds();
	cShowDate = other->isShowDate();
	cWordDate = other->isWordDate();
	cShowSmileys = other->isShowSmileys();
	cRelativeTime = other->isRelativeTime();
//	bJQueryUse = other->isJQueryUsed();
}

SingleOptions::SingleOptions(HANDLE hContact)
{
	currentTemplate = Options::isTemplateInited(hContact);
	//bJQueryUse = Options::
	cBBCodes = Options::isBBCodes(hContact);
	cURLParse = Options::isURLParse(hContact);
	cMessageGrouping = Options::isMessageGrouping(hContact);
	cShowAvatar = Options::isShowAvatar(hContact);
	cShowTime = Options::isShowTime(hContact);
	cShowSeconds = Options::isShowSeconds(hContact);
	cShowDate = Options::isShowDate(hContact);
	cWordDate = Options::isWordDate(hContact);
	cRelativeTime = Options::isRelativeTime(hContact);
	cShowSmileys = Options::isShowSmileys(hContact);

}

SingleOptions::~SingleOptions()
{
	
}

//-- class SingleOptions --/////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- class Options --///////////////////////////////////////////////////////////////////////////////

void Options::destroyTempParts()
{
	if (settingsToSave != NULL) {
		QMapIterator<QString, SAVEOPTIONS*> iter(*settingsToSave);
		SAVEOPTIONS* s;
		while (iter.hasNext()) {
			iter.next();
			s = iter.value();
			delete s;
		}
		settingsToSave->~QMap();
		settingsToSave = NULL;
	}
	if (currentTab != NULL) {
		currentTab->~QString();
		currentTab = NULL;
	}
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
	if(GetOpenFileNameW(&ofn)) {
		//wchar_t path2[MAX_PATH] = L"";
		//CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path2);
		//MessageBoxW(NULL, path2, path, MB_OK);
		//SetWindowTextW(hwndDlg, path2);
		return true;
	}
	return false;
}

void Options::getUINs(HANDLE hContact, char* &uinIn, char* &uinOut) {
	CONTACTINFO ci;
	char buf[128];
	char *szProto;
	hContact = getRealContact(hContact);
	szProto = getProto(hContact);
	ZeroMemory(&ci, sizeof(ci));
	ci.cbSize = sizeof(ci);
	ci.hContact = hContact;
	ci.szProto = szProto;
	ci.dwFlag = CNF_UNIQUEID;
	buf[0] = 0;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		switch (ci.type) {
		case CNFT_ASCIIZ:
			mir_snprintf(buf, sizeof(buf), "%s", ci.pszVal);
			mir_free(ci.pszVal);
			break;
		case CNFT_DWORD:
			mir_snprintf(buf, sizeof(buf), "%u", ci.dVal);
			break;
		}
	}
	uinIn = Utils::UTF8Encode(buf);
	ci.hContact = NULL;
	buf[0] = 0;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		switch (ci.type) {
		case CNFT_ASCIIZ:
			mir_snprintf(buf, sizeof(buf), "%s", ci.pszVal);
			mir_free(ci.pszVal);
			break;
		case CNFT_DWORD:
			mir_snprintf(buf, sizeof(buf), "%u", ci.dVal);
			break;
		}
	}
	uinOut = Utils::UTF8Encode(buf);
	delete szProto;
}

void Options::getUINs(HANDLE hContact, char* &uinIn) {
	CONTACTINFO ci;
	char buf[128];
	char* szProto;
	hContact = getRealContact(hContact);
	szProto = getProto(hContact);
	ZeroMemory(&ci, sizeof(ci));
	ci.cbSize = sizeof(ci);
	ci.hContact = hContact;
	ci.szProto = szProto;
	ci.dwFlag = CNF_UNIQUEID;
	buf[0] = 0;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		switch (ci.type) {
		case CNFT_ASCIIZ:
			mir_snprintf(buf, sizeof(buf), "%s", ci.pszVal);
			mir_free(ci.pszVal);
			break;
		case CNFT_DWORD:
			mir_snprintf(buf, sizeof(buf), "%u", ci.dVal);
			break;
		}
	}
	uinIn = Utils::UTF8Encode(buf);
	delete szProto;
}

HANDLE Options::getRealContact(HANDLE hContact) {
	char* szProto = (char*)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0);
	if (szProto != NULL && !strcmp(szProto,"MetaContacts")) {
		hContact = (HANDLE) CallService(MS_MC_GETMOSTONLINECONTACT, (WPARAM) hContact, 0);
	}
	//delete szProto;
	return hContact;
}

char* Options::getProto(HANDLE hContact) {
	return Utils::dupString((char*)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0));
}

char* Options::getProto(const char *proto, HANDLE hContact) {
	if (proto != NULL) {
		return Utils::dupString(proto);
	}
	return Utils::dupString((char*)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0));
}

HANDLE Options::wndToContact(HWND hwnd)
{
	//-- Thanks to Awkward for this
	HANDLE hContact;
	MessageWindowInputData mwid;
	MessageWindowData mwod;
	//MessageBoxA(NULL, "1", "Debug", MB_OK);
	//hwnd = GetParent(hwnd);
	hContact = (HANDLE)CallService(MS_DB_CONTACT_FINDFIRST, 0, 0);
	mwid.cbSize = sizeof(mwid);
	mwid.uFlags = MSG_WINDOW_UFLAG_MSG_BOTH;

	mwod.cbSize = sizeof(mwod);
	while (hContact)
	{
		//MessageBoxA(NULL, "1", "Debug", MB_OK);
		mwid.hContact = hContact;
		if (!CallService(MS_MSG_GETWINDOWDATA, (WPARAM)&mwid, (LPARAM)&mwod)) {
			//MessageBoxA(NULL, "2", "Debug", MB_OK);
			//if ((mwod.uState && MSG_WINDOW_STATE_FOCUS) && (mwod.hwndWindow == hwnd))
				//return mwid.hContact;
			if (mwod.hwndWindow == hwnd) {
				//MessageBoxA(NULL, "3", "Debug", MB_OK);
				return mwid.hContact;
			}
		}
		//MessageBoxA(NULL, "4", "Debug", MB_OK);
		hContact = (HANDLE)CallService(MS_DB_CONTACT_FINDNEXT, (WPARAM)hContact, 0);
		//MessageBoxA(NULL, "5", "Debug", MB_OK);
	}
	return NULL;
}

void Options::buildProtocolList(HWND hwnd)
{
	int protoCount;
	//PROTOCOLDESCRIPTOR** pProtos;
	PROTOACCOUNT** pProtos;
	wchar_t* wszProto;

	//CallService(MS_PROTO_ENUMPROTOCOLS, (WPARAM)&protoCount, (LPARAM)&pProtos);
	/*for (int i = 0; i < protoCount+1; i++) {
		if (i==0) {
			wszProto = Utils::convertToWCS(Translate(ELISE_DEFAULT_OPT_N));
			SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wszProto);
		}
		else {
			if ((pProtos[i-1]->type == PROTOTYPE_PROTOCOL) && strcmp(pProtos[i-1]->szName,"MetaContacts")) {
				if ((CallProtoService(pProtos[i-1]->szName, PS_GETCAPS, PFLAGNUM_1, 0) & PF1_IM) == 0) {
					continue;
				}
				wszProto = Utils::convertToWCS(pProtos[i-1]->szName);
				SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wszProto);
			}
			else
				continue;
		} //else
	} //for*/
	ProtoEnumAccounts(&protoCount, &pProtos);
	for (int i = 0; i < protoCount+1; i++) {
		//ProtocolSettings *proto;
		//char tmpPath[MAX_PATH];
		//char dbsName[256];
		if (i==0) {
			wszProto = Utils::convertToWCS(Translate(ELISE_DEFAULT_OPT_N));
			SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wszProto);
		} else if (strcmp(pProtos[i-1]->szModuleName,"MetaContacts")) {
			if ((CallProtoService(pProtos[i-1]->szModuleName, PS_GETCAPS, PFLAGNUM_1, 0) & PF1_IM) == 0) {
				continue;
			}
			//proto = new ProtocolSettings(pProtos[i-1]->szModuleName);
			wszProto = Utils::convertToWCS(pProtos[i-1]->szModuleName);
			SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)wszProto);
		} else {
			continue;
		}
	}
	delete wszProto;
}

void Options::updateProtocolSettings(HWND hwndDlg, wchar_t proto[MAX_PATH])
{
	HWND hControl;
	wchar_t path[MAX_PATH] = L"";
	QString qsProto;
	//UINT state;

	qsProto = QString::fromWCharArray(proto);

	if (currentTab == NULL)
		currentTab = new QString();
	*currentTab = qsProto;

	if (settingsToSave != NULL) {
		if (settingsToSave->contains(qsProto)) {
			//-- Load from save-map
			SAVEOPTIONS* current = settingsToSave->value(qsProto);

			//-- Path to template
			hControl = GetDlgItem(hwndDlg, IDC_TEMPLATE_PATH);
			SetWindowTextW(hControl, current->pszPath);

			//-- BBCodes			
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_BBCODES);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cBBCodes), 0);

			//-- URL parsing			
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_URL);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cURLParse), 0);

			//-- Message grouping
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_GROUP);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cMessageGrouping), 0);

			//-- Show avatar
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_AVATARS);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cShowAvatar), 0);

			//-- Show time
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_TIME);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cShowTime), 0);

			//-- Show seconds
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SECONDS);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cShowSeconds), 0);

			//-- Show date
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATE);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cShowDate), 0);

			//-- Date word
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATEWORD);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cWordDate), 0);

			//-- Relative date
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATERELAT);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cRelativeTime), 0);

			//-- Smileys
			hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SMILEYS);
			SendMessage(hControl, BM_SETCHECK, updateCheckBox(current->cShowSmileys), 0);

			return;
		} //if
	} //if

	//-- Load from map of settings
	QString qstrPath;

	SingleOptions* curSet = settings.value(qsProto);

	//-- Path to template
	wchar_t path2[MAX_PATH];
	qstrPath = curSet->currentTemplate->getRealTemplatePath();
	qstrPath.toWCharArray(path);
	CallService(MS_UTILS_PATHTORELATIVEW, (WPARAM)path, (LPARAM)path2);
	hControl = GetDlgItem(hwndDlg, IDC_TEMPLATE_PATH);
	SetWindowTextW(hControl, path2);

	//-- BBCodes	
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_BBCODES);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isBBCodes()), 0);

	//-- URL parsing	
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_URL);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isURLParse()), 0);

	//-- Message grouping
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_GROUP);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isMessageGrouping()), 0);

	//-- Show avatar
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_AVATARS);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isShowAvatar()), 0);

	//-- Show time
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_TIME);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isShowTime()), 0);

	//-- Show seconds
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SECONDS);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isShowSeconds()), 0);

	//-- Show date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATE);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isShowDate()), 0);

	//-- Word date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATEWORD);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isWordDate()), 0);

	//-- Relative date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATERELAT);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isRelativeTime()), 0);

	//-- Smileys
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SMILEYS);
	SendMessage(hControl, BM_SETCHECK, updateCheckBox(curSet->isShowSmileys()), 0);

}

int Options::prepareToSave(HWND hwndDlg)
{
	QString qstrProto;
	HWND hControl;
	//unsigned char state;

	qstrProto = *currentTab;
	
	SAVEOPTIONS* current = new SAVEOPTIONS;

	//-- Path to template
	wchar_t pszPath[MAX_PATH];
	GetDlgItemTextW(hwndDlg, IDC_TEMPLATE_PATH,current->pszPath, MAX_PATH);
	//MessageBoxW(NULL, current->pszPath, L"Debug - save template path", MB_OK);

	//-- BBCodes
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_BBCODES);	
	current->cBBCodes = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- URL parsing
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_URL);	
	current->cURLParse = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Message grouping
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_GROUP);	
	current->cMessageGrouping = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Show avatar
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_AVATARS);	
	current->cShowAvatar = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Show time
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_TIME);	
	current->cShowTime = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Show seconds
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SECONDS);	
	current->cShowSeconds = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Show date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATE);	
	current->cShowDate = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Word date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATEWORD);	
	current->cWordDate = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Relative date
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_DATERELAT);	
	current->cRelativeTime = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	//-- Smileys
	hControl = GetDlgItem(hwndDlg, IDC_CHECKBOX_SMILEYS);	
	current->cShowSmileys = readCheckBox(SendMessage(hControl, BM_GETCHECK, 0, 0));

	if (settingsToSave == NULL) {
		settingsToSave = new QMap<QString, SAVEOPTIONS*>();
	}
	else
		if (settingsToSave->contains(qstrProto)) {
			SAVEOPTIONS* del;
			del = settingsToSave->value(qstrProto);
			delete del;
			settingsToSave->remove(qstrProto);
		}
	settingsToSave->insert(qstrProto, current);

	return 0;
}

void Options::saveSettings(HWND hwndDlg) {

	prepareToSave(hwndDlg);

	//-- Save settings and delete them immediately
	QMapIterator<QString, SAVEOPTIONS*> iter(*settingsToSave);
	SAVEOPTIONS* s;
	QString qsProto;
	while (iter.hasNext()) {
		iter.next();
		s = iter.value();
		qsProto = iter.key();
		saveSingleSettings(hwndDlg, qsProto, NULL, s);
		delete s;
	}
	settingsToSave->~QMap();
	settingsToSave = NULL;

}

void Options::saveSingleSettings(HWND hwndDlg, QString qsProto, HANDLE hContact, SAVEOPTIONS* opt)
{
	//-- Note: we must not call delete szProto because it point to QByteArray ar.
	char* szProto = NULL;
	QByteArray ar;
	char dbsName[256];

	//-- If we are saving protocol's settings then we must add "proto" to setting's name
	if (hContact == NULL) {
		ar = qsProto.toAscii();
		szProto = ar.data();
	}

	updateSettingsInMap(qsProto, opt);

	//-- Save template path
	sprintf(dbsName, "%s.%s", szProto, ELISE_TEMPLATE_PATH);
	DBWriteContactSettingTString(NULL, eliseModuleName, dbsName, opt->pszPath);

	//-- Save BBCodes
	sprintf(dbsName, "%s.%s", szProto, ELISE_BBCODES_ENABLE);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cBBCodes);

	//-- Save URL parse
	sprintf(dbsName, "%s.%s", szProto, ELISE_URL_PARSE);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cURLParse);

	//-- Save message grouping
	sprintf(dbsName, "%s.%s", szProto, ELISE_MESSG_GROUP);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cMessageGrouping);

	//-- Save show avatar
	sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_AVATAR);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cShowAvatar);

	//-- Save show time
	sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_TIME);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cShowTime);

	//-- Save show seconds
	sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_SEC);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cShowSeconds);

	//-- Save show date
	sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_SHOW);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cShowDate);

	//-- Save word date
	sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_WORD);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cWordDate);

	//-- Save relative date
	sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_RELAT);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cRelativeTime);

	//-- Save smileys
	sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_SMILEYS);
	DBWriteContactSettingByte(hContact, eliseModuleName, dbsName, opt->cShowSmileys);

}

int Options::updateSettingsInMap(QString qsProto, SAVEOPTIONS* opt)
{
	if (!settings.contains(qsProto))
		return 1;

	SingleOptions* current = settings.value(qsProto);

	//-- Flags
	current->setBBcodes(opt->cBBCodes);
	current->setMessageGrouping(opt->cMessageGrouping);
	current->setRelativeTime(opt->cRelativeTime);
	current->setShowDate(opt->cShowDate);
	current->setShowAvatar(opt->cShowAvatar);
	current->setShowSeconds(opt->cShowSeconds);
	current->setShowTime(opt->cShowTime);
	current->setURLParse(opt->cURLParse);
	current->setWordDate(opt->cWordDate);
	current->setShowSmileys(opt->cShowSmileys);

	//-- Template path
	TemplateMap* templ = current->currentTemplate;
	templ->setTemplatePath(opt->pszPath);

	//-- Try to load template
	//if (!templ->getTemplatePath()->isEmpty())
	//-- Load
	if (templ->loadTemplate()) {
		//-- If loading fails for the default protocol
		if (qsProto == QString::fromAscii(ELISE_DEFAULT_OPT)) {
			QMessageBox qmes;
			qmes.setText("Loading default template fails.\nCan not open file for reading.\n" + templ->getRealTemplatePath());
			qmes.setWindowTitle("Elise messages - error");
			qmes.exec();
		}
	}

	return 0;
}

int Options::loadSingleSettings(char* szProto, HANDLE hContact)
{
	DBVARIANT dbv={0};
	wchar_t pszPath[MAX_PATH];	//-- Used to get template path from db
	char dbsName[256];			//-- tmp string, used in calls of DBGetContactSettingByte
	unsigned char dbResult;		//-- Results of DBGetContactSettingByte

	SingleOptions* newOpt = new SingleOptions();

	//-- Path to template
	sprintf(dbsName, "%s.%s", szProto, ELISE_TEMPLATE_PATH);
	DBGetContactSettingTString(hContact,  eliseModuleName, dbsName, &dbv);

	if (lstrcmp(dbv.ptszVal, NULL) == 0) {
		DBFreeVariant(&dbv);
		
		//-- If settings for this proto was not found then init by default valie
		QString ttmp = cqstrNotSet;
		newOpt->currentTemplate->setTemplatePath(&ttmp);

		//-- Default value
		unsigned char defVal = cNotSet;

		//-- Default options can't have BST_INDETERMINATE value (cNotSet)
		if (!strcmp(szProto, ELISE_DEFAULT_OPT)) {
			defVal = 0;			//-- Note: "0" is disable
		}
		
		newOpt->setBBcodes(defVal);
		newOpt->setURLParse(defVal);
		newOpt->setMessageGrouping(defVal);
		newOpt->setShowAvatar(defVal);
		newOpt->setShowTime(defVal);
		newOpt->setShowSeconds(defVal);
		newOpt->setShowDate(defVal);
		newOpt->setWordDate(defVal);
		newOpt->setRelativeTime(defVal);
		newOpt->setShowSmileys(defVal);
	}
	else {
		//-- If settings was found then load it
		
		//-- Path to template
		CallService(MS_UTILS_PATHTOABSOLUTEW, (WPARAM)dbv.ptszVal, (LPARAM)pszPath);
		newOpt->currentTemplate->setTemplatePath(pszPath);
		DBFreeVariant(&dbv);

		//-- Default value
		unsigned char defVal = cNotSet;
		//-- Default options can't have BST_INDETERMINATE value (cNotSet)
		if (!strcmp(szProto, ELISE_DEFAULT_OPT))
			defVal = 0;

		//-- BBCodes
		sprintf(dbsName, "%s.%s", szProto, ELISE_BBCODES_ENABLE);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setBBcodes(dbResult);
		else
			newOpt->setBBcodes(defVal);

		//-- URL parsing
		sprintf(dbsName, "%s.%s", szProto, ELISE_URL_PARSE);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setURLParse(dbResult);
		else
			newOpt->setURLParse(defVal);

		//-- Message grouping
		sprintf(dbsName, "%s.%s", szProto, ELISE_MESSG_GROUP);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setMessageGrouping(dbResult);
		else
			newOpt->setMessageGrouping(defVal);

		//-- Show avatar
		sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_AVATAR);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setShowAvatar(dbResult);
		else
			newOpt->setShowAvatar(defVal);

		//-- Show time
		sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_TIME);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setShowTime(dbResult);
		else
			newOpt->setShowTime(defVal);

		//-- Show seconds
		sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_SEC);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setShowSeconds(dbResult);
		else
			newOpt->setShowSeconds(defVal);

		//-- Show date
		sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_SHOW);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setShowDate(dbResult);
		else
			newOpt->setShowDate(defVal);

		//-- Word date
		sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_WORD);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setWordDate(dbResult);
		else
			newOpt->setWordDate(defVal);

		//-- Relative date
		sprintf(dbsName, "%s.%s", szProto, ELISE_DATE_RELAT);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setRelativeTime(dbResult);
		else
			newOpt->setRelativeTime(defVal);

		//-- Smileys
		sprintf(dbsName, "%s.%s", szProto, ELISE_SHOW_SMILEYS);
		dbResult = DBGetContactSettingByte(hContact, eliseModuleName, dbsName, -1);
		if (dbResult != -1)
			newOpt->setShowSmileys(dbResult);
		else
			newOpt->setShowSmileys(defVal);


		TemplateMap* templ = newOpt->currentTemplate;

		//-- Try to load template
		if (!templ->getTemplatePath()->isEmpty())
			//-- Load
			if (templ->loadTemplate())
				//-- If loading fails for the default protocol
				if (!strcmp(szProto, ELISE_DEFAULT_OPT))
					//-- If path is not NOTSET
					if (templ->getRealTemplatePath() != cqstrNotSet) {
						QMessageBox qmes;
						qmes.setText("Loading default template fails.\nCan not open file for reading.\n" + templ->getRealTemplatePath());
						qmes.setWindowTitle("Elise messages - error");
						qmes.exec();
					}
	}

	//-- Adding options to map
	QString tmp = QString::fromAscii(szProto);
	settings.insert(tmp, newOpt);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//-- This functions translate call to map members
TemplateMap* Options::isTemplateInited(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	TemplateMap* result;

	findSettingsInMap(hContact, qsUIN, qsProto);

	//-- First, check contact settings
	if (settings.contains(qsUIN)) {
		result = settings.value(qsUIN)->currentTemplate;
		if (result->isTemplateInited())
			return result;
	}
	//-- Second, check protocol settings
	if (settings.contains(qsProto)) {
		result = settings.value(qsProto)->currentTemplate;
		if (result->isTemplateInited())
			return result;
	}
	//-- The last try is checking default settings
	result = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->currentTemplate;
	return result;
}
unsigned char Options::isBBCodes(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isBBCodes();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isBBCodes();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isBBCodes();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}
	return 0;
}
unsigned char Options::isURLParse(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;
	
	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isURLParse();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isURLParse();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isURLParse();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isMessageGrouping(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;
	
	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isMessageGrouping();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);
		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isMessageGrouping();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isMessageGrouping();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isShowAvatar(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isShowAvatar();

	if (dresult != cNotSet && ServiceExists(MS_AV_GETMYAVATAR)) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isShowAvatar();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isShowAvatar();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isShowTime(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;
	
	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isShowTime();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isShowTime();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isShowTime();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isShowSeconds(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;
	
	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isShowSeconds();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isShowSeconds();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isShowSeconds();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isShowDate(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isShowDate();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isShowDate();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isShowDate();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isWordDate(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isWordDate();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isWordDate();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isWordDate();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isRelativeTime(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isRelativeTime();

	if (dresult != cNotSet) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isRelativeTime();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isRelativeTime();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
unsigned char Options::isShowSmileys(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	unsigned char result;
	unsigned char dresult;

	dresult = settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->isShowSmileys();

	if (dresult != cNotSet && ServiceExists(MS_SMILEYADD_BATCHPARSE)) {
		findSettingsInMap(hContact, qsUIN, qsProto);

		if (settings.contains(qsUIN)) {
			result = settings.value(qsUIN)->isShowSmileys();
			if (result != cNotSet)
				return result;
		}
		if (settings.contains(qsProto)) {
			result = settings.value(qsProto)->isShowSmileys();
			if (result != cNotSet)
				return result;
		}
		return dresult;
	}

	return 0;
}
bool Options::isJQueryUsed(HANDLE hContact)
{
	QString qsProto;
	QString qsUIN;
	TemplateMap* result;

	findSettingsInMap(hContact, qsUIN, qsProto);

	//-- First, check contact settings
	if (settings.contains(qsUIN)) {
		result = settings.value(qsUIN)->currentTemplate;
		if (result->isTemplateInited())
			return result->isJQueryUsed();
	}
	//-- Then check protocol settings
	if (settings.contains(qsProto)) {
		result = settings.value(qsProto)->currentTemplate;
		if (result->isTemplateInited())
			return result->isJQueryUsed();
	}
	//-- The last try is checking default settings
	return settings.value(QString::fromAscii(ELISE_DEFAULT_OPT))->currentTemplate->isJQueryUsed();
	//return result;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

int Options::initOptions()
{
	//-- On init we must load default settings and settings for protocols
	int protoCount;
	//PROTOCOLDESCRIPTOR** pProtos;
	PROTOACCOUNT **pProtos;
	ProtoEnumAccounts(&protoCount, &pProtos);

	ProtoEnumAccounts(&protoCount, &pProtos);
	for (int i = 0; i < protoCount+1; i++) {
		//ProtocolSettings *proto;
		//char tmpPath[MAX_PATH];
		//char dbsName[256];
		if (i==0) {
			loadSingleSettings(ELISE_DEFAULT_OPT, NULL);
		}
		else {
			if (strcmp(pProtos[i-1]->szModuleName,"MetaContacts")) {
				//-- hmmmm....
				//if ((CallProtoService(pProtos[i-1]->szName, PS_GETCAPS, PFLAGNUM_1, 0) & PF1_IM) == 0) {
				//	//continue;
				//}
				//MessageBoxA(NULL, pProtos[i-1]->szName, "Debug", MB_OK);
				loadSingleSettings(pProtos[i-1]->szModuleName, NULL);
			}
			else
				continue;
		}
	}
	
	//CallService(MS_PROTO_ENUMPROTOCOLS, (WPARAM)&protoCount, (LPARAM)&pProtos);	
	/*for (int i = 0; i < protoCount+1; i++) {
		if (i==0) {
			loadSingleSettings(ELISE_DEFAULT_OPT, NULL);
		}
		else {
			if ((pProtos[i-1]->type == PROTOTYPE_PROTOCOL) && strcmp(pProtos[i-1]->szName,"MetaContacts")) {
				//-- hmmmm....
				//if ((CallProtoService(pProtos[i-1]->szName, PS_GETCAPS, PFLAGNUM_1, 0) & PF1_IM) == 0) {
				//	//continue;
				//}
				//MessageBoxA(NULL, pProtos[i-1]->szName, "Debug", MB_OK);
				loadSingleSettings(pProtos[i-1]->szName, NULL);
			}
			else
				continue;
		} //else
	} //for*/

	return 0;
}

//-- Options init
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
	//odp.nIDBottomSimpleControl = 0;
	for (i = 0; i < SIZEOF(tabPages); i++) {
		odp.pszTemplate = MAKEINTRESOURCEA(tabPages[i].dlgId);
		odp.pfnDlgProc = tabPages[i].dlgProc;
		odp.ptszTab = tabPages[i].tabName;
		//CallService(MS_OPT_ADDPAGE, wParam, (LPARAM) & odp);
		Options_AddPage(wParam, &odp);
	}
	return 0;
}

//-- class Options --///////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
