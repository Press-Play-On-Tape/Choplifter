#include "src/utils/Arduboy2Ext.h"
#include <ArduboyTones.h>
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"
#include "src/sounds/Sounds.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.on);

Helicopter heli;

uint8_t frame = 0;
int8_t backgroundXOffset = 0;
uint8_t dead = 0;
uint8_t safe = 0;
uint8_t inHelicopter = 0;
uint8_t sortieNumber = 0;

Stack <uint8_t, 10> playerStack;

Hostage hostages[NUMBER_OF_HOSTAGES];
Dormitory dormitories[NUMBER_OF_DORMITORIES];
Tank tanks[NUMBER_OF_TANKS];
Bullet playerBullets[NUMBER_OF_PLAYER_BULLETS];
Bullet tankBullets[NUMBER_OF_TANK_BULLETS];
BulletExplosion playerBulletExplosion;
BulletExplosion tankBulletExplosion;

GameState gameState = GameState::Initialise;


// --------------------------------------------------------------------------------------
//  Setup ..
//
void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(25);
  arduboy.initRandomSeed();

  resetGame();
  gameState = GameState::Initialise;
  
}


// --------------------------------------------------------------------------------------
//  Main Loop ..
//
void loop() {

  if (!(arduboy.nextFrame())) return;
  frame++; if (frame == 4) frame = 0;
  arduboy.pollButtons();
  arduboy.clear();


  switch (gameState) {

    case GameState::Initialise:
      resetGame();
      break;

    case GameState::Introduction:
      introduction();
      break;

    case GameState::Sortie:
      resetSortie();
      render(sortieNumber);
      
      if (arduboy.justPressed(A_BUTTON)) { gameState = GameState::PlayGame; }
      break;

    case GameState::PlayGame:
      play();
      break;

    case GameState::GameOver:
      gameOver();
      break;

  }


  arduboy.display();

}


// --------------------------------------------------------------------------------------
//  Introduction ..
//
void introduction() {

  arduboy.drawCompressedMirror(16, 24, choplifter, WHITE, false);
  if (arduboy.justPressed(A_BUTTON)) { sound.tones(introduction_sound); gameState = GameState::Sortie; }

}


// --------------------------------------------------------------------------------------
//  Game Over ..
//
void gameOver() {

  arduboy.drawCompressedMirror(45, 21, the_end, WHITE, false);
  drawScoreBoard(false);
  if (arduboy.justPressed(A_BUTTON)) { gameState = GameState::Initialise; }

}


