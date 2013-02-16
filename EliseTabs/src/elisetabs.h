#ifndef ELISETABS_ELISETABS_H_
#define ELISETABS_ELISETABS_H_

#include <QWidget>

class QApplication;
class QQuickWindow;
class QQuickView;

class EliseTabs : public QObject
{
	Q_OBJECT
	//Q_PROPERTY(QString name READ name WRITE setName
	//		   NOTIFY nameChanged)
	//Q_PROPERTY(QPoint pos READ getPos WRITE setPos NOTIFY posChanged)
	Q_PROPERTY(Qt::CursorShape cursorShape READ cursorShape WRITE setCursorShape
			   NOTIFY cursorShapeChanged)
private:
	QQuickView*	view;
	int			currentShape;
public:
	EliseTabs();
	~EliseTabs();

	//QSize	getSize();
	//QPoint	getPos();
	//void	setPos(QPoint);
	//void	setSize(QSize);
	Qt::CursorShape		cursorShape() const;
	Q_INVOKABLE void	setCursorShape(Qt::CursorShape cursorShape);

signals:
	//void	posChanged(QPoint);
	void	cursorShapeChanged();
};

#endif // ELISETABS_MAINWINDOW_H_
