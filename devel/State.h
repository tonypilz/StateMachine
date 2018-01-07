
#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <optional>
#include <variant>

//nulltransition bei generic state gibts keine -> die conditions müssen die nulltransistion // any transition nicht möglich da random event,
template<typename Event, typename Actions>
void executeAll(Event event, Actions const& actions){
    for(auto const& action:actions) action(event);
}

template<typename Event, typename... OtherStates>
struct GenericState {

    using Classtype = GenericState<Event,OtherStates...>;

    using State = std::variant<OtherStates..., Classtype*>;
    using OptionalState = std::optional<State>;

    using OptionalEvent = std::optional<Event>;

    using Action = std::function<void(OptionalEvent)>;
    using OptionalAction = std::optional<Action>;
    using Actions = std::vector<Action>;


    using Condition     = std::function<bool(Event)>;
    using NullCondition = std::function<bool()>;

    using Transition     = std::tuple<    Condition,State,OptionalAction>;
    using NullTransition = std::tuple<NullCondition,State,OptionalAction>;


    Actions entryActions;
    Actions exitActions;
    Actions selfTransitionActions;

    void entry(OptionalEvent event){executeAll(event, entryActions);}
    void exit(OptionalEvent event){executeAll(event,exitActions);}
    void selfTransition(OptionalEvent event){executeAll(event,selfTransitionActions);}

    template<typename St>
    void defineTransition(Condition cond, St* state, OptionalAction action = OptionalAction{}) {
        transitions.emplace_back(cond,state, action);
    }

    template<typename St>
    void defineTransition(NullCondition cond, St* state, OptionalAction action = OptionalAction{}) {
        nullTransition.emplace_back(cond,state,action);
    }

    using OptionalTransition = std::optional<std::tuple<State, OptionalAction>>;

    OptionalTransition nextState_() const{ //Event can be nulltransition
        auto it = std::find_if(nullTransition.begin(),nullTransition.end(), [](auto&& transition){return std::get<NullCondition>(transition)();}); //todo check for more than one condition true -> throw
        if (it==nullTransition.end()) return {};
        return OptionalTransition{std::make_tuple(std::get<State>(*it), std::get<OptionalAction>(*it))};
    }

    OptionalTransition nextState_(Event event) const{ //more than one event possible by inheriting from multiple generic states -> pull nulltransition up to have only one => maybe separeate event and other states!
        auto it = std::find_if(transitions.begin(),transitions.end(), [event](auto&& transition){return std::get<Condition>(transition)(event);}); //todo check for more than one condition true -> throw
        if (it==transitions.end()) return {};
        return OptionalTransition{std::make_tuple(std::get<State>(*it), std::get<OptionalAction>(*it))};
    }

    OptionalTransition nextState(OptionalEvent event) const{ //more than one event possible by inheriting from multiple generic states -> pull nulltransition up to have only one => maybe separeate event and other states!
        return event.has_value() ? nextState_(event.value()) : nextState_();
    }

    std::vector<Transition> transitions;
    std::vector<NullTransition> nullTransition;
};
