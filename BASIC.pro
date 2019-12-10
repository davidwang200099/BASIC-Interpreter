QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BASIC
TEMPLATE = app

CONFIG += c++11

SOURCES += \
        main.cpp \
        coderunner.cpp \
        evaluate.cpp \
        linecode.cpp \
        namedvariable.cpp \
        MainWindow.cpp \
        Console.cpp \

HEADERS += \
        coderunner.h \
        evaluate.h \
        linecode.h \
        namedvariable.h \
        Stack.h \
        Vector.h \
        MainWindow.h \
        Console.h \
