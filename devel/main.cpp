
#include "GenericState.h"
#include "StateA.h"
#include "StateB.h"
#include "StateMachine.h"
#include "NestedState.h"
#include <iostream>
#include <examples/simple/genericStates/lightSwitch.h>

int main()
{
    main_lightSwitch_minimal_generic();

    return 0;
    using AllEventsVariant = std::variant<int,const char*,ExitEvent,EntryEvent>;
    using G = GenericState<AllEventsVariant,StateA*,StateB*>;
    using State = G::State;
    using M = StateMachine<State>;

    StateB state2;
    StateA state1;
    G stateg;
    G stateInitial;

    using FuncVV = std::function<void()>;
    using FuncVI = std::function<void(int)>;

    stateg.defineEntryAction(FuncVV{[](){std::cout<<"GenericState::entry\n";}});
    stateg.defineExitAction(FuncVI([](int ){std::cout<<"GenericState::exit\n";}));
    stateg.defineSelfTransitionAction(FuncVV([](){std::cout<<"GenericState::selfTransition\n";}));

    using Trans = G::TransitionT;

    using FuncI = std::function<bool(int)>;
    using FuncV = std::function<bool()>;


    stateInitial.defineNullTransition(FuncV([]() { return true;}),&stateg);

    stateg.defineTransition(FuncI([](int event) { return event==4;}),&state1);
    stateg.defineTransition(FuncI([](int event) { return event==5;}),&state2);
    stateg.defineNullTransition(FuncV([]() { return true;}),&stateg);

    state1.stateDefault = &stateg;

    state2.any = &state1;

    M m{&stateInitial};

    if(false)
    {
        std::cout << "start" << std::endl;
        m.processEvent();

        std::cout << "4" << std::endl;
        m.processEvent(4);
        std::cout << "5" << std::endl;
        m.processEvent(5);
        std::cout << "6" << std::endl;
        m.processEvent("6");
    }



    {
        G stateInactive;

        using S = NestedState<M, AllEventsVariant>;
        S sub(m,stateInitial,stateInactive);

        using GG = GenericState<AllEventsVariant,S*>;

        GG initialState;

        initialState.defineNullTransition(FuncV([]() {return true;}),&sub);


        using M2 = StateMachine<std::variant<S*,GG*,GenericState<AllEventsVariant>*>>;
        M2 m2(&initialState);

        std::cout << "xstart" << std::endl;
        m2.processEvent();
        std::cout << "x4" << std::endl;
        m2.processEvent(7);
        std::cout << "x5" << std::endl;
        m2.processEvent(8);
        std::cout << "x6" << std::endl;
        m2.processEvent("9");
    }

}



