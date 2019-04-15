#include "main.h"

namespace BallIntake {
    pros::Task task (run);
    bool active = false;
    bool ballLoaded = true;
    uint32_t loadingUntil = 0;
    okapi::Motor motor (BALL_INTAKE);
	pros::ADILineSensor ballDetector (BALL_DETECTOR);

    void start() {
        active = true;
        motor.moveVoltage(12000 * BALL_INTAKE_SPEED);
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
        // Pot Position
        int32_t value = ballDetector.get_value();
        if (value != PROS_ERR) {
            pros::lcd::print(3, "BallDetector Value: %d\n", value);
        }
        else {
            pros::lcd::print(3, "Error fetching Detector value, errno: %d", errno);
        }
        pros::lcd::print(4, "Ball detected: %d (threshold: %d)\n", ballDetected(), BALL_DETECTOR_THRESHOLD);
        pros::lcd::print(5, "Robot thinks there's a ball loaded: %d\n", ballIsLoaded());
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
        ballLoaded = true;
        start();
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // catapult is up, assume no ball
            if (Catapult::getPosition() > CATAPULT_LOAD) {
                ballLoaded = false;
            }

            // don't control anything if not active
            if (!active) continue;

            // ball is detected by sensor
            if (ballDetected()) {
                // catapult is up, stop intake
                if (Catapult::getPosition() > CATAPULT_LOAD) {
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
