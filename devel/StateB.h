#pragma once

#include <optional>
#include <iostream>
#include <variant>

struct StateA;

struct StateB {


    template<typename T> void entry(T ){std::cout<<"StateB::entry B"<<std::endl;}
    template<typename T> void exit(T ){std::cout<<"StateB::exit"<<std::endl;}
    template<typename T> void selfTransition(T){std::cout<<"StateB::selfTransition"<<std::endl;}

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;
    using V = std::variant<StateA*>;


    template<typename NewState>
    bool makeTransition(std::optional<int> event, std::function<void(NewState)> changeState){
        changeState(any);
        return true;
    }
    template<typename NewState>
    bool makeTransition(std::optional<const char*>, std::function<void(NewState)> changeState){
        std::cout<<"nextState(const char*)\n";
        changeState(any);
        return true;
    }


    StateA* any;
};