// --------------------------------------------------------------------------------------
//  Play Choplifter !
//
void play() {


  // Handle player actions ..

  if (playerStack.isEmpty()) { playerMovements(); }

  if (arduboy.justPressed(A_BUTTON) && (absT(heli.stance) <= 12 || absT(heli.stance) >=20)) {

    switch (absT(heli.stance)) {

      case 1 ... 3:
      case 13 ... 17:

        for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
          
          Bullet *bullet = &playerBullets[i];

          if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

            sound.tones(player_firing);

            bullet->xPos = heli.xPos + (heli.stance < 0 ? -13 : 13);
            bullet->yPos = heli.yPos + 15;
            bullet->yDelta = BULLET_SHOOT_HORIZONTAL;
            bullet->xDelta = (heli.stance < 0 ? 8 : -8);
            bullet->startYPos = bullet->yPos;
            break;

          }

        }

        break;

      default:

        for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
          
          Bullet *bullet = &playerBullets[i];

          if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

            sound.tones(player_firing);

            bullet->xPos = heli.xPos;
            bullet->yPos = heli.yPos + 19;
            bullet->yDelta = 1;
            bullet->xDelta = heli.xDelta * 2;
            bullet->startYPos = bullet->yPos;
            break;

          }

        }

        break;

    }

  }


  // Update the screen and object positions ..

  if (!playerStack.isEmpty()) {

    if (arduboy.everyXFrames(2)) {
      heli.stance = playerStack.pop();
    }

  }

  if (arduboy.everyXFrames(4)) {
  
    if (heli.xInc == DELTA_X_INCREASE)     { heli.xDelta = calcSpeed(heli.xDelta, true); }
    if (heli.xInc == DELTA_X_DECREASE)     { heli.xDelta = calcSpeed(heli.xDelta, false); }
    if (heli.xDelta == 0)                  { heli.xInc = DELTA_X_DO_NOTHING; }
  
  }


  // Change the helicopter's position ..

  switch (heli.yDelta) {

    case -8 ... -1:                         // Going up!

      if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 4) && heli.yDelta == -4) { heli.yDelta = -2; }
      if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 2) && heli.yDelta == -2) { heli.yDelta = -1; }
      if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 1) && heli.yDelta == -1) { heli.yDelta = 0; }

      heli.yPos = heli.yPos + heli.yDelta;
      break;

    case 0:
      break;

    case 1 ... 8:                           // We are falling ..

      if (heli.yPos < (HELICOPTER_MINIMUM_HEIGHT - heli.yDelta)) {

        heli.yPos = heli.yPos + heli.yDelta;

      }
      else {                                // Almost about to land or crash!

        switch (heli.stance) {

          case 1 ... 3:
          case 7 ... 10:
          case 13 ... 14:
          case 11:       
              
            if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 4) && heli.yDelta == 4)  { heli.yDelta = 2; }
            if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 2) && heli.yDelta == 2)  { heli.yDelta = 1; }
            if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 1) && heli.yDelta == 1)  { heli.yDelta = 0; }          

            heli.yPos = heli.yPos + heli.yDelta;
            tankBulletExplosion.yPos = tankBulletExplosion.yPos + heli.yDelta;
            break;

          default:

            if (heli.yDelta == 4 && absT(heli.xDelta) >= 2) {

              sound.tones(exploding);

              heli.yPos = (HELICOPTER_MINIMUM_HEIGHT - heli.yDelta);
              tankBulletExplosion.xPos = heli.xPos + 16;
              tankBulletExplosion.yPos = heli.yPos + 4;
              tankBulletExplosion.explosionType = ExplosionType::Large_1;
              heli.yDelta = 0;
              heli.countDown = 1;


              // Kill any hostages that were in the helicopter ..

              dead = dead + inHelicopter;
              inHelicopter = 0;

              for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {

                Hostage *hostage = &hostages[i];
                if (hostage->stance == HostageStance::In_Helicopter) hostage->stance = HostageStance::Dead;

              }

            }
            else {
              
              if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 4) && heli.yDelta == 4)  { heli.yDelta = 2; }
              if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 2) && heli.yDelta == 2)  { heli.yDelta = 1; }
              if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 1) && heli.yDelta == 1)  { heli.yDelta = 0; }

              heli.yPos = heli.yPos + heli.yDelta;
              
            }

            break;

        }

      }

      break;

  }



  heli.yPos = heli.yPos + heli.yDelta;
  tankBulletExplosion.yPos = tankBulletExplosion.yPos + heli.yDelta;
  backgroundXOffset = backgroundXOffset - heli.xDelta;
  heli.xPos = heli.xPos - heli.xDelta;
  if (backgroundXOffset < -64) backgroundXOffset = backgroundXOffset + 64;
  if (backgroundXOffset > 0)   backgroundXOffset = backgroundXOffset - 64;


  // Did we just land on hostages?

  if (heli.yPos - heli.yDelta < 38 && heli.yPos > 38) {  // Check to see if we just landed.

    for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {

      Hostage *hostage = &hostages[i];
      int16_t diff = absT(hostage->xPos - heli.xPos);

      if (hostage->stance >= HostageStance::Running_Left_1 && hostage->stance <= HostageStance::Waving_22) {

        switch (absT(heli.stance)) {

          case 1 ... 6:
          case 13 ... 17:
          case 20:
      
            if (diff < 7) {

              hostage->stance = HostageStance::Dying_2;
              dead++;

            }

            break;

          case 7 ... 12:
          case 18 ... 19:

            if (diff < 4) {

              hostages->stance = HostageStance::Dying_2;
              dead++;

            }

            break;

        }

      }

    }

  }


  // Update hostages ..

  hostageMovements();


  // Update player bullets ..

  for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {

    Bullet *bullet = &playerBullets[i];

    if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {


      // Update bullet position ..

      bullet->xPos = bullet->xPos - bullet->xDelta;
      if (absT(bullet->xPos - heli.xPos) > 70) { bullet->xPos = BULLET_INACTIVE_X_VALUE; }

      if (bullet->yDelta != BULLET_SHOOT_HORIZONTAL) {
        bullet->yPos = bullet->yPos + bullet->yDelta;
        bullet->yDelta = calcSpeed(bullet->yDelta, true);
      }



      // Check to see if we hit anything ..

      bulletHit(bullet, &playerBulletExplosion, true);

    }

  }




  // Update tank bullets ..

  for (int i = 0; i < NUMBER_OF_TANK_BULLETS; i++) {

    Bullet *bullet = &tankBullets[i];

    if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {


      // Update bullet position ..

      bullet->xPos = bullet->xPos - bullet->xDelta;
      if (absT(bullet->xPos - heli.xPos) > 70) { bullet->xPos = BULLET_INACTIVE_X_VALUE; }

      bullet->yPos = bullet->yPos + bullet->yDelta;
      bullet->yDelta = calcSpeed(bullet->yDelta, true);


      // Check to see if we hit anything?

      bulletHit(bullet, &tankBulletExplosion, false);

    }

  }


  // Update tank movements ..

  tankMovements();


  // Render screen ..

  render(0);


  // -- DEBUG -------------------------------------------------------------------------------

  #ifdef DEBUG

    arduboy.setCursor(1, 10);
    arduboy.print(image);

    arduboy.setCursor(20, 10);
    arduboy.print(deltaX);
    arduboy.setCursor(35, 10);
    arduboy.print(deltaY);
    arduboy.setCursor(50, 10);
    arduboy.print(backgroundXOffset);
    arduboy.setCursor(70, 10);
    arduboy.print(y);

  #endif

  // ----------------------------------------------------------------------------------------
  
  // switch (absT(heli.stance)) {

  //   case 1 ... 4:
  //   case 7 ... 10:
  //   case 13 ... 14:
  //   case 11:

  //     if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 4) && heli.yDelta == -4) { heli.yDelta = -2; }
  //     if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 2) && heli.yDelta == -2) { heli.yDelta = -1; }
  //     if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 1) && heli.yDelta == -1) { heli.yDelta = 0; }

  //     // if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 4) && heli.yDelta == 4)  { heli.yDelta = 2; }
  //     // if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 2) && heli.yDelta == 2)  { heli.yDelta = 1; }
  //     // if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 1) && heli.yDelta == 1)  { heli.yDelta = 0; }

  //     break;

  // }


}


