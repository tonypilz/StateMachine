//nextState soll auch andere Rückgabetypen akzeptieren -> pointer nicht optionals

#include "State.h"
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


int main()
{
    using Event = int;
    using G = GenericState<Event,StateA*,StateB*>;
    using State = G::State;
    using M = StateMachine<State>;

    StateB state2;
    StateA state1;
    G stateg;

    stateg.entryActions.emplace_back([](){std::cout<<"GenericState::entry\n";});
    stateg.exitActions.emplace_back([](){std::cout<<"GenericState::exit\n";});
    stateg.selfTransitionActions.emplace_back([](){std::cout<<"GenericState::selfTransition\n";});

    stateg.defineTransition([](int event) { return event==4;}, &state1);
    stateg.defineTransition([](int event) { return event==5;}, &state2);
    stateg.defineTransition([]() { return true;}, &stateg); //todo should be ininite loop

    state1.stateDefault = &stateg;

    state2.any = &state1;

    M m{&stateg};

    using S = NestedState<M, Event>;
    using M2 = StateMachine<S::State>;
    S sub(m);

    M2 m2(&sub);

    std::cout << "4" << std::endl;
    m2.processEvent(4);
    std::cout << "5" << std::endl;
    m2.processEvent(5);
    std::cout << "6" << std::endl;
    m2.processEvent("6");

}



