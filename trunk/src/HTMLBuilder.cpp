
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


const QString oneShot = "oneShot";
const QString noAvatar = "img/noavatar.jpg";
const QString noTemplate = "No templates were found";

HTMLBuilder::HTMLBuilder(Elise* view, HWND hwndParent)
{
	parent = hwndParent;
	//-- First, get contact's handle
	hContact = Options::wndToContact(parent);
	
	options = new SingleOptions(hContact);

	initHead();
	
	setLastEventType(-1);
	setLastEventTime(time(NULL));
	lastEventTime = time(NULL);
	startedTime = time(NULL);
	parentView = view;

	lastIEViewEvent.cbSize = sizeof (IEVIEWEVENT);
	lastIEViewEvent.iType = IEE_LOG_MEM_EVENTS;
	lastIEViewEvent.codepage = CP_UTF8;
	lastIEViewEvent.pszProto = NULL;
	lastIEViewEvent.count = 0;
	lastIEViewEvent.dwFlags = 0;
	lastIEViewEvent.hContact = NULL;
	lastIEViewEvent.hwnd = NULL;
	lastIEViewEvent.eventData = NULL;
}

HTMLBuilder::~HTMLBuilder() {
	if (lastIEViewEvent.pszProto != NULL) {
		delete (char*)lastIEViewEvent.pszProto;
	}
	//-- No no no! see SingleOptions::SingleOptions(HANDLE hContact);
	//options->currentTemplate->~TemplateMap();
	options->~SingleOptions();
}

HANDLE HTMLBuilder::getContact()
{
	return hContact;
}

HWND HTMLBuilder::getParent()
{
	return parent;
}

