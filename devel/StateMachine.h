#pragma once

#include "EventProcessingResult.h"

/**
 * A state-machine consists of a set of states which are linked by state-transitions.
 * This class points to the currently active state of the set of states.
 *
 * That means it forwards events to the state possibly changing the stated pointed to.
 *
 */
template<typename State>
struct StateMachine{

    StateMachine(State initialState_) : activeState(initialState_){}
    State activeState; //there is alway a usable state!

    using SetFunction = std::function<void(State)>;

    template<typename Event, typename ActiveState>
    auto processEventT(std::optional<Event> event, ActiveState currentStateT, int) -> decltype(currentStateT->makeTransition(event,SetFunction{}),EventProcessingResult()) {
        return currentStateT->makeTransition(event,SetFunction{[this](State nextState){activeState = nextState;}});
    }

    template<typename Event, typename ActiveState>
    EventProcessingResult processEventT(std::optional<Event>, ActiveState, long){
        return EventProcessingResult::transitionError;
    }


    template<typename ActiveState>
    auto processEventTX(ActiveState currentStateT, int) -> decltype(currentStateT->makeTransition(SetFunction{}),EventProcessingResult()) {
        return currentStateT->makeTransition(SetFunction{[this](State nextState){activeState = nextState;}});
    }

    template<typename ActiveState>
    EventProcessingResult processEventTX(ActiveState currentStateT, long){
        return EventProcessingResult::transitionError;
    }


    EventProcessingResult processEvent(){

        //null transitions
        for(int i=0;i<infinitLoopThreshold;++i)
            if (auto r = std::visit([this](auto&& currentStateT) { return processEventTX<decltype(currentStateT)>(currentStateT,0); }, activeState);r != EventProcessingResult::transitionCompleted)
                return r; //noting changed so we do not try more null transitions

        throw 42;//infinite loop detected

        return EventProcessingResult::transitionError;
    }

    template<typename Event>
    EventProcessingResult processEvent(std::optional<Event> event){

        //initial event
        if (auto r = std::visit([event,this](auto&& currentStateT) { return processEventT<Event,decltype(currentStateT)>(event,currentStateT,0); }, activeState);r != EventProcessingResult::transitionCompleted)
            return r; //nothing changed so we do not try null transitions

        return processEvent();
    }

    template<typename Event>
    EventProcessingResult processEvent(Event event){
        return processEvent(std::optional<Event>{event});
    }

    static constexpr auto infinitLoopThreshold = 100;

};
