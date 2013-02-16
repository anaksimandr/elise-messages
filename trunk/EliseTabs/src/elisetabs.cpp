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
	currentShape = -1;

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

	view->setSource(QUrl("qml/mainWindow.qml"));
	view->setResizeMode(QQuickView::SizeRootObjectToView);
	//view->setResizeMode(QQuickView::SizeViewToRootObject);
	view->show();
	QQmlContext* context = view->engine()->rootContext();
	context->setContextProperty("_window", this);
	context->setContextProperty("_view", view);

}

EliseTabs::~EliseTabs()
{
	delete view;
}

Qt::CursorShape EliseTabs::cursorShape() const
{
	return view->cursor().shape();
}

void EliseTabs::setCursorShape(Qt::CursorShape cursorShape)
{
	if (currentShape == (int)cursorShape)
		return;

	view->setCursor(cursorShape);
	emit cursorShapeChanged();

	currentShape = cursorShape;
}

void EliseTabs::moveLeft(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setWidth(view->width() + (oldPos.x() - newPos.x()));
	view->setX(view->x() + (newPos.x() - oldPos.x()));
}

void EliseTabs::moveTop(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setHeight(view->height() + (oldPos.y() - newPos.y()));
	view->setY(view->y() + (newPos.y() - oldPos.y()));
}

void EliseTabs::moveRight(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setWidth(view->width() + (newPos.x() - oldPos.x()));
}

void EliseTabs::moveRightBottom(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setWidth(view->width() + (newPos.x() - oldPos.x()));
	view->setHeight(view->height() + (newPos.y() - oldPos.y()));
}

void EliseTabs::moveBottom(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setHeight(view->height() + (newPos.y() - oldPos.y()));
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
