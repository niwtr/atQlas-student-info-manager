#-------------------------------------------------
#
# Project created by QtCreator 2016-04-30T17:36:43
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += network
CONFIG += c++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AtQlas_alpha
TEMPLATE = app


SOURCES += main.cpp\
        atqlass_main.cpp \
    defschool.cpp \
    account.cpp \
    login.cpp \
    admin_cockpit.cpp \
    admin_stmanager.cpp \
    admin_stmanager_detail.cpp \
    admin_lesson_db.cpp \
    xparser.cpp \
    admin_tchmanager.cpp \
    admin_tchmanager_detail.cpp \
    stu_manage.cpp \
    tch_manage.cpp \
    tch_manager_detail.cpp \
    admin_lesson_db_new_lesson.cpp \
    TIME_MACHINE.cpp \
    timemachine_main.cpp \
    capsule.cpp \
    kompiler.cpp \
    interval_movie.cpp \
    talkserver.cpp

HEADERS  += atqlass_main.h \
    defschool.h \
    account.h \
    admin_cockpit.h \
    admin_stmanager.h \
    admin_stmanager_detail.h \
    admin_lesson_db.h \
    xparser.h \
    admin_tchmanager.h \
    admin_tchmanager_detail.h \
    stu_manage.h \
    tch_manage.h \
    tch_manager_detail.h \
    admin_lesson_db_new_lesson.h \
    common_animation.h \
    login.h \
    TIME_MACHINE.h \
    timemachine_main.h \
    capsule.h \
    kompiler.h \
    def.h \
    sortapi.h \
    interval_movie.h \
    talkserver.h

FORMS    += atqlass_main.ui \
    login.ui \
    admin_cockpit.ui \
    admin_stmanager.ui \
    admin_stmanager_detail.ui \
    admin_lesson_db.ui \
    admin_tchmanager.ui \
    admin_tchmanager_detail.ui \
    stu_manage.ui \
    tch_manage.ui \
    tch_manager_detail.ui \
    admin_lesson_db_new_lesson.ui \
    timemachine_main.ui \
    capsule.ui \
    kompiler.ui \
    interval_movie.ui

RESOURCES += \
    atqres.qrc
