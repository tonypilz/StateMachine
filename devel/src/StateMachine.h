#pragma once

#include "EventProcessingResult.h"
#include "stateMachineHelper.h"


/**
 * A state-machine consists of a set of states which are linked by state-transitions.
 *
 * This class points to the currently active state of the set of states, and it
 * forwards events to the state which might lead to changing the
 * the current stated pointed to.
 *
 */
template<typename State>
struct StateMachine{

    StateMachine(State initialState_) : activeState(initialState_){}
    State activeState; //there is alway a usable state!

    EventProcessingResult processEvent(){

        //run to completion by executing null transitions
        for(int i=0;i<infinitLoopThreshold;++i)
            if (auto r = std::visit([this](auto&& currentStateT) {
                                        return tryCallMakeTransition(
                                            currentStateT,
                                            [this](auto&& nextState){activeState = nextState;},
                                            0); },
                                    activeState);
                    r != EventProcessingResult::transitionCompleted)
                return r; //noting changed so we do not try more null transitions

        throw 42;//infinite loop detected

        return EventProcessingResult::transitionError;
    }

    template<typename Event>
    EventProcessingResult processEvent(std::optional<Event> event){

        //initial event
        if (auto r = std::visit([event,this](auto&& currentStateT) {
                                    return tryCallMakeTransition(
                                        event,
                                        currentStateT,
                                        [this](auto&& nextState){activeState = nextState;},
                                        0); },
                                activeState);
                r != EventProcessingResult::transitionCompleted)
            return r; //nothing changed so we do not try null transitions

        return processEvent();
    }

    template<typename Event>
    EventProcessingResult processEvent(Event event){
        return processEvent(std::optional<Event>{event});
    }

    static constexpr auto infinitLoopThreshold = 100;

};
