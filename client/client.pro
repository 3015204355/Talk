#-------------------------------------------------
#
# Project created by QtCreator 2019-10-30T14:42:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += \
    client.cpp \
    main.cpp \
    register.cpp \
    login.cpp \
    talk.cpp \
    buttonlistwidget.cpp \
    view.cpp

HEADERS  += \
    client.h \
    register.h \
    login.h \
    talk.h \
    buttonlistwidget.h \
    view.h

FORMS    += client.ui \
    register.ui \
    login.ui \
    talk.ui \
    view.ui
