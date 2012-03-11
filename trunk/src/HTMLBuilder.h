
class HTMLBuilder;

#ifndef HTMLBUILDER_H
#define HTMLBUILDER_H

#include <QString>
//#include <QtWebKit>
#include <time.h>
#include "main.h"
#include "m_ieview.h"
#include "Elise.h"
#include "utils.h"
#include <m_contacts.h>
#include "template.h"
#include "options.h"

#define EVENTTYPE_STATUSCHANGE 25368
#define EVENTTYPE_JABBER_CHATSTATES     2000

class Elise;

class HTMLBuilder {
private:
	DWORD lastEventTime;
	int iLastEventType;
	enum ENCODEFLAGS {
		ENF_NONE = 0,
		ENF_SMILEYS = 1,
		ENF_NAMESMILEYS = 2,
		ENF_BBCODES = 4,
		ENF_LINKS = 8,
		ENF_ALL = 255,
		ENF_CHAT_FORMATTING = 256
	};
	Elise*			parentView;
	QString			header;
	QString			footer;
	QString			history;
	QString			lastEvent;
	//QString			document;

	IEVIEWEVENT		lastIEViewEvent;
	time_t			startedTime;
	char*			timestampToString(DWORD dwFlags, time_t check, int mode);
	void			getUINs(HANDLE hContact, QString& uinIn, QString& uinOut);
	void			getAvatar(HANDLE hContact, const char* szProto, QString& result);
	void			replaceBBCodes(QString& text);
	void			replaceURL(QString& text);
	void			replaceSmileys(QString& text, bool isSent, HANDLE hContact, char* szProto);
	void			appendEventTemplate(Elise* view, IEVIEWEVENT* event);
	void			setLastIEViewEvent(IEVIEWEVENT* event);
	bool			isDbEventShown(DWORD dwFlags, DBEVENTINFO* dbei);
	bool			isDbEventShown(DBEVENTINFO* dbei);
	wchar_t*		getContactName(HANDLE hContact, const char* szProto);
	DWORD			getLastEventTime();
	void			setLastEventTime(DWORD);
	int				getLastEventType();
	void			setLastEventType(int);
	//void addToDoc();
public:
	HTMLBuilder(Elise* view);
	~HTMLBuilder();

	QString			getHead();
	QString			getLastEvent();
	QString			getHistory();
	//QString			getDocument();
	//void			setDocument(QString doc);
	void			initHead();
	void			clearDoc(IEVIEWEVENT* event);
	void			appendEventOld(Elise*, IEVIEWEVENT* event);
	void			appendEventNew(Elise* view, IEVIEWEVENT* event);
	void			appendEvent(Elise*, IEVIEWEVENT* event);
	time_t			getStartedTime();
};


#endif // HTMLBUILDER_H
