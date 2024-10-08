QT       += core gui multimedia 

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mysimplealarm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    timer.cpp \
    alarm.cpp \
    fileio.cpp \
    schedule.cpp \
    schedulecollection.cpp \
    snooze.cpp 

HEADERS  += mainwindow.h \
    timer.h \
    alarm.h \
    fileio.h \
    schedule.h \
    schedulecollection.h \
    snooze.h \

FORMS    += mainwindow.ui \
    snooze.ui
CONFIG += precompile_header
