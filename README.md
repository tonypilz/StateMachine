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
- no virtual function calls used.

Note that allocation of states is not done by the state machine and must be done by the user.

# Drawbacks
The current implementation has the following problems:
 - defining states is rather complicated due to templates
 - compiler-messages are hard to read due to extensive template usage


# Todo
The following issues are currently open:
 - simplify the definition if possible
 - verify usability on production scale state machines
 - add examples
 - add missing features eg historizing
 - check speed and assembly size
 - some function calls not optimizied
 - add documentation
 - improve naming compliance with https://en.wikipedia.org/wiki/UML_state_machine
 - improve consistency of pointers template arguments
