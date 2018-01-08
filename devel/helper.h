#pragma once


struct EntryEvent{};
struct ExitEvent{};


template<typename Event, typename Actions>
void executeAll(Event event, Actions const& actions){
    for(auto const& action:actions) action(event);
}

