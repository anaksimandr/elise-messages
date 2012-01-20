

QT  += core gui webkit

TARGET  = Elise_Messages_Qt

TEMPLATE    = lib
CONFIG  += shared

VERSION = 0.0.0.3
DEFINES += UNICODE

SOURCES += \
    src/main.cpp \
    src/utils.cpp \
    src/options.cpp \
    src/services.cpp \
    src/Elise.cpp \
    src/template.cpp \
    src/HTMLBuilder.cpp

HEADERS += \
    src/main.h \
    src/utils.h \
    src/options.h \
    resource.h \
    src/services.h \
    src/m_ieview.h \
    src/Elise.h \
    src/template.h \
    src/HTMLBuilder.h


win32:RC_FILE += \
    elise_resources.rc

include(F:\Sources\qtwinmigrate\src\qtwinmigrate.pri)
INCLUDEPATH += \
    F:\Qt\4.8.0\include \
    F:\Sources\Miranda\miranda\include






















