#pragma once


struct State2 {
    void onEnter(){std::cout<<"State2::onEnter"<<std::endl;}
    void onLeave(){std::cout<<"State2::onLeave"<<std::endl;}
    void onLeaveEnter(){std::cout<<"State2::onLeaveEnter"<<std::endl;}

    std::optional<std::variant<State1*>> nextState(int event){return any;}//any event cond
    std::optional<std::variant<State1*>> nextState(const char*){std::cout<<"nextState(const char*)\n";return any;}//any event cond

    std::optional<std::variant<State1*>> nextState(){return any;}

    State1* any;
};

