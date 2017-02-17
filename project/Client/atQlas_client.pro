#-------------------------------------------------
#
# Project created by QtCreator 2016-06-28T15:32:31
#
#-------------------------------------------------

QT       += core gui
QT       += network
CONFIG  += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = atQlas_client
TEMPLATE = app


SOURCES += main.cpp\
        atqlas_main.cpp \
    login.cpp \
    talkclient.cpp \
    stumanage.cpp \
    tchmanage.cpp \
    tchmanager_detail.cpp

HEADERS  += atqlas_main.h \
    login.h \
    common_animation.h \
    def.h \
    stumanage.h \
    csv.h \
    tchmanage.h \
    tchmanager_detail.h \
    block_encapsules.h \
    talkclient.h

FORMS    += atqlas_main.ui \
    login.ui \
    stumanage.ui \
    tchmanage.ui \
    tchmanager_detail.ui

RESOURCES += \
    atqres.qrc
