#include "main.h"

static Motor flipper (FLIPPER);

void setFlipperManual(int speed) {
    flipper.moveVoltage(speed * 12000);
    
#if DEBUG >= 5
	pros::lcd::print(4, "Flipper voltage: %.2f\n", speed * 12.0);
#endif
}
