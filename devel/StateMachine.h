#pragma once






template<typename Event, typename State>
void callOnLeave(std::optional<Event> event, State state){
    state->exit(event);
}

template<typename Event, typename... States>
void callOnLeave(std::optional<Event> event, std::variant<States...> states){
    std::visit([event](auto&& state) { callOnLeave(event, state); }, states);
}

template<typename Event, typename State>
void callOnLeave(std::optional<Event> event, std::optional<State> state){
    if (state.has_value())  callOnLeave(event, state.value());
}
template<typename Event, typename State>
void callOnEnter(std::optional<Event> event, State state){
    state->entry(event);
}

template<typename Event, typename... States>
void callOnEnter(std::optional<Event> event, std::variant<States...> states){
    std::visit([event](auto&& state) { callOnEnter(event, state); }, states);
}

template<typename Event, typename State>
void callOnEnter(std::optional<Event> event, std::optional<State> state){
    if (state.has_value())  callOnEnter(event, state.value());
}

template<typename Event, typename State>
void callOnLeaveEnter(std::optional<Event> event, State state){
    state->selfTransition(event);
}

template<typename Event, typename... States>
void callOnLeaveEnter(std::optional<Event> event, std::variant<States...> states){
    std::visit([event](auto&& state) { callOnLeaveEnter(event, state); }, states);
}

template<typename Event, typename State>
void callOnLeaveEnter(std::optional<Event> event, std::optional<State> state){
    if (state.has_value())  callOnLeaveEnter(event, state.value());
}




template<typename State>
struct StateMachine{

    StateMachine(State defaultState_) : activeState(defaultState_), defaultState(defaultState_){}
    std::optional<State> activeState;
    State defaultState;

    enum TransitionResult {transitionExecuted, noTransition};

    template<typename Event>
    void clearCurrentState(){
        setCurrentState(std::optional<Event>{}, std::optional<State>{});
    }

    template<typename Event>
    void resetCurrentState() {
        setCurrentState(std::optional<Event>{}, std::optional<State>{defaultState});
    }

    bool isValid() const { return activeState.has_value();}


    template<typename Event, typename CurrentStateT>
    void setCurrentStateN(std::optional<Event> event, CurrentStateT currentStateT, CurrentStateT nextStateT){
        if (currentStateT==nextStateT) {
            currentStateT->selfTransition(event);
        }else {
            currentStateT->exit(event);
            activeState = nextStateT;
            nextStateT->entry(event);
        }
    }

    template<typename Event, typename CurrentStateT, typename NextStateT>
    void setCurrentStateN(std::optional<Event> event, CurrentStateT currentStateT, NextStateT nextStateT){
        currentStateT->exit(event);
        activeState = nextStateT;
        nextStateT->entry(event);
    }

    template<typename Event, typename CurrentStateT, typename... States>
    TransitionResult setCurrentState(std::optional<Event> event, CurrentStateT currentStateT, std::optional<std::variant<States...>> nextState){

        if (nextState.has_value()==false) {
            currentStateT->exit(event);
            activeState = std::optional<State>{};
            return transitionExecuted;
        }

        std::visit([currentStateT,this,event](auto&& nextStateT) { setCurrentStateN(event,currentStateT,nextStateT); }, nextState.value());

        return transitionExecuted;

    }

    template<typename Event, typename... States>
    TransitionResult setCurrentState(std::optional<Event> event, std::optional<std::variant<States...>> nextState){

        if (activeState.has_value()==false) {
            if (nextState.has_value()==false) return noTransition; //noting to do
            activeState = nextState;
            callOnEnter(event,activeState);
            return transitionExecuted;
        }

        return std::visit([this,nextState,event](auto&& currentStateT) { return setCurrentState(event, currentStateT,nextState); }, activeState.value());
    }



    template<typename Event,typename CurrentStateT>
    auto processEventRT(std::optional<Event> event, CurrentStateT currentStateT, int) -> decltype(currentStateT->nextState(event),TransitionResult()){
        return setCurrentState(event, currentStateT, currentStateT->nextState(event)); //todo: nextState must also provide action to be executed on the transition -> rename to getTransition(event);
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
