#pragma once

#include "src/utils/Arduboy2Ext.h"

#define _DEBUG

struct Hostage {
  uint8_t stance;   
  uint8_t countDown;
  int16_t xPos;
};

struct Dormitory {
  uint8_t state;    // 0 .. intact.  1 shot open.
  int16_t xPos;
};

#define HELICOPTER_MAXIMUM_HEIGHT           (int8_t)-3
#define HELICOPTER_MINIMUM_HEIGHT           (int8_t)40

#define PREV_TURN_FROM_LEFT                 0
#define PREV_TURN_FROM_RIGHT                1

#define DELTA_X_DO_NOTHING                  0
#define DELTA_X_INCREASE /*DELTA_X_INCREASE_TO_ZERO*/            1
#define DELTA_X_DECREASE /*DELTA_X_DECREASE_TO_ZERO*/            2
#define DELTA_X_INCREASE                    3
#define DELTA_X_DECREASE                    4

#define DORMITORY_STATE_INTACT              0
#define DORMITORY_STATE_OPEN                1

#define HOSTAGE_IN_DORM                     0
#define HOSTAGE_RUNNING_LEFT_1              1
#define HOSTAGE_RUNNING_LEFT_2              2
#define HOSTAGE_RUNNING_LEFT_3              3
#define HOSTAGE_RUNNING_LEFT_4              4
#define HOSTAGE_RUNNING_RIGHT_1             5
#define HOSTAGE_RUNNING_RIGHT_2             6
#define HOSTAGE_RUNNING_RIGHT_3             7
#define HOSTAGE_RUNNING_RIGHT_4             8
#define HOSTAGE_WAVING_11                   9
#define HOSTAGE_WAVING_12                   10
#define HOSTAGE_WAVING_21                   11
#define HOSTAGE_WAVING_22                   12
#define HOSTAGE_LEAVING_DORM                13
#define HOSTAGE_DEAD                        14
#define HOSTAGE_SAFE                        15
#define HOSTAGE_IN_HELICOPTER               16
