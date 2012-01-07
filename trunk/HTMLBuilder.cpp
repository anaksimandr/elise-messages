
#include "HTMLBuilder.h"
//#include <QMessageBox>

// tabsrmm stuff

#define MWF_LOG_SHOWNICK 512
#define MWF_LOG_SHOWTIME 1024
#define MWF_LOG_SHOWSECONDS 2048
#define MWF_LOG_SHOWDATES 4096
#define MWF_LOG_NEWLINE 8192
#define MWF_LOG_INDENT 16384
#define MWF_LOG_RTL 32768
#define MWF_LOG_UNDERLINE 65536
#define MWF_LOG_SWAPNICK 131072
#define MWF_LOG_SHOWICONS 262144

#define MWF_LOG_INDENTWITHTABS 1048576
#define MWF_LOG_SYMBOLS 0x200000
#define MWF_LOG_TEXTFORMAT 0x2000000
#define MWF_LOG_GRID 0x4000000
#define MWF_LOG_INDIVIDUALBKG 0x8000000

#define MWF_DIVIDERWANTED 0x40000000
#define MWF_LOG_GROUPMODE 0x80000000
#define MWF_LOG_LONGDATES 64
#define MWF_LOG_USERELATIVEDATES 1

#define MWF_SHOW_URLEVENTS 1
#define MWF_SHOW_FILEEVENTS 2
#define MWF_SHOW_INOUTICONS 4
#define MWF_SHOW_EMPTYLINEFIX 8
#define MWF_SHOW_MICROLF 16
#define MWF_SHOW_MARKFOLLOWUPTS 32

#define SRMSGMOD "SRMsg"
#define SRMSGMOD_T "Tab_SRMsg"
#define TABSRMM_FONTMODULE "TabSRMM_Fonts"

#define EVENTTYPE_DIVIDER 25367
#define EVENTTYPE_ERRMSG 25366

#define SRMSGSET_SHOWURLS          "ShowURLs"
#define SRMSGSET_SHOWFILES         "ShowFiles"
#define SRMSGSET_SHOWSTATUSCHANGES "ShowFiles"

#define MWF_LOG_DEFAULT (MWF_LOG_SHOWTIME | MWF_LOG_SHOWNICK | MWF_LOG_SHOWDATES)

#define FONTF_BOLD   1
#define FONTF_ITALIC 2
#define FONTF_UNDERLINE 4

#define FONT_NUM 19


extern QUrl skinDir;


HTMLBuilder::HTMLBuilder(Elise* view) {
//HTMLBuilder::HTMLBuilder() {
    startedTime = time(NULL);
    parentView = view;
    //webView = view->getWebView();
}

HTMLBuilder::~HTMLBuilder() {
    // some code
}

