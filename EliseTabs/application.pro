QT += widgets qml quick
CONFIG += console
TARGET = application

SOURCES += \
	src/main.cpp

OTHER_FILES += \
	res/qml/main.qml \
	res/qml/*.qml \
	res/qml/border/*.qml

RESOURCES += \
    resources.qrc

MOC_DIR = ./.moc
OBJECTS_DIR = ./.obj
UI_DIR = ./.ui
RCC_DIR = ./.rcc
