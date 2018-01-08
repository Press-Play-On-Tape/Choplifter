#pragma once

#include "src/utils/Arduboy2Ext.h"

#define _DEBUG

#define HELICOPTER_MAXIMUM_HEIGHT           (int8_t)-3
#define HELICOPTER_MINIMUM_HEIGHT           (int8_t)40




#define PREV_TURN_FROM_LEFT                 0
#define PREV_TURN_FROM_RIGHT                1

#define DELTA_X_DO_NOTHING                  0
#define DELTA_X_INCREASE /*DELTA_X_INCREASE_TO_ZERO*/            1
#define DELTA_X_DECREASE /*DELTA_X_DECREASE_TO_ZERO*/            2
#define DELTA_X_INCREASE                    3
#define DELTA_X_DECREASE                    4

