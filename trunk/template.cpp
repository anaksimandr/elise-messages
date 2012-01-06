
#include "template.h"

QMap<QString, QString> TemplateMap::templateMap;

static const int templatesNumber = 48;
static const QString templateNames[48] = {
	"<!--HTMLStart-->",
	"<!--MessageIn-->",
	"<!--MessageOut-->",
	"<!--hMessageIn-->",
	"<!--hMessageOut-->",
	"<!--File-->",
	"<!--hFile-->",
	"<!--URL-->",
	"<!--hURL-->",
	"<!--Status-->",
	"<!--hStatus-->",
	"<!--MessageInGroupStart-->",
	"<!--MessageInGroupInner-->",
	"<!--MessageInGroupEnd-->",
	"<!--hMessageInGroupStart-->",
	"<!--hMessageInGroupInner-->",
	"<!--hMessageInGroupEnd-->",
	"<!--MessageOutGroupStart-->",
	"<!--MessageOutGroupInner-->",
	"<!--MessageOutGroupEnd-->",
	"<!--hMessageOutGroupStart-->",
	"<!--hMessageOutGroupInner-->",
	"<!--hMessageOutGroupEnd-->",
	"<!--FileIn-->",
	"<!--hFileIn-->",
	"<!--FileOut-->",
	"<!--hFileOut-->",
	"<!--URLIn-->",
	"<!--hURLIn-->",
	"<!--URLOut-->",
	"<!--hURLOut-->",

	"<!--HTMLStartRTL-->",
	"<!--MessageInRTL-->",
	"<!--MessageOutRTL-->",
	"<!--hMessageInRTL-->",
	"<!--hMessageOutRTL-->",
	"<!--MessageInGroupStartRTL-->",
	"<!--MessageInGroupInnerRTL-->",
	"<!--MessageInGroupEndRTL-->",
	"<!--hMessageInGroupStartRTL-->",
	"<!--hMessageInGroupInnerRTL-->",
	"<!--hMessageInGroupEndRTL-->",
	"<!--MessageOutGroupStartRTL-->",
	"<!--MessageOutGroupInnerRTL-->",
	"<!--MessageOutGroupEndRTL-->",
	"<!--hMessageOutGroupStartRTL-->",
	"<!--hMessageOutGroupInnerRTL-->",
	"<!--hMessageOutGroupEndRTL-->"
};

void TemplateMap::LoadTemplate(const QString &fileName) {
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		MessageBox(NULL, _T("Can not open file for reading."),_T("Error"),MB_OK);
		return;
	}

	QTextStream in(&file);

	QString buf;
	QString bufTrim;
	QString tmplBuf;
	QString tmplName;
	bool parseInited = false;
	while (!in.atEnd()) {
		buf = in.readLine(0);
		bufTrim = buf.trimmed();
		// если дошли до нового шаблона - сохраняемся
		for (int i=0; i < templatesNumber; i++){
			if (bufTrim == templateNames[i]) {
				if (parseInited) templateMap[tmplName] = tmplBuf;
				tmplBuf.clear();
				tmplName = bufTrim;
				if (!parseInited) parseInited = true;
			}
		}
		// иначе продолжаем запись старого
		tmplBuf += buf;
	}
	templateMap[tmplName] = tmplBuf;
}

