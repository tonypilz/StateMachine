#pragma once

#include "GenericState.h"
#include <iostream>
#include "NestedState.h"

struct StateB;

/**
 * @brief The StateA struct is a dummy implementation for proving that custom implementations of GenericState are feasible.
 */

struct StateA {

    template<typename T> void entry(T ){std::cout<<"State1::entry A"<<std::endl;}
    template<typename T> void exit(T ){std::cout<<"State1::exit"<<std::endl;}
    template<typename T> void selfTransition(T){std::cout<<"State1::selfTransition"<<std::endl;}


    using G = GenericState<std::variant<int,const char*,ExitEvent,EntryEvent>, StateA*,StateB*>;
    using V = std::variant<StateB*,StateA*, G*>;

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;


    template<typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<int> event, std::function<void(NewStateVariant)> changeState){

        if (event.has_value()==false) { changeState(variantCast<NewStateVariant>(stateDefault)); return EventProcessingResult::transitionCompleted;}
        if (event.value()==4) { changeState(variantCast<NewStateVariant>(stateOn4)); return EventProcessingResult::transitionCompleted;}
        if (event.value()==5) { changeState(variantCast<NewStateVariant>(stateOn5)); return EventProcessingResult::transitionCompleted;}

        return EventProcessingResult::transitionError;
    }

    template<typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<ExitEvent> event, std::function<void(NewStateVariant)> changeState){

        changeState(stateOnExit);
        return EventProcessingResult::transitionCompleted;

    }

    template<typename Func>
    void for_each_reachable_state(Func func, std::set<void*>& reached){

        void* this_v = static_cast<void*>(this);

        if (reached.find(this_v)!=reached.end()) return;

        reached.insert(static_cast<void*>(this));
        func(this);

    }

    template<typename NextStateVariant>
    void defineTransition( std::function<bool(ExitEvent)>, NextStateVariant nextState_ ){
        stateOnExit = nextState_;
    }


    V stateOn4;
    V stateOn5;
    V stateDefault;
    V stateOnExit;
};
