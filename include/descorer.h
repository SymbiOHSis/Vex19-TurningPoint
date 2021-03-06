#pragma once

namespace Descorer {
    extern okapi::Motor motor;
    
    void start();

    void stop();

    bool isActive();

    void debug();

    double getPosition();

    void run(void* param);
}
