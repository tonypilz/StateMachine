
#pragma once

#include <vector>
#include <tuple>
#include <functional>
#include <optional>
#include <variant>

//nulltransition bei generic state gibts keine -> die conditions müssen die nulltransistion // any transition nicht möglich da random event,

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

    using Action = std::function<void()>;
    using Actions = std::vector<Action>;

    Actions enterOperations;
    Actions leaveOperations;
    Actions leaveEnterOperations;

    void onEnter(){std::for_each(enterOperations.begin(),enterOperations.end(),[](Action const& action){action();});}
    void onLeave(){std::for_each(leaveOperations.begin(),leaveOperations.end(),[](Action const& action){action();});}
    void onLeaveEnter(){std::for_each(leaveEnterOperations.begin(),leaveEnterOperations.end(),[](Action const& action){action();});}

    template<typename St>
    void defineTransition(Condition cond, St* state) {
        transitions.emplace_back(cond,state);
    }

    template<typename St>
    void defineTransition(St* state) {
        nullTransition = state;
    }

    std::optional<State> nextState() const{ //Event can be nulltransition
        return nullTransition;
    }

    std::optional<State> nextState(Event event) const{ //more than one event possible by inheriting from multiple generic states -> pull nulltransition up to have only one => maybe separeate event and other states!
        auto it = std::find_if(transitions.begin(),transitions.end(), [event](auto&& transition){return std::get<conditionIdx>(transition)(event);}); //todo check for more than one condition true -> throw
        if (it==transitions.end()) return std::optional<State>();
        return std::get<stateIdx>(*it);
    }

    std::vector<Transition> transitions;
    std::optional<State> nullTransition;
};
