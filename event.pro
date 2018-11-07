QT += \
    core \
    widgets \

CONFIG += console

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    abstractSerializer.cpp \
    eventrecorder.cpp \
    abstractrecorder.cpp \
    fileserializer.cpp \
    testcontrol.cpp \
    abstractplayer.cpp \
    eventplayer.cpp \
    widgetidcontroller.cpp \
    modemanager.cpp

HEADERS += \
    mainwidget.h \
    abstractplayer.h \
    eventrecorder.h \
    abstractrecorder.h \
    abstractserializer.h \
    fileserializer.h \
    testcontrol.h \
    eventplayer.h \
    eventconstants.h \
    widgetidcontroller.h \
    modemanager.h
