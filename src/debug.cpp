#include "main.h"

#if DEBUG == 0

void debug() {}

#else

static Motor catapult (CATAPULT);
static Motor ballIntake (BALL_INTAKE);
static Motor descorer (DESCORER);
static Motor flipper (FLIPPER);
static uint32_t nextUpdateTime = 0;

void debug() {
    if (nextUpdateTime > pros::millis()) {
        return;
    }
    nextUpdateTime = pros::millis() + 50;

    pros::lcd::print(0, "Buttons: %d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
                        (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
                        (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

    #if DEBUG >= 5
        pros::lcd::print(0, "Debug level: %d\n", DEBUG);
        pros::lcd::print(1, "Catapult voltage: %.2f\n", catapult.getVoltage() / 1000.0);
        pros::lcd::print(2, "Ball intake voltage: %.2f\n", ballIntake.getVoltage() / 1000.0);
        pros::lcd::print(3, "Descorer voltage: %.2f\n", descorer.getVoltage() / 1000.0);
        pros::lcd::print(4, "Flipper voltage: %.2f\n", flipper.getVoltage() / 1000.0);
    #endif
}

#endif //
