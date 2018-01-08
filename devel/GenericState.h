#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <variant>

#include "Transition.h"
#include "EventProcessingResult.h"

template<typename Event, typename Actions>
void executeAll(Event event, Actions const& actions){
    for(auto const& action:actions) action(event);
}





//zuerst die transtions, dann die nulltransitions -> erst alle mit optional(x) dann mit optional (null) durchsuchen
//wir müssen wissen wenn ein event nicht konsumiert werden konnte -> condition für alle events auf false

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



    void entry(OptionalEvent event){executeAll(event, entryActions);}
    void exit(OptionalEvent event){executeAll(event,exitActions);}
    void selfTransition(OptionalEvent event){executeAll(event,selfTransitionActions);}


    template<typename NewStateVariant>
    EventProcessingResult makeTransition(std::function<void(NewStateVariant)> changeState){

        auto it = std::find_if(nullTransitions.begin(),
                               nullTransitions.end(),
                               [changeState,this](auto&& transition){
                                        return transition.apply(this, OptionalEvent{},changeState);}); //todo check for more than one condition true -> throw

        return it!=nullTransitions.end() ? EventProcessingResult::transitionCompleted : EventProcessingResult::eventNotProcessed;

    }

    template<typename Event, typename NewStateVariant>
    EventProcessingResult makeTransition(std::optional<Event> event, std::function<void(NewStateVariant)> changeState){

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
};
