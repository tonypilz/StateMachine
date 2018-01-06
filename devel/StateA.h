#pragma once

#include "State.h"
#include <iostream>

struct State2;

struct State1 {
    void onEnter(){std::cout<<"State1::onEnter"<<std::endl;}
    void onLeave(){std::cout<<"State1::onLeave"<<std::endl;}
    void onLeaveEnter(){std::cout<<"State1::onLeaveEnter"<<std::endl;}

    using G = GenericState<int, State1*,State2*>;
    using V = std::variant<State2*,State1*, G*>;

    std::optional<V> nextState(int s){
        if (s==4) return stateOn4;
        if (s==5) return stateOn5;
        return V{};
    }

    std::optional<V> nextState(){return stateDefault;}

    V stateOn4;
    V stateOn5;
    V stateDefault;
};
