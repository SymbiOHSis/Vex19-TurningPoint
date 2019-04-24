#include "main.h"

void clearLcd() {
    for (int i = 0; i < 8; i++) {
        pros::lcd::print(i, "\n");
    }
}
