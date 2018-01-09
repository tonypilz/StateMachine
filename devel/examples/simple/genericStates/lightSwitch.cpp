
#include <GenericState.h>
#include <StateMachine.h>
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
    StateMachine m{&stateOff};


    //start interaction

    SwitchEventType e; //the value is not evaluated by the guards so it doesnt matter here

    for(int i=0;i<3;++i) {
        std::cout << "pressing button \n"; //send event
        m.processEvent(e);
    }

//    prints:
//            pressing button
//            Light switchted on
//            pressing button
//            Light switchted off
//            pressing button
//            Light switchted on

    return 0;


}



