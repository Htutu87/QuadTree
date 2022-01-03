TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
#CONFIG -= qt

QT += core gui widgets network positioning xml


SOURCES += main.cpp \
    submodules/RailroadTrack.cpp

HEADERS += \
    src/QuadTree.h \
    submodules/RailroadTrack.h
