//nextState soll auch andere Rückgabetypen akzeptieren -> pointer nicht optionals

#include "State.h"
#include "StateA.h"
#include "StateB.h"
#include "StateMachine.h"
#include "StateMachineState.h"


#include <iostream>



// einen renderer für die state machine ->  xml -> graph lib, evtl einen generator xml -> code ähnlich protobuf

// graph doesnt know anything about Machine -> auch wenn ein knoten auch wieder eine machine -> dann weiss die untermaschine nichts von der maschine
// composability (ein bestehendes netz in ein anderes einbauen)

// todo: visitation aller sub maschinen -> darüber auch xml konvertierung

// 4 ampeln parallel

//übergabeparameter enter leave leaveneter


int main()
{
    using Event = int;
    using G = GenericState<Event,State1*,State2*>;
    using State = G::State;
    using M = Machine<State>;

    State2 state2;
    State1 state1;
    G stateg;

    stateg.defineTransition([](int event) { return event==4;}, &state1);
    stateg.defineTransition([](int event) { return event==5;}, &state2);
    stateg.defineTransition(&stateg); //todo should be ininite loop

    state1.stateDefault = &stateg;

    state2.any = &state1;

    M m{&stateg};

    using S = SubMachine<M, Event>;
    using M2 = Machine<S::State>;
    S sub(m);

    M2 m2(&sub);

    std::cout << "4" << std::endl;
    m2.processEvent(4);
    std::cout << "5" << std::endl;
    m2.processEvent(5);
    std::cout << "6" << std::endl;
    m2.processEvent("6");

}



