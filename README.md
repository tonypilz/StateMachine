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
Currently the following features are implemented:
- Unrestricted type of events, eg could be int or std::string
- Unrestricted type of states, custom states need only provide the functions they want to support
- Nesting of state machines
- Run to completion
- Arbitrary guards and actions on transitions
- Arbitrary actions on entry, exit and self transtion
- Lightweight (~400sloc) and therefore easy to reason about
- No direct memory allocations by the library (indirect allocations happend due to using eg std::vector)
- Header Only, with no depencies except for the stl

# Drawbacks
The drawbacks of the approach are:
 - defining state machines with mixed custom- and generic states is rather complicated due to templates
 - defining hierachical state machines is complex due to templates
 - compiler-messages are hard to read due to extensive template usage (could be improved by static_asserts)
 - Raw Pointer to states are beeing used, which cannot be avoided easily

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
