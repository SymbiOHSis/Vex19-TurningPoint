#include "main.h"

static okapi::Motor descorer (DESCORER);

void setDescorerManual(float speed) {
    descorer.moveVoltage(speed * 12000);
}
