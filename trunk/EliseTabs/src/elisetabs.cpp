#include <QApplication>
#include <QDesktopWidget>
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

	view->setFlags(Qt::Fr);
	view->setFlags(Qt::FramelessWindowHint);

	//QSurfaceFormat format;
	//format.setAlphaBufferSize(8);
	//view->setFormat(format);
	view->setClearBeforeRendering(true);
	view->setColor(QColor(Qt::transparent));

	view->setSource(QUrl("qml/MainViewBorder.qml"));
	view->setResizeMode(QQuickView::SizeRootObjectToView);
	//view->setResizeMode(QQuickView::SizeViewToRootObject);

	view->resize(750, 550);
	QRect d = QApplication::desktop()->screenGeometry();
	view->setX(d.width() / 2 - view->width() / 2);
	view->setY(d.height() / 2 - view->height() / 2);

	view->show();
	QQmlContext* context = view->engine()->rootContext();
	context->setContextProperty("_window", this);
	context->setContextProperty("_view", view);

}

EliseTabs::~EliseTabs()
{
	delete view;
}

void EliseTabs::close()
{
	QApplication::exit();
}

void EliseTabs::minimize()
{
	view->setWindowState(Qt::WindowMinimized);
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
	view->setX(view->x() + (newPos.x() - oldPos.x()));
	view->setWidth(view->width() + (oldPos.x() - newPos.x()));
}

void EliseTabs::moveLeftTop(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setX(view->x() + (newPos.x() - oldPos.x()));
	view->setY(view->y() + (newPos.y() - oldPos.y()));
	view->setWidth(view->width() + (oldPos.x() - newPos.x()));
	view->setHeight(view->height() + (oldPos.y() - newPos.y()));
}

void EliseTabs::moveTop(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setY(view->y() + (newPos.y() - oldPos.y()));
	view->setHeight(view->height() + (oldPos.y() - newPos.y()));
}

void EliseTabs::moveTopRight(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setY(view->y() + (newPos.y() - oldPos.y()));
	view->setHeight(view->height() + (oldPos.y() - newPos.y()));
	view->setWidth(view->width() + (newPos.x() - oldPos.x()));
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

void EliseTabs::moveBottomLeft(QPoint newPos, QPoint oldPos)
{
	newPos = view->mapToGlobal(newPos);
	oldPos = view->mapToGlobal(oldPos);
	view->setX(view->x() + (newPos.x() - oldPos.x()));
	view->setWidth(view->width() + (oldPos.x() - newPos.x()));
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
