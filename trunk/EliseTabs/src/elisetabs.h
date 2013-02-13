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
	//Q_PROPERTY(QSize size READ getSize WRITE setSize)
private:
	QQuickView*	view;
public:
	EliseTabs();
	~EliseTabs();

	//QSize	getSize();
	//QPoint	getPos();
	//void	setPos(QPoint);
	//void	setSize(QSize);

signals:
	void	posChanged(QPoint);
};

#endif // ELISETABS_MAINWINDOW_H_
