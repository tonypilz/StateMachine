#pragma once

#include "EventProcessingResult.h"

template<typename State>
struct StateMachine{

    StateMachine(State initialState_) : activeState(initialState_), initialState(initialState_){}
    std::optional<State> activeState;
    State initialState;



    void clearCurrentState(){
//        setCurrentState(std::optional<Event>{},  std::optional<
//                        std::tuple<
//                         State,
//                         std::optional<std::function<void(std::optional<Event>)>>
//                                  >> {});
    }


    void resetCurrentState() {
//        setCurrentState(std::optional<Event>{},
//                        std::optional<
//                                               std::tuple<
//                                                State,
//                                                std::optional<std::function<void(std::optional<Event>)>>
//                                                         >> {std::make_tuple(defaultState,std::optional<std::function<void(std::optional<Event>)>>{})});
    }

    bool isValid() const { return activeState.has_value();}

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
    EventProcessingResult processEventTX(ActiveState, long){
        return EventProcessingResult::transitionError;
    }


    EventProcessingResult processEvent(){

        //null transitions
        for(int i=0;i<infinitLoopThreshold;++i)
            if (activeState.has_value()==false ||
                    std::visit([this](auto&& currentStateT) { return processEventTX<decltype(currentStateT)>(currentStateT,0); }, activeState.value()) != EventProcessingResult::transitionCompleted)
                return EventProcessingResult::eventNotProcessed; //noting changed so we do not try more null transitions

        throw 42;//infinite loop detected

        return EventProcessingResult::transitionError;
    }

    template<typename Event>
    EventProcessingResult processEvent(std::optional<Event> event){

        //initial event
        if (activeState.has_value()==false ||
                std::visit([event,this](auto&& currentStateT) { return processEventT<Event,decltype(currentStateT)>(event,currentStateT,0); }, activeState.value()) != EventProcessingResult::transitionCompleted)
            return EventProcessingResult::eventNotProcessed; //nothing changed so we do not try null transitions

        return processEvent();
    }

    template<typename Event>
    EventProcessingResult processEvent(Event event){
        return processEvent(std::optional<Event>{event});
    }

    static constexpr auto infinitLoopThreshold = 100;

};
