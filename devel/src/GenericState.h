#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <variant>

#include "Transition.h"
#include "EventProcessingResult.h"
#include <set>

#include "helper.h"

/**
 * This class represetns a single state in a state network.
 *
 * It allows to register actions for entry, exit and reentry of the state.
 *
 * It handles incoming events by excuting all registered transitions, which
 * causes a state change if a registered transition fits the the event.
 *
 */
template<typename AllEventsVariant, typename... OtherStates>
struct GenericState {

    using Classtype = GenericState<AllEventsVariant,OtherStates...>;

    using State = std::variant<OtherStates..., Classtype*>;
    using OptionalState = std::optional<State>;

    using OptionalEvent = std::optional<AllEventsVariant>;

    using Action = std::function<void(OptionalEvent)>;
    using OptionalAction = std::optional<Action>;
    using Actions = std::vector<Action>;

    using TransitionT     = Transition<    AllEventsVariant,State>;

    Actions entryActions;
    Actions exitActions;
    Actions selfTransitionActions;

    void defineEntryAction(          std::function<void(     )> func){         entryActions.emplace_back(generalize<OptionalEvent>(func,func));}

    template<typename Event>
    void defineEntryAction(          std::function<void(Event)> func){         entryActions.emplace_back(generalize<Event,OptionalEvent>(func));}

    void defineExitAction(           std::function<void(     )> func){          exitActions.emplace_back(generalize<OptionalEvent>(func,func));}

    template<typename Event>
    void defineExitAction(           std::function<void(Event)> func){          exitActions.emplace_back(generalize<Event,OptionalEvent>(func));}

    void defineSelfTransitionAction( std::function<void(     )> func){selfTransitionActions.emplace_back(generalize<OptionalEvent>(func,func));}

    template<typename Event>
    void defineSelfTransitionAction( std::function<void(Event)> func){selfTransitionActions.emplace_back(generalize<Event,OptionalEvent>(func));}

    template<typename... Args>
    void defineTransition(Args&&... args){transitions.emplace_back(std::forward<Args>(args)...);}

    template<typename... Args>
    void defineNullTransition(Args&&... args){nullTransitions.emplace_back(std::forward<Args>(args)...);}






    void entry(OptionalEvent event){executeAll(event, entryActions);}
    void exit(OptionalEvent event){executeAll(event,exitActions);}
    void selfTransition(OptionalEvent event){executeAll(event,selfTransitionActions);}


    template<typename ChangeStateFunc>
    EventProcessingResult makeTransition(ChangeStateFunc changeState){

        auto it = std::find_if(nullTransitions.begin(),
                               nullTransitions.end(),
                               [changeState,this](auto&& transition){
                                        return transition.apply(this, OptionalEvent{},changeState);}); //todo check for more than one condition true -> throw

        return it!=nullTransitions.end() ? EventProcessingResult::transitionCompleted : EventProcessingResult::eventNotProcessed;

    }

    template<typename Event, typename ChangeStateFunc>
    EventProcessingResult makeTransition(std::optional<Event> event, ChangeStateFunc changeState){

        if (event.has_value()) {

            auto it = std::find_if(transitions.begin(),
                                   transitions.end(),
                                   [event,changeState,this](auto&& transition){
                                        return transition.apply(this,event,changeState);}); //todo check for more than one condition true -> throw
            return it!=transitions.end() ? EventProcessingResult::transitionCompleted : EventProcessingResult::eventNotProcessed;
        }

        return makeTransition(changeState); //nulltransition
    }

    std::vector<TransitionT> transitions;
    std::vector<TransitionT> nullTransitions;



    //utility functions

    template<typename Func>
    void for_each_reachable_state(Func func){

        std::set<void*> reached;
        for_each_reachable_state(func, reached); //entry
    }

    template<typename Func>
    void for_each_reachable_state(Func func, std::set<void*>& reached){

        void* this_v = static_cast<void*>(this);

        if (reached.find(this_v)!=reached.end()) return;

        reached.insert(static_cast<void*>(this));
        func(this);


        for_each_reachable_state(func,reached, transitions);
        for_each_reachable_state(func,reached, nullTransitions);

    }

    template<typename Func>
    void for_each_reachable_state(Func func, std::set<void*>& reached,std::vector<TransitionT>const& trans){

        for(const auto& t:trans)
            std::visit([func,&reached](auto&& state){
                state->for_each_reachable_state(func,reached);
            },t.nextState);

    }

};
