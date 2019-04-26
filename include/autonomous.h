#pragma once

namespace Autonomous {
    extern pros::ADIPotentiometer pot;
    
    void initializeSelector();

    void debug();
    
    void runSelectedAuton();
    
}