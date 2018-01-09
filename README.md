# StateMachine
This library implements lightweight header only finite state machine using the c++17 features std::optional and std::variant and NOT inheritance / virtual function calls.

# Goals
The main goals were to evaluate:
 - if a full featured state machine can be implemented by only using std::optional, std::variant and templates and NOT by using inheritance / virtual function calls
 - if the implementation is scalable to real world production size state machines.

The goals were met partially:
 - It can be done
 - The code is small (~400sloc, requires 4 classes wich are loosely coupled but have rather complex templates, see devel/src/*) but not all features are implemeneted yet (namely external transitions, historizing)
 - Scalability remains open to be evaluated.


# Status
The current implementation is a proof of concept.

# Examples

see
 - devel/examples/simple/genericStates/lightSwitch_generic.cpp
 - devel/examples/simple/customStates/lightSwitch_custom.cpp

# Features
The current implementation properties are:
- The type of the events is not restriced, eg types could be int or std::string.
- The type of the states is not restricted, custom states need only provide the functions they want to support.
- Hierachical state machines are supported.
- Run to completion is supported.
- Transtions and Null-Transitions can have arbitrary guards and actions.
- Arbitrary actions on entry, exit and self transtion.
- Lightweight (~350sloc) and therefore easy to reason about.
- No direct memory allocations by library (but indirect by using eg std::vector).
- Header Only, with no depencies except for the stl

# Drawbacks
The current implementation drawbacks are:
 - defining state machines with mixed custom- and generic states is rather complicated due to templates
 - defining hierachical state machines is complex due to templates
 - compiler-messages are hard to read due to extensive template usage


# Todo
The following issues are currently open:
 - needs more testing
 - usability on production scale state machines must be evaluated
 - examples missing
 - features missing, eg historizing, external/internal transitions
 - speed and assembly size must be evaluated
 - documentation
 - compliance of names with https://en.wikipedia.org/wiki/UML_state_machine should be improved
 - consistency of pointers in template arguments should be checked
 - nullptr checks missing
 - add public/private qualifier
