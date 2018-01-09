TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17


SOURCES += main.cpp \
    examples/simple/genericStates/lightSwitch.cpp


HEADERS += \
    StateA.h \
    StateB.h \
    StateMachine.h \
    NestedState.h \
    GenericState.h \
    Transition.h \
    EventProcessingResult.h \
    transitionHelper.h \
    helper.h \
    examples/simple/genericStates/lightSwitch.h

DISTFILES += \
    ../README.md





