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

    using Trans = std::optional<
                    std::tuple<
                        V,
                        OptionalAction
                              >>;

    Trans nextState(std::optional<int> s){return std::make_tuple(any, OptionalAction{}); }
    Trans nextState(std::optional<const char*>){std::cout<<"nextState(const char*)\n"; return std::make_tuple(any, OptionalAction{}); }


    StateA* any;
};

