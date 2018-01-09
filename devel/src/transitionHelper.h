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
std::function<bool(AllEventsVariantOptional)> generalize(std::function<bool(Event)> func, bool noArgReturnValue, bool unexpectedArgTypeReturnValue){

    if (!func) return std::function<bool(AllEventsVariantOptional)>{};

    return [=](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return noArgReturnValue;
        auto e = ev.value();
        if (std::holds_alternative<Event>(e)==false) return unexpectedArgTypeReturnValue;
        auto event = std::get<Event>(e);
        return func(event);
    };
}

template<typename Event, typename AllEventsVariantOptional>
std::function<void(AllEventsVariantOptional)> generalize(std::function<void(Event)> func){

    if (!func) return std::function<void(AllEventsVariantOptional)>{};

    return [=](AllEventsVariantOptional ev){
        if (ev.has_value()==false) return;
        auto e = ev.value();
        if (std::holds_alternative<Event>(e)==false) return;
        auto event = std::get<Event>(e);
        func(event);
    };
}

template<typename AllEventsVariantOptional>
std::function<void(AllEventsVariantOptional)> generalize(std::function<void()> func, std::function<void()> funcNoValue){

    if (!func || !funcNoValue) return std::function<void(AllEventsVariantOptional)>{};

    return [=](AllEventsVariantOptional ev){
        if (ev.has_value()) func(); else funcNoValue();
    };

}

template<typename AllEventsVariantOptional>
std::function<bool(AllEventsVariantOptional)> generalize( std::function<bool()> func, std::function<bool()> funcNoValue){

    if (!func || !funcNoValue) return std::function<bool(AllEventsVariantOptional)>{};

    return [=](AllEventsVariantOptional ev){
        return ev.has_value() ? func() : funcNoValue();
    };
}
