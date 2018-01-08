//nextState soll auch andere Rückgabetypen akzeptieren -> pointer nicht optionals

#include "GenericState.h"
#include "StateA.h"
#include "StateB.h"
#include "StateMachine.h"
#include "NestedState.h"

#include <iostream>


//todo:

// exit mit optional event
// nulltransition mit bei nextstate zurück liefern -> nicht weil man dann nicht von fehler unterscheiden kann

//arbitrary objects can be states without inheritance




// graph doesnt know anything about Machine -> auch wenn ein knoten auch wieder eine machine -> dann weiss die untermaschine nichts von der maschine
// composability (ein bestehendes netz in ein anderes einbauen)

// todo: visitation aller sub maschinen -> darüber auch xml konvertierung

// 4 ampeln parallel

//übergabeparameter enter leave leaveneter

template<typename Event, typename AllEventsVariant>
std::function<bool(AllEventsVariant)> asVariant(std::function<bool(Event)> func){

    return [func](AllEventsVariant events){
        if (std::holds_alternative<Event>(events))
           std::visit([func](auto&& event) { return func(event); }, events);
    };

    //visit

}



int main()
{
    using AllEventsVariant = std::variant<int,const char*>;
    using G = GenericState<AllEventsVariant,StateA*,StateB*>;
    using State = G::State;
    using M = StateMachine<State>;

    StateB state2;
    StateA state1;
    G stateg;

    stateg.entryActions.emplace_back([](std::optional<AllEventsVariant>){std::cout<<"GenericState::entry\n";});
    stateg.exitActions.emplace_back([](std::optional<AllEventsVariant>){std::cout<<"GenericState::exit\n";});
    stateg.selfTransitionActions.emplace_back([](std::optional<AllEventsVariant>){std::cout<<"GenericState::selfTransition\n";});

    using Trans = G::TransitionT;

    using FuncI = std::function<bool(int)>;
    using FuncV = std::function<bool()>;

    stateg.transitions.push_back(Trans(FuncI([](int event) { return event==4;}),&state1));
    stateg.transitions.push_back(Trans(FuncI([](int event) { return event==5;}),&state2));
    stateg.nullTransitions.push_back(Trans(FuncV([]() { return true;}),&stateg));

    state1.stateDefault = &stateg;

    state2.any = &state1;

    M m{&stateg};


    {



        std::cout << "4" << std::endl;
        m.processEvent(4);
        std::cout << "5" << std::endl;
        m.processEvent(5);
        std::cout << "6" << std::endl;
        m.processEvent("6");
    }

//    {
//        using S = NestedState<M, Event>;
//        using M2 = StateMachine<S::State>;
//        S sub(m);

//        M2 m2(&sub);

//        std::cout << "4" << std::endl;
//        m2.processEvent(4);
//        std::cout << "5" << std::endl;
//        m2.processEvent(5);
//        std::cout << "6" << std::endl;
//        m2.processEvent("6");
//    }

}



