#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

Arduboy2Ext arduboy;
uint8_t frame = 0;
int8_t image = 1;

int8_t y = 32;
uint8_t incX = DELTA_X_DO_NOTHING;
int8_t deltaX = 0;
int8_t deltaY = 0;
uint8_t prevTurn = PREV_TURN_FROM_LEFT; 

int8_t backgroundXOffset = 0;
int16_t backgroundX = 0;

Stack <uint8_t, 20> playerStack;

Hostage hostages[64];
Dormitory dormitories[4];



void resetGame() {

  deltaX = 0;
  deltaY = 0;
  backgroundXOffset = 0;
  backgroundX = 0;


  dormitories[0] = { DORMITORY_STATE_OPEN,    400 };
  dormitories[1] = { DORMITORY_STATE_INTACT,  800 };
  dormitories[2] = { DORMITORY_STATE_INTACT, 1200 };
  dormitories[3] = { DORMITORY_STATE_INTACT, 1600 };

  for (int i = 0; i < 64; i++) {
     
      hostages[i] = { (i < 16 ? HOSTAGE_LEAVING_DORM : HOSTAGE_IN_DORM), random(0, 256), dormitories[i / 16].xPos };

  }

}



// -- Setup -------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.setFrameRate(25);
  resetGame();
  
}


// -- Loop -------------------------------------------------------------------------------

