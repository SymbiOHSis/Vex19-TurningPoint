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
	auto drive = ChassisControllerFactory::create(
		{ DRIVE_LEFT_FRONT,  DRIVE_LEFT_BACK  },
		{ DRIVE_RIGHT_FRONT, DRIVE_RIGHT_BACK },
		AbstractMotor::gearset::green,
		{ WHEEL_DIAMETER, CHASSIS_WIDTH }
	);

	Controller ctl;
	Motor catapult (CATAPULT);
	while (true) {
		debug();
		
		/* DRIVE */
		drive.arcade(
			ctl.getAnalog(okapi::ControllerAnalog::leftY),
			ctl.getAnalog(okapi::ControllerAnalog::leftX),
			CONTROLLER_THRESHOLD
		);

		/* BALL & CATAPULT */
		if (ctl.getDigital(okapi::ControllerDigital::X)) {
			startAutomaticBallIntake();
		}
		else if (ctl.getDigital(okapi::ControllerDigital::L1)) {
			stopAutomaticBallIntake();
			catapult.moveVelocity(BALL_INTAKE_SPEED * 12000);
		}
		else if (ctl.getDigital(okapi::ControllerDigital::L2)) {
			stopAutomaticBallIntake();
			catapult.moveVelocity(BALL_INTAKE_SPEED * 12000);
		}
		else if (!automaticBallIntakeIsActive()) {
			startAutomaticBallIntake(false);
		}

		if (ctl.getDigital(okapi::ControllerDigital::A)) {
			fireCatapult();
		}
		ballIntakeManager();
		catapultManager();

		/* FLIPPER */
		if (abs(ctl.getAnalog(okapi::ControllerAnalog::rightY)) > CONTROLLER_THRESHOLD) {
			setFlipperManual(ctl.getAnalog(okapi::ControllerAnalog::rightY));
		}
		else {
			setFlipperManual(0);
		}

		/* DESCORER */
		setDescorerManual((int) ctl.getDigital(okapi::ControllerDigital::R1) - (int) ctl.getDigital(okapi::ControllerDigital::R2));

		// loop at 100Hz
		pros::delay(10);
	}
}
