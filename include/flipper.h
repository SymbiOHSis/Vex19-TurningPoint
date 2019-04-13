#pragma once

namespace Flipper {
    extern okapi::Motor motor;
    
    void initialize();
    
    void start();

    void stop();

    bool isActive();

    void debug();

    double getPosition();

    void run(void* param);
}
