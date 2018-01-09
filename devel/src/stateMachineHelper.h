#pragma once

#include "EventProcessingResult.h"

template<typename Event, typename ActiveState,typename ChangeStateFunc>
auto tryCallMakeTransition(std::optional<Event> event, ActiveState currentStateT, ChangeStateFunc func, int)  -> decltype(currentStateT->makeTransition(event,func),EventProcessingResult()) {
    return  currentStateT->makeTransition(event,func);
}


template<typename Event, typename ActiveState,typename ChangeStateFunc>
EventProcessingResult tryCallMakeTransition(std::optional<Event>, ActiveState, ChangeStateFunc, long){
    return EventProcessingResult::transitionError;
}


template<typename ActiveState,typename ChangeStateFunc>
auto tryCallMakeTransition(ActiveState currentStateT, ChangeStateFunc func, int) -> decltype(currentStateT->makeTransition(func),EventProcessingResult()) {
    return currentStateT->makeTransition(func);
}


template<typename ActiveState,typename ChangeStateFunc>
EventProcessingResult tryCallMakeTransition(ActiveState, ChangeStateFunc, long){
    return EventProcessingResult::transitionError;
}







template<typename Event, typename State>
auto tryCallExitX( State state, Event event, int) -> decltype(state->exit(event),void()){
    state->exit(event);
}
template<typename Event, typename State>
void tryCallExitX( State state, Event event, long) {
}

template<typename Event, typename... States>
void tryCallExit( std::variant<States...> states, Event event){
   std::visit([event](auto&& state) { tryCallExitX(state,event,0); }, states);
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




template<typename NewVariant, typename... OldStates>
NewVariant variantCast(std::variant<OldStates...> old){
    return std::visit([](auto&& oldT){return NewVariant{oldT};},old);
}

template<typename NewVariant, typename OldType>
NewVariant variantCast(OldType old){
    return NewVariant{old};
}

template<typename NewVariant, typename OldVariant>
std::optional<NewVariant> optionalVariantCast(std::optional<OldVariant> old){
    return old.has_value() ? variantCast<NewVariant,OldVariant>(old.value()) : std::optional<NewVariant> {};
}

