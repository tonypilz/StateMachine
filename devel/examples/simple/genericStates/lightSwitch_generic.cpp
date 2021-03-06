#include <src/GenericState.h>
#include <src/StateMachine.h>
#include <iostream>


int main_lightSwitch_minimal_generic()
{

    //
    // +----+    bool   +-----+
    // |    | --------->|     |
    // | on |           | off |
    // |    | <---------|     |
    // +----+    bool   +-----+
    //


    //define types
    using SwitchEventType = bool;
    using SwitchEvents = std::variant<SwitchEventType>;
    using SwitchState = GenericState<SwitchEvents>;
    using SwitchStates = std::variant<GenericState<SwitchEvents>*>;
    using StateMachine = StateMachine<SwitchStates>;

    //create states
    SwitchState stateOn;
    SwitchState stateOff;

    //create actions
    using Action = std::function<void()>;
    stateOn.defineEntryAction(Action{[](){std::cout<<"Light switchted on \n";}});
    stateOff.defineEntryAction(Action{[](){std::cout<<"Light switchted off \n";}});

    //create transitions
    using Guard = std::function<bool(SwitchEventType)>;
    stateOn.defineTransition(Guard{[](SwitchEventType) { return true;}},&stateOff);
    stateOff.defineTransition(Guard{[](SwitchEventType) { return true;}},&stateOn);

    //create machine
    StateMachine machine{&stateOff};


    //start interaction

    SwitchEventType e; //the value is not evaluated by the guards so it doesnt matter here

    for(int i=0;i<3;++i) {
        std::cout << "pressing button " << i << " \n";
        machine.processEvent(e);
    }

//    prints:

//    pressing button 0
//    Light switchted on
//    pressing button 1
//    Light switchted off
//    pressing button 2
//    Light switchted on

    return 0;


}



