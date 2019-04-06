#include "main.h"

Motor catapult (CATAPULT);
Motor ballIntake (BALL_INTAKE);
pros::ADIAnalogIn ballDetector (BALL_DETECTOR);
pros::ADIPotentiometer catapultPot (CATAPULT_POT);

bool ballLoaded = true;
bool ballIntakeActive = false;
bool ballIntakeReversed = false;
uint32_t feedingUntil = 0;
bool catapultActive = false;
bool catapultFiring = false;

bool ballIsDetected() {
    return ballDetector.get_value() < BALL_DETECTOR_THRESHOLD;
}

bool catapultIsDown() {
    return catapultPot.get_value() < CATAPULT_LOAD + CATAPULT_POT_THRESHOLD;
}

void startBallIntake() {
    feedingUntil = pros::millis() + BALL_FEED_TIME;
    ballIntakeActive = true;
    ballIntakeReversed = false;
}

void reverseBallIntake() {
    feedingUntil = 0;
    ballIntakeActive = true;
    ballIntakeReversed = true;
}

void stopBallIntake() {
    feedingUntil = 0;
    ballIntakeActive = false;
    ballIntakeReversed = false;
}

void fireCatapult() {
    catapultActive = true;
    catapultFiring = true;
}

void stopCatapult() {
    catapultActive = false;
    catapultFiring = false;
}

void catapultManager() {
    // 
    if (catapultActive) {
        // finished firing
        if (catapultFiring && !catapultIsDown()) {
            catapultFiring = false;
        }
        // hold catapult down
        else if (!catapultFiring && catapultIsDown()) {
            catapult.moveVoltage(CATAPULT_HOLD_SPEED * 12000);
        }
        // firing or resetting
        else {
            catapult.moveVoltage(CATAPULT_SPEED * 12000);
        }
    }
    // stop catapult
    else {
        catapult.moveVoltage(0);
    }
}

void ballIntakeManager() {
    // 
    if (ballIntakeActive) {
        // manual control?
        if (ballIntakeReversed) {
            ballIntake.moveVoltage(-BALL_INTAKE_SPEED * 12000);
        }

        // ball intake running normally
        else {
            // run if feeding ball
            if (feedingUntil > pros::millis()) {
                ballIntake.moveVoltage(BALL_INTAKE_SPEED * 12000);
            }
            
            // if we detect a ball
            else if (ballIsDetected()) {
                // if catapult is down, feed ball
                if (catapultIsDown()) {
                    startBallIntake();
                }
                // if catapult is up, stop intake
                else {
                    ballIntake.moveVoltage(0);
                }
            }
        }
    }
    // stop ball intake
    else {
        ballIntake.moveVoltage(0);
    }
}
