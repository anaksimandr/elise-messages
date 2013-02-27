#ifndef ELISETABS_PROXYTEXTEDIT_H_
#define ELISETABS_PROXYTEXTEDIT_H_
#include <QQuickItem>
#include <QGraphicsItem>
#include <QTextEdit>
#include <QGraphicsProxyWidget>
#include <QString>



class ProxyTextEdit : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
	Q_PROPERTY(int width READ getWidgetWidth WRITE setWidgetWidth)
	Q_PROPERTY(int height READ getWidgetHeight WRITE setWidgetHeight)
public:
	explicit ProxyTextEdit(QQuickItem* parent = 0);
	~ProxyTextEdit();
	QString getText() const;
	int getWidgetWidth() const;
	int getWidgetHeight() const;
	void setText(const QString& text);
	void setWidgetWidth(const int width);
	void setWidgetHeight(const int height);
signals:
	void textChanged();
private:
	QTextEdit* textEdit;
	QGraphicsProxyWidget* proxyWidget;
};
#endif // ELISETABS_PROXYTEXTEDIT_H_
