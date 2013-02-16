QT  += qml quick widgets
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

#OTHER_FILES += \
#	res/qml/mainWindow.qml \
#	res/qml/LeftBorder.qml \
#	res/qml/RightBorder.qml \
#	res/qml/BottomBorder.qml \
#	res/qml/BottomRightBorder.qml

RESOURCES +=

HEADERS += \
	src/elisetabs.h

SOURCES += \
	src/main.cpp \
	src/elisetabs.cpp

CONFIG (debug, debug|release) {
	DESTDIR = ../Plugins/EliseTabs/Debug

}else{
	DESTDIR = ../Plugins/EliseTabs/Release
}

# Add more folders to ship with the application, here
folder_01.source = res/img
folder_01.target = $$DESTDIR/
folder_02.source = res/qml
folder_02.target = $$DESTDIR/
DEPLOYMENTFOLDERS = folder_01 folder_02

# Please do not modify the following two lines. Required for deployment.
include(qml.pri)
qtcAddDeployment()
