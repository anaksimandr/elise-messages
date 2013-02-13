#ifndef ELISETABS_ELISETABS_H_
#define ELISETABS_ELISETABS_H_

#include <QWidget>

class QQuickWindow;
class QQuickView;

class EliseTabs : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ name WRITE setName
			   NOTIFY nameChanged)
private:
	QQuickView*	view;
public:
	EliseTabs();
	~EliseTabs();

};

#endif // ELISETABS_MAINWINDOW_H_
