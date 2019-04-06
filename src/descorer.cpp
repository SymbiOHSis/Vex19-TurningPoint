#include "main.h"

Motor descorer (DESCORER);

void setDescorerManual(int speed) {
    descorer.moveVoltage(speed * 12000);
}
