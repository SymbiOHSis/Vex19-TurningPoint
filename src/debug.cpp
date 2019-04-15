#include "main.h"

 namespace Debug {
    int debug = 0;
	bool active = false;

	void backButtonCb() {
		debug--;
	}

	void nextButtonCb() {
		debug++;
	}

	void initialize() {
        pros::Task task (run);
		pros::lcd::register_btn0_cb(backButtonCb);
		pros::lcd::register_btn2_cb(nextButtonCb);
    }

    void start() {
        active = true;
    }

    void stop() {
        active = false;
    }

    void run(void* param) {
        while (true) {
            // loop at 100Hz
            pros::delay(10);

            // don't control anything if not active
            if (!active) continue;

            switch (debug % 4) {
				case 0:
					Catapult::debug();
					pros::lcd::print(7, "  <<<<<    Debugging:  Catapult    >>>>>  \n");
					break;
				case 1:
					BallIntake::debug();
					pros::lcd::print(7, "  <<<<<    Debugging:  BallIntake  >>>>>  \n");
					break;
				case 2:
					Flipper::debug();
					pros::lcd::print(7, "  <<<<<    Debugging:  Flipper     >>>>>  \n");
					break;
				case 3:
					Descorer::debug();
					pros::lcd::print(7, "  <<<<<    Debugging:  Descorer    >>>>>  \n");
					break;
			}
        }
    }
}
