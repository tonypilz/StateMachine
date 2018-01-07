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

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;

    using Trans = std::optional<
                    std::tuple<
                        V,
                        OptionalAction
                              >>;

    Trans nextState(std::optional<int> s){
        if (s.has_value()==false) return std::make_tuple(stateDefault, OptionalAction{});
        if (s.value()==4)         return std::make_tuple(stateOn4, OptionalAction{});
        if (s.value()==5)         return std::make_tuple(stateOn5, OptionalAction{});
        return Trans{};
    }

    V stateOn4;
    V stateOn5;
    V stateDefault;
};