void HTMLBuilder::appendEventTemplate(Elise* view, IEVIEWEVENT* event) {
    HANDLE hRealContact;
    char* szRealProto = NULL;
    char* szProto = NULL;

    hRealContact = getRealContact(event->hContact);
    szRealProto = getProto(hRealContact);
    szProto = getProto(event->pszProto, event->hContact);

    IEVIEWEVENTDATA* eventData = event->eventData;
    for (int eventIdx = 0; eventData!=NULL && (eventIdx < event->count || event->count==-1); eventData = eventData->next, eventIdx++) {
        if (eventData->iType == IEED_EVENT_MESSAGE) {
            int isSent = (eventData->dwFlags & IEEDF_SENT);
            //int isRTL = (eventData->dwFlags & IEEDF_RTL) && tmpm->isRTL();
            int isRTL = (eventData->dwFlags & IEEDF_RTL);
            int isHistory = (eventData->time < (DWORD)getStartedTime() && (eventData->dwFlags & IEEDF_READ || eventData->dwFlags & IEEDF_SENT));
            int isGroupBreak = TRUE;
            //if ((getFlags(protoSettings) & Options::LOG_GROUP_MESSAGES) && eventData->dwFlags == LOWORD(getLastEventType())
            //        && eventData->iType == IEED_EVENT_MESSAGE && HIWORD(getLastEventType()) == IEED_EVENT_MESSAGE
            //        && (isSameDate(eventData->time, getLastEventTime()))
            //        //&& ((eventData->time < today) == (getLastEventTime() < today))
            //        && (((eventData->time < (DWORD)startedTime) == (getLastEventTime() < (DWORD)startedTime)) || !(eventData->dwFlags & IEEDF_READ))) {
            //    isGroupBreak = FALSE;
            //}

            QString szName;
            QString szText;

            // TODO часть с Ascii, наверно, надо убрать..нахрена она тут нужна? юникод же
            if (event->eventData->dwFlags & IEEDF_UNICODE_NICK) {
                szName = QString::fromWCharArray(eventData->pszNickW);
            } else {
                szName = QString::fromAscii(eventData->pszNick);
            }
            if (eventData->dwFlags & IEEDF_UNICODE_TEXT) {
                szText = QString::fromWCharArray(eventData->pszTextW);
            } else {
                szText = QString::fromAscii(eventData->pszText);
            }

            addToDoc(szText, szName);
        }
    }

    //if (szBase!=NULL) delete szBase;
    if (szRealProto!=NULL) delete szRealProto;
    if (szProto!=NULL) delete szProto;
    //if (szUINIn!=NULL) delete szUINIn;
    //if (szUINOut!=NULL) delete szUINOut;
    //if (szNoAvatar!=NULL) delete szNoAvatar;
    //if (szAvatarIn!=NULL) delete szAvatarIn;
    //if (szAvatarOut!=NULL) delete szAvatarOut;
    //if (szNameIn!=NULL) delete szNameIn;
    //if (szNameOut!=NULL) delete szNameOut;
    //if (szNickIn!=NULL) delete szNickIn;
    //if (szNickOut!=NULL) delete szNickOut;
    //if (szStatusMsg!=NULL) delete szStatusMsg;
}

