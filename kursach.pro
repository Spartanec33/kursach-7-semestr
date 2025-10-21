QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += model
INCLUDEPATH += view
INCLUDEPATH += controller


SOURCES += \
    controller/graphcontroller.cpp \
    main.cpp \
    mainwindow.cpp \
    model/edge.cpp \
    model/graph.cpp \
    model/node.cpp \
    view/edgeform.cpp \
    view/graphview.cpp \
    view/nodeform.cpp

HEADERS += \
    controller/graphcontroller.h \
    mainwindow.h \
    model/edge.h \
    model/graph.h \
    model/node.h \
    view/edgeform.h \
    view/graphview.h \
    view/nodeform.h \
    view/stylesheet.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
