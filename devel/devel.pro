TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17


SOURCES += main.cpp


HEADERS += \
    State.h \
    StateA.h \
    StateB.h \
    StateMachine.h \
    NestedState.h





