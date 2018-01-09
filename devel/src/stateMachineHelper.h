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

