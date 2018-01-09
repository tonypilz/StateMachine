#include "lightSwitch_custom.h"

#include <optional>
#include <iostream>
#include <variant>
#include <functional>

#include <src/EventProcessingResult.h>
#include <src/StateMachine.h>

struct StateOff;

struct StateOn {

   void entry(std::optional<bool>){std::cout<<"Light switchted on! \n";}

   template<typename Func>
   EventProcessingResult makeTransition(std::optional<bool> event, Func changeState){
       changeState(event,stateOff,[](auto&&){});
       return EventProcessingResult::transitionCompleted;
   }

   StateOff* stateOff = nullptr;
};

struct StateOff {

   void entry(std::optional<bool>){std::cout<<"Light switchted off! \n";}

   template<typename Func>
   EventProcessingResult makeTransition(std::optional<bool> event, Func changeState){
       changeState(event,stateOn,[](auto&&){});
       return EventProcessingResult::transitionCompleted;
   }

   StateOn* stateOn = nullptr;
};


int lightSwitch_custom()
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
    using SwitchStates = std::variant<StateOn*, StateOff*>;
    using StateMachine = StateMachine<SwitchStates>;

    //create states
    StateOn stateOn;
    StateOff stateOff;

    //link states
    stateOn.stateOff = &stateOff;
    stateOff.stateOn = &stateOn;

    //create machine
    StateMachine machine{&stateOff};


    //start interaction

    SwitchEventType e; //the value is not evaluated so it doesnt matter here

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
