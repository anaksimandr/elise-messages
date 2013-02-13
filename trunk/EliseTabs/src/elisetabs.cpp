//#include <QApplication>
//#include <QQuickWindow>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include "elisetabs.h"

EliseTabs::EliseTabs()
{
	//QApplication app();
	view = new QQuickView();
	//view->connect(view->engine(), SIGNAL(quit()), &app, SLOT(quit()));
	view->setSource(QUrl("qrc:/qml/mainWindow.qml"));
	view->setResizeMode(QQuickView::SizeRootObjectToView);
	view->show();
	QQmlContext* context = view->engine()->rootContext();
	context->setContextProperty("_core", this);
}

EliseTabs::~EliseTabs()
{
	delete view;
}
