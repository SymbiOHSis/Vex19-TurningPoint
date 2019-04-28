#include "main.h"

#define P   okapi::Point
#define FLIP_CAP_VELOCITY 160

namespace Autonomous {
    const bool PARK = true;
    const int NUM_AUTONS = 4;
    int lastAutonDisplayed = -1;

    enum {
        FRONT_RED,
        FRONT_BLUE,
        BACK_RED,
        BACK_BLUE
    };

	okapi::Controller ctl;
	pros::ADIPotentiometer pot (AUTON_POT);

    auto drive = okapi::ChassisControllerFactory::create(
        { DRIVE_LEFT_FRONT,  DRIVE_LEFT_BACK  },
        { DRIVE_RIGHT_FRONT, DRIVE_RIGHT_BACK },
        okapi::AbstractMotor::gearset::green,
        { WHEEL_DIAMETER, CHASSIS_WIDTH }
    );

    auto profile = okapi::AsyncControllerFactory::motionProfile(
        1.0,  // Maximum linear velocity of the Chassis in m/s
        1.8,  // Maximum linear acceleration of the Chassis in m/s/s
        8.0, // Maximum linear jerk of the Chassis in m/s/s/s
        drive // Chassis Controller
    );

    int getSelectedAuton() {
        int value = pot.get_value();

        if (value <= 1096) {
            return FRONT_RED;
        }
        else if (1024 <= value && value <= 2048) {
            return FRONT_BLUE;
        }
        else if (2048 <= value && value <= 3072) {
            return BACK_RED;
        }
        else if (3072 <= value) {
            return BACK_BLUE;
        }
    }

    void debug() {
        int selectedAuton = getSelectedAuton();

        // Pot Position
        int32_t value = pot.get_value();
        if (value != PROS_ERR_F) {
            pros::lcd::print(1, "Pot Value: %d\n", value);
        }
        else {
            pros::lcd::print(1, "Error fetching pot value, errno: %d", errno);
        }

        // if pot close to an edge, 'hide' selection (to make the user move it to the correct place)
        if (abs(value - 0) < 256 || abs(value - 1024) < 256 || abs(value - 2048) < 256 || abs(value - 3072) < 256 || abs(value - 4096) < 256) {
            pros::lcd::print(0, "Selected Auton: TOO CLOSE TO EDGE\n");
        }
        else {
            // update brain LCD
            switch (selectedAuton) {
                case FRONT_RED:
                    pros::lcd::print(0, "Selected Auton: front red\n");
                    break;
                case FRONT_BLUE:
                    pros::lcd::print(0, "Selected Auton: front blue\n");
                    break;
                case BACK_RED:
                    pros::lcd::print(0, "Selected Auton: back red\n");
                    break;
                case BACK_BLUE:
                    pros::lcd::print(0, "Selected Auton: back blue\n");
                    break;
            }
        }

        // pot returns 0 when uninitialized. only update controller screen on change
        if (value != 0 && lastAutonDisplayed != selectedAuton) {
            lastAutonDisplayed = selectedAuton;

            switch (selectedAuton) {
                case FRONT_RED:
                    ctl.setText(0, 0, "front  red\n");
                    break;
                case FRONT_BLUE:
                    ctl.setText(0, 0, "front blue\n");
                    break;
                case BACK_RED:
                    ctl.setText(0, 0, "back   red\n");
                    break;
                case BACK_BLUE:
                    ctl.setText(0, 0, "back  blue\n");
                    break;
            }
        }
    }

    /*  Sets each drive side to SPEED for specified TIME
        -1 <= SPEED <= 1
        TIME is in millisecs  */
    void driveTime(float speedLeft, float speedRight, int delayTime) {
        drive.left(speedLeft);
        drive.right(speedRight);
        pros::delay(delayTime);
        drive.left(0);
        drive.right(0);
    }
    void driveTime(float speed, int delayTime) {
        driveTime(speed, speed, delayTime);
    }