void HTMLBuilder::appendEventOld(Elise* view, IEVIEWEVENT* event) {
	IEVIEWEVENT newEvent;
	IEVIEWEVENTDATA* eventData;
	IEVIEWEVENTDATA* prevEventData = NULL;

	HANDLE hDbEvent = event->hDbEventFirst;
	event->hDbEventFirst = NULL;
	newEvent.cbSize = sizeof (IEVIEWEVENT);
	newEvent.iType = IEE_LOG_MEM_EVENTS;
	newEvent.codepage = CP_UTF8;
	// не самый понятный иевьюшный фальклер...хотя...он тут везде
	// TODO заменить на szProto = getProto(event->hContact);
	char* szProto = NULL;
	if (event->cbSize >= IEVIEWEVENT_SIZE_V3 && event->pszProto != NULL) {
		szProto = Utils::dupString(event->pszProto);
	} else {
		szProto = getProto(event->hContact);
	}
	newEvent.pszProto = szProto;
	newEvent.count = 0;
	newEvent.dwFlags = event->dwFlags;
	newEvent.hContact = event->hContact;
	newEvent.hwnd = event->hwnd;
	newEvent.eventData = NULL;


	for (int eventIdx = 0; hDbEvent!=NULL && (eventIdx < event->count || event->count==-1); eventIdx++) {
		DBEVENTINFO dbei = { 0 };
		dbei.cbSize = sizeof(dbei);
		dbei.cbBlob = CallService(MS_DB_EVENT_GETBLOBSIZE, (WPARAM) hDbEvent, 0);
		if (dbei.cbBlob == 0xFFFFFFFF) {
			hDbEvent = (HANDLE) CallService(MS_DB_EVENT_FINDNEXT, (WPARAM) hDbEvent, 0);
			continue;
		}
		dbei.pBlob = (PBYTE) malloc(dbei.cbBlob);
		CallService(MS_DB_EVENT_GET, (WPARAM)  hDbEvent, (LPARAM) & dbei);
		if (!(dbei.flags & DBEF_SENT) && (dbei.eventType == EVENTTYPE_MESSAGE || dbei.eventType == EVENTTYPE_URL)) {
			CallService(MS_DB_EVENT_MARKREAD, (WPARAM) event->hContact, (LPARAM) hDbEvent);
			CallService(MS_CLIST_REMOVEEVENT, (WPARAM) event->hContact, (LPARAM) hDbEvent);
		} else if (dbei.eventType == EVENTTYPE_STATUSCHANGE) {
			CallService(MS_DB_EVENT_MARKREAD, (WPARAM) event->hContact, (LPARAM) hDbEvent);
		}
		if (!isDbEventShown(&dbei)) {
			free(dbei.pBlob);
			hDbEvent = (HANDLE) CallService(MS_DB_EVENT_FINDNEXT, (WPARAM) hDbEvent, 0);
			continue;
		}
		eventData = new IEVIEWEVENTDATA;
		eventData->cbSize = sizeof(IEVIEWEVENTDATA);
		eventData->dwFlags = IEEDF_UNICODE_TEXT | IEEDF_UNICODE_NICK | IEEDF_UNICODE_TEXT2 |
							 (dbei.flags & DBEF_READ ? IEEDF_READ : 0) | (dbei.flags & DBEF_SENT ? IEEDF_SENT : 0) | (dbei.flags & DBEF_RTL ? IEEDF_RTL : 0);
		if (event->dwFlags & IEEF_RTL) {
			eventData->dwFlags  |= IEEDF_RTL;
		}
		eventData->time = dbei.timestamp;
		eventData->pszNickW = NULL;
		eventData->pszTextW = NULL;
		eventData->pszText2W = NULL;
		if (dbei.flags & DBEF_SENT) {
			eventData->pszNickW = getContactName(NULL, szProto);
			eventData->bIsMe = TRUE;
		} else {
			eventData->pszNickW = getContactName(event->hContact, szProto);
			eventData->bIsMe = FALSE;
		}
		if (dbei.eventType == EVENTTYPE_MESSAGE || dbei.eventType == EVENTTYPE_URL || dbei.eventType == EVENTTYPE_STATUSCHANGE || dbei.eventType == EVENTTYPE_JABBER_CHATSTATES) {
			DBEVENTGETTEXT temp = { &dbei, DBVT_WCHAR + ((event->dwFlags & IEEF_NO_UNICODE) ? DBVTF_DENYUNICODE : 0), newEvent.codepage  };
			WCHAR* pwszEventText = (WCHAR*)CallService(MS_DB_EVENT_GETTEXT,0,(LPARAM)&temp);
			eventData->pszTextW = Utils::dupString( pwszEventText );
			mir_free( pwszEventText );
			if (dbei.eventType == EVENTTYPE_MESSAGE) {
				eventData->iType = IEED_EVENT_MESSAGE;
			} else if (dbei.eventType == EVENTTYPE_URL) {
				eventData->iType = IEED_EVENT_URL;
			} else {
				eventData->iType = IEED_EVENT_STATUSCHANGE;
			}
		} else if (dbei.eventType == EVENTTYPE_FILE) {
			//blob is: sequenceid(DWORD),filename(ASCIIZ),description(ASCIIZ)
			char* filename = ((char *)dbei.pBlob) + sizeof(DWORD);
			char* descr = filename + lstrlenA(filename) + 1;
			TCHAR *tStr = DbGetEventStringT(&dbei, filename);
			eventData->ptszText = Utils::dupString(tStr);
			mir_free(tStr);
			if (*descr != '\0') {
				tStr = DbGetEventStringT(&dbei, descr);
				eventData->ptszText2 = Utils::dupString(tStr);
				mir_free(tStr);
			}
			eventData->iType = IEED_EVENT_FILE;
		} else if (dbei.eventType == EVENTTYPE_AUTHREQUEST) {
			//blob is: uin(DWORD), hContact(DWORD), nick(ASCIIZ), first(ASCIIZ), last(ASCIIZ), email(ASCIIZ)
			eventData->ptszText = Utils::dupString(TranslateT(" requested authorisation"));
			TCHAR *tStr = DbGetEventStringT(&dbei, (char *)dbei.pBlob + 8);
			eventData->ptszNick = Utils::dupString(tStr);
			mir_free(tStr);
			eventData->iType = IEED_EVENT_SYSTEM;
		} else if (dbei.eventType == EVENTTYPE_ADDED) {
			//blob is: uin(DWORD), hContact(DWORD), nick(ASCIIZ), first(ASCIIZ), last(ASCIIZ), email(ASCIIZ)
			eventData->ptszText = Utils::dupString(TranslateT(" was added."));
			TCHAR *tStr = DbGetEventStringT(&dbei, (char *)dbei.pBlob + 8);
			eventData->ptszNick = Utils::dupString(tStr);
			mir_free(tStr);
			eventData->iType = IEED_EVENT_SYSTEM;
		}
		free(dbei.pBlob);
		eventData->next = NULL;
		if (prevEventData != NULL) {
			prevEventData->next = eventData;
		} else {
			newEvent.eventData = eventData;
		}
		prevEventData = eventData;
		newEvent.count++;
		event->hDbEventFirst = hDbEvent;
		hDbEvent = (HANDLE) CallService(MS_DB_EVENT_FINDNEXT, (WPARAM) hDbEvent, 0);
	}
	appendEventNew(view, &newEvent);
	for (IEVIEWEVENTDATA* eventData2 = newEvent.eventData; eventData2 != NULL; eventData2 = eventData) {
		eventData = eventData2->next;
		if (eventData2->pszTextW != NULL) {
			delete (wchar_t*)eventData2->pszTextW;
		}
		if (eventData2->pszText2W != NULL) {
			delete (wchar_t*)eventData2->pszText2W;
		}
		if (eventData2->pszNickW != NULL) {
			delete (wchar_t*)eventData2->pszNickW;
		}
		delete eventData2;
	}
	if (szProto != NULL) {
		delete szProto;
	}
}

