в *.pro прописать
include(d:\Miranda_Source\Miranda\Qt\qtwinmigrate-2.8-opensource\src\qtwinmigrate.pri)
INCLUDEPATH += \
    d:\Miranda_Source\Miranda\Qt\4.8.0\include \
    d:\Miranda_Source\Miranda\Include_Old \
    d:\Miranda_Source\Miranda\ExternalAPI\

запустить командную строку vc, перейти в папку плагина и выполнить
qmake -tp vc
