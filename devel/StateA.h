#pragma once

#include "GenericState.h"
#include <iostream>

struct StateB;



struct StateA {

    template<typename T> void entry(T ){std::cout<<"State1::entry A"<<std::endl;}
    template<typename T> void exit(T ){std::cout<<"State1::exit"<<std::endl;}
    template<typename T> void selfTransition(T){std::cout<<"State1::selfTransition"<<std::endl;}


    using G = GenericState<std::variant<int,const char*>, StateA*,StateB*>;
    using V = std::variant<StateB*,StateA*, G*>;

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;


    template<typename NewStateVariant>
    bool makeTransition(std::optional<int> event, std::function<void(NewStateVariant)> changeState){

        if (event.has_value()==false) { changeState(variantCast<NewStateVariant>(stateDefault)); return true;}
        if (event.value()==4) { changeState(variantCast<NewStateVariant>(stateOn4)); return true;}
        if (event.value()==5) { changeState(variantCast<NewStateVariant>(stateOn5)); return true;}

    }


    V stateOn4;
    V stateOn5;
    V stateDefault;
};
