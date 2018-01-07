#pragma once

#include "StateMachine.h"
#include "State.h"
#include <optional>


template<typename Machine, typename Event, typename... OtherStates>
struct NestedState : public GenericState<Event, NestedState<Machine, Event, OtherStates...>*, OtherStates...>{

    using Classtype = NestedState<Machine, Event, OtherStates...>;
    using Superclass = GenericState<Event, Classtype*, OtherStates...>;
    using State = typename Superclass::State;

    NestedState(Machine& mach):Superclass(),machine(mach){}

    void entry(std::optional<Event> event){ Superclass::entry(event); machine.template resetCurrentState<Event>(); } //no entry for default state
    void exit(std::optional<Event> event){ Superclass::exit(event); machine.template clearCurrentState<Event>(); }
    void selfTransition(std::optional<Event> event){ Superclass::selfTransition(event);}

    Machine& machine;

    std::optional<State> nextState(std::optional<Event> event) {

        auto next = Superclass::nextState(event);
        if(next.has_value())
            return next;

        //no next state - forward to sub machine
        machine.processEvent(event);

        return machine.isValid() ? this : std::optional<State>();  //propagate errors up
    }
};
