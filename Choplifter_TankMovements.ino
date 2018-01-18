#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

void tankMovements() {


  // Update tank ..

  for (int i=0; i < NUMBER_OF_TANKS; i++) {

    Tank *tank = &tanks[i];

    int16_t tankDif = tank->xPos - backgroundX;

    if (tank->state > TANK_STATE_DEAD_3) {

      switch (tank->state) {

        case TANK_STATE_MOVE_LEFT:
          tank->xPos+=2;
          break;

        case TANK_STATE_MOVE_RIGHT:

          if (tank->xPos > TANK_FAR_RIGHT_POS) {
            tank->xPos-=2;
          }
          else {
            tank->state = TANK_STATE_STATIONARY;
          }
          break;

      }


      // Move turrent ..

      switch (tankDif) {

        case -999 ... -40:
          tank->turrentDirection = TANK_TURRENT_DIR_LEFT_LOW;
          break;

        case -39 ... -10:
          tank->turrentDirection = TANK_TURRENT_DIR_LEFT_MID;
          break;

        case -9 ... 9:
          tank->turrentDirection = TANK_TURRENT_DIR_UPRIGHT;
          break;

        case 10 ... 39:
          tank->turrentDirection = TANK_TURRENT_DIR_RIGHT_MID;
          break;

        case 40 ... 999:
          tank->turrentDirection = TANK_TURRENT_DIR_RIGHT_LOW;
          break;

      }


      tank->countDown--;

      if (tank->countDown == 0) {

        switch (random(0, 3)) {

          case 0 ... 1:

            if (tankDif < -20) {

              tank->state = TANK_STATE_MOVE_LEFT;

            }
            else if (tankDif > 20) {

              tank->state = TANK_STATE_MOVE_RIGHT;
              
            }

            break;

          case 2:

            tank->state = TANK_STATE_STATIONARY;
            
            break;

        }

        tank->countDown = random(4, 12);

      }


      // Are we in a position to fire ?

      if (absT(tankDif) < 45) {

        if (random(0, 25) == 0) {

          for (int i = 0; i < NUMBER_OF_TANK_BULLETS; i++) {
            
            Bullet *bullet = &tankBullets[i];

            if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

              bullet->yPos = 40;
              bullet->yDelta = -8;
              bullet->startYPos = bullet->yPos;

              switch (tank->turrentDirection) {

                case TANK_TURRENT_DIR_LEFT_LOW:
                  bullet->xPos = tank->xPos + 11;
                  bullet->xDelta = -4;
                  break;

                case TANK_TURRENT_DIR_LEFT_MID:
                  bullet->xPos = tank->xPos + 9;
                  bullet->xDelta = -2;
                  break;

                case TANK_TURRENT_DIR_UPRIGHT:
                  bullet->xPos = tank->xPos;
                  bullet->xDelta = random(-1, 2);
                  break;

                case TANK_TURRENT_DIR_RIGHT_MID:
                  bullet->xPos = tank->xPos - 9;
                  bullet->xDelta = 2;
                  break;

                case TANK_TURRENT_DIR_RIGHT_LOW:
                  bullet->xPos = tank->xPos - 11;
                  bullet->xDelta = 4;
                  break;

              }

              break;

            }

          }

        }

      }

    }

  }

}