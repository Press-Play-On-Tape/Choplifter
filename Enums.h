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

enum class TankState : uint8_t {
  Dead1,
  Dead2,
  Dead3,
  Move_Left,
  Move_Right,
  Stationary
};

enum class TurrentDirection : uint8_t {
  Left_Low,
  Left_Mid,
  Upright,
  Right_Mid,
  Right_Low
};

enum class HostageStance : uint8_t {
  In_Dorm,
  Running_Left_1,
  Running_Left_2,
  Running_Left_3,
  Running_Left_4,
  Running_Right_1,
  Running_Right_2,
  Running_Right_3,
  Running_Right_4,
  Waving_11,
  Waving_12,
  Waving_21,
  Waving_22,
  Leaving_Dorm,
  Dead,
  Dying_1,
  Dying_2,
  Safe,
  In_Helicopter
};

enum class ExplosionType : uint8_t {
  None,
  Small,
  Medium,
  Both_None,
  Both_Small,
  Both_Medium,
  Large_1,
  Large_2,
  Large_3,
  Large_4,
  Large_End
};

enum class DormitoryState : uint8_t {
  Intact,
  Open
};

struct Helicopter {
  int8_t stance;
  int16_t xPos;
  int8_t yPos;
  int8_t xDelta;
  int8_t yDelta;
  uint8_t prevTurn; 
  uint8_t hits;
  uint8_t countDown;  // used when dying;
  uint8_t xInc;
};

struct Bullet {
  int16_t xPos;
  uint8_t yPos;
  int8_t xDelta;
  uint8_t yDelta;
  uint8_t startYPos;
};

struct BulletExplosion {
  int16_t xPos;
  uint16_t yPos;
  ExplosionType explosionType;
};

struct Hostage {
  HostageStance stance;   
  uint8_t countDown;
  int16_t xPos;
};

struct Dormitory {
  DormitoryState state;  
  int16_t xPos;
  uint8_t numberOfHits;
};

struct Tank {
  TankState state; 
  TurrentDirection turrentDirection;
  uint8_t countDown;
  int16_t xPos;
  bool track;
  uint8_t numberOfHits;
};
 

#define NUMBER_OF_PLAYER_BULLETS            6
#define NUMBER_OF_HOSTAGES                  64
#define NUMBER_OF_DORMITORIES               4
#define NUMBER_OF_TANKS                     6

#define BULLET_INACTIVE_X_VALUE             5000
#define BULLET_SHOOT_HORIZONTAL             255

#define NUMBER_OF_TANK_BULLETS              2
#define TANK_SPACING                        400
#define TANK_BULLET_MIN_Y_VALUE             25
#define TANK_BULLET_NUMBER_OF_HITS          4
#define TANK_FAR_RIGHT_POS                  225

#define HELICOPTER_MAXIMUM_HEIGHT           (int8_t)-3
#define HELICOPTER_MINIMUM_HEIGHT           (int8_t)40
#define HELICOPTER_HOSTAGE_CAPACITY         16
#define HELICOPTER_BULLET_NUMBER_OF_HITS    3

#define PREV_TURN_FROM_LEFT                 0
#define PREV_TURN_FROM_RIGHT                1

#define DELTA_X_DO_NOTHING                  0
#define DELTA_X_INCREASE                    1
#define DELTA_X_DECREASE                    2

#define DORMITORY_HITS_REQUIRED             5
#define DORMITORY_SPACING                   400
#define DORMITORY_HOSTAGE_CAPACITY          16

#define HOSTAGE_FAR_RIGHT_POS               210

