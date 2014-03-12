#-------------------------------------------------
#
# Project created by QtCreator 2014-02-25T19:47:55
#
#-------------------------------------------------

QT       += core widgets qml

QT       -= gui

TARGET = 2safe
CONFIG += app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    trayicon.cpp \
    safeclient.cpp

HEADERS += \
    trayicon.h \
    safeclient.h \
    safecommon.h

folder_01.source = qml
DEPLOYMENTFOLDERS = folder_01

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc
