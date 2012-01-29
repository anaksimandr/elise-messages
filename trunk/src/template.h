
class TempalteMap;

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QString>
#include <Qfile>
#include <QTextStream>
#include <QMap>
#include "main.h"
#include "Elise.h"

//extern QString skinPath;

class TemplateMap {
private:

public:
	static QMap<QString, QString> templateMap;
	static QMap<QString, QRegExp> templateBBCodes;
	static void loadTemplate(const QString &fileName);
	static void loadBBCodes();	

};

#endif // TEMPLATE_H
