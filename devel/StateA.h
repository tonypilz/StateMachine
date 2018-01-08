#pragma once

#include "GenericState.h"
#include <iostream>

struct StateB;

template<typename NewVariant, typename OldVariant>
NewVariant variantCast(OldVariant old){
    return std::visit([](auto&& oldT){return NewVariant{oldT};},old);
}


struct StateA {
    void entry(std::optional<int> event){std::cout<<"State1::entry"<<std::endl;}

    void exit(std::optional<int> event){std::cout<<"State1::exit"<<std::endl;}
    void selfTransition(std::optional<int> event){std::cout<<"State1::selfTransition"<<std::endl;}


    using G = GenericState<std::variant<int,const char*>, StateA*,StateB*>;
    using V = std::variant<StateB*,StateA*, G*>;

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;


    template<typename NewStateVariant>
    bool makeTransition(int event, std::function<void(NewStateVariant)> changeState){

        if (event==4) { changeState(variantCast<NewStateVariant>(stateOn4)); return true;}
        if (event==5) { changeState(variantCast<NewStateVariant>(stateOn5)); return true;}

        changeState(variantCast<NewStateVariant>(stateDefault));
        return true;

    }

    V stateOn4;
    V stateOn5;
    V stateDefault;
};
