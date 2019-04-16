#include "main.h"
#define R   * (red ? 1 : -1)
#define P   okapi::Point

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

void frontAuton(bool red) {
    BallIntake::start();

    // Generate first path
    profile.generatePath({okapi::Point{0_ft, 0_ft, 0_deg}, okapi::Point{30_in, 0_in, 0_deg}}, "capToGrabBallFrom");

    profile.setTarget("capToGrabBallFrom", 0);

    // Generate rest of paths
    profile.generatePath({okapi::Point{0_ft, 0_ft, 0_deg}, okapi::Point{12_in, 20_in, 90_deg}}, "capToFlip");
    profile.generatePath({okapi::Point{0_ft, 0_ft, 0_deg}, okapi::Point{24_in, 20_in, 90_deg}}, "lineUpAgainstWall");
    profile.generatePath({okapi::Point{0_ft, 0_ft, 0_deg}, okapi::Point{6_in, 6_in, 90_deg}}, "lineUpWithFlags");

    // grab ball from cap
    profile.waitUntilSettled();
    profile.setTarget("capToGrabBallFrom", true);
    profile.waitUntilSettled();
    
    // lower flipper and move to front cap
    Flipper::motor.move_absolute(FLIPPER_DOWN, 200);
    profile.setTarget("capToFlip", 0);
    profile.waitUntilSettled();

    // flip front cap
    Flipper::motor.move_absolute(FLIPPER_UP, 200);
    Flipper::waitUntilSettled();

    // line up against wall
    profile.setTarget("lineUpAgainstWall", true);
    profile.waitUntilSettled();

    // line up with flags
    profile.setTarget("lineUpWithFlags");
    profile.waitUntilSettled();
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
    Debug::start();

    //frontAuton(true);
    
    profile.generatePath({okapi::Point{0_ft, 0_ft, 0_deg}, okapi::Point{0_ft, 0_ft, 90_deg}}, "A");
    profile.setTarget("A");
    profile.waitUntilSettled();
    
}