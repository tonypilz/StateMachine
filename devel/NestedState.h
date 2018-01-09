#pragma once

#include "StateMachine.h"
#include "GenericState.h"
#include <optional>

/**
 * This class links a state-machine with to a state which allows building hierachical state-machines.
 *
 * Thus, the linked state should be part of another
 * (super-) state-machine and NOT part of the linking state-machine.
 *
 * Entering a NestedState will cause the linked state-machine to go to the initial state.
 *
 * Leaving the NestedState will cause the linked state-machine to enter the inactive state.
 *
 * Events posted to the NestedState will be forwarded to the linked state-machine or
 * consumed by the state itelft if appropriate.
 *
 *
 */

template<typename StateMachineT, typename AllEventsVariant, typename... OtherStates>
struct NestedState : public GenericState<AllEventsVariant, NestedState<StateMachineT, AllEventsVariant, OtherStates...>*, OtherStates...>{

    using Classtype = NestedState<StateMachineT, AllEventsVariant, OtherStates...>;
    using Superclass = GenericState<AllEventsVariant, Classtype*, OtherStates...>;
    using State = typename Superclass::State;
    using OptionalAction = typename Superclass::OptionalAction;

    template<typename InitialState, typename InactiveState>
    NestedState(StateMachineT& mach,InitialState& initialState, InactiveState& inactiveState):Superclass(),machine(mach){

        using FuncEntry = std::function<bool(EntryEvent)>;
        inactiveState.defineTransition(FuncEntry([](EntryEvent) {return true;}),&initialState);

        machine.activeState = &inactiveState; //todo

        auto r = [&inactiveState](auto&& reachableState){
            using FuncExit = std::function<bool(ExitEvent)>;
            reachableState->defineTransition(FuncExit([](ExitEvent) {return true;}),&inactiveState);
        };

        initialState.for_each_reachable_state(r);

    }

    void entry(std::optional<AllEventsVariant> event){ Superclass::entry(event); machine.processEvent(EntryEvent{}); }
    void exit(std::optional<AllEventsVariant> event){ Superclass::exit(event); machine.processEvent(ExitEvent{}); }
    void selfTransition(std::optional<AllEventsVariant> event){ Superclass::selfTransition(event);}

    StateMachineT& machine;

    template<typename Event, typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<Event> event, std::function<void(NewStateVariant)> changeState){

        if(Superclass::makeTransition(event,changeState)==EventProcessingResult::transitionCompleted)
            return EventProcessingResult::transitionCompleted;

        //no next state - forward to sub machine
        return machine.processEvent(event);
    }

    template<typename NewStateVariant>
    EventProcessingResult makeTransition(std::function<void(NewStateVariant)> changeState){
        return Superclass::template makeTransition<NewStateVariant>(changeState);
    }

};


