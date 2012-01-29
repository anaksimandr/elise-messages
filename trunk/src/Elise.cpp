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
bool Elise::templateInitialized = false;

// эта хрень пока повисит тут, т.к. опций нет
// путь задается относительно miranda.exe
// moved to Options::skinPath
//QString skinPath("Skins/IEView/testSkin/myskin.ivt");
//QUrl skinDir = QUrl(QDir::currentPath() + "/" + skinPath);
QString noTemplate = "Go to options";

Elise::Elise(HWND parent, int x, int y, int cx, int cy) {

	this->parent = parent;
	prev = next = NULL;
	hwnd = NULL;

	height = cy;
	width = cx;    

	builder = new HTMLBuilder(this);
	mainWnd = new QWinWidget(parent);
	webView = new QMyWebView(mainWnd, this);
	//bufView = new QWebView();

	webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	QObject::connect(webView, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)),
		Qt::DirectConnection);	

	hwnd = mainWnd->winId();

	webView->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
	webView->settings()->setAttribute(QWebSettings::PrivateBrowsingEnabled, true);
	webView->settings()->setMaximumPagesInCache(0);
	webView->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);

	if (templateInitialized) {
		builder->initHead();
		webView->setHtml(builder->getHead(), Options::getTemplateUrl());
		//isHeaderSet = true;
	}
	else {
		webView->setHtml(noTemplate, Options::getTemplateUrl());
		//isHeaderSet = false;
	}
	//webView->page()->mainFrame()->documentElement().findFirst("body").appendInside(" ");
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
	//bufView->~QWebView();
	mainWnd->~QWinWidget();
}

QMyWebView::QMyWebView(QWidget* parentWidget, Elise* elise) {
	this->setParent(parentWidget);
	parent = elise;
}

QMyWebView::~QMyWebView() {
	inspector->~QWebInspector();
}

void QMyWebView::contextMenuEvent(QContextMenuEvent* e) {
	QMenu* menu = new QMenu(this);

	QAction* menuAction = new QAction("Web Inspector",this);
	connect(menuAction,SIGNAL(triggered()),this,SLOT(callWebInspector()));

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
	return webView->page()->mainFrame()->toHtml();
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
	if (templateInitialized) {
		if (event->eventData == NULL) {return; }
		builder->appendEventNew(this, event);
		//webView->setHtml(builder->getLastEvent(), Options::getTemplateUrl());
		//addToDoc();
	}
}

void Elise::appendEventOld(IEVIEWEVENT* event) {	
	if (templateInitialized) {
		//bufView->setHtml(builder->getHead(), Options::getTemplateUrl());
		builder->appendEventOld(this, event);
		//webView->setHtml(builder->getHistory(), Options::getTemplateUrl());
		//addToDoc();
	}
}

void Elise::reloadDoc() {
	webView->setHtml(builder->getHistory(), Options::getTemplateUrl());
}

