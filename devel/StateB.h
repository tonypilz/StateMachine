#pragma once

#include <optional>
#include <iostream>
#include <variant>

struct StateA;

struct StateB {
    void entry(std::optional<int> event){std::cout<<"State2::entry"<<std::endl;}
    void exit(std::optional<int> event){std::cout<<"State2::exit"<<std::endl;}
    void selfTransition(std::optional<int> event){std::cout<<"State2::selfTransition"<<std::endl;}


    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;
    using V = std::variant<StateA*>;


    template<typename NewState>
    bool makeTransition(int event, std::function<void(NewState)> changeState){
        changeState(any);
        return true;
    }
    template<typename NewState>
    bool makeTransition(const char*, std::function<void(NewState)> changeState){
        std::cout<<"nextState(const char*)\n";
        changeState(any);
        return true;
    }


    StateA* any;
};

