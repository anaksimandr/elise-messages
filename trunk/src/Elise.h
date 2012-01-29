/*
Elise Messages Plugin for Miranda IM

*/

class Elise;

#ifndef ELISE_H
#define ELISE_H

#include <QtCore>
#include <QtGui>
#include <QWebView>
#include <QtWebKit>
#include <QWinWidget>

#include "main.h"
#include "m_ieview.h"
#include "template.h"
#include "HTMLBuilder.h"

class QMyWebView : public QWebView {
    Q_OBJECT
private:
    Elise*			parent;
	QWebInspector*	inspector;
    virtual void	contextMenuEvent(QContextMenuEvent* e);
public slots:
    void			callWebInspector();
public:
    QMyWebView(QWidget* parentWidget, Elise* elise);
    ~QMyWebView();
};

class Elise : public QWidget {
    Q_OBJECT
private:
    static Elise* list;
    static CRITICAL_SECTION mutex;
    //static HWND hElise;
    static bool		isInited;
	static bool		templateInitialized;
	//bool			isHeaderSet;
    HWND            hwnd;
    HWND            parent;
    Elise           *prev, *next;
    int             height;
    int             width;
    QWinWidget*     mainWnd;
    QMyWebView*     webView;
	//QWebView*		bufView;
    HTMLBuilder*    builder;
protected:
    //void			navigate(const char *);
    //void			navigate(const wchar_t *);
public:
    Elise(HWND parent, int x, int y, int cx, int cy);
    ~Elise();
    HWND            getHWND();
    static Elise*   get(HWND);
    QString         getHTML();
    void            setWindowPos(int x, int y, int cx, int cy);
    void            scrollToBottom();
    void            appendEvent(IEVIEWEVENT * event);
    void            appendEventOld(IEVIEWEVENT * event);
    void            clear(IEVIEWEVENT * event);
    int             getSelection(IEVIEWEVENT * event);
    void            addToDoc(QString add);	
	void			reloadDoc();
    void            saveDocument();
	
	static void		setTemplateInit(bool isInit);
    static int      InitEliseMessages(void);
    static void     ReleaseEliseMessages(void);
//public slots:
    //void            loadStarted();
    //void            loadFinished();
private slots:
    void			linkClicked(QUrl url);
};


#endif // ELISE_H
