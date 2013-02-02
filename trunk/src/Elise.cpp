/*
Elise Messages Plugin for Miranda IM

*/

#include "main.h"
#include "Elise.h"
#include "utils.h"
#include "options.h"
#include "services.h"
#include <QWebElement>

Elise* Elise::list = NULL;
CRITICAL_SECTION Elise::mutex;
bool Elise::isInited = false;

Elise::Elise(HWND parent, int x, int y, int cx, int cy, bool showIt)
{
	prev = next = NULL;
	hwnd = NULL;

	height = cy;
	width = cx;    

	builder = new HTMLBuilder(this, parent);
	mainWnd = new QWinWidget(parent);
	webView = new QMyWebView(mainWnd, this);
	//bufView = new QWebView();

	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	QObject::connect(webView, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)),
		Qt::DirectConnection);	

	hwnd = mainWnd->winId();

	QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
	webView->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
	webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
	webView->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	webView->settings()->setMaximumPagesInCache(0);
	webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

	webView->setHtml(builder->getHead(), builder->getTemplateUrl());

	//webView->settings()->JavaEnabled = QWebSettings::JavascriptEnabled;
	//webView->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);

	if (showIt)
		mainWnd->show();

	EnterCriticalSection(&mutex);
	next = list;
	if (next != NULL) {
		next->prev = this;
	}
	list = this;
	LeaveCriticalSection(&mutex);	
}

