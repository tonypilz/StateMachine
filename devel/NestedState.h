#pragma once

#include "StateMachine.h"
#include "GenericState.h"
#include <optional>


template<typename Machine, typename AllEventsVariant, typename... OtherStates>
struct NestedState : public GenericState<AllEventsVariant, NestedState<Machine, AllEventsVariant, OtherStates...>*, OtherStates...>{

    using Classtype = NestedState<Machine, AllEventsVariant, OtherStates...>;
    using Superclass = GenericState<AllEventsVariant, Classtype*, OtherStates...>;
    using State = typename Superclass::State;
    using OptionalTransition = typename Superclass::OptionalTransition;
    using OptionalAction = typename Superclass::OptionalAction;

    NestedState(Machine& mach):Superclass(),machine(mach){}

    void entry(std::optional<AllEventsVariant> event){ Superclass::entry(event); machine.resetCurrentState(); } //no entry for default state
    void exit(std::optional<AllEventsVariant> event){ Superclass::exit(event); machine.clearCurrentState(); }
    void selfTransition(std::optional<AllEventsVariant> event){ Superclass::selfTransition(event);}

    Machine& machine;

    template<typename Event, typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<Event> event, std::function<void(NewStateVariant)> changeState){

        if(Superclass::makeTransition(event,changeState))
            return true;

        //no next state - forward to sub machine
        return machine.processEvent(event);
    }


};


