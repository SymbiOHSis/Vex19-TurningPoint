#include "main.h"

namespace Descorer {
    pros::Task task (run);
    bool active = false;
    okapi::Motor motor (DESCORER);

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
        // Motor Voltage
        int32_t voltage = motor.getVoltage();
        if (voltage != PROS_ERR) {
            pros::lcd::print(0, "Motor Voltage: %.2fV\n", voltage / 1000.0);
        }
        else {
            pros::lcd::print(0, "Error fetching motor voltage, errno: %d", errno);
        }
        // Motor Position
        double position = motor.getPosition();
        if (position != PROS_ERR) {
            pros::lcd::print(1, "Motor Position: %.1f\n", position);
        }
        else {
            pros::lcd::print(1, "Error fetching motor position, errno: %d", errno);
        }
        // Motor Temperature
        double temperature = motor.getTemperature();
        if (temperature != PROS_ERR) {
            pros::lcd::print(2, "Motor Temp: %.0fC\n", temperature);
        }
        else {
            pros::lcd::print(2, "Error fetching motor temp, errno: %d", errno);
        }
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
