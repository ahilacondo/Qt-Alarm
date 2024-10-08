QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-alarm
TEMPLATE = app

# Rutas a las carpetas
SOURCES += sources/main.cpp \
           sources/mainwindow.cpp \
           sources/timer.cpp \
           sources/alarm.cpp \
           sources/fileio.cpp \
           sources/schedule.cpp \
           sources/schedulecollection.cpp \
           sources/snooze.cpp

HEADERS  += headers/mainwindow.h \
            headers/timer.h \
            headers/alarm.h \
            headers/fileio.h \
            headers/schedule.h \
            headers/schedulecollection.h \
            headers/snooze.h

FORMS    += forms/mainwindow.ui \
            forms/snooze.ui

INCLUDEPATH += headers
CONFIG += precompile_header
