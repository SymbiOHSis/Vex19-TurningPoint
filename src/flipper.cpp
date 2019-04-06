#include "main.h"

Motor flipper (FLIPPER);

void setFlipperManual(int speed) {
    flipper.moveVoltage(speed * 12000);
}
