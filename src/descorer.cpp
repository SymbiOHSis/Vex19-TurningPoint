#include "main.h"

namespace Descorer {
    bool active = false;
    okapi::Motor motor (DESCORER);

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
        pros::lcd::print(3, "\n");
        pros::lcd::print(4, "\n");
        pros::lcd::print(5, "\n");
        pros::lcd::print(6, "\n");
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
