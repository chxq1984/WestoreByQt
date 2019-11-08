#-------------------------------------------------
#
# Project created by QtCreator 2019-09-13T21:38:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WeShop_Client
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

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    login.cpp \
    registered.cpp \
    smtp.cpp \
    forget_password.cpp \
    personcentral.cpp \
    change_name.cpp \
    globals.cpp \
    command_explain.cpp \
    moneyin.cpp \
    buying_page.cpp \
    tcp.cpp \
    friendlist.cpp \
    addfriend.cpp \
    givecomment.cpp \
    colletion.cpp \
    shoppingcar.cpp \
    showcomments.cpp

HEADERS += \
        mainwindow.h \
    login.h \
    registered.h \
    smtp.h \
    forget_password.h \
    personcentral.h \
    change_name.h \
    globals.h \
    command_explain.h \
    moneyin.h \
    buying_page.h \
    tcp.h \
    friendlist.h \
    addfriend.h \
    givecomment.h \
    colletion.h \
    shoppingcar.h \
    showcomments.h

FORMS += \
        mainwindow.ui \
    login.ui \
    registered.ui \
    forget_password.ui \
    personcentral.ui \
    change_name.ui \
    moneyin.ui \
    buying_page.ui \
    friendlist.ui \
    addfriend.ui \
    givecomment.ui \
    colletion.ui \
    shoppingcar.ui \
    showcomments.ui
QT+=network
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
