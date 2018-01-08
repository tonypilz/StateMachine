#pragma once

#include <functional>
#include <optional>
#include <variant>
#include "transitionHelper.h"


/**
 * This class represents a state-transtion between 2 states of a state machine.
 *
 * It consists of a guard-condition, a transition action and the target state.
 *
 * It also performs the actual transition operations.
 */
template<typename AllEventsVariant, typename NextStateVariant>
struct Transition {

    using Classtype = Transition<AllEventsVariant,NextStateVariant>;

    using OptionalState = std::optional<NextStateVariant>;

    using OptionalEvent = std::optional<AllEventsVariant>;

    using Action = std::function<void(OptionalEvent)>;
    using OptionalAction = std::optional<Action>;

    using Guard = std::function<bool(OptionalEvent)>;

    template<typename Event>
    Transition( std::function<bool(Event)> guard_, NextStateVariant nextState_ ):
        guard(generalize<Event,std::optional<AllEventsVariant>> (guard_,false,false)),nextState(nextState_){}


    Transition( std::function<bool()> guard_, NextStateVariant nextState_ ):
        guard(generalize<std::optional<AllEventsVariant>> ([](){return false;},guard_)),nextState(nextState_){}

//    Transition( Guard guard_, NextStateVariant nextState_ ):
//        guard(guard_),nextState(nextState_){}

    template<typename OldState, typename NewState>
    bool apply(OldState oldState, OptionalEvent event, std::function<void(NewState)> changeState){
          if (guard(event)==false) return false;//transition not applicable

        //apply transition

        tryCallExit(oldState, event);

        std::visit([this,changeState](auto&& nextStateT){changeState(nextStateT);},nextState);

        if (action.has_value()) action.value()(event); //transition action

        tryCallEntry(nextState,event);

        return true;
    }

    template<typename OldState, typename Event, typename NewState>
    bool apply(OldState oldState, std::optional<Event> ev, std::function<void(NewState)> changeState){
        return apply(oldState,ev.has_value() ? ev.value() : OptionalEvent{},changeState);
    }

    Guard guard;
    OptionalAction action;
    NextStateVariant nextState;

};
