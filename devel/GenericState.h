#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <variant>

#include "Transition.h"

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

    void entry(OptionalEvent event){executeAll(event, entryActions);}
    void exit(OptionalEvent event){executeAll(event,exitActions);}
    void selfTransition(OptionalEvent event){executeAll(event,selfTransitionActions);}


    template<typename Event, typename NewStateVariant>
    bool makeTransition(Event event, std::function<void(NewStateVariant)> changeState){

        auto it = std::find_if(transitions.begin(),
                               transitions.end(),
                               [event,changeState,this](auto&& transition){return transition.apply(this,event,changeState);}); //todo check for more than one condition true -> throw
        return it!=transitions.end();

    }

    template<typename NewStateVariant>
    bool makeTransition(std::function<void(NewStateVariant)> changeState){ //optional für beide fälle

        auto it = std::find_if(nullTransitions.begin(),
                               nullTransitions.end(),
                               [changeState,this](auto&& transition){return transition.apply(this,changeState);}); //todo check for more than one condition true -> throw
        return it!=nullTransitions.end();

    }

    std::vector<TransitionT> transitions;
    std::vector<TransitionT> nullTransitions;
};
