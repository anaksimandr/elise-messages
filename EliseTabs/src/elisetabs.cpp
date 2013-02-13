#include <QApplication>
//#include <QQuickWindow>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include "elisetabs.h"

EliseTabs::EliseTabs()
{
	//QApplication app();
	view = new QQuickView();
	view->connect(view->engine(), &QQmlEngine::quit, QApplication::quit);

	/*setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setAttribute(Qt::WA_NoSystemBackground);
	view = new QmlView(this);
	view->viewport()->setAutoFillBackground(false);*/
	view->setFlags(Qt::FramelessWindowHint);
	//QSurfaceFormat format;
	//format.setAlphaBufferSize(8);
	//view->setFormat(format);
	view->setClearBeforeRendering(true);
	view->setColor(QColor(Qt::transparent));

	view->setSource(QUrl("qrc:/qml/mainWindow.qml"));
	view->setResizeMode(QQuickView::SizeRootObjectToView);
	//view->setResizeMode(QQuickView::SizeViewToRootObject);
	view->show();
	QQmlContext* context = view->engine()->rootContext();
	//context->setContextProperty("_mainWindow", this);

	context->setContextProperty("_mainWindow", view);

}

EliseTabs::~EliseTabs()
{
	delete view;
}

/*QPoint EliseTabs::getPos()
{
	return view->position();
}

void EliseTabs::setPos(QPoint point)
{
	view->setPosition(point);
}

QSize EliseTabs::getSize()
{
	return view->size();
}

void EliseTabs::setSize(QSize size)
{
	view->resize(size);
}*/
