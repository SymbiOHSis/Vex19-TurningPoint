#include "main.h"

namespace Catapult {
    enum {
        STOPPED,
        FIRE,
        RESET,
        FIRE_RESET
    };

    bool active = false;
    int target = STOPPED;
    okapi::Motor motor (CATAPULT);
	pros::ADIPotentiometer pot (CATAPULT_POT);

    void initialize() {
        pros::Task task (run);
    }

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

    void debug() {
        if (int v = motor.getVoltage() != PROS_ERR) {
            pros::lcd::print(0, "Motor Voltage: %.2fV\n", v / 1000.0);
        }
        else {
            pros::lcd::print(0, "Error fetching voltage, errno: %d", errno);
        }
        if (double p = motor.getPosition() != PROS_ERR) {
            pros::lcd::print(1, "Motor Position: %.2f\n", p);
        }
        else {
            pros::lcd::print(1, "Error fetching position, errno: %d", errno);
        }
        pros::lcd::print(2, "Motor Temp: %.0f\n", motor.getTemperature());
        pros::lcd::print(3, "Pot Position: %d\n", pot.get_value());
        pros::lcd::print(4, "\n");
        pros::lcd::print(5, "\n");
        pros::lcd::print(6, "\n");
    }

    double getPosition() {
        return pot.get_value();
    }

    void fireAndReset() {
        target = FIRE_RESET;
        start();
    }

    void reset() {
        target = RESET;
        start();
    }

    void fire() {
        target = FIRE;
        start();
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // don't control anything if not active
            if (!active) continue;

            // stopped
            else if (target == FIRE_RESET) {
                motor.moveVoltage(0);
            }

            // fire and reset
            else if (target == FIRE_RESET) {
                // finished firing, start resetting
                if (getPosition() > CATAPULT_UP - CATAPULT_POT_THRESHOLD) {
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
                if (getPosition() > CATAPULT_UP - CATAPULT_POT_THRESHOLD) {
                    motor.moveVoltage(0);
                }
                // keep firing
                else {
                    motor.moveVoltage(12000 * CATAPULT_SPEED);
                }
            }

            // fire and reset
            else if (target == RESET) {
                // finished resetting
                if (getPosition() > CATAPULT_UP - CATAPULT_POT_THRESHOLD) {
                    motor.moveVoltage(12000 * CATAPULT_HOLD_SPEED);
                }
                // keep resetting
                else {
                    motor.moveVoltage(12000 * CATAPULT_SPEED);
                }
            }
        }
    }
}
