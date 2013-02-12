QT  += quick widgets
TARGET  = EliseTabs
TEMPLATE	= app
#TEMPLATE    = lib
#CONFIG  += shared
VERSION = 0.0.0.1
DEFINES += UNICODE

#include(F:\Sources\qtwinmigrate\src\qtwinmigrate.pri)
#win32:INCLUDEPATH += \
#	F:\Qt\4.8.0\include \
#	F:\Sources\Miranda\miranda\include

#win32:LIBS += \
#	path/to/Psapi.lib

OTHER_FILES += \
    res/qml/mainWindow.qml

RESOURCES += \
    res/res.qrc

HEADERS += \
	src/elisetabs.h

SOURCES += \
	src/main.cpp \
	src/elisetabs.cpp
