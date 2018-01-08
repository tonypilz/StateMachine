#pragma once

#include <functional>
#include <optional>
#include <variant>


template<typename NewVariant, typename OldVariant>
NewVariant variantCast(OldVariant old){
    return std::visit([](auto&& oldT){return NewVariant{oldT};},old);
}

template<typename NewVariant, typename OldVariant>
std::optional<NewVariant> optionalVariantCast(std::optional<OldVariant> old){
    return old.has_value() ? variantCast<NewVariant,OldVariant>(old.value()) : std::optional<NewVariant> {};
}





template<typename State, typename Event>
void tryCallExit(State state, Event event){
    state->exit(event);
}

template<typename Event, typename State>
auto tryCallEntryX( State state, Event event, int) -> decltype(state->entry(event),void()){
    state->entry(event);
}
template<typename Event, typename State>
void tryCallEntryX( State state, Event event, long) {
}

template<typename Event, typename... States>
void tryCallEntry( std::variant<States...> states, Event event){
   std::visit([event](auto&& state) { tryCallEntryX(state,event,0); }, states);
}





template<typename Event, typename AllEventsVariantOptional>
std::function<bool(AllEventsVariantOptional)> generalize(std::function<bool(Event)> func){

    return [func](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return false;
        auto e = ev.value();
        if (std::holds_alternative<Event>(e)==false) return false;
        auto event = std::get<Event>(e);
        return func(event);
    };
}

template<typename Event, typename AllEventsVariantOptional>
std::function<void(AllEventsVariantOptional)> generalize(std::function<void(Event)> func){

    return [func](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return;
        auto e = ev.value();
        if (std::holds_alternative<Event>(e)==false) return;
        auto event = std::get<Event>(e);
        func(event);
    };
}

template<typename AllEventsVariantOptional>
std::function<void(AllEventsVariantOptional)> generalize(std::function<void()> func){

    return [func](AllEventsVariantOptional ev){
        if (ev.has_value()) return;
        func();
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

    using Guard = std::function<bool(OptionalEvent)>;

    template<typename Event>
    Transition( std::function<bool(Event)> guard_, NextStateVariant nextState_ ):
        guard(generalize<Event,std::optional<AllEventsVariant>> (guard_)),nextState(nextState_){}


    Transition( std::function<bool()> guard_, NextStateVariant nextState_ ):
        guard(generalize<std::optional<AllEventsVariant>> (guard_)),nextState(nextState_){}

//    Transition( Guard guard_, NextStateVariant nextState_ ):
//        guard(guard_),nextState(nextState_){}



    template<typename OldState, typename Event, typename NewState>
    bool make(OldState oldState, std::optional<Event> ev, std::function<void(NewState)> changeState){

        OptionalEvent event = ev.has_value() ? ev.value() : OptionalEvent{}; //cast to correct type

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