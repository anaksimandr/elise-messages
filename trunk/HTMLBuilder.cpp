
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


QString oneShot = "oneShot";
extern QUrl skinDir;


HTMLBuilder::HTMLBuilder(Elise* view) {
    startedTime = time(NULL);
    parentView = view;
}

HTMLBuilder::~HTMLBuilder() {
    // some code
}

void HTMLBuilder::appendEventTemplate(Elise* view, IEVIEWEVENT* event) {
    HANDLE hRealContact;
    char* szRealProto = NULL;
    char* szProto = NULL;
    QString qsUIN;
    QString qsUINIn;
    QString qsUINOut;
    QString qsName;
    QString qsNameIn;
    QString qsNameOut;
    QString qsAvatar;
    QString qsAvatarIn;
    QString qsAvatarOut;
    QString qsText;
    QString qsTime;
    QString qsDate;

    hRealContact = getRealContact(event->hContact);
    szRealProto = getProto(hRealContact);
    szProto = getProto(event->pszProto, event->hContact);

    // вот тут в ieview создают карту шаблона..при каждом полученном сообщении? втф?

    if (event->hContact != NULL) {
        getUINs(event->hContact, qsUINIn, qsUINOut);
    }

	// names
	if (event->hContact != NULL) {
		qsNameOut = QString::fromWCharArray(getContactName(NULL, szProto));
		qsNameIn = QString::fromWCharArray(getContactName(event->hContact, szProto));
	} else {
		qsNameOut = "&nbsp;";
		qsNameIn = "&nbsp;";
	}

	// avatars
	if(event->hContact != NULL) {
		getAvatar(event->hContact, szRealProto, qsAvatarIn);
	}
	if (qsAvatarIn == NULL) {
		qsAvatarIn = "<img src=\"img/noavatar.jpg\">";
	}
	getAvatar(NULL, szRealProto, qsAvatarOut);
	if (qsAvatarOut == NULL) {
		qsAvatarOut = "<img src=\"img/noavatar.jpg\">";
	}
	/*if(event->hContact != NULL) {
		if (!DBGetContactSetting(event->hContact, "CList", "StatusMsg",&dbv)) {
			if (strlen(dbv.pszVal) > 0) {
				szStatusMsg = Utils::UTF8Encode(dbv.pszVal);
			}
			DBFreeVariant(&dbv);
		}
	}*/

    IEVIEWEVENTDATA* eventData = event->eventData;
    for (int eventIdx = 0; eventData!=NULL && (eventIdx < event->count || event->count==-1); eventData = eventData->next, eventIdx++) {
        if (eventData->iType == IEED_EVENT_MESSAGE) {
            int isSent = (eventData->dwFlags & IEEDF_SENT);
            //int isRTL = (eventData->dwFlags & IEEDF_RTL) && tmpm->isRTL();
            int isRTL = (eventData->dwFlags & IEEDF_RTL);
            int isHistory = (eventData->time < (DWORD)getStartedTime() && (eventData->dwFlags & IEEDF_READ || eventData->dwFlags & IEEDF_SENT));
            //int isGroupBreak = TRUE;
            //if ((getFlags(protoSettings) & Options::LOG_GROUP_MESSAGES) && eventData->dwFlags == LOWORD(getLastEventType())
            //        && eventData->iType == IEED_EVENT_MESSAGE && HIWORD(getLastEventType()) == IEED_EVENT_MESSAGE
            //        && (isSameDate(eventData->time, getLastEventTime()))
            //        //&& ((eventData->time < today) == (getLastEventTime() < today))
            //        && (((eventData->time < (DWORD)startedTime) == (getLastEventTime() < (DWORD)startedTime)) || !(eventData->dwFlags & IEEDF_READ))) {
            //    isGroupBreak = FALSE;
            //}

            qsText = "";
            qsName = "";

            // TODO часть с Ascii, наверно, надо убрать..нахрена она тут нужна? юникод же
            if (event->eventData->dwFlags & IEEDF_UNICODE_NICK) {
                qsName = QString::fromWCharArray(eventData->pszNickW);
            } else {
                qsName = QString::fromAscii(eventData->pszNick);
            }
            if (eventData->dwFlags & IEEDF_UNICODE_TEXT) {
                qsText = QString::fromWCharArray(eventData->pszTextW);
            } else {
                qsText = QString::fromAscii(eventData->pszText);
            }

            // save old last event
            lastEvent.replace(oneShot, "");
            document += lastEvent;

            if (isSent) {
                qsAvatar = qsAvatarOut;
                qsUIN = qsUINOut;
                lastEvent = TemplateMap::templateMap["<!--MessageOut-->"];
            } else {
                qsAvatar = qsAvatarIn;
                qsUIN = qsUINIn;
                lastEvent = TemplateMap::templateMap["<!--MessageIn-->"];
            }

            //qsTime = QString::fromAscii(timestampToString(getFlags(protoSettings), eventData->time, 1));
            qsTime = QString::fromAscii(timestampToString(NULL, eventData->time, 1));
            qsDate = QString::fromAscii(timestampToString(NULL, eventData->time, 0));

            // add to doc
            //lastEvent.replace(oneShot, "");
            //document += lastEvent;
            //lastEvent = TemplateMap::templateMap["<!--MessageIn-->"];

            // workin with url's
            qsText.replace(QRegExp("((?:https?|ftp)://\\S+)"), "<a class=\"link\" target=\"_self\" href=\"\\1\">\\1</a>");

            // final step of making message
            lastEvent.replace("%base%", skinDir.path());  // base URL
            lastEvent.replace("%name%", qsName);          // contact's name or user's name (depends on context)
            lastEvent.replace("%time%", qsTime);          // event's time
            lastEvent.replace("%date%", qsDate);          // event's date
            lastEvent.replace("%text%", qsText);          // event's text
            lastEvent.replace("%cid%", qsUIN);            // contact's ID or user's ID (depends on context)
            lastEvent.replace("%avatar%", qsAvatar);      // link to contact's picture file or user's picure file (depends on context)
            lastEvent.replace("%avatarIn%", qsAvatarIn);  // link to contact's picture
            lastEvent.replace("%avatarOut%",qsAvatarOut); // link to user's picture
            lastEvent.replace("%nameIn%", qsNameIn);      // contact's name
            lastEvent.replace("%nameOut%", qsNameOut);    // users's name
            lastEvent.replace("%proto%", QString::fromAscii(szProto)); // protocol name

        }
    }

    if (szRealProto!=NULL) delete szRealProto;
    if (szProto!=NULL) delete szProto;

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
	// честно..пока не знаю, зачем оно тут.
	setLastIEViewEvent(event);
	// --
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

// TODO привести в божеский вид
void HTMLBuilder::getAvatar(HANDLE hContact, const char* szProto, QString& result) {
	//DBVARIANT dbv;
	//char tmpPath[MAX_PATH];
	//char* result = NULL;

	if (ServiceExists(MS_AV_GETMYAVATAR)) {
		struct avatarCacheEntry* ace  = NULL;
		if (hContact == NULL) {
			ace = (struct avatarCacheEntry*)CallService(MS_AV_GETMYAVATAR, (WPARAM)0, (LPARAM)szProto);
		} else {
			ace = (struct avatarCacheEntry*)CallService(MS_AV_GETAVATARBITMAP, (WPARAM)hContact, (LPARAM)0);
		}
		if (ace!=NULL) {
			result = QString::fromAscii(ace->szFilename);
		}
	}

	// flash is not supported now
	/*if (!DBGetContactSetting(hContact, "ContactPhoto", "File",&dbv)) {
		if (strlen(dbv.pszVal) > 0) {
			char* ext = strrchr(dbv.pszVal, '.');
			if (ext && strcmpi(ext, ".xml") == 0) {
				result = (char *)getFlashAvatar(dbv.pszVal, (hContact == NULL) ? 1 : 0);
			} else {
				if (result == NULL) {
					// relative -> absolute
					strcpy (tmpPath, dbv.pszVal);
					if (ServiceExists(MS_UTILS_PATHTOABSOLUTE)&& strncmp(tmpPath, "http://", 7)) {
						CallService(MS_UTILS_PATHTOABSOLUTE, (WPARAM)dbv.pszVal, (LPARAM)tmpPath);
					}
					result = tmpPath;
				}
			}
		}
		DBFreeVariant(&dbv);
	}*/

	if (result != NULL) {
		result.replace(result, "<img src=\"" + result + "\">");
	}
	//return result;
}/*
const char* HTMLBuilder::getFlashAvatar(const char* file, int index) {
	if (time(NULL) - flashAvatarsTime[index] > 600 || flashAvatars[index] == NULL) {
		if (flashAvatars[index] != NULL) {
			delete flashAvatars[index];
			flashAvatars[index] = NULL;
		}
		flashAvatarsTime[index] = time(NULL);
		int src = _open(file, _O_BINARY | _O_RDONLY);
		if (src != -1) {
			char pBuf[2048];
			char *urlBuf;
			_read(src, pBuf, 2048);
			_close(src);
			urlBuf = strstr(pBuf, "<URL>");
			if(urlBuf) {
				flashAvatars[index]  = Utils::dupString(strtok(urlBuf + 5, "<\t\n\r"));
			}
		}
 }
 return flashAvatars[index];
}*/

// TODO перелопатить
char* HTMLBuilder::timestampToString(DWORD dwFlags, time_t check, int mode) {
	static char szResult[512];
	char str[80];
	DBTIMETOSTRING dbtts;
	dbtts.cbDest = 70;
	dbtts.szDest = str;
	szResult[0] = '\0';
	if (mode) { //time
		//dbtts.szFormat = (dwFlags & Options::LOG_SHOW_SECONDS) ? (char *)"s" : (char *)"t";
		dbtts.szFormat = (char *)"s";
		CallService(MS_DB_TIME_TIMESTAMPTOSTRING, check, (LPARAM) & dbtts);
		strncat(szResult, str, 500);
	} else {    //date
		struct tm tm_now, tm_today;
		time_t now = time(NULL);
		time_t today;
		tm_now = *localtime(&now);
		tm_today = tm_now;
		tm_today.tm_hour = tm_today.tm_min = tm_today.tm_sec = 0;
		today = mktime(&tm_today);
		//if (dwFlags & Options::LOG_RELATIVE_DATE && check >= today) {
		if (check >= today) {
			strcpy(szResult, Translate("Today"));
		//} else if(dwFlags & Options::LOG_RELATIVE_DATE && check > (today - 86400)) {
		} else if(check > (today - 86400)) {
			strcpy(szResult, Translate("Yesterday"));
		} else {
			//dbtts.szFormat = (dwFlags & Options::LOG_LONG_DATE) ? (char *)"D" : (char *)"d";
			dbtts.szFormat = (char *)"D";
			CallService(MS_DB_TIME_TIMESTAMPTOSTRING, check, (LPARAM) & dbtts);
			strncat(szResult, str, 500);
		}
	}
	Utils::UTF8Encode(szResult, szResult, 500);
	return szResult;
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
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// вот тут может крашить
	if (lastIEViewEvent.pszProto != NULL) {
		delete (char*)lastIEViewEvent.pszProto ;
	}
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
}

QString HTMLBuilder::getDoc() {
	return document + lastEvent;
}

void HTMLBuilder::getUINs(HANDLE hContact, QString& uinIn, QString& uinOut) {
	// взято в ieview, как и многое остальное.
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
				miranda_sys_free(ci.pszVal);
				break;
			case CNFT_DWORD:
				mir_snprintf(buf, sizeof(buf), "%u", ci.dVal);
				break;
		}
	}
	uinIn = QString::fromUtf8(buf);

	ci.hContact = NULL;
	buf[0] = 0;
	if (!CallService(MS_CONTACT_GETCONTACTINFO, 0, (LPARAM) & ci)) {
		switch (ci.type) {
			case CNFT_ASCIIZ:
				mir_snprintf(buf, sizeof(buf), "%s", ci.pszVal);
				miranda_sys_free(ci.pszVal);
				break;
			case CNFT_DWORD:
				mir_snprintf(buf, sizeof(buf), "%u", ci.dVal);
				break;
		}
	}
	uinOut = QString::fromUtf8(buf);

	delete szProto;
}