void HTMLBuilder::appendEventNew(Elise* view, IEVIEWEVENT* event) {
	//setLastIEViewEvent(event);
	appendEvent(view, event);
}

void HTMLBuilder::appendEvent(Elise* view, IEVIEWEVENT* event) {
	//ProtocolSettings *protoSettings = getSRMMProtocolSettings(event->hContact);
	//if (protoSettings == NULL) {
	//	return;
	//}
	//if (protoSettings->getSRMMMode() == Options::MODE_TEMPLATE) {
	//	appendEventTemplate(view, event, protoSettings);
	//} else {
	//	appendEventNonTemplate(view, event);
	//}
	appendEventTemplate(view, event);
}

wchar_t* HTMLBuilder::getContactName(HANDLE hContact, const char* szProto) {
	CONTACTINFO ci;
	wchar_t *szName = NULL;
	ZeroMemory(&ci, sizeof(ci));
	ci.cbSize = sizeof(ci);
	ci.hContact = hContact;
	ci.szProto = (char *)szProto;
	ci.dwFlag = CNF_DISPLAY | CNF_UNICODE;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		if (ci.type == CNFT_ASCIIZ) {
			if (ci.pszVal) {
				if(!wcscmp((wchar_t *)ci.pszVal, TranslateW(L"'(Unknown Contact)'"))) {
					ci.dwFlag &= ~CNF_UNICODE;
					if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
						szName = Utils::convertToWCS((char *)ci.pszVal);
					}
				} else {
					szName = Utils::dupString((wchar_t *)ci.pszVal);
				}
				miranda_sys_free(ci.pszVal);
			}
		}
	}
	if (szName != NULL) return szName;
	ci.dwFlag = CNF_UNIQUEID;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		if (ci.type == CNFT_ASCIIZ) {
			if (ci.pszVal) {
				szName = Utils::convertToWCS((char *)ci.pszVal);
				miranda_sys_free(ci.pszVal);
			}
		}
	}
	if (szName != NULL) return szName;
	char *szNameStr = (char *)CallService(MS_CLIST_GETCONTACTDISPLAYNAME, (WPARAM)hContact, 0);
	if (szNameStr != NULL) {
		return Utils::convertToWCS(szNameStr);
	}
	return Utils::dupString(TranslateT("(Unknown Contact)"));
}

char* HTMLBuilder::getProto(HANDLE hContact) {
	return Utils::dupString((char*)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0));
}

char* HTMLBuilder::getProto(const char *proto, HANDLE hContact) {
	if (proto != NULL) {
		return Utils::dupString(proto);
	}
	return Utils::dupString((char *)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0));
}

