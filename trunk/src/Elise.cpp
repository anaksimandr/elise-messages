/*
Elise Messages Plugin for Miranda IM

*/

#include "main.h"
#include "Elise.h"
#include "utils.h"
#include "options.h"
#include "services.h"

//#include <QMessageBox>

Elise* Elise::list = NULL;
CRITICAL_SECTION Elise::mutex;
bool Elise::isInited = false;

// эта хрень пока повисит тут, т.к. опций нет
// путь задается относительно miranda.exe
QString skinPath("Skins/IEView/testSkin/myskin.ivt");
QUrl skinDir = QUrl(QDir::currentPath() + "/" + skinPath);

Elise::Elise(HWND parent, int x, int y, int cx, int cy) {

	this->parent = parent;
	prev = next = NULL;
	hwnd = NULL;

	height = cy;
	width = cx;    

	builder = new HTMLBuilder(this);
	mainWnd = new QWinWidget(parent);
	webView = new QMyWebView(mainWnd, this);

	builder->initDoc();

	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	QObject::connect(webView, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)),
		Qt::DirectConnection);

	hwnd = mainWnd->winId();

	webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
	webView->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	webView->settings()->setMaximumPagesInCache(0);

	webView->setHtml(builder->getDoc(), skinDir);
	//webView->settings()->JavaEnabled = QWebSettings::JavascriptEnabled;
	//webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);

	mainWnd->show();

	EnterCriticalSection(&mutex);
	next = list;
	if (next != NULL) {
		next->prev = this;
	}
	list = this;
	LeaveCriticalSection(&mutex);
}

Elise::~Elise() {
	EnterCriticalSection(&mutex);
	if (list == this) {
		list = next;
	} else if (prev!=NULL) {
		prev->next = next;
	}
	if (next != NULL) {
		next->prev = prev;
	}
	prev = NULL;
	next = NULL;
	LeaveCriticalSection(&mutex);

	builder->~HTMLBuilder();
	webView->~QMyWebView();
	mainWnd->~QWinWidget();
}

QMyWebView::QMyWebView(QWidget* parentWidget, Elise* elise) {
	this->setParent(parentWidget);
	parent = elise;
}

QMyWebView::~QMyWebView() {

}

void QMyWebView::contextMenuEvent(QContextMenuEvent* e) {
	QMenu* menu = new QMenu(this);

	QAction* menuAction = new QAction("Show HTML source",this);
	connect(menuAction,SIGNAL(triggered()),this,SLOT(showText()));

	menu->addAction(menuAction);
	menu->exec(e->globalPos());
	delete menuAction;
	delete menu;
}

HWND Elise::getHWND() {
	return hwnd;
}

Elise* Elise::get(HWND hwnd) {
	Elise* ptr;
	if (list == NULL) return NULL;
	EnterCriticalSection(&mutex);
	for (ptr = list; ptr !=NULL; ptr=ptr->next) {
		if (ptr->hwnd == hwnd) {
			break;
		}
	}
	LeaveCriticalSection(&mutex);
	return ptr;
}

QString Elise::getHTML() {
	return builder->getDoc();
}

void Elise::setWindowPos(int x, int y, int cx, int cy) {    

	height = cy;
	width = cx;

	SetWindowPos(hwnd, HWND_TOP, x, y, cx, cy, 0);
	webView->resize(width, height);
}

void Elise::scrollToBottom() {
	webView->page()->mainFrame()->scroll(0, webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void Elise::appendEvent(IEVIEWEVENT* event) {
	if (event->eventData == NULL) {return; }
	builder->appendEventNew(this, event);
	webView->setHtml(builder->getDoc(), skinDir);
}

void Elise::appendEventOld(IEVIEWEVENT* event) {
	builder->appendEventOld(this, event);
	webView->setHtml(builder->getDoc(), skinDir);    
}

void Elise::clear(IEVIEWEVENT* event) {
	// пока не понял, что тут должно быть =)
	// наверное очищение окна лога..ЗАВТРОМЭН!
}

int Elise::getSelection(IEVIEWEVENT* event) {
	// ЗАВТРОМЭН!
	return 0;
}

void Elise::saveDocument() {
	// TODO: реализация сохранения лога как страницы..
	// ВСЕ БУДЕТ СДЕЛАНО, НО ЗАВТРО! ЗАВТРОМЭН!
}

void Elise::linkClicked(QUrl url) {
	QDesktopServices::openUrl(url);
}

int Elise::InitEliseMessages(void)
{
	if (isInited) return 0;
	isInited = true;
	InitializeCriticalSection(&mutex);

	Utils::hookEvent_Ex(ME_OPT_INITIALISE, Options::InitOptions);
	//Utils::hookEvent_Ex(ME_SYSTEM_MODULESLOADED, Utils::ModulesLoaded);
	//Utils::hookEvent_Ex(ME_SYSTEM_PRESHUTDOWN, PreShutdown);

	Utils::createServiceFunction_Ex(MS_IEVIEW_WINDOW, (MIRANDASERVICE)HandleEliseWindow);
	Utils::createServiceFunction_Ex(MS_IEVIEW_EVENT, (MIRANDASERVICE)HandleEliseEvent);
	//utils::createServiceFunction_Ex(MS_IEVIEW_EVENT,(MIRANDASERVICE) HandleIENavigate);
	hHookOptionsChanged = CreateHookableEvent(ME_IEVIEW_OPTIONSCHANGED);

	TemplateMap::LoadTemplate(skinPath);
	TemplateMap::LoadBBCodes();

	return 0;
}

void Elise::ReleaseEliseMessages() {
	EnterCriticalSection(&mutex);
	while (list != NULL) {
		delete list;
	}
	LeaveCriticalSection(&mutex);
	DeleteCriticalSection(&mutex);
}

void QMyWebView::showText() {
	//QDialog* dlg = new QDialog(this);
	QTextEdit* view = new QTextEdit();
	view->resize(500, 500);
	view->setWindowTitle("HTML code");
	view->setText(parent->getHTML());
	view->show();
	//QProcess* proc=new QProcess(this);
	//proc->start("explorer D:/file.doc");
}
