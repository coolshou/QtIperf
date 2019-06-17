TARGET = iperfd
TEMPLATE = app

QT -= gui

QT += network core xml
CONFIG += c++11 console
CONFIG -= app_bundle

include(iperf.pri)

#include(libmaia/maia.pri)
include(../jcon-cpp/jcon-cpp.pri)

include(../qtservice/src/qtservice.pri)

#libmaia
#INCLUDEPATH += ./libmaia
#LIBS += ./libmaia/libmaia.a

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/iperfcore.cpp \
    src/iperfd.cpp \
    src/logger.cpp \
    src/iperfserver.cpp \
    src/iperfclient.cpp \
    src/broadcast.cpp \
    ../src/iperfxmlcfg.cpp \
    ../src/iperfxmlpair.cpp \
    src/endpointservice.cpp

DISTFILES += \
    readme.md

Unix {
DISTFILES += \
    bin/linux/x86/iperf3 \
    bin/linux/x86_64/iperf3
}
Windows {
DISTFILES += \
    bin/Windows/cyggcc_s-1.dll \
    bin/Windows/cygwin1.dll \
    bin/Windows/iperf3.exe
}

HEADERS += \
    src/iperfcore.h \
    src/iperfd.h \
    src/logger.h \
    src/iperfserver.h \
    src/iperfclient.h \
    src/sleeper.h \
    src/broadcast.h \
    ../src/iperfxmlcfg.h \
    ../src/iperfxmlpair.h \
    ../src/const.h \
    src/endpointservice.h

