#-------------------------------------------------
#
# Project created by QtCreator 2019-07-28T17:45:34
#
#-------------------------------------------------

QT       += core gui xml 3dcore 3drender 3dinput 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PlatesEditor
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
        abstarctgraphicseffect.cpp \
        basicitem.cpp \
        boundrect.cpp \
        chanels.cpp \
        heigthtonormalgraphicseffect.cpp \
        imagetexture.cpp \
        itemsoverview.cpp \
        lights.cpp \
        main.cpp \
        mainwindow.cpp \
        managers.cpp \
        material.cpp \
        modeleditorwidget.cpp \
        modelitem.cpp \
        nodes.cpp \
        offscreenengine.cpp \
        properties.cpp \
        renderableentity.cpp \
        sceneloader.cpp \
        settingswidget.cpp \
        textureeditorwidget.cpp \
        texturegenerator.cpp \
        toolbar.cpp \
        treemodel.cpp \
        viewwidget.cpp

HEADERS += \
        abstarctgraphicseffect.h \
        basicitem.h \
        boundrect.h \
        chanels.h \
        heigthtonormalgraphicseffect.h \
        imagetexture.h \
        itemsoverview.h \
        lights.h \
        mainwindow.h \
        managers.h \
        material.h \
        modeleditorwidget.h \
        modelitem.h \
        nodes.h \
        offscreenengine.h \
        properties.h \
        renderableentity.h \
        sceneloader.h \
        settingswidget.h \
        textureeditorwidget.h \
        texturegenerator.h \
        toolbar.h \
        treemodel.h \
        viewwidget.h

RC_ICONS = icon2.ico


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
