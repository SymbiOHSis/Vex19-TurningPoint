#include "main.h"

static Motor catapult (CATAPULT);
static Motor ballIntake (BALL_INTAKE);
static pros::ADIAnalogIn ballDetector (BALL_DETECTOR);
static pros::ADIPotentiometer catapultPot (CATAPULT_POT);

static bool ballLoaded = true;
static bool automaticBallIntakeActive = false;
static uint32_t feedingUntil = 0;
static bool catapultActive = false;
static bool catapultFiring = false;

bool ballIsDetected() {
    return ballDetector.get_value() < BALL_DETECTOR_THRESHOLD;
}

bool catapultIsDown() {
    return catapultPot.get_value() < CATAPULT_LOAD + CATAPULT_POT_THRESHOLD;
}

bool automaticBallIntakeIsActive() {
    return automaticBallIntakeActive;
}

void startAutomaticBallIntake(bool forceFeed) {
    if (forceFeed) {
        feedingUntil = pros::millis() + BALL_FEED_TIME;
    }
    automaticBallIntakeActive = true;
}

void stopAutomaticBallIntake() {
    feedingUntil = 0;
    automaticBallIntakeActive = false;
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
            ballLoaded = false;
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
    if (automaticBallIntakeActive) { 
        // run if feeding ball
        if (feedingUntil > pros::millis()) {
            ballIntake.moveVoltage(BALL_INTAKE_SPEED * 12000);
        }
        
        // if we detect a ball
        else if (ballIsDetected()) {
            // if catapult is down (and no ball loaded), feed ball
            if (catapultIsDown() && !ballLoaded) {
                startAutomaticBallIntake();
                ballLoaded = true;
            }
            // if catapult is up or ball is already loaded, stop intake
            else {
                ballIntake.moveVoltage(0);
            }
        }
    }
}
