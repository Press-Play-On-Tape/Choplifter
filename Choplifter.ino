#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

Arduboy2Ext arduboy;
uint8_t frame = 0;
int8_t image = 1;

int8_t y = 40;
uint8_t incX = DELTA_X_DO_NOTHING;
int8_t deltaX = 0;
int8_t deltaY = 0;
uint8_t prevTurn = PREV_TURN_FROM_LEFT; 

int8_t backgroundXOffset = 0;
int16_t backgroundX = 0;

Stack <uint8_t, 20> playerStack;

Hostage hostages[NUMBER_OF_HOSTAGES];
Dormitory dormitories[NUMBER_OF_DORMITORIES];
Tank tank;
Bullet playerBullets[NUMBER_OF_PLAYER_BULLETS];
BulletExplosion playerBulletExplosion;

uint8_t dead = 0;
uint8_t safe = 0;
uint8_t inHeli = 0;

void resetGame() {

  deltaX = 0;
  deltaY = 0;
  backgroundXOffset = 0;
  backgroundX = 0;

  playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;

  dormitories[0] = { DORMITORY_STATE_OPEN,    400 };
  dormitories[1] = { DORMITORY_STATE_INTACT,  800 };
  dormitories[2] = { DORMITORY_STATE_INTACT,  1200 };
  dormitories[3] = { DORMITORY_STATE_INTACT,  1600 };

  tank = {1, 0, 5, 400 + random(100, 600), true};
  

  for (int i = 0; i < 64; i++) {
     
      hostages[i] = { (i < 16 ? HOSTAGE_LEAVING_DORM : HOSTAGE_IN_DORM), ((i % 16) * 15) + 10, dormitories[i / 16].xPos };

  }

  for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
     
      playerBullets[i] = { BULLET_INACTIVE_X_VALUE, 0, 0 };

  }

}



// -- Setup -------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(25);
  arduboy.initRandomSeed();

  resetGame();
  
}


// -- Loop -------------------------------------------------------------------------------

