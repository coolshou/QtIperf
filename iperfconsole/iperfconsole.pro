#-------------------------------------------------
#
# Project created by QtCreator 2018-09-04T21:46:28
#
#-------------------------------------------------
TARGET = iperfconsole

QT       += core gui network xml
#require Qt 5.7
QT       += charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app

#include(libmaia/maia.pri)
include(../jcon-cpp.pri)

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/tpchart.cpp \
    src/dlgpair.cpp \
    src/anisystrayicon.cpp \
    src/treemodel.cpp \
    src/treeitem.cpp \
    src/myfun.cpp \
    ../src/iperfxmlcfg.cpp \
    src/broadcastreceiver.cpp \
    src/testcfg.cpp \
    src/endpointclient.cpp \
    ../src/iperfxmlpair.cpp

HEADERS += \
        src/mainwindow.h \
    src/tpchart.h \
    src/dlgpair.h \
    src/anisystrayicon.h \
    src/treemodel.h \
    src/treeitem.h \
    src/myfun.h \
    ../src/iperfxmlcfg.h \
    src/broadcastreceiver.h \
    ../src/const.h \
    src/testcfg.h \
    src/endpointclient.h \
    ../src/iperfxmlpair.h

FORMS += \
        src/mainwindow.ui \
    src/dlgpair.ui

RESOURCES += \
    src/images.qrc

DISTFILES += \
    src/images/iperfconsole.png \
    src/images/iperfconsole.ico
