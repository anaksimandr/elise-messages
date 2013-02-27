#include "proxytextedit.h"

ProxyTextEdit::ProxyTextEdit(QQuickItem *parent) : QQuickItem(parent)
{
	textEdit = new QTextEdit;
	//proxyWidget = new QGraphicsProxyWidget(this);
	//proxyWidget->setWidget(textEdit);
}

ProxyTextEdit::~ProxyTextEdit()
{
	delete textEdit;
	delete proxyWidget;
}

QString ProxyTextEdit::getText() const
{
	return textEdit->toPlainText();
}

void ProxyTextEdit::setText(const QString &text)
{
	textEdit->setText(text);
}

int ProxyTextEdit::getWidgetWidth() const
{
	return proxyWidget->geometry().width();
}

void ProxyTextEdit::setWidgetWidth(const int width)
{
	proxyWidget->resize(width, proxyWidget->geometry().height());
}

int ProxyTextEdit::getWidgetHeight() const
{
	return proxyWidget->geometry().height();
}

void ProxyTextEdit::setWidgetHeight(const int height)
{
	proxyWidget->resize(proxyWidget->geometry().width(), height);
}