void loop() {

  if (!(arduboy.nextFrame())) return;
  frame++; if (frame == 4) frame = 0;
  arduboy.pollButtons();
  arduboy.clear();

  if (playerStack.isEmpty()) {

    playerMovements();

  }

  if (!playerStack.isEmpty()) {

    if (arduboy.everyXFrames(2)) {
      image = playerStack.pop();
    }

  }

  if (arduboy.everyXFrames(4)) {
  
    if (incX == DELTA_X_INCREASE)     { deltaX = calcSpeed(deltaX, true); }
    if (incX == DELTA_X_DECREASE)     { deltaX = calcSpeed(deltaX, false); }
    if (deltaX == 0)                  { incX = DELTA_X_DO_NOTHING; }
  
  }

  y = y + deltaY;

  backgroundXOffset = backgroundXOffset - deltaX;
  backgroundX = backgroundX - deltaX;
  if (backgroundXOffset < -64) backgroundXOffset = backgroundXOffset + 64;
  if (backgroundXOffset > 0) backgroundXOffset = backgroundXOffset - 64;


  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 4) && deltaY == -4)  { deltaY = -2; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 2) && deltaY == -2) { deltaY = -1; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 1) && deltaY == -1) { deltaY = 0; }

  if (y > (HELICOPTER_MINIMUM_HEIGHT - 4) && deltaY == 4)  { deltaY = 2; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 2) && deltaY == 2) { deltaY = 1; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 1) && deltaY == 1) { deltaY = 0; }



  // Update hostages ..

  int randomTopLimit = 0;

  for (int i = 0; i < 64; i++) {

    Hostage *hostage = &hostages[i];

    switch (hostage->stance) {

      case HOSTAGE_RUNNING_LEFT_1:
      case HOSTAGE_RUNNING_LEFT_2:
      case HOSTAGE_RUNNING_LEFT_3:

        hostage->stance++;
        hostage->countDown--;
        hostage->xPos++;
        randomTopLimit = HOSTAGE_WAVING_22;
        break;

      case HOSTAGE_RUNNING_LEFT_4:

        hostage->stance = HOSTAGE_RUNNING_LEFT_1;
        hostage->countDown--;
        hostage->xPos++;
        randomTopLimit = HOSTAGE_WAVING_22;
        break;

      case HOSTAGE_RUNNING_RIGHT_1:
      case HOSTAGE_RUNNING_RIGHT_2:
      case HOSTAGE_RUNNING_RIGHT_3:

        hostage->stance++;
        hostage->countDown--;
        hostage->xPos--;
        randomTopLimit = HOSTAGE_WAVING_22;
        break;

      case HOSTAGE_RUNNING_RIGHT_4:

        hostage->stance = HOSTAGE_RUNNING_RIGHT_1;
        hostage->countDown--;
        hostage->xPos--;
        randomTopLimit = HOSTAGE_WAVING_22;
        break;

      case HOSTAGE_WAVING_11:
      case HOSTAGE_WAVING_12:

        hostage->stance++;
        hostage->countDown--;
        randomTopLimit = HOSTAGE_RUNNING_RIGHT_4;
        break;

      case HOSTAGE_WAVING_21:
      case HOSTAGE_WAVING_22:

        hostage->stance++;  
        if (hostage->stance > HOSTAGE_WAVING_22) hostage->stance = HOSTAGE_WAVING_11;
        hostage->countDown--;
        randomTopLimit = HOSTAGE_RUNNING_RIGHT_4;
        break;

      case HOSTAGE_LEAVING_DORM:

        hostage->countDown--;
        randomTopLimit = HOSTAGE_RUNNING_RIGHT_4;
        break;

    }


    if (hostage->countDown == 0) {

      hostage->stance = random(HOSTAGE_RUNNING_LEFT_1, randomTopLimit + 1);
      hostage->countDown = random(0, 6);

    }

  }





  // Render background ..

  for (int16_t i = backgroundXOffset; i < backgroundXOffset + 150; i = i + (int16_t)64) {
    arduboy.drawCompressedMirror(i, 43, background, WHITE, false);
  }

  drawHorizontalDottedLine(backgroundXOffset % 2, WIDTH, 51);


  // Draw base ..

  if (backgroundX > -158 && backgroundX < 105) {

    arduboy.fillRect(backgroundX + 91, 42, 42, 8, BLACK);
    arduboy.drawCompressedMirror(backgroundX + 27, 31, base, WHITE, false);

  }


  // Draw dormitories ..

  for (int i = 0; i < 4; i++) {

    if ((dormitories[i].xPos > backgroundX - 144) && (dormitories[i].xPos < backgroundX + 144)) {

      if (dormitories[i].state == DORMITORY_STATE_INTACT) {

        arduboy.drawCompressedMirror(backgroundX - dormitories[i].xPos + 64 - 15, 40, dormitory_01_mask, BLACK, false);
        arduboy.drawCompressedMirror(backgroundX - dormitories[i].xPos + 64 - 15, 40, dormitory_01, WHITE, false);

      }
      else {

        arduboy.drawCompressedMirror(backgroundX - dormitories[i].xPos + 64 - 15, 40, dormitory_02_mask, BLACK, false);
        arduboy.drawCompressedMirror(backgroundX - dormitories[i].xPos + 64 - 15, 40, dormitory_02, WHITE, false);
        
      }

    }

  }


  // Draw hostages ..

  for (int i = 0; i < 64; i++) {
    
    if ((hostages[i].xPos > backgroundX - 133) && (hostages[i].xPos < backgroundX + 133)) {

      switch (hostages[i].stance) {

        case HOSTAGE_RUNNING_LEFT_1 ... HOSTAGE_RUNNING_LEFT_4:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 53, hostage_images[hostages[i].stance - 1], WHITE, false);
          break;

        case HOSTAGE_RUNNING_RIGHT_1 ... HOSTAGE_RUNNING_RIGHT_4:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 53, hostage_images[hostages[i].stance - 5], WHITE, true);
          break;

        case HOSTAGE_WAVING_11 ... HOSTAGE_WAVING_12:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 53, hostage_05, WHITE, false);
          break;

        case HOSTAGE_WAVING_21 ... HOSTAGE_WAVING_22:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 53, hostage_06, WHITE, false);
          break;

      }

    }

  }

  // -- DEBUG -------------------------------------------------------------------------------

  #ifdef DEBUG

    arduboy.setCursor(1, 0);
    arduboy.print(image);

    arduboy.setCursor(20, 0);
    arduboy.print(deltaX);
    arduboy.setCursor(35, 0);
    arduboy.print(deltaY);
    arduboy.setCursor(50, 0);
    arduboy.print(backgroundXOffset);
    arduboy.setCursor(70, 0);
    arduboy.print(incX);

    // arduboy.setCursor(1, 0);
    // arduboy.print(hostages[0].xPos);

    // arduboy.setCursor(25, 0);
    // arduboy.print(hostages[0].countDown);

    // arduboy.setCursor(50, 0);
    // arduboy.print(hostages[0].stance);

  #endif

  // ----------------------------------------------------------------------------------------

  drawHelicopter(45, y, image);
  arduboy.display();
  
}


