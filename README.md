# StateMachine
A lightweight header only C++17 finite state machine library

# Status
The current implementation is a proof of concept.

# Example

see devel/main.cpp

# Features
The current implementation properties are:
- The type of the events is not restriced (eg int, std::string)
- The type of the states is not restricted, states must only provide the functions they support.
- Hierachical state machines are supported.
- Run to completion is supported.
- Transtions and Null-Transitions can have arbitrary guards and actions.
- Arbitrary actions on entry, exit and self transtion.
- No virtual function calls used.
- Small (~350sloc) and therefore easy to reason about.
- No direct memory allocations (Note that allocation of states and machines is not done by the state machine and must be done by the user).
- Header Only, with no depencies except stl

# Drawbacks
The current implementation has the following problems:
 - defining states is rather complicated due to templates
 - compiler-messages are hard to read due to extensive template usage


# Todo
The following issues are currently open:
 - simplification of thedefinition of states (especially nested states)
 - usability on production scale state machines must be evaluated
 - examples missing
 - features missing, eg historizing, external/internal transitions
 - speed and assembly size must be evaluated
 - documentation
 - compliance of names with https://en.wikipedia.org/wiki/UML_state_machine should be improved
 - consistency of pointers in template arguments should be checked
 - tests need to be added
 - nullptr checks missing
