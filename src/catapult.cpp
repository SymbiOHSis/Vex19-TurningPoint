#include "main.h"

namespace Catapult {
    enum {
        STOPPED,
        FIRE,
        RESET,
        FIRE_RESET
    };

    pros::Task task (run);
    bool active = false;
    int target = STOPPED;
    okapi::Motor motor (CATAPULT);
	pros::ADIPotentiometer pot (CATAPULT_POT);

    void start() {
        active = true;
    }

    void stop() {
        active = false;
        target = STOPPED;
        motor.moveVoltage(0);
    }

    bool isActive() {
        return active;
    }

    bool isUp() {
        return getPosition() > CATAPULT_UP - CATAPULT_POT_THRESHOLD;
    }

    bool isReadyToLoad() {
        return getPosition() < CATAPULT_LOAD + CATAPULT_POT_THRESHOLD;
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
        // Pot Position
        int32_t value = getPosition();
        if (value != PROS_ERR_F) {
            pros::lcd::print(3, "Pot Value: %d\n", value);
        }
        else {
            pros::lcd::print(3, "Error fetching pot value, errno: %d", errno);
        }
    }

    double getPosition() {
        return pot.get_value();
    }

    void fireAndReset() {
        target = FIRE_RESET;
        start();
    }

    void fire() {
        target = FIRE;
        start();
    }

    void reset() {
        target = RESET;
        start();
    }

    void waitUntilFired() {
        while (target == FIRE || target == FIRE_RESET) {
            pros::delay(10);
        }
    }

    void waitUntilReset() {
        while (!isReadyToLoad()) {
            pros::delay(10);
        }
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // don't control anything if not active
            if (!active) continue;

            // stopped
            if (target == STOPPED) {
                motor.moveVoltage(0);
            }

            // fire and reset
            else if (target == FIRE_RESET) {
                // finished firing, start resetting
                if (isUp()) {
                    target = RESET;
                }
                // keep firing
                else {
                    motor.moveVoltage(12000 * CATAPULT_SPEED);
                }
            }

            // fire
            else if (target == FIRE) {
                // finished firing
                if (isUp()) {
                    motor.moveVoltage(0);
                }
                // keep firing
                else {
                    motor.moveVoltage(12000 * CATAPULT_SPEED);
                }
            }

            // fire and reset
            else if (target == RESET) {
                // stop if ready to load (no holding power because we have a ratchet)
                if (isReadyToLoad()) {
                    motor.moveVoltage(0);
                }
                // drive down until ready to load
                else {
                    motor.moveVoltage(12000 * CATAPULT_SPEED);
                }
            }
        }
    }
}
