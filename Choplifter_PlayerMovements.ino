#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

void playerMovements() {


  // Up and down movements ..

  if (arduboy.pressed(UP_BUTTON)) {

    if (y > HELICOPTER_MAXIMUM_HEIGHT) {

      deltaY = calcSpeed(deltaY, false);  

    }

  } 
  
  else {
    
    if (y < HELICOPTER_MINIMUM_HEIGHT) {

      deltaY = calcSpeed(deltaY, true);  

    }

  }


  // Turn the helicoper ..

  if (arduboy.pressed(B_BUTTON)) {

    if (deltaX == 0) {

      switch (image) {

        case 1:
          playerStack.push(10, 9);
          playerStack.push(8, 7);
          prevTurn = PREV_TURN_FROM_LEFT;
          break;

        case -1:
          playerStack.push(10, -9);
          playerStack.push(-8, -7);
          prevTurn = PREV_TURN_FROM_RIGHT;
          break;

        case 10:

          if (prevTurn == PREV_TURN_FROM_LEFT) {
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

      switch (image) {

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

    switch (image) {

      case 1:
        playerStack.push(6, 5, 4);
        playerStack.push(3, 2);
        incX = DELTA_X_DECREASE;          
        break;

      case 10:
        playerStack.push(12, 11);
        incX = DELTA_X_DECREASE;          
        break;

      case 17:
        playerStack.push(6, 5, 4);
        playerStack.push(3, 2, 1);
        playerStack.push(13, 14);
        playerStack.push(15, 16);
        incX = DELTA_X_DECREASE;          
        break;

      case -1:
        playerStack.push(-17, -16, -15);
        playerStack.push(-14, -13);
        incX = DELTA_X_DECREASE;          
        break;

      case -6:
        playerStack.push(-17, -16, -15);
        playerStack.push(-14, -13);
        playerStack.push(-1, -2, -3);
        playerStack.push(-4, -5);
        incX = DELTA_X_DECREASE;          
        break;

      case -12:
        playerStack.push(12, 11);
        playerStack.push(10, -11);
        incX = DELTA_X_INCREASE;          
        break;

      default:
        incX = DELTA_X_DECREASE;          
        break;
      
    }

  } 

  else if (arduboy.pressed(RIGHT_BUTTON)) {

    switch (image) {

      case -1:
        playerStack.push(-6, -5, -4);
        playerStack.push(-3, -2);
        incX = DELTA_X_INCREASE; 
        break;

      case 10:
        playerStack.push(-12, -11);
        incX = DELTA_X_INCREASE; 
        break;

      case -17:
        playerStack.push(-6, -5, -4);
        playerStack.push(-3, -2, -1);
        playerStack.push(-13, -14);
        playerStack.push(-15, -16);
        incX = DELTA_X_INCREASE; 
        break;

      case 1:
        playerStack.push(17, 16, 15);
        playerStack.push(14, 13);
        incX = DELTA_X_INCREASE;       
        break;

      case 6:
        playerStack.push(17, 16, 15);
        playerStack.push(14, 13);
        playerStack.push(1, 2, 3);
        playerStack.push(4, 5);
        incX = DELTA_X_INCREASE;       
        break;

      case 12:
        playerStack.push(-12, -11);
        playerStack.push(10, 11);
        incX = DELTA_X_DECREASE;       
        break;

      default:
        incX = DELTA_X_INCREASE; 
        break;

    }

  } 
  else {  // Return from current position to upright ..

    switch (image) {

      case 12:
        playerStack.push(10, 11);
        if (deltaX != 0) incX = DELTA_X_INCREASE;
        break;

      case -12:
        playerStack.push(10, -11);
        if (deltaX != 0) incX = DELTA_X_DECREASE;
        break;

      case 6:
        playerStack.push(1, 2, 3);
        playerStack.push(4, 5);
        if (deltaX != 0) incX = DELTA_X_INCREASE;
        break;

      case -6:
        playerStack.push(-1, -2, -3);
        playerStack.push(-4, -5);
        if (deltaX != 0) incX = DELTA_X_DECREASE;
        break;
      
      case 17:
        playerStack.push(1, 13, 14);
        playerStack.push(15, 16);
        if (deltaX != 0) incX = DELTA_X_DECREASE;
        break;

      case -17:
        playerStack.push(-1, -13, -14);
        playerStack.push(-15, -16);
        if (deltaX != 0) incX = DELTA_X_INCREASE;
        break;

      default: break;
      
    }

  }

}