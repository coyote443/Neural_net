TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    net.cpp \
    neuron.cpp \
    teacher.cpp

HEADERS += \
    header.h \
    net.h \
    neuron.h \
    teacher.h
