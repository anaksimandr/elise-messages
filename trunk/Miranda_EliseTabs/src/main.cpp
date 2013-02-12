#include <QApplication>
#include "elisetabs.h"

/*#ifndef NDEBUG
#include <QFile>
void messageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	QFile file("log.txt");
	file.open(QIODevice::Append | QIODevice::Text);
	QTextStream log(&file);
	switch (type) {
		case QtDebugMsg:
			log << "Debug: " << msg << "\n";
			break;
		case QtWarningMsg:
			log << "Warning: " << msg << "\n";
			break;
		case QtCriticalMsg:
			log << "Critical: " << msg << "\n";
			break;
		case QtFatalMsg:
			log << "Fatal: " << msg << "\n";
	}
	log << "File:		" << context.file << "\n" << "Function:	" << context.function << "\n";
	log << "Line:		" << context.line << "\n\n";
	file.close();
}
#endif //NDEBUG*/

int main(int argc, char* argv[])
{
/*#ifndef NDEBUG
	qInstallMessageHandler(messageOutput);
#endif //NDEBUG*/
	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(true);

	new EliseTabs();


	/*QGuiApplication app(argc,argv);
	QQuickView view;
	view.connect(view.engine(), SIGNAL(quit()), &app, SLOT(quit()));
	view.setSource(QUrl("qrc:///" #NAME ".qml"));
	if (QGuiApplication::platformName() == QLatin1String("qnx") ||
		  QGuiApplication::platformName() == QLatin1String("eglfs")) {
		view.setResizeMode(QQuickView::SizeRootObjectToView);
		view.showFullScreen();
	} else {
		view.show();
	}
	return app.exec();*/


	return app.exec();
}
