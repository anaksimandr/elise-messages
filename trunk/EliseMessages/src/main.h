

#ifndef MAIN_H
#define MAIN_H

//#define _UNICODE

//замена __declspec(dllexport)
//#define D_SHARED Q_DECL_EXPORT

#define SIZEOF(X) (sizeof(X)/sizeof(X[0]))

#define MIRANDA_VER 0x0900

//#include <QMfcApp>
//#include <QWinWidget>
//#include <CommCtrl.h>
#include <QtCore>
#include <QMap>
#include <windows.h>
#include <tchar.h>
#include <newpluginapi.h>
#include <m_langpack.h>
#include <m_clist.h>
#include <m_skin.h>
#include <m_system.h>
#include <m_options.h>
#include <m_database.h>
#include <m_message.h>
#include <m_protocols.h>
#include "m_metacontacts.h"
#include <m_avatars.h>
#include "m_smileyadd.h"
#include <m_protosvc.h>
//#include <QMessageBox>

extern HINSTANCE hEliseInst;
extern char* eliseModuleName;
extern HANDLE hHookOptionsChanged;

#endif // MAIN_H
