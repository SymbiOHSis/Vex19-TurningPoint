#pragma once

namespace Catapult {
    extern okapi::Motor motor;
    extern pros::ADIPotentiometer pot;
    
    void start();

    void stop();

    bool isActive();

    bool isUp();

    bool isReadyToLoad();

    void debug();

    double getPosition();

    void fireAndReset();

    void fire();

    void reset();

    void run(void* param);
}
