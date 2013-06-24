QT  += widgets qml quick
TARGET  = EliseTabs
TEMPLATE	= app
#TEMPLATE    = lib
#CONFIG  += shared
VERSION = 0.0.0.1
DEFINES += UNICODE

RESOURCES +=

HEADERS +=


SOURCES += \
    src/main.cpp


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

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# Please do not modify the following two lines. Required for deployment.
include(qml.pri)
qtcAddDeployment()

OTHER_FILES += \
	res/qml/border/*.qml \
	res/qml/*.qml