void Elise::addToDoc(QString add) {	
	
	webView->page()->mainFrame()->documentElement().findFirst("body").appendInside(add);

	QRegExp rxScriptSplit = QRegExp("<script([^>]*)>([^>]*)</script>", Qt::CaseInsensitive);
	QRegExp rxScriptReplace = QRegExp(".*<script([^>]*)>(.*)</script>.*", Qt::CaseInsensitive);
	rxScriptSplit.setMinimal(true);

	QString qstrClass;
	QString qstrSource;
	
	for (int i=0; i<add.count(rxScriptSplit); i++) {
		//qstrClass = add.section(rxScriptSplit, i, i, QString::SectionIncludeTrailingSep);
		//qstrSource = qstrClass;
		qstrSource = add.section(rxScriptSplit, i, i, QString::SectionIncludeTrailingSep);
		qstrClass.replace(rxScriptReplace, "\\1");
		qstrSource.replace(rxScriptReplace, "\\2");
		//if (qstrClass.isEmpty())
			webView->page()->mainFrame()->documentElement().findFirst("body").lastChild().evaluateJavaScript(qstrSource);
		/*else {
			//webView->page()->mainFrame()->documentElement().findFirst("body").findFirst("script[class=first]").evaluateJavaScript(qstrSource);
			//QVariant returned = webView->page()->mainFrame()->documentElement().findFirst("body").findFirst("script[class=first]").evaluateJavaScript(qstrSource);
			//QWebPage tmpPage;
			//tmpPage.mainFrame()->setHtml(builder->getHead(), Options::getTemplateUrl());
			//bufView->page()->mainFrame()->documentElement().findFirst("body").evaluateJavaScript("_getitall('testtesttest','anaksimandr','anaksimandr@jabber.ru','F:\Miranda\miranda-im-v0.9.34-unicode\Skins\IEView\testSkin\myskin.ivt',meldungsart[0]);");
			QWebElement* elem =new QWebElement( webView->page()->mainFrame()->documentElement().findFirst("body").findFirst("script[class=first]"));
			elem->evaluateJavaScript("_getitall('testtesttest','anaksimandr','anaksimandr@jabber.ru','F:\Miranda\miranda-im-v0.9.34-unicode\Skins\IEView\testSkin\myskin.ivt',meldungsart[0]);");
			QMessageBox qmes;
			qmes.setText(elem->toPlainText());
			qmes.exec();
			//webView->page()->mainFrame()->documentElement().findFirst("body").findFirst("script[class=first]").parent().setPlainText("<script>_getitall('кен','anaksimandr','anaksimandr@jabber.ru','F:\Miranda\miranda-im-v0.9.34-unicode\Skins\IEView\testSkin\myskin.ivt',meldungsart[0]);</script>");
			webView->page()->mainFrame()->documentElement().findFirst("body").findFirst("script[class=first]").setAttribute("class", "notAvailable");
		}*/
	}

	//QMessageBox qmes;
	//for (int i=0; i<countScripts*2; i++) {
	//	qmes.setText(list[i]);
	//	qmes.exec();
	//}
}

void Elise::clear(IEVIEWEVENT* event) {
	//builder->clearLastEvent();	
	if (templateInitialized) {
		builder->initHead();
		webView->setHtml(builder->getHead(), Options::getTemplateUrl());
	}
	else
		webView->setHtml(noTemplate, Options::getTemplateUrl());
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

void Elise::setTemplateInit(bool isInit) {
	templateInitialized = isInit;
}

int Elise::InitEliseMessages(void)
{
	if (isInited) return 0;
	isInited = true;
	InitializeCriticalSection(&mutex);	

	if (Options::initOptions()) {		
		TemplateMap::loadTemplate(Options::getRealTemplatePath());		
	}
	TemplateMap::loadBBCodes();

	Utils::hookEvent_Ex(ME_OPT_INITIALISE, Options::initOptionsPage);
	//Utils::hookEvent_Ex(ME_SYSTEM_MODULESLOADED, Options::modulesLoaded);
	//Utils::hookEvent_Ex(ME_SYSTEM_PRESHUTDOWN, PreShutdown);

	Utils::createServiceFunction_Ex(MS_IEVIEW_WINDOW, (MIRANDASERVICE)HandleEliseWindow);
	Utils::createServiceFunction_Ex(MS_IEVIEW_EVENT, (MIRANDASERVICE)HandleEliseEvent);
	//utils::createServiceFunction_Ex(MS_IEVIEW_EVENT,(MIRANDASERVICE) HandleIENavigate);
	hHookOptionsChanged = CreateHookableEvent(ME_IEVIEW_OPTIONSCHANGED);

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

void QMyWebView::callWebInspector() {
	if (!inspector) {
		inspector = new QWebInspector();
		inspector->setPage(this->page());
		inspector->resize(800, 600);
	}
	inspector->show();
}
