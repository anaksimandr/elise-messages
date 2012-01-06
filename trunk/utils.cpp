/*
Elise Messages Plugin for Miranda IM

*/

#include "utils.h"

//wchar_t Utils::base_dir[MAX_PATH];
unsigned Utils::hookNum = 0;
unsigned Utils::serviceNum = 0;
HANDLE* Utils::hHooks = NULL;
HANDLE* Utils::hServices = NULL;


HANDLE Utils::hookEvent_Ex(const char *name, MIRANDAHOOK hook) {
    hookNum ++;
    hHooks = (HANDLE *) mir_realloc(hHooks, sizeof(HANDLE) * (hookNum));
    hHooks[hookNum - 1] = HookEvent(name, hook);
    return hHooks[hookNum - 1] ;
}

HANDLE Utils::createServiceFunction_Ex(const char *name, MIRANDASERVICE service) {
    serviceNum++;
    hServices = (HANDLE *) mir_realloc(hServices, sizeof(HANDLE) * (serviceNum));
    hServices[serviceNum - 1] = CreateServiceFunction(name, service);
    return hServices[serviceNum - 1] ;
}

void Utils::unhookEvents_Ex() {
	unsigned i;
	for (i=0; i<hookNum ; ++i) {
		if (hHooks[i] != NULL) {
			UnhookEvent(hHooks[i]);
		}
	}
	mir_free(hHooks);
	hookNum = 0;
	hHooks = NULL;
}

void Utils::destroyServices_Ex() {
	unsigned i;
	for (i=0; i<serviceNum; ++i) {
		if (hServices[i] != NULL) {
			DestroyServiceFunction(hServices[i]);
		}
	}
	mir_free(hServices);
	serviceNum = 0;
	hServices = NULL;
}

char* Utils::dupString(const char* a) {
	if (a!=NULL) {
		char *b = new char[strlen(a)+1];
		strcpy(b, a);
		return b;
	}
	return NULL;
}

char* Utils::dupString(const char* a, int l) {
	if (a!=NULL) {
		char *b = new char[l+1];
		strncpy(b, a, l);
		b[l] ='\0';
		return b;
	}
	return NULL;
}

wchar_t* Utils::dupString(const wchar_t* a) {
	if (a!=NULL) {
		wchar_t *b = new wchar_t[wcslen(a)+1];
		wcscpy(b, a);
		return b;
	}
	return NULL;
}

wchar_t* Utils::dupString(const wchar_t *a, int l) {
	if (a!=NULL) {
		wchar_t* b = new wchar_t[l+1];
		wcsncpy(b, a, l);
		b[l] ='\0';
		return b;
	}
	return NULL;
}

wchar_t* Utils::convertToWCS(const char* a) {
	if (a!=NULL) {
		int len = (int)strlen(a)+1;
		wchar_t *b = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, a, len, b, len);
		return b;
	}
	return NULL;
}

wchar_t* Utils::convertToWCS(const char* a, int cp) {
	if (a!=NULL) {
		int len = (int)strlen(a)+1;
		wchar_t* b = new wchar_t[len];
		MultiByteToWideChar(cp, 0, a, len, b, len);
		return b;
	}
	return NULL;
}

char* Utils::UTF8Encode(const wchar_t *wtext) {
	unsigned char *szOut;
	int len, i;
	const wchar_t *w;

	if (wtext == NULL) return NULL;
	for (len=0, w=wtext; *w; w++) {
		if (*w < 0x0080) len++;
		else if (*w < 0x0800) len += 2;
		else len += 3;
	}
	szOut = new unsigned char [len+1];
	if (szOut == NULL) return NULL;

	for (i=0, w=wtext; *w; w++) {
		if (*w < 0x0080)
			szOut[i++] = (unsigned char) *w;
		else if (*w < 0x0800) {
			szOut[i++] = 0xc0 | ((*w) >> 6);
			szOut[i++] = 0x80 | ((*w) & 0x3f);
		}
		else {
			szOut[i++] = 0xe0 | ((*w) >> 12);
			szOut[i++] = 0x80 | (((*w) >> 6) & 0x3f);
			szOut[i++] = 0x80 | ((*w) & 0x3f);
		}
	}
	szOut[i] = '\0';
	return (char *) szOut;
}

char* Utils::UTF8Encode(const char *text) {
	wchar_t *wtext = Utils::convertToWCS(text);
	char *atext = UTF8Encode(wtext);
	delete wtext;
	return atext;
}

void Utils::UTF8Encode(const char *text, char *output, int maxLen) {
	wchar_t *wtext = Utils::convertToWCS(text);
	char *atext = UTF8Encode(wtext);
	int slen = (int)strlen(atext) + 1;
	memcpy(output, atext, slen > maxLen ? maxLen : slen);
	output[maxLen - 1] = '\0';
	delete atext;
	delete wtext;
}

void Utils::UTF8Encode(const wchar_t *wtext, char *output, int maxLen) {
	char *atext = UTF8Encode(wtext);
	int slen = (int)strlen(atext) + 1;
	memcpy(output, atext, slen > maxLen ? maxLen : slen);
	output[maxLen - 1] = '\0';
	delete atext;
}
