QT  += widgets qml quick
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

OTHER_FILES += \
	res/qml/border/LeftBorder.qml \
    res/qml/border/LeftTopBorder.qml \
	res/qml/border/TopBorder.qml \
	res/qml/border/TopRightBorder.qml \
	res/qml/border/RightBorder.qml \
	res/qml/border/RightBottomBorder.qml \
	res/qml/border/BottomBorder.qml \
	res/qml/border/BottomLeftBorder.qml \
	res/qml/border/Button.qml \
    res/qml/MainViewBorder.qml \
	res/qml/MainView.qml \
	res/qml/Tab.qml