HANDLE HTMLBuilder::getRealContact(HANDLE hContact) {
	char *szProto = (char *)CallService(MS_PROTO_GETCONTACTBASEPROTO, (WPARAM) hContact, 0);
	if (szProto != NULL && !strcmp(szProto,"MetaContacts")) {
		hContact = (HANDLE) CallService(MS_MC_GETMOSTONLINECONTACT, (WPARAM) hContact, 0);
	}
	return hContact;
}

void HTMLBuilder::setLastIEViewEvent(IEVIEWEVENT* event) {
	lastIEViewEvent.cbSize = sizeof (IEVIEWEVENT);
	lastIEViewEvent.iType = event->iType;
	lastIEViewEvent.codepage = CP_UTF8;
	if (event->cbSize >= IEVIEWEVENT_SIZE_V2) {
		lastIEViewEvent.codepage = event->codepage;
	}
	lastIEViewEvent.count = 0;
	lastIEViewEvent.dwFlags = event->dwFlags;
	lastIEViewEvent.hContact = event->hContact;
	lastIEViewEvent.hwnd = event->hwnd;
	lastIEViewEvent.eventData = NULL;
	// какого то хрена крашит
	//if (lastIEViewEvent.pszProto != NULL) {
	//	delete (char*)lastIEViewEvent.pszProto ;
	//}
	if (event->cbSize >= IEVIEWEVENT_SIZE_V3 && event->pszProto != NULL) {
		lastIEViewEvent.pszProto = Utils::dupString(event->pszProto);
	} else {
		lastIEViewEvent.pszProto = getProto(event->hContact);
	}
}

bool HTMLBuilder::isDbEventShown(DWORD dwFlags, DBEVENTINFO* dbei) {
    switch (dbei->eventType) {
        case EVENTTYPE_MESSAGE:
            return 1;
            break;
        case EVENTTYPE_STATUSCHANGE:
            return 1;
            break;
        case EVENTTYPE_URL:
            if(dwFlags & MWF_SHOW_URLEVENTS) return 1;
            break;
        case EVENTTYPE_FILE:
            if(dwFlags & MWF_SHOW_FILEEVENTS) return 1;
            break;
    }
    return 0;
}

bool HTMLBuilder::isDbEventShown(DBEVENTINFO* dbei) {
    DWORD dwFlags2 = DBGetContactSettingByte(NULL, SRMSGMOD_T, SRMSGSET_SHOWURLS, 0) ? MWF_SHOW_URLEVENTS : 0;
    dwFlags2 |= DBGetContactSettingByte(NULL, SRMSGMOD_T, SRMSGSET_SHOWFILES, 0) ? MWF_SHOW_FILEEVENTS : 0;
    return isDbEventShown(dwFlags2, dbei);
}

time_t HTMLBuilder::getStartedTime() {
	return startedTime;
}

void HTMLBuilder::initDoc() {
	document = TemplateMap::templateMap["<!--HTMLStart-->"];

	//page.mainFrame()->setHtml(TemplateMap::templateMap["<!--HTMLStart-->"], skinDir);
}

QString HTMLBuilder::getDoc() {
	return document;
}
/*
QWebElement HTMLBuilder::getElem() {
	return page.mainFrame()->documentElement().document();
}*/

void HTMLBuilder::addToDoc(QString text, QString name) {
	QString templ = TemplateMap::templateMap["<!--MessageIn-->"];
	templ.replace("%name%", name);
	document += templ.replace("%text%", text);
	//parentView->addToDoc(templ.replace("%text%", text));
	//webView->page()->mainFrame()->documentElement().appendInside("хехей!");

	//QWebPage tmpPage;
	//tmpPage.mainFrame()->setHtml(templ, skinDir);
	//webView->page()->mainFrame()->documentElement().appendInside(page.mainFrame()->documentElement().document());

	//page.mainFrame()->documentElement().appendInside(tmpPage.mainFrame()->documentElement().document());
}

void HTMLBuilder::saveDoc(QString doc) {
	document = doc;
	//page.mainFrame()->documentElement().replace(elem);
}
