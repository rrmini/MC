#-------------------------------------------------
#
# Project created by QtCreator 2013-09-25T12:07:19
#
#-------------------------------------------------
QT += widgets sql
#QT += sql

TARGET = MC
TEMPLATE = app


SOURCES +=  main.cpp\
            mainwindow.cpp \
            formdialogs/databaseconnectiondialog.cpp \
    connectionwidget.cpp \
    widget.cpp

HEADERS  += mainwindow.h \
            formdialogs/databaseconnectiondialog.h \
    connectionwidget.h \
    widget.h

TRANSLATIONS = translations/mainwindow_ru.ts \
                translations/mainwindow_ru.qm

RESOURCES += mc.qrc
