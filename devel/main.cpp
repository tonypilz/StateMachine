#include <iostream>
#include <examples/simple/genericStates/lightSwitch_generic.h>
#include <examples/simple/customStates/lightSwitch_custom.h>
#include <testing/testing.h>

int main()
{
    std::cout<<"main() start\n";
    lightSwitch_custom();
    //main_lightSwitch_minimal_generic();
    //main_testing();
    std::cout<<"main() end\n";
}



