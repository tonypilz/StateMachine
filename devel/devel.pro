TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17


include($$PWD/src/src.pri)
include($$PWD/testing/testing.pri)
include($$PWD/examples/examples.pri)


SOURCES += main.cpp

HEADERS += \


DISTFILES += \
    ../README.md





