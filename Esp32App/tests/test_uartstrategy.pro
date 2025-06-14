QT += core testlib serialport

CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app
TARGET = test_uartstrategy

SOURCES += test_uartstrategy.cpp \
           ../src/uartstrategy.cpp

HEADERS += ../src/uartstrategy.h
