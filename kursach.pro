QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += model
INCLUDEPATH += view
INCLUDEPATH += view\ui
INCLUDEPATH += controller

RC_ICONS = appIcon.ico

SOURCES += \
    controller/graphcontroller.cpp \
    main.cpp \
    view/ui/mainwindow.cpp \
    model/edge.cpp \
    model/graph.cpp \
    model/node.cpp \
    view/ui/edgeform.cpp \
    view/graphview.cpp \
    view/ui/nodeform.cpp \
    view/ui/sidebarpanel.cpp \
    view/ui/tipslabel.cpp

HEADERS += \
    controller/graphcontroller.h \
    view/ui/mainwindow.h \
    model/edge.h \
    model/graph.h \
    model/node.h \
    view/ui/config.h \
    view/ui/edgeform.h \
    view/graphview.h \
    view/ui/nodeform.h \
    view/ui/sidebarpanel.h \
    view/ui/tipslabel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