void HTMLBuilder::appendEventTemplate(Elise* view, IEVIEWEVENT* event)
{
	if (!options->currentTemplate->isTemplateInited())
		return;

	HANDLE hCurContact;
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
	//QString qsFile;

	QString templateBuf;

	hCurContact = Options::getRealContact(event->hContact);
	szRealProto = Options::getProto(hCurContact);
	szProto = Options::getProto(event->pszProto, event->hContact);

	//-- 

	if (event->hContact != NULL) {
		getUINs(event->hContact, qsUINIn, qsUINOut);
		//-- Names
		qsNameOut = QString::fromWCharArray(getContactName(NULL, szProto));
		qsNameIn = QString::fromWCharArray(getContactName(event->hContact, szProto));
		//-- Avatars
		if (options->isShowAvatar()) {
			getAvatar(event->hContact, szRealProto, qsAvatarIn);
			if (qsAvatarIn.isEmpty())
				qsAvatarIn = noAvatar;
			getAvatar(NULL, szRealProto, qsAvatarOut);
			if (qsAvatarOut.isEmpty())
				qsAvatarOut = noAvatar;
		}
	}
	else {
		qsNameOut = "&nbsp;";
		qsNameIn = "&nbsp;";
		qsUINIn = "&nbsp;";
		qsUINOut = "&nbsp;";
		qsAvatarIn = "&nbsp;";
		qsAvatarOut = "&nbsp;";
	}

	int isHistory = 0;

	IEVIEWEVENTDATA* eventData = event->eventData;
	for (int eventIdx = 0; eventData!=NULL && (eventIdx < event->count || event->count==-1); eventData = eventData->next, eventIdx++) {
		if (eventData->iType == IEED_EVENT_MESSAGE || eventData->iType == IEED_EVENT_STATUSCHANGE || eventData->iType == IEED_EVENT_FILE || eventData->iType == IEED_EVENT_URL || eventData->iType == IEED_EVENT_SYSTEM) {
			int isSent = (eventData->dwFlags & IEEDF_SENT);
			//int isRTL = (eventData->dwFlags & IEEDF_RTL) && tmpm->isRTL();
			int isRTL = (eventData->dwFlags & IEEDF_RTL);
			isHistory = (eventData->time < (DWORD)getStartedTime() && (eventData->dwFlags & IEEDF_READ || eventData->dwFlags & IEEDF_SENT));
			int isGroupBreak = TRUE;
			if ((options->isMessageGrouping()) && eventData->dwFlags == LOWORD(getLastEventType())
			        && eventData->iType == IEED_EVENT_MESSAGE && HIWORD(getLastEventType()) == IEED_EVENT_MESSAGE
			        && (isSameDate(eventData->time, getLastEventTime()))
			        //&& ((eventData->time < today) == (getLastEventTime() < today))
			        && (((eventData->time < (DWORD)startedTime) == (getLastEventTime() < (DWORD)startedTime)) || !(eventData->dwFlags & IEEDF_READ))) {
				   isGroupBreak = FALSE;
			}

			qsText = "";
			qsName = "";
			//qsFile = "";
			templateBuf = "";

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
			//if (eventData->dwFlags & IEEDF_UNICODE_TEXT2) {
			//	qsFile = QString::fromWCharArray(eventData->pszText2W);
   			//} else {
			//	qsFile = QString::fromAscii(eventData->pszText2);
			//}

			if (isSent) {
				qsAvatar = qsAvatarOut;
				qsUIN = qsUINOut;
			} else {
				qsAvatar = qsAvatarIn;
				qsUIN = qsUINIn;
			}
			
			switch (eventData->iType) {
			case (IEED_EVENT_MESSAGE):
				{
					if (!isRTL) {
						if (options->isMessageGrouping()) {
							if (isGroupBreak) {
								lastEvent = isHistory ? isSent ? "<!--hMessageOutGroupStart-->" : "<!--hMessageInGroupStart-->" : isSent ? "<!--MessageOutGroupStart-->" : "<!--MessageInGroupStart-->";
							} else {
								lastEvent = isHistory ? isSent ? "<!--hMessageOutGroupInner-->" : "<!--hMessageInGroupInner-->" : isSent ? "<!--MessageOutGroupInner-->" : "<!--MessageInGroupInner-->";
							}
							templateBuf = isHistory ? isSent ? "<!--hMessageOutGroupEnd-->" : "<!--hMessageInGroupEnd-->" : isSent ? "<!--MessageOutGroupEnd-->" : "<!--MessageInGroupEnd-->";
						} else {
							lastEvent = isHistory ? isSent ? "<!--hMessageOut-->" : "<!--hMessageIn-->" : isSent ? "<!--MessageOut-->" : "<!--MessageIn-->";
						}
					} else {/*
							if (options->isMessageGrouping()) {
							if (isGroupBreak) {
							lastEvent = isHistory ? isSent ? "<!--hMessageOutGroupStartRTL-->" : "<!--hMessageInGroupStartRTL-->" : isSent ? "<!--MessageOutGroupStartRTL-->" : "<!--MessageInGroupStartRTL-->";
							} else {
							lastEvent = isHistory ? isSent ? "<!--hMessageOutGroupInnerRTL-->" : "<!--hMessageInGroupInnerRTL-->" : isSent ? "<!--MessageOutGroupInnerRTL-->" : "<!--MessageInGroupInnerRTL-->";
							}
							groupTemplate = isHistory ? isSent ? "<!--hMessageOutGroupEndRTL-->" : "<!--hMessageInGroupEndRTL-->" : isSent ? "<!--MessageOutGroupEndRTL-->" : "<!--MessageInGroupEndRTL-->";
							} else {
							lastEvent = isHistory ? isSent ? "<!--hMessageOutRTL-->" : "<!--hMessageInRTL-->" : isSent ? "<!--MessageOutRTL-->" : "<!--MessageInRTL-->";
							}*/
					}
				}
				break;
			case (IEED_EVENT_FILE):
				{
					if (options->currentTemplate->isFilesInOut())
						lastEvent = isHistory ? isSent ? "<!--hFileOut-->" : "<!--hFileIn-->" : isSent ? "<!--FileOut-->" : "<!--FileIn-->";
					else
						lastEvent = isHistory ? "<!--hFile-->" : "<!--File-->";				
				}
				break;
			case (IEED_EVENT_URL):
				{
					if (options->currentTemplate->isURLInOut())
						lastEvent = isHistory ? isSent ? "<!--hURLOut-->" : "<!--hURLIn-->" : isSent ? "<!--URLOut-->" : "<!--URLIn-->";
					else
						lastEvent = isHistory ? "<!--hURL-->" : "<!--URL-->";

				}
				break;
			case (IEED_EVENT_STATUSCHANGE):
			case (IEED_EVENT_SYSTEM):
				{
					lastEvent = isHistory ? "<!--hStatus-->" : "<!--Status-->";
				}
				break;
			}

			QMap<QString, QString>* templMap = &options->currentTemplate->templateMap;

			if (options->isMessageGrouping()) {
				if (templMap->contains(lastEvent) && templMap->contains(templateBuf))
					lastEvent = templMap->value(templateBuf) + templMap->value(lastEvent);
				else
					lastEvent = "Error";
			}
			else {
				if (templMap->contains(lastEvent))
					lastEvent = templMap->value(lastEvent);
				else
					lastEvent = "Error\nTemplate not found - " + lastEvent;
			}

			//-- Worcking with date and time
			getTime(qsTime, qsDate, eventData->time);
			
			//-- New lines
			qsText.replace("\n", "<br>\n");

			//-- Workin with url's
			if (options->isURLParse())
				replaceURL(qsText);

			//-- Working with BBCodes
			if (options->isBBCodes())
				replaceBBCodes(qsText);

			//-- Workin with smileys         
			replaceSmileys(qsText,isSent, event->hContact, szProto);

			//-- Final step of making message
			lastEvent.replace("%base%", options->currentTemplate->getRealTemplatePath());  // base URL
			lastEvent.replace("%time%", qsTime);          // event's time
			lastEvent.replace("%date%", qsDate);          // event's date
			lastEvent.replace("%cid%", qsUIN);            // contact's ID or user's ID (depends on context)
			lastEvent.replace("%cidIn%", qsUINIn);          // user's ID
			lastEvent.replace("%cidOut%", qsUINOut);         // contact's ID
			lastEvent.replace("%avatar%", qsAvatar);      // link to contact's picture file or user's picure file (depends on context)
			lastEvent.replace("%avatarIn%", qsAvatarIn);  // link to contact's picture
			lastEvent.replace("%avatarOut%",qsAvatarOut); // link to user's picture
			lastEvent.replace("%name%", qsName);          // contact's name or user's name (depends on context)
			lastEvent.replace("%nameIn%", qsNameIn);      // contact's name
			lastEvent.replace("%nameOut%", qsNameOut);    // users's name
			lastEvent.replace("%proto%", QString::fromAscii(szProto)); // protocol name
			lastEvent.replace("%text%", qsText);          // event's text
			//parentView->addToDoc(lastEvent);
			if (isHistory)
				history += lastEvent;

			setLastEventType(MAKELONG(eventData->dwFlags, eventData->iType));
			setLastEventTime(eventData->time);
		}
	}

	if (isHistory)
		parentView->reloadDoc();
	else		
		parentView->addToDoc();

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
	newEvent.codepage = CP_ACP;
	// не самый понятный иевьюшный фальклер...хотя...он тут везде
	// TODO заменить на szProto = getProto(event->hContact);
	char* szProto = NULL;
	if (event->cbSize >= IEVIEWEVENT_SIZE_V3 && event->pszProto != NULL) {
		szProto = Utils::dupString(event->pszProto);
	} else {
		szProto = Options::getProto(event->hContact);
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
	appendEventTemplate(view, event);
}

/*void HTMLBuilder::appendEvent(Elise* view, IEVIEWEVENT* event) {
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
}*/

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

void HTMLBuilder::getUINs(HANDLE hContact, QString& uinIn, QString& uinOut)
{
	char* szUINIn = NULL;
	char* szUINOut = NULL;

	Options::getUINs(hContact, szUINIn, szUINOut);

	uinIn = QString::fromAscii(szUINIn);
	uinOut = QString::fromAscii(szUINOut);

	if (szUINIn!=NULL) delete szUINIn;
	if (szUINOut!=NULL) delete szUINOut;
}

void HTMLBuilder::getTime(QString& qsTime, QString& qsDate, time_t time)
{
	QDateTime date = QDateTime::fromTime_t(time);
	//-- Time
	if (options->isShowTime()) {
		if (options->isShowSeconds())
			qsTime = date.toString("hh:mm:ss");
		else
			qsTime = date.toString("hh:mm");
	}
	else
		qsTime = "";
	//-- Date
	if (options->isShowDate()) {
		QDate now = QDate::currentDate();
		int yy;
		int mm;
		int dd;
		now.getDate(&yy, &mm, &dd);
		QDate yesterday = QDate(yy, mm, dd - 1);
		if (options->isRelativeTime() && date.date() == now) {
			qsDate = QString::fromLocal8Bit(Translate("Today"));
		}
		else if (options->isRelativeTime() && date.date() == yesterday) {
			qsDate = QString::fromLocal8Bit(Translate("Yesterday"));
		}
		else
			if (options->isWordDate()) {
				qsDate = date.toString("dddd dd MMMM yyyy");
			}
			else {
				qsDate = date.toString("dd.MM.yyyy");
			}
	}
	else
		qsDate = "";
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

		/*if (result != NULL) {
			result.replace(result, "<img src=\"" + result + "\">");
		}*/
		//return result;
}
/*const char* HTMLBuilder::getFlashAvatar(const char* file, int index) {
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

void HTMLBuilder::replaceBBCodes(QString& text) {    
	text.replace(TemplateMap::templateBBCodes["b"], "<b>\\1</b>");
	text.replace(TemplateMap::templateBBCodes["s"], "<s>\\1</s>");
	text.replace(TemplateMap::templateBBCodes["i"], "<i>\\1</i>");
	text.replace(TemplateMap::templateBBCodes["u"], "<u>\\1</u>");
	text.replace(TemplateMap::templateBBCodes["img"], "<div><img class=\"img\" style=\"max-width:100%;\" src=\"\\1\" ></div>");
	text.replace(TemplateMap::templateBBCodes["code"], "<tt>\\1</tt>");
	text.replace(TemplateMap::templateBBCodes["quote"], "<blockquote>\\1</blockquote>");
	text.replace(TemplateMap::templateBBCodes["size"], "<span style=\"font-size:\\1pt;\">\\2</span>");
	text.replace(TemplateMap::templateBBCodes["color"], "<span style=\"color:\\1;\">\\2</span>");

}

void HTMLBuilder::replaceURL(QString& text) {
	//text.replace(QRegExp("((?:https?|ftp)://\\S+)"), "<a class=\"link\" target=\"_self\" href=\"\\1\">\\1</a>");
	//((mailto\:|(news|(ht|f)tp(s?))\://){1}\S+) - source, not working in qt and not match url, that begins like "www..."
	//text.replace(QRegExp("((((mailto\:|(news|(ht|f)tp(s?))\://))|www)\\S+)(?!"), "<a class=\"link\" target=\"_self\" href=\"\\1\">\\1</a>");
	//-- Create URL regExp
	QRegExp rx = QRegExp("((((mailto\:|(news|(ht|f)tp(s?))\://))|www)\\S+)");
	QString qstrBuf;
	QString qstrCurURL;
	int pos = 0;
	int len = 0;
	//-- While finds matche
	while ((pos = rx.indexIn(text, pos)) != -1) {
		//-- Get length of matche
		len = rx.matchedLength();
		//-- Save matche
		qstrCurURL = rx.cap(0);
		//-- Get wide string (+10 characters at start and end, thought it's enough)
		qstrBuf = text.mid(pos - 10, 10) + qstrCurURL + text.mid(pos + len, 10);
		//-- If the wide string does not matche [img] bbcode
		if (!TemplateMap::templateBBCodes["img"].exactMatch(qstrBuf))
			//-- Replace matche with HTML-formated URL
			text.replace(pos, len,"<a class=\"link\" target=\"_self\" href=\"\\1\">" + qstrCurURL + "</a>");
		//-- Position incrementation
		pos += len;
	}
}

void HTMLBuilder::replaceSmileys(QString& text, bool isSent, HANDLE hContact, char* szProto)
{	
	SMADD_BATCHPARSE2 sp;
	SMADD_BATCHPARSERES* spRes;

	if (!ServiceExists(MS_SMILEYADD_BATCHPARSE)) {
		return;
	}
	sp.cbSize = sizeof(sp);
	sp.Protocolname = szProto;
	sp.flag = SAFL_PATH | SAFL_UNICODE | (isSent ? SAFL_OUTGOING : 0);

	//-- Translate QString to wchar_t*
	wchar_t* wszText = new wchar_t[text.size()+1];
	text.toWCharArray(wszText);
	wszText[text.size()] = 0;
	sp.str = (wchar_t*)wszText;
	
	sp.hContact = hContact;
	spRes = (SMADD_BATCHPARSERES*) CallService(MS_SMILEYADD_BATCHPARSE, 0, (LPARAM)&sp);
	int last_pos = 0;
	if (spRes != NULL) {
		for (int i = 0; i < (int)sp.numSmileys; i++) {
			if (spRes[i].filepath != NULL && strlen((char*)spRes[i].filepath) > 0) {
				text.replace(spRes[i].startChar, spRes[i].size, "<img class=\"img\" style=\"max-width:100%;\" src=\"" + QString::fromWCharArray(spRes[i].filepath) + "\" >");
				last_pos = spRes[i].startChar + spRes[i].size;
			}
		}
		CallService(MS_SMILEYADD_BATCHFREE, 0, (LPARAM)spRes);
		
	}
	delete[] wszText;
	return;
}

bool HTMLBuilder::isSameDate(time_t time1, time_t time2) {
    struct tm tm_t1, tm_t2;
    tm_t1 = *localtime((time_t *)(&time1));
    tm_t2 = *localtime((time_t *)(&time2));
    if (tm_t1.tm_year == tm_t2.tm_year && tm_t1.tm_mon == tm_t2.tm_mon
		&& tm_t1.tm_mday == tm_t2.tm_mday) {
		return true;
	}
	return false;
}

void HTMLBuilder::setLastIEViewEvent(IEVIEWEVENT* event) {
	lastIEViewEvent.cbSize = sizeof (IEVIEWEVENT);
	lastIEViewEvent.iType = event->iType;
	lastIEViewEvent.codepage = CP_ACP;
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
		lastIEViewEvent.pszProto = Options::getProto(event->hContact);
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

QUrl HTMLBuilder::getTemplateUrl()
{
	return options->currentTemplate->getTemplateUrl();
}

void HTMLBuilder::initHead()
{
	if (options->currentTemplate->isTemplateInited()) {
		//-- Defines
		HANDLE hRealContact;
		char* szRealProto = NULL;
		char* szProto = NULL;
		QString qsTime;
		QString qsDate;
		QString qsUINIn;
		QString qsUINOut;
		QString qsAvatarIn;
		QString qsAvatarOut;
		QString qsNameIn;
		QString qsNameOut;
		//--Proto
		hRealContact = Options::getRealContact(hContact);
		szRealProto = Options::getProto(hRealContact);
		szProto = Options::getProto(hContact);
		//-- Template
		QMap<QString, QString>* templMap = &options->currentTemplate->templateMap;
		header = templMap->value("<!--HTMLStart-->");
		footer = templMap->value("<!--HTMLEnd-->");
		//-- Time and date
		getTime(qsTime, qsDate, time(0));
		if (hContact != NULL) {
			//-- UINs
			getUINs(hContact, qsUINIn, qsUINOut);
			//-- Names
			qsNameOut = QString::fromWCharArray(getContactName(NULL, szProto));
			qsNameIn = QString::fromWCharArray(getContactName(hContact, szProto));
			//-- Avatars
			if (options->isShowAvatar()) {				
				getAvatar(hContact, szRealProto, qsAvatarIn);
				if (qsAvatarIn.isEmpty())
					qsAvatarIn = noAvatar;
				getAvatar(NULL, szRealProto, qsAvatarOut);
				if (qsAvatarOut.isEmpty())
					qsAvatarOut = noAvatar;
			}
		}
		else {
			qsNameOut = "&nbsp;";
			qsNameIn = "&nbsp;";
			qsUINIn = "&nbsp;";
			qsUINOut = "&nbsp;";
			qsAvatarIn = "&nbsp;";
			qsAvatarOut = "&nbsp;";
		}
		header.replace("%base%", options->currentTemplate->getRealTemplatePath());  // base URL
		header.replace("%time%", qsTime);          // event's time
		header.replace("%date%", qsDate);          // event's date
		header.replace("%cidIn%", qsUINIn);          // user's ID
		header.replace("%cidOut%", qsUINOut);         // contact's ID
		header.replace("%avatarIn%", qsAvatarIn);  // link to contact's picture
		header.replace("%avatarOut%",qsAvatarOut); // link to user's picture
		header.replace("%nameIn%", qsNameIn);      // contact's name
		header.replace("%nameOut%", qsNameOut);    // users's name
		header.replace("%proto%", QString::fromAscii(szProto)); // protocol name
	}
	else
		header = noTemplate;
}

QString HTMLBuilder::getLastEvent() {
	//return header + document + lastEvent + "</body></html>";
	return lastEvent;
}

QString HTMLBuilder::getHead() {
	//return header + document + lastEvent + "</body></html>";
	return header + footer;
}

QString HTMLBuilder::getHistory() {
	return header + history + footer;
}

void HTMLBuilder::clearDoc(IEVIEWEVENT* event) {
	header.clear();
	footer.clear();
	history.clear();
	lastEvent.clear();

	if (event != NULL) {
		setLastIEViewEvent(event);
	}
	if (lastIEViewEvent.pszProto != NULL || event->hContact == NULL) {
		setLastEventType(-1);
	}
}

int HTMLBuilder::getLastEventType() {
	return iLastEventType;
}

void HTMLBuilder::setLastEventType(int t) {
	iLastEventType = t;
}

DWORD HTMLBuilder::getLastEventTime() {
	return lastEventTime;
}

void HTMLBuilder::setLastEventTime(DWORD t) {
	lastEventTime = t;
}
