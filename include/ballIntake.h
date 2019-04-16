#pragma once

namespace BallIntake {
    extern okapi::Motor motor;
    
    void start();

    void stop();

    bool isActive();

    void debug();

    double getPosition();

    bool ballIsLoaded();

    bool ballDetected();

    void loadBall();

    void run(void* param);
}
