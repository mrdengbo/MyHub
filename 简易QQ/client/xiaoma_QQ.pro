#-------------------------------------------------
#
# Project created by QtCreator 2018-01-02T19:45:42
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xiaoma_QQ
TEMPLATE = app

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
        main.cpp \
    chatwidget.cpp \
    logindialog.cpp \
    registerdialog.cpp \
    connectnetworkdialog.cpp \
    maindialog_2.cpp \
    edutdialog.cpp \
    queryfrienddialog.cpp

HEADERS += \
    chatwidget.h \
    logindialog.h \
    mymacro.h \
    registerdialog.h \
    connectnetworkdialog.h \
    maindialog_2.h \
    edutdialog.h \
    queryfrienddialog.h

RESOURCES += \
    qq_image.qrc

FORMS += \
    registerdialog.ui \
    connectnetworkdialog.ui \
    maindialog_2.ui \
    edutdialog.ui \
    queryfrienddialog.ui
