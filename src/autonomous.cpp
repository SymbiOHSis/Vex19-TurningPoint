#include "main.h"

#define P   okapi::Point

namespace Autonomous {
    const int NUM_AUTONS = 4;
    int autonSelected = 2;

	okapi::Controller ctl;

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

    void runSelectedAuton() {
        ctl.setText(0, 0, "Running Auton\n");
        clearLcd();

        switch (autonSelected % NUM_AUTONS) {
            case 0:
                pros::lcd::print(0, "Running Auton: front red\n");
                frontRed();
                break;
            case 1:
                pros::lcd::print(0, "Running Auton: front blue\n");
                frontBlue();
                break;
            case 2:
                pros::lcd::print(0, "Running Auton: back red\n");
                backRed();
                break;
            case 3:
                pros::lcd::print(0, "Running Auton: back blue\n");
                backBlue();
                break;
        }
    }

    void updateLcds() {
        switch (autonSelected % NUM_AUTONS) {
            case 0:
                pros::lcd::print(0, "Selected Auton: front red\n");
                ctl.setText(1, 0, "front  red\n");
                break;
            case 1:
                pros::lcd::print(0, "Selected Auton: front blue\n");
                ctl.setText(1, 0, "front blue\n");
                break;
            case 2:
                pros::lcd::print(0, "Selected Auton: back red\n");
                ctl.setText(1, 0, "back   red\n");
                break;
            case 3:
                pros::lcd::print(0, "Selected Auton: back blue\n");
                ctl.setText(1, 0, "back  blue\n");
                break;
        }
    }
    
    // cycle through autons
	void middleButtonCb() {
        // not on auton selection screen
        if (Debug::debug != 0) {
            return;
        }

        autonSelected++;
        updateLcds();
	}

	void initializeSelector() {
		pros::lcd::register_btn1_cb(middleButtonCb);
        pros::lcd::print(0, "Selected Auton:\n");
        ctl.setText(0, 0, "Selected Auton\n");
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

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();

        // lower flipper and move to flat cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip flat cap
        Flipper::motor.move_absolute(FLIPPER_UP, 200);
        Flipper::waitUntilSettled();

        // reverse away from cap
        driveTime(-0.5, 500);
    }
    
    void backBlue() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);
        
        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 24_in, 90_deg}}, "capToFlip");

        // grab ball from cap & reverse to starting position
        profile.waitUntilSettled();
        profile.setTarget("capToGrabBallFrom", true);
        profile.waitUntilSettled();

        // lower flipper and move to flat cap
        Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
        pros::delay(200);
        profile.setTarget("capToFlip");
        profile.waitUntilSettled();

        // flip flat cap
        Flipper::motor.move_absolute(FLIPPER_UP, 200);
        Flipper::waitUntilSettled();

        // reverse away from cap
        driveTime(-0.5, 500);
    }

    void frontRed() {
        BallIntake::start();

        // Generate first path
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");
        profile.setTarget("capToGrabBallFrom", 0);

        // Generate some more paths
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{12_in, 20_in, 90_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{22_in, 26_in, 90_deg}}, "lineUpAgainstWall");

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
        Flipper::motor.move_absolute(FLIPPER_UP, 200);
        Flipper::waitUntilSettled();

        // line up against wall while generating more paths
        profile.setTarget("lineUpAgainstWall", true);
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, 6_in, 90_deg}}, "lineUpWithFlags");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{27_in, 0_in, 0_deg}}, "advanceToMidFlag");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, 2_in, 0_deg}}, "whackLowFlag");
        profile.waitUntilSettled();
        driveTime(-0.4, 350);

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
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{12_in, -20_in, -90_deg}}, "capToFlip");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{22_in, -26_in, 95_deg}}, "lineUpAgainstWall");

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
        Flipper::motor.move_absolute(FLIPPER_UP, 200);
        Flipper::waitUntilSettled();

        // line up against wall while generating more paths
        profile.setTarget("lineUpAgainstWall", true);
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{9_in, -9_in, -100_deg}}, "lineUpWithFlags");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{27_in, 0_in, 0_deg}}, "advanceToMidFlag");
        profile.generatePath({P{0_in, 0_in, 0_deg}, P{6_in, -2_in, 0_deg}}, "whackLowFlag");
        profile.waitUntilSettled();
        driveTime(-0.4, 350);

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