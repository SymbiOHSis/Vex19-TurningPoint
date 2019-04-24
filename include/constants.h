#pragma once

// Misc
#define CONTROLLER_THRESHOLD    0.03

// Drive
#define DRIVE_LEFT_FRONT        18
#define DRIVE_LEFT_BACK         20
#define DRIVE_RIGHT_FRONT       -17
#define DRIVE_RIGHT_BACK        -19

#define WHEEL_DIAMETER          4_in
#define CHASSIS_WIDTH           (10_in * 1.30)

// Ball Intake
#define BALL_INTAKE             15
#define BALL_INTAKE_SPEED       1.0 // range -1 <= x <= 1
#define BALL_DETECTOR           'C'
#define BALL_DETECTOR_THRESHOLD 2500 // 2800 no ball
#define BALL_FEED_TIME          300

// Catapult
#define CATAPULT                13
#define CATAPULT_POT            'B'
#define CATAPULT_POT_THRESHOLD  100
#define CATAPULT_UP             3260
#define CATAPULT_LOAD           (2250 - 15)
//      CATAPULT_FIRE           2200
#define CATAPULT_HOLD_SPEED     -0   // range -1 <= x <= 1
#define CATAPULT_SPEED          -1.0 // range -1 <= x <= 1

// Cap Flipper
#define FLIPPER                 14
#define FLIPPER_SPEED           1.0 // range -1 <= x <= 1
#define FLIPPER_UP              0
#define FLIPPER_DOWN            -360

// Cap Descorer
#define DESCORER                12
#define DESCORER_SPEED          1.0 // range -1 <= x <= 1
