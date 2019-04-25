#pragma once

namespace Autonomous {
    extern pros::ADIPotentiometer pot;
    
    void initializeSelector();

    void debug();
    
    void frontRed();

    void frontBlue();

    void backRed();

    void backBlue();
}