    // Actual autonomous //
    void backRed() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);

        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, -24_in, 0_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{36_in, 24_in, 0_deg}}, "capToFlipReverse");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{20_in, 26_in, 0_deg}}, "park");

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();

        // lower flipper and move to flat cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip flat cap & reverse to wall
        Flipper::motor.move_absolute(FLIPPER_UP, FLIP_CAP_VELOCITY);
        Flipper::waitUntilSettled();
        profile.setTarget("capToFlipReverse", true);
        profile.waitUntilSettled();
        driveTime(-0.3, 400);

        // park
        if (PARK) {
            profile.setTarget("park");
            profile.waitUntilSettled();
            driveTime(0.3, 400);
            driveTime(1, 2000);
        }
    }
    
    void backBlue() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);
        
        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 19_in, 0_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{36_in, -19_in, 0_deg}}, "capToFlipReverse");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{20_in, -36_in, 0_deg}}, "park");

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();

        // lower flipper and move to flat cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip flat cap & reverse to wall
        Flipper::motor.move_absolute(FLIPPER_UP, FLIP_CAP_VELOCITY);
        Flipper::waitUntilSettled();
        profile.setTarget("capToFlipReverse", true);
        profile.waitUntilSettled();
        driveTime(-0.3, 400);

        // park
        if (PARK) {
            profile.setTarget("park");
            profile.waitUntilSettled();
            driveTime(0.3, 400);
            driveTime(1, 1700);
        }
    }

    void frontRed() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);

        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{12_in, 16_in, 90_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{14_in, 26_in, -93_deg}}, "lineUpAgainstWall");

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();
        
        // lower flipper and move to front cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip front cap
        Flipper::motor.move_absolute(FLIPPER_UP, FLIP_CAP_VELOCITY);
        Flipper::waitUntilSettled();

        // line up against wall while generating more paths
        profile.setTarget("lineUpAgainstWall", true);
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, 6_in, 90_deg}}, "lineUpWithFlags");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{27_in, 0_in, 0_deg}}, "advanceToMidFlag");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, 3_in, 0_deg}}, "whackLowFlag");
        profile.waitUntilSettled();
        pros::delay(500);
        driveTime(-0.3, -0.5, 800);

        // line up with flags
        profile.moveTo({P{0_in, 0_in, 0_deg}, P{3_in, 0_in, 0_deg}});
        profile.setTarget("lineUpWithFlags");
        profile.waitUntilSettled();

        // fire first ball
        Catapult::fireAndReset();
        Catapult::waitUntilFired();

        // move forward to shoot mid flag
        profile.setTarget("advanceToMidFlag");
        profile.waitUntilSettled();

        // fire second ball
        pros::delay(1000); // massive delay to wait for ball to settle
        Catapult::fireAndReset();
        Catapult::waitUntilFired();

        // move forward to whack low flag
        profile.setTarget("whackLowFlag");
        profile.waitUntilSettled();
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        Flipper::waitUntilSettled();
    }

    void frontBlue() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);

        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{12_in, -16_in, -90_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{16_in, -26_in, 95_deg}}, "lineUpAgainstWall");

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();
        
        // lower flipper and move to front cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip front cap
        Flipper::motor.move_absolute(FLIPPER_UP, FLIP_CAP_VELOCITY);
        Flipper::waitUntilSettled();

        // line up against wall while generating more paths
        profile.setTarget("lineUpAgainstWall", true);
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{9_in, -9_in, -100_deg}}, "lineUpWithFlags");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{27_in, 0_in, 0_deg}}, "advanceToMidFlag");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, -4_in, 0_deg}}, "whackLowFlag");
        profile.waitUntilSettled();
        driveTime(-0.25, 700);

        // line up with flags
        profile.moveTo({P{0_in, 0_in, 0_deg}, P{3_in, 0_in, 0_deg}});
        profile.setTarget("lineUpWithFlags");
        profile.waitUntilSettled();

        // fire first ball
        Catapult::fireAndReset();
        Catapult::waitUntilFired();

        // move forward to shoot mid flag
        profile.setTarget("advanceToMidFlag");
        profile.waitUntilSettled();

        // fire second ball
        pros::delay(1000); // massive delay to wait for ball to settle
        Catapult::fireAndReset();
        Catapult::waitUntilFired();

        // move forward to whack low flag
        profile.setTarget("whackLowFlag");
        profile.waitUntilSettled();
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        Flipper::waitUntilSettled();
    }

    void runSelectedAuton() {
        // sometimes pot value is zero, wait for it to be ready (1sec timeout)
        uint32_t timeout = pros::millis() + 1000;
        while (pot.get_value() == 0 && pros::millis() < timeout) {
            pros::delay(10);
        }

        int selectedAuton = getSelectedAuton();
        debug();

        switch (selectedAuton) {
            case FRONT_RED:
                frontRed();
                break;
            case FRONT_BLUE:
                frontBlue();
                break;
            case BACK_RED:
                backRed();
                break;
            case BACK_BLUE:
                backBlue();
                break;
        }
    }
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
    Autonomous::runSelectedAuton();
}