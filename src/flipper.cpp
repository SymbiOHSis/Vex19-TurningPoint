#include "main.h"

static Motor flipper (FLIPPER);

void setFlipperManual(float speed) {
    flipper.moveVoltage(speed * 12000);
}
