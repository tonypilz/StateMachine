#pragma once

#include "State.h"
#include <iostream>

struct StateB;

struct StateA {
    void entry(std::optional<int> event){std::cout<<"State1::entry"<<std::endl;}
    void exit(std::optional<int> event){std::cout<<"State1::exit"<<std::endl;}
    void selfTransition(std::optional<int> event){std::cout<<"State1::selfTransition"<<std::endl;}

    using G = GenericState<int, StateA*,StateB*>;
    using V = std::variant<StateB*,StateA*, G*>;

    std::optional<V> nextState(std::optional<int> s){
        if (s.has_value()==false) return stateDefault;
        if (s.value()==4) return stateOn4;
        if (s.value()==5) return stateOn5;
        return V{};
    }

    V stateOn4;
    V stateOn5;
    V stateDefault;
};
