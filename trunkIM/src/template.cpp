
#include "template.h"

class Elise;

//QMap<QString, QString> TemplateMap::templateMap;
QMap<QString, QRegExp> TemplateMap::templateBBCodes;

static const int templatesNumber = 50;
static const QString templateNames[templatesNumber] = {
	"<!--HTMLStart-->",
	"<!--HTMLEnd-->",
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
	"<!--HTMLEndRTL-->",
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

int TemplateMap::loadTemplate() {	
	inited = false;
	//groupsSupported = false;
	filesInOut = false;
	urlInOut = false;
	QFile file(qstrSkinPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return 1;

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
				if (parseInited)
					templateMap[tmplName] = tmplBuf;
				tmplBuf.clear();
				tmplName = bufTrim;
				if (!parseInited) parseInited = true;
			}
		}
		// иначе продолжаем запись старого
		tmplBuf += buf;
		tmplBuf += "\n";
	}
	templateMap[tmplName] = tmplBuf;
	inited = true;

	//-- Now chech group support
	/*if (templateMap.contains("<!--MessageInGroupStart-->"))
	if (templateMap.contains("<!--MessageInGroupStart-->"))
	if (templateMap.contains("<!--MessageInGroupInner-->"))
	if (templateMap.contains("<!--MessageInGroupEnd-->"))
	if (templateMap.contains("<!--hMessageInGroupStart-->"))
	if (templateMap.contains("<!--hMessageInGroupInner-->"))
	if (templateMap.contains("<!--hMessageInGroupEnd-->"))
	if (templateMap.contains("<!--MessageOutGroupStart-->"))
	if (templateMap.contains("<!--MessageOutGroupInner-->"))
	if (templateMap.contains("<!--MessageOutGroupEnd-->"))
	if (templateMap.contains("<!--hMessageOutGroupStart-->"))
	if (templateMap.contains("<!--hMessageOutGroupInner-->"))
	if (templateMap.contains("<!--hMessageOutGroupEnd-->"))
		groupsSupported = true;*/

	//-- Check file in/out support
	if (templateMap.contains("<!--FileIn-->"))
	if (templateMap.contains("<!--hFileIn-->"))
	if (templateMap.contains("<!--FileOut-->"))
	if (templateMap.contains("<!--hFileOut-->"))
		filesInOut = true;

	//-- Check URL in/out support
	if (templateMap.contains("<!--URLIn-->"))
	if (templateMap.contains("<!--hURLIn-->"))
	if (templateMap.contains("<!--URLOut-->"))
	if (templateMap.contains("<!--hURLOut-->"))
		urlInOut = true;

	return 0;
}


void TemplateMap::loadBBCodes() {

	/*templateBBCodes["b"] = QRegExp("[[]b[]](.*)[[]/b[]]", Qt::CaseInsensitive);				// bolded text
	templateBBCodes["s"] = QRegExp("[[]s[]](.*)[[]/s[]]", Qt::CaseInsensitive);				// strikethrough text
	templateBBCodes["i"] = QRegExp("[[]i[]](.*)[[]/i[]]", Qt::CaseInsensitive);				// italicized text
	templateBBCodes["u"] = QRegExp("[[]u[]](.*)[[]/u[]]", Qt::CaseInsensitive);				// underlined text
	templateBBCodes["img"] = QRegExp("[[]img[]](.*)[[]/img[]]", Qt::CaseInsensitive);		// image
	templateBBCodes["code"] = QRegExp("[[]code[]](.*)[[]/code[]]", Qt::CaseInsensitive);	// monospaced text
	templateBBCodes["quote"] = QRegExp("[[]quote[]](.*)[[]/quote[]]", Qt::CaseInsensitive);			// quoted text
	templateBBCodes["size"] = QRegExp("[[]size=(\\d+)[]](.*)[[]/size[]]", Qt::CaseInsensitive);		// sized text
	templateBBCodes["color"] = QRegExp("[[]color=(.+)[]](.*)[[]/color[]]", Qt::CaseInsensitive);	// colored text*/
	templateBBCodes["b"] = QRegExp("\\[b\\](.*)\\[/b\\]", Qt::CaseInsensitive);				// bolded text
	templateBBCodes["s"] = QRegExp("\\[s\\](.*)\\[/s\\]", Qt::CaseInsensitive);				// strikethrough text
	templateBBCodes["i"] = QRegExp("\\[i\\](.*)\\[/i\\]", Qt::CaseInsensitive);				// italicized text
	templateBBCodes["u"] = QRegExp("\\[u\\](.*)\\[/u\\]", Qt::CaseInsensitive);				// underlined text
	templateBBCodes["img"] = QRegExp("\\[img\\](.*)\\[/img\\]", Qt::CaseInsensitive);		// image
	templateBBCodes["code"] = QRegExp("\\[code\\](.*)\\[/code\\]", Qt::CaseInsensitive);	// monospaced text
	templateBBCodes["quote"] = QRegExp("\\[quote\\](.*)\\[/quote\\]", Qt::CaseInsensitive);			// quoted text
	templateBBCodes["size"] = QRegExp("\\[size=([^\\]\\[]+)\\](.*)\\[/size\\]", Qt::CaseInsensitive);		// sized text
	templateBBCodes["color"] = QRegExp("\\[color=([^\\]\\[]+)\\](.*)\\[/color\\]", Qt::CaseInsensitive);	// colored text

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

QString* TemplateMap::getTemplatePath() {	
	return &qstrSkinPath;
}

QString TemplateMap::getRealTemplatePath() {	
	return qstrSkinPath;
}

QUrl TemplateMap::getTemplateUrl() {
	return qurlSkinPath;
}

void TemplateMap::setTemplatePath(wchar_t pszPath[MAX_PATH])
{
	wchar_t pszPath2[MAX_PATH];
	CallService(MS_UTILS_PATHTOABSOLUTEW, (WPARAM)pszPath, (LPARAM)pszPath2);
	qstrSkinPath = QString::fromWCharArray(pszPath2);
	qurlSkinPath = QUrl::fromLocalFile(qstrSkinPath);
}

void TemplateMap::setTemplatePath(QString* path)
{
	qstrSkinPath = *path;
	qurlSkinPath = QUrl::fromLocalFile(qstrSkinPath);
}

TemplateMap::TemplateMap()
{
	inited = false;
	filesInOut = false;
	urlInOut = false;
	qurlSkinPath = cqurlNotSet;
	qstrSkinPath = cqstrNotSet;
}

TemplateMap::TemplateMap(TemplateMap* other)
{
	inited = other->inited;
	filesInOut = other->isFilesInOut();
	urlInOut = other->isURLInOut();
	setTemplatePath(other->getTemplatePath());
	templateMap = other->templateMap;
}

TemplateMap::~TemplateMap()
{

}

