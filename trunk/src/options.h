/*
Elise Messages Plugin for Miranda IM

*/

class Options;

#ifndef OPTIONS_H
#define OPTIONS_H

#include "main.h"
#include "template.h"
#include "../resource.h"

//-- This class is used to store one exemplar of settings
//-- (i.e. settings for one contact or protocol... or general default settings).
class SingleOptions {
private:
	QString			qstrName;

	QUrl			qurlSkinPath;
	QString			qstrSkinPath;

	unsigned char	bBBcodes;
	unsigned char	bURLParse;
	unsigned char	bMessageGrouping;
	unsigned char	bShowNick;
	unsigned char	bShowTime;
	unsigned char	bShowSeconds;
	unsigned char	bShowDate;
	unsigned char	bWordDate;
	unsigned char	bRelativeTime;
public:
					SingleOptions(QString& name);
					~SingleOptions();

	QUrl			getTemplateUrl();
	QString*		getTemplatePath();
	QString			getRealTemplatePath();

	//-- We are setting qurlSkinPath in setTemplatePath.
	//void			setTemplateUrl();
	void			setTemplatePath(wchar_t pszPath[MAX_PATH]);

	//-- Getters
	inline unsigned char	isBBcodes() {return bBBcodes;};
	inline unsigned char	isURLParse() {return bURLParse;};
	inline unsigned char	isMessageGrouping() {return bMessageGrouping;};
	inline unsigned char	isShowNick() {return bShowNick;};
	inline unsigned char	isShowTime() {return bShowTime;};
	inline unsigned char	isShowSeconds() {return bShowSeconds;};
	inline unsigned char	isShowDate() {return bShowDate;};
	inline unsigned char	isWordDate() {return bWordDate;};
	inline unsigned char	isRelativeTime() {return bRelativeTime;};

	//-- Setters
	inline void			setBBcodes(unsigned char val);
	inline void			setURLParse(unsigned char val);
	inline void			setMessageGrouping(unsigned char val);
	inline void			setShowNick(unsigned char val);
	inline void			setShowTime(unsigned char val);
	inline void			setShowSeconds(unsigned char val);
	inline void			setShowDate(unsigned char val);
	inline void			setWordDate(unsigned char val);
	inline void			setRelativeTime(unsigned char val);
};

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
