#pragma once



template<typename Machine, typename Event, typename... OtherStates>
struct SubMachine : public GenericState<Event, SubMachine<Machine, Event, OtherStates...>*, OtherStates...>{

    using Classtype = SubMachine<Machine, Event, OtherStates...>;
    using Superclass = GenericState<Event, Classtype*, OtherStates...>;
    using State = typename Superclass::State;

    SubMachine(Machine& mach):Superclass(),machine(mach){}

    void onEnter(){ Superclass::onEnter(); machine.resetCurrentState(); } //no onEnter for default state
    void onLeave(){ Superclass::onLeave(); machine.clearCurrentState(); }
    void onLeaveEnter(){ Superclass::onLeaveEnter(); std::cout<<"SubMachine"<<std::endl;}

    Machine& machine;

    std::optional<State> nextState() {

        auto next = Superclass::nextState();
        if(next.has_value())
            return next;

        //no next state - forward to sub machine
        machine.processEvent();

        return machine.isValid() ? this : std::optional<State>();  //propagate errors up
    }

    std::optional<State> nextState(Event event) {

        auto next = Superclass::nextState(event);
        if(next.has_value())
            return next;

        //no next state - forward to sub machine
        machine.processEvent(event);

        return machine.isValid() ? this : std::optional<State>();  //propagate errors up
    }
};
