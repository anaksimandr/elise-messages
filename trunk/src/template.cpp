
#include "template.h"

class Elise;

QMap<QString, QString> TemplateMap::templateMap;
QMap<QString, QRegExp> TemplateMap::templateBBCodes;

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

void TemplateMap::loadTemplate(const QString &fileName) {	
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		MessageBox(NULL, _T("Can not open file for reading."),_T("Error"),MB_OK);
		Elise::setTemplateInit(false);
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
	Elise::setTemplateInit(true);	
}


void TemplateMap::loadBBCodes() {

	templateBBCodes["b"] = QRegExp("[[]b[]](.*)[[]/b[]]", Qt::CaseInsensitive);          // bolded text
	templateBBCodes["s"] = QRegExp("[[]s[]](.*)[[]/s[]]", Qt::CaseInsensitive);          // strikethrough text
	templateBBCodes["i"] = QRegExp("[[]i[]](.*)[[]/i[]]", Qt::CaseInsensitive);          // italicized text
	templateBBCodes["u"] = QRegExp("[[]u[]](.*)[[]/u[]]", Qt::CaseInsensitive);          // underlined text
	templateBBCodes["img"] = QRegExp("[[]img[]](.*)[[]/img[]]", Qt::CaseInsensitive);    // image
	templateBBCodes["code"] = QRegExp("[[]code[]](.*)[[]/code[]]", Qt::CaseInsensitive); // monospaced text
	templateBBCodes["quote"] = QRegExp("[[]quote[]](.*)[[]/quote[]]", Qt::CaseInsensitive);        // quoted text
	templateBBCodes["size"] = QRegExp("[[]size=(\\d+)[]](.*)[[]/size[]]", Qt::CaseInsensitive);    // sized text
	templateBBCodes["color"] = QRegExp("[[]color=(.+)[]](.*)[[]/color[]]", Qt::CaseInsensitive); // colored text

	templateBBCodes["b"].setMinimal(true);
	templateBBCodes["s"].setMinimal(true);
	templateBBCodes["i"].setMinimal(true);
	templateBBCodes["u"].setMinimal(true);
	templateBBCodes["img"].setMinimal(true);
	templateBBCodes["code"].setMinimal(true);
	templateBBCodes["quote"].setMinimal(true);
	templateBBCodes["size"].setMinimal(true);
	templateBBCodes["color"].setMinimal(true);
}

