#pragma once

#include <optional>
#include <iostream>
#include <variant>

#include "EventProcessingResult.h"
#include "NestedState.h"

struct StateA;
/**
 * @brief The StateB struct is a dummy implementation for proving that custom implementations of GenericState are feasible.
 */
struct StateB {


    template<typename T> void entry(T ){std::cout<<"StateB::entry B"<<std::endl;}
    template<typename T> void exit(T ){std::cout<<"StateB::exit"<<std::endl;}
    template<typename T> void selfTransition(T){std::cout<<"StateB::selfTransition"<<std::endl;}

    using OptionalAction = std::optional<std::function<void(std::optional<int>)>>;
    using V = std::variant<StateA*>;


    template<typename NewState>
    EventProcessingResult makeTransition(std::optional<int> event, std::function<void(NewState)> changeState){
        changeState(any);
        return EventProcessingResult::transitionCompleted;
    }
    template<typename NewState>
    EventProcessingResult makeTransition(std::optional<const char*>, std::function<void(NewState)> changeState){
        std::cout<<"nextState(const char*)\n";
        changeState(any);
        return EventProcessingResult::transitionCompleted;
    }

    template<typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<ExitEvent> event, std::function<void(NewStateVariant)> changeState){

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

    }

    StateA* any;
};

