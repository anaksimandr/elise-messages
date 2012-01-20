
class TempalteMap;

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <QString>
#include <Qfile>
#include <QTextStream>
#include <QMap>
#include "main.h"

extern QString skinPath;

class TemplateMap {
private:

public:
    static QMap<QString, QString> templateMap;
    static QMap<QString, QRegExp> templateBBCodes;
    static void LoadTemplate(const QString &fileName);
    static void LoadBBCodes();

};

#endif // TEMPLATE_H
