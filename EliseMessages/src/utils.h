/*
Elise Messages Plugin for Miranda IM

*/

class Utils;

#ifndef UTILS_H
#define UTILS_H

#include "main.h"


class Utils {
private:
	static unsigned hookNum;
	static unsigned serviceNum;
	static HANDLE* hHooks;
	static HANDLE* hServices;
public:
	static HANDLE hookEvent_Ex(const char *name, MIRANDAHOOK hook);
	static HANDLE createServiceFunction_Ex(const char *name, MIRANDASERVICE service);
	static void unhookEvents_Ex();
	static void destroyServices_Ex();
	static char* dupString(const char* a);
	static char* dupString(const char* a, int l);
	static wchar_t* dupString(const wchar_t* a);
	static wchar_t* dupString(const wchar_t* a, int l);
	static wchar_t* convertToWCS(const char* a);
	static wchar_t* convertToWCS(const char* a, int cp);
	//static wchar_t* newWCS(const wchar_t* a);
	static char* convertToString(const wchar_t *a);
	// надобность вот этих, вот, функций еще стоит проверить
	static char* UTF8Encode(const wchar_t *wtext);
	static char* UTF8Encode(const char *text);
	static void  UTF8Encode(const char *text, char *output, int maxLen);
	static void  UTF8Encode(const wchar_t *text, char *output, int maxLen);
};

#endif // UTILS_H
