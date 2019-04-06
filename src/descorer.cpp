#include "main.h"

static Motor descorer (DESCORER);

void setDescorerManual(int speed) {
    descorer.moveVoltage(speed * 12000);
}