Elise::~Elise()
{
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

QMyWebView::QMyWebView(QWidget* parentWidget, Elise* elise)
{
	this->setParent(parentWidget);
	parent = elise;
	inspector = 0;
}

QMyWebView::~QMyWebView()
{
	//inspector->~QWebInspector();
	delete inspector;
}

void QMyWebView::contextMenuEvent(QContextMenuEvent* e)
{
	QMenu* menu = new QMenu(this);

	QAction* menuAction = new QAction("Web Inspector",this);
	connect(menuAction,SIGNAL(triggered()),this,SLOT(callWebInspector()));

	menu->addAction(menuAction);
	menu->exec(e->globalPos());
	delete menuAction;
	delete menu;
}

HWND Elise::getHWND()
{
	return hwnd;
}

Elise* Elise::get(HWND hwnd)
{
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

QString Elise::getHTML()
{
	return webView->page()->mainFrame()->toHtml();
}

void Elise::setWindowPos(int x, int y, int cx, int cy) {    

	height = cy;
	width = cx;

	SetWindowPos(hwnd, HWND_TOP, x, y, cx, cy, 0);
	webView->resize(width, height);
}

void Elise::scrollToBottom()
{
	webView->page()->mainFrame()->scroll(0, webView->page()->mainFrame()->scrollBarMaximum(Qt::Vertical));
}

void Elise::appendEvent(IEVIEWEVENT* event)
{
	if (event->eventData == NULL) {return; }
	builder->appendEventNew(this, event);
}

void Elise::appendEventOld(IEVIEWEVENT* event)
{	
	builder->appendEventOld(this, event);
}

void Elise::reloadDoc()
{
	webView->setHtml(builder->getHistory(), builder->getTemplateUrl());
}

void Elise::addToDoc()
{
	QString add = builder->getLastEvent();
	QWebElement bodyElem = webView->page()->mainFrame()->documentElement().findFirst("body");

	//-- Add block to DOM
	if (Options::isJQueryUsed(builder->getContact())) {
		//-- If JQuery supported
		//-- Without it JQuery will not work
		add.remove("\n", Qt::CaseInsensitive);

		add = "var $bodyy=$('body');"
			"$bodyy.append('" + add + "');"
			"null;";
		bodyElem.evaluateJavaScript(add);
	}
	else {
		bodyElem.appendInside(add);
	}
	//-- Now create and dispatch event EliseMessageInserted
	bodyElem.evaluateJavaScript(
			" var elem = document.body;"
			" var evt = document.createEvent(\"MutationEvent\");"
			" evt.initMutationEvent (\"EliseMessageInserted\", true, false, null, null, null, null, MutationEvent.MODIFICATION);"
			" elem.dispatchEvent(evt);"
			" null;"
			);
}

void Elise::clear(IEVIEWEVENT* event)
{
	builder->clearDoc(event);
	builder->initHead();
	webView->setHtml(builder->getHistory(), builder->getTemplateUrl());
}

int Elise::getSelection(IEVIEWEVENT* event)
{
	// ЗАВТРОМЭН!
	return 0;
}

void Elise::saveDocument()
{
	// TODO: реализация сохранения лога как страницы..
	// ВСЕ БУДЕТ СДЕЛАНО, НО ЗАВТРО! ЗАВТРОМЭН!
}

void Elise::linkClicked(QUrl url)
{
	QDesktopServices::openUrl(url);
}

LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM)
{
	return 0;
}
HWND bumpQt()
{
	WNDCLASSEX wcx;  
	// Fill in the window class structure with parameters 
	// that describe the main window.  
	wcx.cbSize = sizeof(wcx);          // size of structure 
	wcx.style = CS_HREDRAW | 
		CS_VREDRAW | WS_VISIBLE ;                    // redraw if size changes 
	wcx.lpfnWndProc = MainWndProc;     // points to window procedure 
	wcx.cbClsExtra = 0;                // no extra class memory 
	wcx.cbWndExtra = 0;                // no extra window memory 
	wcx.hInstance = hEliseInst;         // handle to instance 
	wcx.hIcon = 0;              // predefined app. icon 
	wcx.hCursor = 0;                    // predefined arrow 
	wcx.hbrBackground = 0;                  // white background brush 
	wcx.lpszMenuName =  0;    // name of menu resource 
	wcx.lpszClassName = L"MainWClass";  // name of window class 
	wcx.hIconSm = 0;  
	// Register the window class.  
	RegisterClassEx(&wcx);

	return CreateWindow( 
		L"MainWClass",        // name of window class 
		L"Sample",            // title-bar string 
		WS_OVERLAPPEDWINDOW, // top-level window 
		CW_USEDEFAULT,       // default horizontal position 
		CW_USEDEFAULT,       // default vertical position 
		CW_USEDEFAULT,       // default width 
		CW_USEDEFAULT,       // default height 
		(HWND) NULL,         // no owner window 
		(HMENU) NULL,        // use class menu 
		hEliseInst,           // handle to application instance 
		(LPVOID) NULL);      // no window-creation data 

}

int Elise::InitEliseMessages(void)
{
	if (isInited) return 0;
	isInited = true;
	InitializeCriticalSection(&mutex);	

	Options::initOptions();

	TemplateMap::loadBBCodes();

	HWND bumpWND = bumpQt();
	Elise* view = new Elise(bumpWND, 0, 0, 0, 0, 0);

	Utils::hookEvent_Ex(ME_OPT_INITIALISE, Options::initOptionsPage);
	//Utils::hookEvent_Ex(ME_SYSTEM_MODULESLOADED, Options::modulesLoaded);
	//Utils::hookEvent_Ex(ME_SYSTEM_PRESHUTDOWN, PreShutdown);

	Utils::createServiceFunction_Ex(MS_IEVIEW_WINDOW, (MIRANDASERVICE)HandleEliseWindow);
	Utils::createServiceFunction_Ex(MS_IEVIEW_EVENT, (MIRANDASERVICE)HandleEliseEvent);
	//utils::createServiceFunction_Ex(MS_IEVIEW_EVENT,(MIRANDASERVICE) HandleIENavigate);
	hHookOptionsChanged = CreateHookableEvent(ME_IEVIEW_OPTIONSCHANGED);

	view->~Elise();
	DestroyWindow(bumpWND);

	return 0;
}

void Elise::ReleaseEliseMessages()
{
	EnterCriticalSection(&mutex);
	while (list != NULL) {
		delete list;
	}
	LeaveCriticalSection(&mutex);
	DeleteCriticalSection(&mutex);
}

void QMyWebView::callWebInspector()
{
	if (!inspector) {
		inspector = new QWebInspector();
		inspector->setPage(this->page());
		inspector->resize(800, 600);
	}
	inspector->show();
	
	/*QTextEdit* view = new QTextEdit();
    view->resize(500, 500);
    view->setWindowTitle("HTML code");
    view->setText(parent->getHTML());
    view->show();*/
}
