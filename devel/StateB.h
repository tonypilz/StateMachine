#pragma once

#include <optional>
#include <iostream>
#include <variant>

struct StateA;

struct StateB {
    void entry(){std::cout<<"State2::entry"<<std::endl;}
    void exit(){std::cout<<"State2::exit"<<std::endl;}
    void selfTransition(){std::cout<<"State2::selfTransition"<<std::endl;}

    std::optional<std::variant<StateA*>> nextState(std::optional<int> event){return any;}//any event cond
    std::optional<std::variant<StateA*>> nextState(std::optional<const char*>){std::cout<<"nextState(const char*)\n";return any;}//any event cond


    StateA* any;
};

