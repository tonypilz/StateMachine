# StateMachine
This library implements lightweight header only finite state machine NOT using the c++17 features std::optional and std::variant and NOT inheritance / virtual function calls.

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

# Example

see devel/examples/simple/genericStates/lightSwitch.cpp

# Features
The current implementation properties are:
- The type of the events is not restriced (eg int, std::string)
- The type of the states is not restricted, states must only provide the functions they support.
- Hierachical state machines are supported.
- Run to completion is supported.
- Transtions and Null-Transitions can have arbitrary guards and actions.
- Arbitrary actions on entry, exit and self transtion.
- Small (~350sloc) and therefore easy to reason about.
- No direct memory allocations (Note that allocation of states and machines is not done by the state machine and must be done by the user).
- Header Only, with no depencies except stl

# Drawbacks
The current implementation has the following problems:
 - defining states is rather complicated due to templates
 - compiler-messages are hard to read due to extensive template usage


# Todo
The following issues are currently open:
 - usability on production scale state machines must be evaluated
 - examples missing
 - features missing, eg historizing, external/internal transitions
 - speed and assembly size must be evaluated
 - documentation
 - compliance of names with https://en.wikipedia.org/wiki/UML_state_machine should be improved
 - consistency of pointers in template arguments should be checked
 - tests need to be added
 - nullptr checks missing
