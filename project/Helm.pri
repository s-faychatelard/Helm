#-------------------------------------------------
#
# Project created by QtCreator 2012-10-10T10:24:23
#
#-------------------------------------------------

QT          += core widgets network webkit webkitwidgets xml

TARGET      = Helm
TEMPLATE    = app

QMAKE += -j3

DESTDIR = ./

INCLUDEPATH += ../../includes/others/mac/
INCLUDEPATH     += /System/Library/Frameworks/AppKit.framework/Headers
INCLUDEPATH     += /System/Library/Frameworks/IOKit.framework/Headers

HEADERS     +=  ../../includes/application.h \
                ../../includes/centralwidget.h \
                ../../includes/control.h \
                ../../includes/fullscreenbutton.h \
                ../../includes/home.h \
                ../../includes/mainwindow.h \
                ../../includes/scrollbar.h \
                ../../includes/searchview.h \
                ../../includes/shareview.h \
                ../../includes/topbar.h \
                ../../includes/urlfield.h \
                ../../includes/webcontainer.h \
                ../../includes/webview.h \
                ../../includes/webviewcookie.h \
                ../../includes/windowbutton.h

SOURCES     +=  ../../sources/application.cpp \
                ../../sources/centralwidget.cpp \
                ../../sources/control.cpp \
                ../../sources/fullscreenbutton.cpp \
                ../../sources/home.cpp \
                ../../sources/main.cpp \
                ../../sources/mainwindow.cpp \
                ../../sources/scrollbar.cpp \
                ../../sources/searchview.cpp \
                ../../sources/shareview.cpp \
                ../../sources/topbar.cpp \
                ../../sources/urlfield.cpp \
                ../../sources/webcontainer.cpp \
                ../../sources/webview.cpp \
                ../../sources/webviewcookie.cpp \
                ../../sources/windowbutton.cpp

RESOURCES += ../../resources/resources.qrc

mac {
    OBJECTS_DIR = ../buildOSX/
    MOC_DIR = ../buildOSX/
    UI_DIR = ../buildOSX/

    ICON = icon.icns


    HEADERS               += ../../includes/others/mac/mac.h

    OBJECTIVE_SOURCES     += ../../sources/others/mac/mac.mm

    LIBS                  += -framework AppKit \
                             -framework IOKit
}

unix:!mac {
    OBJECTS_DIR = ../buildUnix/
    MOC_DIR = ../buildUnix/
    UI_DIR = ../buildUnix/
}

win32 {

    QMAKE_LFLAGS += -static-libstdc++
    OBJECTS_DIR = ../buildWin32/
    MOC_DIR = ../buildWin32/
    UI_DIR = ../buildWin32/

    HEADERS     += ../../includes/others/windows/windows.h

    SOURCES     += ../../sources/others/windows/windows.cpp
}
