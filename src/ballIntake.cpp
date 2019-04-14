#include "main.h"

namespace BallIntake {
    bool active = false;
    bool ballLoaded = true;
    uint32_t loadingUntil = 0;
    okapi::Motor motor (BALL_INTAKE);
	pros::ADILineSensor ballDetector (BALL_DETECTOR);

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

    void ballLoadedAtStartOfAuton(bool _ballLoaded) {
        ballLoaded = _ballLoaded;
    }

    bool ballIsLoaded() {
        return ballLoaded;
    }

    bool ballDetected() {
        return ballDetector.get_value() < BALL_DETECTOR_THRESHOLD;
    }

    void loadBall() {
        loadingUntil = pros::millis() + BALL_FEED_TIME;
        start();
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // don't control anything if not active
            if (!active) continue;

            // catapult is up, assume no ball
            if (Catapult::getPosition() > CATAPULT_LOAD + CATAPULT_POT_THRESHOLD) {
                ballLoaded = false;
            }

            // ball is detected by sensor
            if (ballDetected()) {
                // catapult is up, stop intake
                if (Catapult::getPosition() > CATAPULT_LOAD + CATAPULT_POT_THRESHOLD) {
                    motor.moveVoltage(0);
                }
                else {
                    // catapult is down, loading ball, continue
                    if (loadingUntil > pros::millis()) {
                        motor.moveVoltage(12000 * BALL_INTAKE_SPEED);
                    }
                    // catapult is down, no ball loaded, load ball
                    else if (!ballIsLoaded()) {
                        loadBall();
                    }
                    // catapult is down, ball loaded, stop intake
                    else {
                        motor.moveVoltage(0);
                    }
                }
            }
        }
    }
}
