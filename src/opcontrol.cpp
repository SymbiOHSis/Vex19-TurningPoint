#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	auto drive = okapi::ChassisControllerFactory::create(
		{ DRIVE_LEFT_FRONT,  DRIVE_LEFT_BACK  },
		{ DRIVE_RIGHT_FRONT, DRIVE_RIGHT_BACK },
		okapi::AbstractMotor::gearset::green,
		{ WHEEL_DIAMETER, CHASSIS_WIDTH }
	);

	okapi::Controller ctl;

	Flipper::stop();
	Descorer::stop();
	Debug::start();

	while (true) {
		/* DRIVE */
		drive.arcade( 
			ctl.getAnalog(okapi::ControllerAnalog::leftY),
			ctl.getAnalog(okapi::ControllerAnalog::leftX),
			CONTROLLER_THRESHOLD
		);
		
		/* CATAPULT */
		// stop/relax catapult
		if (ctl.getDigital(okapi::ControllerDigital::X)) {
			Catapult::stop();
		}
		// fire and reset catapult
		else if (ctl.getDigital(okapi::ControllerDigital::A)) {
			Catapult::fireAndReset();
		}

		/* BALL INTAKE */
		// L1 to stop
		if (ctl.getDigital(okapi::ControllerDigital::L1)) {
			BallIntake::stop();
		}
		// L2 to reverse
		else if (ctl.getDigital(okapi::ControllerDigital::L2)) {
			if (BallIntake::isActive()) {
				BallIntake::stop();
			}
			BallIntake::motor.moveVoltage(12000 * -BALL_INTAKE_SPEED);
		}
		// B to force feed/load
		else if (ctl.getDigital(okapi::ControllerDigital::L2)) {
			BallIntake::loadBall();
		}
		// otherwise automatic control
		else {
			BallIntake::start();
		}

		/* FLIPPER */
		float rightY = ctl.getAnalog(okapi::ControllerAnalog::rightY);
		if (abs(rightY) > CONTROLLER_THRESHOLD) {
			Flipper::motor.moveVoltage(rightY * 12000);
		}
		else {
			Flipper::motor.moveVoltage(0);
		}

		/* DESCORER */
		int direction = ctl.getDigital(okapi::ControllerDigital::R1) - ctl.getDigital(okapi::ControllerDigital::R2);
		Descorer::motor.moveVoltage(direction * 12000 * DESCORER_SPEED);

		/* DEBUG */
		// already started

		// loop at 100Hz
		pros::delay(10);
	}
}
