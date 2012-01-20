/*
Elise Messages Plugin for Miranda IM

*/

class Options;

#ifndef OPTIONS_H
#define OPTIONS_H

#include "main.h"
#include "resource.h"

class Options {
private:
    static int initialized;
    static int changed;

    static wchar_t* getTemplatePath();

public:
    static void saveSettings();
    static int InitOptions(WPARAM wParam, LPARAM lParam);
    static void MarkInitialized(int i);
    static void ApplyChanges(int i);
    static void MarkChanges(int i, HWND hWnd);
};

#endif // OPTIONS_H
