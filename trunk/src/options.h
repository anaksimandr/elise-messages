/*
Elise Messages Plugin for Miranda IM

*/

class Options;

#ifndef OPTIONS_H
#define OPTIONS_H

#include "main.h"
#include "template.h"
#include "../resource.h"

class Options {
private:
	//static int initialized;
	//static int changed;
	static QUrl		qurlSkinPath;
	static QString	qstrSkinPath;
public:
	static void		saveSettings(HWND hwnd);
	static int		initOptionsPage(WPARAM wParam, LPARAM lParam);
	//static int		modulesLoaded(WPARAM wParam, LPARAM lParam);
	//static void MarkInitialized(int i);
	//static void ApplyChanges(int i);
	//static void MarkChanges(int i, HWND hWnd);
	static bool		BrowseFile(HWND hwndDlg, wchar_t* filter, wchar_t* defExt,  wchar_t* path, int maxLen);
	static QString*	getTemplatePath();
	static QString	getRealTemplatePath();
	static QUrl		getTemplateUrl();
	static bool		initOptions();
};

#endif // OPTIONS_H
