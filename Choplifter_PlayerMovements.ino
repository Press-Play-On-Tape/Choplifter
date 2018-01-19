#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

void playerMovements() {


  // Up and down movements ..

  if (arduboy.pressed(UP_BUTTON)) {

    if (heli.yPos > HELICOPTER_MAXIMUM_HEIGHT) {

      heli.yDelta = calcSpeed(heli.yDelta, false);  

    }

  } 
  
  else {
    
    if (heli.yPos < HELICOPTER_MINIMUM_HEIGHT) {

      heli.yDelta = calcSpeed(heli.yDelta, true);  

    }

  }


  // Turn the helicoper ..

  if (arduboy.pressed(B_BUTTON)) {

    if (heli.xDelta == 0) {

      switch (heli.stance) {

        case 1:
          playerStack.push(10, 9);
          playerStack.push(8, 7);
          heli.prevTurn = PREV_TURN_FROM_LEFT;
          break;

        case -1:
          playerStack.push(10, -9);
          playerStack.push(-8, -7);
          heli.prevTurn = PREV_TURN_FROM_RIGHT;
          break;

        case 10:

          if (heli.prevTurn == PREV_TURN_FROM_LEFT) {
            playerStack.push(-1, -7);
            playerStack.push(-8, -9);
          }
          else {
            playerStack.push(1, 7);
            playerStack.push(8, 9);
          }

          break;

      }

    }
    else {

      switch (heli.stance) {

        case -17:
          playerStack.push(6, 20);
          playerStack.push(-19, -18);
          break;

        case -12:
          playerStack.push(-6, -20);
          break;

        case 12:
          playerStack.push(6, 20);
          break;
      
        case 17:
          playerStack.push(-6, -20);
          playerStack.push(19, 18);
          break;

        default: break;

      }

    }

  }


  // Left and right movements ..

  if (arduboy.pressed(LEFT_BUTTON)) {

    switch (heli.stance) {

      case 1:
        playerStack.push(6, 5, 4);
        playerStack.push(3, 2);
        heli.xInc = DELTA_X_DECREASE;          
        break;

      case 10:
        playerStack.push(12, 11);
        heli.xInc = DELTA_X_DECREASE;          
        break;

      case 17:
        playerStack.push(6, 5, 4);
        playerStack.push(3, 2, 1);
        playerStack.push(13, 14);
        playerStack.push(15, 16);
        heli.xInc = DELTA_X_DECREASE;          
        break;

      case -1:
        playerStack.push(-17, -16, -15);
        playerStack.push(-14, -13);
        heli.xInc = DELTA_X_DECREASE;          
        break;

      case -6:
        playerStack.push(-17, -16, -15);
        playerStack.push(-14, -13);
        playerStack.push(-1, -2, -3);
        playerStack.push(-4, -5);
        heli.xInc = DELTA_X_DECREASE;          
        break;

      case -12:
        playerStack.push(12, 11);
        playerStack.push(10, -11);
        heli.xInc = DELTA_X_INCREASE;          
        break;

      default:
        heli.xInc = DELTA_X_DECREASE;          
        break;
      
    }

  } 

  else if (arduboy.pressed(RIGHT_BUTTON)) {

    switch (heli.stance) {

      case -1:
        playerStack.push(-6, -5, -4);
        playerStack.push(-3, -2);
        heli.xInc = DELTA_X_INCREASE; 
        break;

      case 10:
        playerStack.push(-12, -11);
        heli.xInc = DELTA_X_INCREASE; 
        break;

      case -17:
        playerStack.push(-6, -5, -4);
        playerStack.push(-3, -2, -1);
        playerStack.push(-13, -14);
        playerStack.push(-15, -16);
        heli.xInc = DELTA_X_INCREASE; 
        break;

      case 1:
        playerStack.push(17, 16, 15);
        playerStack.push(14, 13);
        heli.xInc = DELTA_X_INCREASE;       
        break;

      case 6:
        playerStack.push(17, 16, 15);
        playerStack.push(14, 13);
        playerStack.push(1, 2, 3);
        playerStack.push(4, 5);
        heli.xInc = DELTA_X_INCREASE;       
        break;

      case 12:
        playerStack.push(-12, -11);
        playerStack.push(10, 11);
        heli.xInc = DELTA_X_DECREASE;       
        break;

      default:
        heli.xInc = DELTA_X_INCREASE; 
        break;

    }

  } 
  else {  // Return from current position to upright ..

    switch (heli.stance) {

      case 12:
        playerStack.push(10, 11);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_INCREASE;
        break;

      case -12:
        playerStack.push(10, -11);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_DECREASE;
        break;

      case 6:
        playerStack.push(1, 2, 3);
        playerStack.push(4, 5);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_INCREASE;
        break;

      case -6:
        playerStack.push(-1, -2, -3);
        playerStack.push(-4, -5);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_DECREASE;
        break;
      
      case 17:
        playerStack.push(1, 13, 14);
        playerStack.push(15, 16);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_DECREASE;
        break;

      case -17:
        playerStack.push(-1, -13, -14);
        playerStack.push(-15, -16);
        if (heli.xDelta != 0) heli.xInc = DELTA_X_INCREASE;
        break;

      default: break;
      
    }

  }

}