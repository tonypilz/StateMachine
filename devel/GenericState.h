#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <optional>
#include <variant>


template<typename State, typename Event>
void tryCallExit(State state, std::optional<Event> event){
    state->exit(event);
}

template<typename Event, typename State>
auto tryCallEntryX( State state, std::optional<Event> event, int) -> decltype(state->entry(event),void()){
    state->entry(event);
}
template<typename Event, typename State>
void tryCallEntryX( State state, std::optional<Event> event, long) {
}

template<typename Event, typename... States>
void tryCallEntry( std::variant<States...> states, std::optional<Event> event){
   std::visit([event](auto&& state) { tryCallEntryX(state,event,0); }, states);
}



template<typename Event, typename Actions>
void executeAll(Event event, Actions const& actions){
    for(auto const& action:actions) action(event);
}

template<typename Event, typename AllEventsVariantOptional>
std::function<bool(AllEventsVariantOptional)> generalize( std::function<bool(Event)> func){

    return [func](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return false;
        auto e = ev.value();
        if (std::holds_alternative<Event>(e)==false) return false;
        auto event = std::get<Event>(e);
        return func(event);
    };
}

template<typename AllEventsVariantOptional>
std::function<bool(AllEventsVariantOptional)> generalize( std::function<bool()> func){

    return [func](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return false;
        return func();
    };
}

template<typename AllEventsVariant, typename NextStateVariant>
struct Transition {

    using Classtype = Transition<AllEventsVariant,NextStateVariant>;

    using OptionalState = std::optional<NextStateVariant>;

    using OptionalEvent = std::optional<AllEventsVariant>;

    using Action = std::function<void(OptionalEvent)>;
    using OptionalAction = std::optional<Action>;

    using Guard = std::function<bool(std::optional<AllEventsVariant>)>;

    template<typename Event>
    Transition( std::function<bool(Event)> guard_, NextStateVariant nextState_ ):
        guard(generalize<Event,std::optional<AllEventsVariant>> (guard_)),nextState(nextState_){}


    Transition( std::function<bool()> guard_, NextStateVariant nextState_ ):
        guard(generalize<std::optional<AllEventsVariant>> (guard_)),nextState(nextState_){}

//    Transition( Guard guard_, NextStateVariant nextState_ ):
//        guard(guard_),nextState(nextState_){}



    template<typename OldState, typename Event, typename NewState>
    bool apply(OldState oldState, std::optional<Event> event, std::function<void(NewState)> changeState){

        if (guard(event)==false) return false;//transition not applicable

        //apply transition

        tryCallExit(oldState, event);

        changeState(nextState);

        if (action.has_value()) action.value()(event); //transition action

        tryCallEntry(nextState,event);

        return true;
    }

    Guard guard;
    OptionalAction action;
    NextStateVariant nextState;

};

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
    bool makeTransition(std::optional<Event> event, std::function<void(NewStateVariant)> changeState){
        {

            auto it = std::find_if(transitions.begin(),
                                   transitions.end(),
                                   [event,changeState,this](auto&& transition){return transition.apply(this,event,changeState);}); //todo check for more than one condition true -> throw
            if(it!=transitions.end()) return true;
        }

        {

            auto it = std::find_if(nullTransitions.begin(),
                                   nullTransitions.end(),
                                   [changeState,this](auto&& transition){return transition.apply(this,std::optional<Event>{},changeState);}); //todo check for more than one condition true -> throw
            return it!=nullTransitions.end();
        }

    }

    template<typename Event, typename NewStateVariant>
    bool makeTransition(Event event, std::function<void(NewStateVariant)> changeState){
        return makeTransition(std::optional<Event>{event},changeState);
    }

    template<typename Event, typename NewStateVariant>
    bool makeTransition(std::function<void(NewStateVariant)> changeState){ //optional für beide fälle
        return makeTransition(std::optional<Event>{},changeState);
    }

    std::vector<TransitionT> transitions;
    std::vector<TransitionT> nullTransitions;
};
