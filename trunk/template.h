
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
    static void LoadTemplate(const QString &fileName);

};

#endif // TEMPLATE_H
