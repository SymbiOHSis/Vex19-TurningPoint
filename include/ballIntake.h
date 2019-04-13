#pragma once

namespace BallIntake {
    extern okapi::Motor motor;
    
    void initialize();
    
    void start();

    void stop();

    bool isActive();

    void debug();

    double getPosition();

    void ballLoadedAtStartOfAuton(bool _ballLoaded = true);

    bool ballIsLoaded();

    void loadBall();

    void run(void* param);
}
