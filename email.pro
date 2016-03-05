#-------------------------------------------------
#
# Project created by QtCreator 2013-12-01T17:56:38
#
#-------------------------------------------------

QT       += core gui

QT       += network
QT       += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = email
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    search.cpp \
    setting.cpp \
    compose.cpp \
    account.cpp \
    checkmail.cpp \
    smtp.cpp \
    mailFile.cpp \
    mailContent.cpp \
    attachment.cpp \
    tipdialog.cpp \
    pop3.cpp \
    message.cpp \
    mailheader.cpp

HEADERS  += mainwindow.h \
    search.h \
    setting.h \
    compose.h \
    account.h \
    checkmail.h \
    smtp.h \
    mailContent.h \
    attachment.h \
    mailFile.h \
    tipdialog.h \
    pop3.h \
    message.h \
    mailheader.h

RESOURCES += \
    email.qrc
