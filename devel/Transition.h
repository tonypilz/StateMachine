#pragma once



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
