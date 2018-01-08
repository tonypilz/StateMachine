#pragma once

template<typename State>
struct StateMachine{

    StateMachine(State initialState_) : activeState(initialState_), initialState(initialState_){}
    std::optional<State> activeState;
    State initialState;

    enum TransitionResult {transitionCompleted, noTransition};

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

    template<typename ActiveState>
    auto processEventTX(ActiveState currentStateT, int) -> decltype(currentStateT->makeTransition(SetFunction{}),bool()) {
        return currentStateT->makeTransition(SetFunction{[this](State nextState){activeState = nextState;}});
    }

    template<typename ActiveState>
    bool processEventTX(ActiveState, long){
        return false;
    }

    template<typename Event>
    TransitionResult processEvent(Event event){

        //initial event
        if (activeState.has_value()==false ||
                std::visit([event,this](auto&& currentStateT) { return processEventT<Event,decltype(currentStateT)>(event,currentStateT,0); }, activeState.value()) == false)
            return noTransition;

        //null transitions
        for(int i=0;i<infinitLoopThreshold;++i)
            if (activeState.has_value()==false ||
                    std::visit([this](auto&& currentStateT) { return processEventTX<decltype(currentStateT)>(currentStateT,0); }, activeState.value()) == false)
                return transitionCompleted;

        throw 42;//infinite loop detected

        return transitionCompleted;
    }

    static constexpr auto infinitLoopThreshold = 100;

};
