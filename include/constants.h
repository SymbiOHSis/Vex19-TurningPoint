#pragma once

// Misc
#define CONTROLLER_THRESHOLD    0.03

// Drive
#define DRIVE_LEFT_FRONT        18
#define DRIVE_LEFT_BACK         20
#define DRIVE_RIGHT_FRONT       -17
#define DRIVE_RIGHT_BACK        -19

#define WHEEL_DIAMETER          4_in
#define CHASSIS_WIDTH           10_in

// Ball
#define BALL_INTAKE             15
#define BALL_INTAKE_SPEED       0.8 // range -1 <= x <= 1
#define BALL_DETECTOR           0
#define BALL_DETECTOR_THRESHOLD 0
#define BALL_FEED_TIME          0

// Catapult
#define CATAPULT                -13
#define CATAPULT_POT            'A'
#define CATAPULT_POT_THRESHOLD  99999
#define CATAPULT_UP             99999
#define CATAPULT_LOAD           0
#define CATAPULT_HOLD_SPEED     0   // range -1 <= x <= 1
#define CATAPULT_SPEED          0.8 // range -1 <= x <= 1

// Cap Flipper
#define FLIPPER                 14
#define FLIPPER_SPEED           0.8 // range -1 <= x <= 1

// Cap Descorer
#define DESCORER                12
#define DESCORER_SPEED          0.8 // range -1 <= x <= 1
