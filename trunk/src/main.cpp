
#include <QtCore>
#include <QtGui>
#include <QMfcApp>

#include "main.h"
#include "options.h"
#include "utils.h"
#include "Elise.h"


static const MUUID interfaces[] = {MIID_LOGWINDOW, MIID_LAST};

HINSTANCE hEliseInst;
PLUGINLINK *pluginLink;
char* eliseModuleName;
bool ownApplication = FALSE;

struct MM_INTERFACE mmi;

PLUGININFOEX pluginInfo={
    sizeof(PLUGININFOEX),
    "Elise Messages",
    PLUGIN_MAKE_VERSION(0,0,0,3),
    "Alternate chat log.",
    "anaksimandr",
    "s.andreenko@gmail.com",
    "© 2012 anaksimandr",
    "http://miranda-planet.com/",
    UNICODE_AWARE,
    0,
    {0x0d8f9af0, 0x5302, 0x11e0, { 0xb8, 0xaf, 0x08, 0x00, 0x20, 0x0c, 0x9a, 066 }} //0d8f9af0-5302-11e0-b8af-0800200c9a66
};

extern "C" BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
    // это нужно для работы гуев qt
    ownApplication = QMfcApp::pluginInstance(hinstDLL);

    hEliseInst=hinstDLL;
    return TRUE;
}

extern "C" __declspec(dllexport) PLUGININFOEX* MirandaPluginInfoEx(DWORD mirandaVersion)
{
    if (mirandaVersion < PLUGIN_MAKE_VERSION(0, 9, 0, 0))
        return NULL;
    return &pluginInfo;
}

extern "C" __declspec(dllexport) const MUUID* MirandaPluginInterfaces(void)
{
    return interfaces;
}

extern "C" int __declspec(dllexport) Load(PLUGINLINK* link)
{
    char text[_MAX_PATH];
    char *p, *q;
    GetModuleFileNameA(hEliseInst, text, sizeof(text));
    p = strrchr(text, '\\');
    p++;
    q = strrchr(p, '.');
    *q = '\0';
    eliseModuleName = _strdup(p);
    _strupr(eliseModuleName);

    pluginLink = link;
    mir_getMMI(&mmi);

    return Elise::InitEliseMessages();
}

extern "C" int __declspec(dllexport) Unload(void)
{
	Utils::unhookEvents_Ex();
	Utils::destroyServices_Ex();
	DestroyHookableEvent(hHookOptionsChanged);
	Elise::ReleaseEliseMessages();

	if(ownApplication)
		delete qApp;

	return 0;
}

