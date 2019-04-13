#include "main.h"

 namespace Flipper {
    bool active = false;
    okapi::Motor motor (FLIPPER);

    void initialize() {
        pros::Task task (run);
    }

    void start() {
        active = true;
    }

    void stop() {
        active = false;
        motor.moveVoltage(0);
    }

    bool isActive() {
        return active;
    }

    void debug() {
        pros::lcd::print(0, "Motor Voltage: %.2fV\n", motor.getVoltage() / 1000.0);
        pros::lcd::print(1, "Motor Position: %.2f\n", motor.getPosition());
        pros::lcd::print(2, "Motor Temp: %d\n", motor.getTemperature());
    }

    double getPosition() {
        return motor.getPosition();
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // don't control anything if not active
            if (!active) continue;

            
        }
    }
}
