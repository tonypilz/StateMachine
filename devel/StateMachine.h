#pragma once

template<typename State>
struct StateMachine{

    StateMachine(State initialState_) : activeState(initialState_), initialState(initialState_){}
    std::optional<State> activeState;
    State initialState;

    enum TransitionResult {transitionExecuted, noTransition};

    template<typename Event>
    void clearCurrentState(){
//        setCurrentState(std::optional<Event>{},  std::optional<
//                        std::tuple<
//                         State,
//                         std::optional<std::function<void(std::optional<Event>)>>
//                                  >> {});
    }

    template<typename Event>
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
    auto processEventT(Event event, ActiveState currentStateT, int) -> decltype(currentStateT->makeTransition(event,SetFunction{}),bool()) {
        return currentStateT->makeTransition(event,SetFunction{[this](State nextState){activeState = nextState;}});
    }

    template<typename Event, typename ActiveState>
    bool processEventT(Event, ActiveState, long){
        return false;
    }
    template<typename Event>
    TransitionResult processEvent(Event event){
        return activeState.has_value() &&
                std::visit([event,this](auto&& currentStateT) { return processEventT<Event,decltype(currentStateT)>(event,currentStateT,0); }, activeState.value()) ?
                    transitionExecuted :
                    noTransition;
    }

};