void loop() {

  if (!(arduboy.nextFrame())) return;
  frame++; if (frame == 4) frame = 0;
  arduboy.pollButtons();
  arduboy.clear();


  // Handle player actions ..

  if (playerStack.isEmpty()) { playerMovements(); }

  if (arduboy.justPressed(A_BUTTON) && (absT(image) <= 12 || absT(image) >=20)) {

    for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
      
      Bullet *bullet = &playerBullets[i];

      if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

        bullet->xPos = backgroundX;
        bullet->yPos = y + 19;
        bullet->yDelta = 1;
        bullet->xDelta = deltaX * 2;
        break;

      }

    }

  }


  // Update the screen and object positions ..

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


  // Change the helicopters Y position ..

  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 4) && deltaY == -4) { deltaY = -2; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 2) && deltaY == -2) { deltaY = -1; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 1) && deltaY == -1) { deltaY = 0; }

  if (y > (HELICOPTER_MINIMUM_HEIGHT - 4) && deltaY == 4)  { deltaY = 2; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 2) && deltaY == 2)  { deltaY = 1; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 1) && deltaY == 1)  { deltaY = 0; }


  // Update hostages ..

  hostageMovements();


  // Update bullets ..

  for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {

    Bullet *bullet = &playerBullets[i];

    if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {
      bullet->xPos = bullet->xPos - bullet->xDelta;
      bullet->yPos = bullet->yPos + bullet->yDelta;
      bullet->yDelta = calcSpeed(bullet->yDelta, true);

      switch (bullet->yPos) {

        case 46 ... 52:               // Hitting a hostage?

          for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {
            
            if (hostages[i].stance >= HOSTAGE_RUNNING_LEFT_1 && 
                hostages[i].stance <= HOSTAGE_WAVING_22 && 
                absT(hostages[i].xPos - bullet->xPos) < 3) {

              playerBulletExplosion.xPos = bullet->xPos;
              playerBulletExplosion.yPos = 52;
              playerBulletExplosion.explosionType = EXPLOSION_MED;
              bullet->xPos = BULLET_INACTIVE_X_VALUE;

              hostages[i].stance = HOSTAGE_DYING_2;
              dead++;

            }

          }

          break;

        case 61 ... 74:               //  Hitting the ground ..
          playerBulletExplosion.xPos = bullet->xPos;
          playerBulletExplosion.yPos = 60;
          playerBulletExplosion.explosionType = EXPLOSION_SML;
          bullet->xPos = BULLET_INACTIVE_X_VALUE;
          break;

      }

    }

  }


  // Update tank ..

  int16_t tankDif = tank.xPos - backgroundX;

  if (tank.state != TANK_STATE_DEAD) {

    switch (tank.state) {

      case TANK_STATE_MOVE_LEFT:
        tank.xPos+=2;
        break;

      case TANK_STATE_MOVE_RIGHT:
        tank.xPos-=2;
        break;

    }


    // Move turrent ..

    switch (tankDif) {

      case -999 ... -40:
        tank.turrentDirection = TANK_TURRENT_DIR_LEFT_LOW;
        break;

      case -39 ... -10:
        tank.turrentDirection = TANK_TURRENT_DIR_LEFT_MID;
        break;

      case -9 ... 9:
        tank.turrentDirection = TANK_TURRENT_DIR_UPRIGHT;
        break;

      case 10 ... 39:
        tank.turrentDirection = TANK_TURRENT_DIR_RIGHT_MID;
        break;

      case 40 ... 999:
        tank.turrentDirection = TANK_TURRENT_DIR_RIGHT_LOW;
        break;

    }


    tank.countDown--;

    if (tank.countDown == 0) {

      if (tankDif < -20) {

        tank.state = TANK_STATE_MOVE_LEFT;
        tank.countDown = random(6, 16);

      }
      else if (tankDif > 20) {

        tank.state = TANK_STATE_MOVE_RIGHT;
        tank.countDown = random(6, 16);
        
      }
      else {

        tank.state = TANK_STATE_STATIONARY;
        tank.countDown = random(6, 16);

      }

    }

  }


  // Render background ..

  for (int16_t i = backgroundXOffset; i < backgroundXOffset + 150; i = i + (int16_t)64) {
    arduboy.drawCompressedMirror(i, 43, background, WHITE, false);
  }

  drawHorizontalDottedLine(backgroundXOffset % 2, WIDTH, 51);


  // Draw base ..

  if (backgroundX > -158 && backgroundX < 105) {

    arduboy.fillRect(backgroundX + 90, 43, 42, 9, BLACK);
    arduboy.drawCompressedMirror(backgroundX + 26, 32, base, WHITE, false);

  }


  // Draw dormitories ..

  for (uint8_t i = 0; i < NUMBER_OF_DORMITORIES; i++) {

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

  for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {
    
    if ((hostages[i].xPos > backgroundX - 133) && (hostages[i].xPos < backgroundX + 133)) {

      switch (hostages[i].stance) {

        case HOSTAGE_RUNNING_LEFT_1 ... HOSTAGE_RUNNING_LEFT_4:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 48, hostage_images[hostages[i].stance - 1], WHITE, false);
          break;

        case HOSTAGE_RUNNING_RIGHT_1 ... HOSTAGE_RUNNING_RIGHT_4:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 48, hostage_images[hostages[i].stance - 5], WHITE, true);
          break;

        case HOSTAGE_WAVING_11 ... HOSTAGE_WAVING_12:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 48, hostage_05, WHITE, false);
          break;

        case HOSTAGE_WAVING_21 ... HOSTAGE_WAVING_22:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 48, hostage_06, WHITE, false);
          break;

        case HOSTAGE_DYING_1:
        case HOSTAGE_DYING_2:
          arduboy.drawCompressedMirror(backgroundX - hostages[i].xPos + 64 - 3, 48, hostage_07, WHITE, false);
          hostages[i].stance--;
          break;

      }

    }

  }


  // Draw helicopter ..

  drawHelicopter(45, y, image);


  // Draw tank ..

  if (tank.state != TANK_STATE_DEAD && (tank.xPos > backgroundX - 144) && (tank.xPos < backgroundX + 144)) {

    arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_00_mask, BLACK, false);
    arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, (tank.track ? tank_00 : tank_01), WHITE, false);

    switch (tank.turrentDirection) {

      case TANK_TURRENT_DIR_LEFT_LOW:
        arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_turrent_00, WHITE, true);
        break;

      case TANK_TURRENT_DIR_LEFT_MID:
        arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_turrent_01, WHITE, true);
        break;

      case TANK_TURRENT_DIR_UPRIGHT:
        arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_turrent_02, WHITE, false);
        break;

      case TANK_TURRENT_DIR_RIGHT_MID:
        arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_turrent_01, WHITE, false);
        break;

      case TANK_TURRENT_DIR_RIGHT_LOW:
        arduboy.drawCompressedMirror(backgroundX - tank.xPos + 64 - 15, 47, tank_turrent_00, WHITE, false);
        break;
      

    }



    if (tank.state == TANK_STATE_MOVE_LEFT || tank.state == TANK_STATE_MOVE_RIGHT) tank.track = !tank.track;

  }



  // Draw player bullets ..

  for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {

    Bullet *bullet = &playerBullets[i];

    if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {
  arduboy.setCursor(91, 10);
  arduboy.print(i);
      if ((bullet->xPos > backgroundX - 144) && (bullet->xPos < backgroundX + 144)) {

        arduboy.fillRect(backgroundX - bullet->xPos + 64 - 1, bullet->yPos, 2, 2, WHITE);

      }
   
    }

  }


  // Draw explosion ..

  if (playerBulletExplosion.xPos != BULLET_INACTIVE_X_VALUE) {

      if ((playerBulletExplosion.xPos > backgroundX - 144) && (playerBulletExplosion.xPos < backgroundX + 144)) {

        arduboy.drawCompressedMirror(backgroundX - playerBulletExplosion.xPos + 64 - 4, playerBulletExplosion.yPos, explosions[playerBulletExplosion.explosionType - 1], WHITE, false);
        playerBulletExplosion.explosionType--;
        if (playerBulletExplosion.explosionType == 0) playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;

      }

  }



  // Draw fences ..

  switch (backgroundX) {

    case 120 ... 159:
      arduboy.drawCompressedMirror(-24 + (backgroundX - 150), 1, fence_00_mask, BLACK, true);
      arduboy.drawCompressedMirror(-24 + (backgroundX - 150), 1, fence_00, WHITE, true);
      break;

    case 160 ... 169:
      arduboy.drawCompressedMirror(-16 + (backgroundX - 160), 1, fence_01_mask, BLACK, true);
      arduboy.drawCompressedMirror(-16 + (backgroundX - 160), 1, fence_01, WHITE, true);
      break;

    case 170 ... 179:
      arduboy.drawCompressedMirror(-8 + (backgroundX - 170), 1, fence_02_mask, BLACK, true);
      arduboy.drawCompressedMirror(-8 + (backgroundX - 170), 1, fence_02, WHITE, true);
      break;

    case 180 ... 189:
      arduboy.drawCompressedMirror(60 + (backgroundX - 180), 1, fence_03_mask, BLACK, false);
      arduboy.drawCompressedMirror(60 + (backgroundX - 180), 1, fence_03, WHITE, false);
      break;

    case 190 ... 199:
      arduboy.drawCompressedMirror(68 + (backgroundX - 190), 1, fence_02_mask, BLACK, false);
      arduboy.drawCompressedMirror(68 + (backgroundX - 190), 1, fence_02, WHITE, false);
      break;

    case 200 ... 209:
      arduboy.drawCompressedMirror(76 + (backgroundX - 200), 1, fence_01_mask, BLACK, false);
      arduboy.drawCompressedMirror(76 + (backgroundX - 200), 1, fence_01, WHITE, false);
      break;

    case 210 ... 249:
      arduboy.drawCompressedMirror(84 + (backgroundX - 210), 1, fence_00_mask, BLACK, false);
      arduboy.drawCompressedMirror(84 + (backgroundX - 210), 1, fence_00, WHITE, false);
      break;
    
  }

  // Draw scoreboard ..

  arduboy.setCursor(1, 0);
  arduboy.print(dead);
  arduboy.setCursor(16, 0);
  arduboy.print(inHeli);
  arduboy.setCursor(31, 0);
  arduboy.print(safe);


  // arduboy.setCursor(1, 10);
  // arduboy.print(tankDif);
  // arduboy.setCursor(31, 10);
  // arduboy.print(tank.turrentDirection);
  // arduboy.setCursor(61, 10);
  // arduboy.print(tank.state);

//  arduboy.drawCompressedMirror(0, 0, score_left_mask, BLACK, false);
//  arduboy.drawCompressedMirror(0, 0, score_left, WHITE, false);



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

  arduboy.display();
  
}


