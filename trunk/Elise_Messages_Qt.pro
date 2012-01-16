

QT  += core gui webkit

TARGET  = Elise_Messages_Qt

TEMPLATE    = lib
CONFIG  += shared

VERSION = 0.0.0.2
DEFINES += UNICODE

SOURCES += \
    main.cpp \
    utils.cpp \
    options.cpp \
    services.cpp \
    Elise.cpp \
    template.cpp \
    HTMLBuilder.cpp

HEADERS += \
    main.h \
    utils.h \
    options.h \
    resource.h \
    services.h \
    m_ieview.h \
    Elise.h \
    template.h \
    HTMLBuilder.h


OTHER_FILES += \
    elise_resources.rc

include(F:\Sources\qtwinmigrate\src\qtwinmigrate.pri)
INCLUDEPATH += \
    F:\Qt\4.8.0\include \
    F:\Sources\Miranda\miranda\include






















