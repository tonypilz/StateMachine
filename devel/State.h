
#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <optional>
#include <variant>

//nulltransition bei generic state gibts keine -> die conditions müssen die nulltransistion // any transition nicht möglich da random event,
template<typename Event, typename Actions>
void executeAll(std::optional<Event> event, Actions const& actions){
    for(auto const& action:actions) action(event);
}

template<typename Event, typename... OtherStates>
struct GenericState {

    using Classtype = GenericState<Event,OtherStates...>;

    using State = std::variant<OtherStates..., Classtype*>;

    static constexpr int conditionIdx = 0;
    static constexpr int stateIdx = 1;

    using Condition     = std::function<bool(Event)>;
    using NullCondition = std::function<bool()>;

    using Transition     = std::tuple<    Condition,State>;
    using NullTransition = std::tuple<NullCondition,State>;

    using Action = std::function<void(std::optional<Event>)>;
    using Actions = std::vector<Action>;

    Actions entryActions;
    Actions exitActions;
    Actions selfTransitionActions;


    void entry(std::optional<Event> event){executeAll(event, entryActions);}
    void exit(std::optional<Event> event){executeAll(event,exitActions);}
    void selfTransition(std::optional<Event> event){executeAll(event,selfTransitionActions);}

    template<typename St>
    void defineTransition(Condition cond, St* state) {
        transitions.emplace_back(cond,state);
    }

    template<typename St>
    void defineTransition(NullCondition cond, St* state) {
        nullTransition.emplace_back(cond,state);
    }

    std::optional<State> nextState_() const{ //Event can be nulltransition
        auto it = std::find_if(nullTransition.begin(),nullTransition.end(), [](auto&& transition){return std::get<conditionIdx>(transition)();}); //todo check for more than one condition true -> throw
        if (it==nullTransition.end()) return std::optional<State>();
        return std::get<stateIdx>(*it);
    }

    std::optional<State> nextState_(Event event) const{ //more than one event possible by inheriting from multiple generic states -> pull nulltransition up to have only one => maybe separeate event and other states!
        auto it = std::find_if(transitions.begin(),transitions.end(), [event](auto&& transition){return std::get<conditionIdx>(transition)(event);}); //todo check for more than one condition true -> throw
        if (it==transitions.end()) return std::optional<State>();
        return std::get<stateIdx>(*it);
    }

    std::optional<State> nextState(std::optional<Event> event) const{ //more than one event possible by inheriting from multiple generic states -> pull nulltransition up to have only one => maybe separeate event and other states!
        return event.has_value() ? nextState_(event.value()) : nextState_();
    }

    std::vector<Transition> transitions;
    std::vector<NullTransition> nullTransition;
};
