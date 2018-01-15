#pragma once

#include "src/utils/Arduboy2Ext.h"

#define _DEBUG

enum class GameState : uint8_t {
  Initialise,
  Introduction,
  Sortie,
  PlayGame,
  GameOver
};

struct Bullet {
  int16_t xPos;
  uint16_t yPos;
  int8_t xDelta;
  uint8_t yDelta;
  uint8_t startYPos;
};

struct BulletExplosion {
  int16_t xPos;
  uint16_t yPos;
  uint8_t explosionType;
};

struct Hostage {
  uint8_t stance;   
  uint8_t countDown;
  int16_t xPos;
};

struct Dormitory {
  uint8_t state;    // 0 .. intact.  1 shot open.
  int16_t xPos;
  uint8_t numberOfHits;
};

struct Tank {
  uint8_t state;    // 0 .. dead, 1 Move Left, 2 Move Right
  uint8_t turrentDirection;
  uint8_t countDown;
  int16_t xPos;
  bool track;
  uint8_t numberOfHits;
};


#define NUMBER_OF_PLAYER_BULLETS            8
#define NUMBER_OF_HOSTAGES                  64
#define NUMBER_OF_DORMITORIES               4
#define NUMBER_OF_TANKS                     6

#define BULLET_INACTIVE_X_VALUE             5000
#define BULLET_SHOOT_HORIZONTAL             255

#define TANK_STATE_DEAD_1                   0
#define TANK_STATE_DEAD_2                   1
#define TANK_STATE_DEAD_3                   2
#define TANK_STATE_MOVE_LEFT                3
#define TANK_STATE_MOVE_RIGHT               4
#define TANK_STATE_STATIONARY               5

#define TANK_SPACING                        400
#define TANK_BULLET_MIN_Y_VALUE             25
#define TANK_BULLET_NUMBER_OF_HITS          6

#define TANK_TURRENT_DIR_LEFT_LOW           0
#define TANK_TURRENT_DIR_LEFT_MID           1
#define TANK_TURRENT_DIR_UPRIGHT            2
#define TANK_TURRENT_DIR_RIGHT_MID          3
#define TANK_TURRENT_DIR_RIGHT_LOW          4
#define TANK_FAR_RIGHT_POS                  225

#define HELICOPTER_MAXIMUM_HEIGHT           (int8_t)-3
#define HELICOPTER_MINIMUM_HEIGHT           (int8_t)40
#define HELICOPTER_HOSTAGE_CAPACITY         16

#define PREV_TURN_FROM_LEFT                 0
#define PREV_TURN_FROM_RIGHT                1

#define DELTA_X_DO_NOTHING                  0
#define DELTA_X_INCREASE                    1
#define DELTA_X_DECREASE                    2

#define DORMITORY_STATE_INTACT              0
#define DORMITORY_STATE_OPEN                1
#define DORMITORY_HITS_REQUIRED             5
#define DORMITORY_SPACING                   400
#define DORMITORY_SPACING_FUDGE             DORMITORY_SPACING / 4
#define DORMITORY_HOSTAGE_CAPACITY          16

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
#define HOSTAGE_DYING_1                     15
#define HOSTAGE_DYING_2                     16
#define HOSTAGE_SAFE                        17
#define HOSTAGE_IN_HELICOPTER               18
#define HOSTAGE_FAR_RIGHT_POS               210

#define EXPLOSION_SML                       1
#define EXPLOSION_MED                       2
#define EXPLOSION_LRG_1                     3
#define EXPLOSION_LRG_2                     4
#define EXPLOSION_LRG_3                     5
#define EXPLOSION_LRG_4                     6
