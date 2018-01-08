#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/StackArray.h"

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

StackArray <uint8_t> playerStack;


// -- Setup -------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.setFrameRate(25);
  
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



  // Render background ..

  for (int16_t i = backgroundXOffset; i < backgroundXOffset + 150; i = i + (int16_t)64) {
    arduboy.drawCompressedMirror(i, 44, background, WHITE, false);
  }

  drawHorizontalDottedLine(backgroundXOffset % 2, WIDTH, 52);

  if (backgroundX > -158 && backgroundX < 105) {

    arduboy.fillRect(backgroundX + 91, 44, 43, 8, BLACK);
    arduboy.drawCompressedMirror(backgroundX + 27, 33, base, WHITE, false);

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

  #endif

  // ----------------------------------------------------------------------------------------

  drawHelicopter(45, y, image);
  arduboy.display();
  
}


