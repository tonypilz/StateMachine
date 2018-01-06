#pragma once






template<typename State>
void callOnLeave(State state){
    state->exit();
}

template<typename... States>
void callOnLeave(std::variant<States...> states){
    std::visit([](auto&& state) { callOnLeave(state); }, states);
}

template<typename State>
void callOnLeave(std::optional<State> state){
    if (state.has_value())  callOnLeave(state.value());
}
template<typename State>
void callOnEnter(State state){
    state->entry();
}

template<typename... States>
void callOnEnter(std::variant<States...> states){
    std::visit([](auto&& state) { callOnEnter(state); }, states);
}

template<typename State>
void callOnEnter(std::optional<State> state){
    if (state.has_value())  callOnEnter(state.value());
}

template<typename State>
void callOnLeaveEnter(State state){
    state->selfTransition();
}

template<typename... States>
void callOnLeaveEnter(std::variant<States...> states){
    std::visit([](auto&& state) { callOnLeaveEnter(state); }, states);
}

template<typename State>
void callOnLeaveEnter(std::optional<State> state){
    if (state.has_value())  callOnLeaveEnter(state.value());
}




template<typename State>
struct StateMachine{

    StateMachine(State defaultState_) : activeState(defaultState_), defaultState(defaultState_){}
    std::optional<State> activeState;
    State defaultState;

    enum TransitionResult {transitionExecuted, noTransition};


    void clearCurrentState(){
        setCurrentState(std::optional<State>{});
    }

    void resetCurrentState() {
        setCurrentState(std::optional<State>{defaultState});
    }

    bool isValid() const { return activeState.has_value();}


    template<typename CurrentStateT>
    void setCurrentStateN(CurrentStateT currentStateT, CurrentStateT nextStateT){
        if (currentStateT==nextStateT) {
            currentStateT->selfTransition();
        }else {
            currentStateT->exit();
            activeState = nextStateT;
            nextStateT->entry();
        }
    }

    template<typename CurrentStateT, typename NextStateT>
    void setCurrentStateN(CurrentStateT currentStateT, NextStateT nextStateT){
        currentStateT->exit();
        activeState = nextStateT;
        nextStateT->entry();
    }

    template<typename CurrentStateT, typename... States>
    TransitionResult setCurrentState(CurrentStateT currentStateT, std::optional<std::variant<States...>> nextState){

        if (nextState.has_value()==false) {
            currentStateT->exit();
            activeState = std::optional<State>{};
            return transitionExecuted;
        }

        std::visit([currentStateT,this](auto&& nextStateT) { setCurrentStateN(currentStateT,nextStateT); }, nextState.value());

        return transitionExecuted;

    }

    template<typename... States>
    TransitionResult setCurrentState(std::optional<std::variant<States...>> nextState){

        if (activeState.has_value()==false) {
            if (nextState.has_value()==false) return noTransition; //noting to do
            activeState = nextState;
            callOnEnter(activeState);
            return transitionExecuted;
        }

        return std::visit([this,nextState](auto&& currentStateT) { return setCurrentState(currentStateT,nextState); }, activeState.value());
    }



    template<typename Event,typename CurrentStateT>
    auto processEventRT(std::optional<Event> event, CurrentStateT currentStateT, int) -> decltype(currentStateT->nextState(event),TransitionResult()){
        return setCurrentState(currentStateT, currentStateT->nextState(event));
    }
    template<typename Event,typename CurrentStateT>
    TransitionResult processEventRT(std::optional<Event>, CurrentStateT, long){
        std::cout<<"wrong event type\n";
        return noTransition;
    }





    template<typename Event>
    TransitionResult processEventR(std::optional<Event> event){
        return activeState.has_value() ?
                    std::visit([event,this](auto&& currentStateT) { return processEventRT(event,currentStateT, 0); }, activeState.value()) :
                    noTransition; //todo enter new state
    }



    template<typename Event>
    TransitionResult runToCompletion(){

        TransitionResult res = noTransition;

        std::optional<Event> noEvent;

        for(int i=0; i<infiniteLoopThreshold; ++i){ //infinite loop detection
            if (processEventR(noEvent)==noTransition) return res; //return bool if transion was taken
            res = transitionExecuted;
        }
        throw 42;
        return res;
    }

    template<typename Event>
    TransitionResult processEvent(std::optional<Event> event){
        const auto t1 = processEventR(event);
        const auto t2 = runToCompletion<Event>();
        return t1 == transitionExecuted || t2 == transitionExecuted ? transitionExecuted : noTransition;
    }

    template<typename Event>
    TransitionResult processEvent(Event event){
        return processEvent(std::optional<Event>{event});
    }
    template<typename Event>
    TransitionResult processEvent(){
        return processEvent(std::optional<Event>{});
    }


    static constexpr int infiniteLoopThreshold = 1000;
};
