#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

Arduboy2Ext arduboy;

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

Helicopter heli;

uint8_t frame = 0;
int8_t backgroundXOffset = 0;

Stack <uint8_t, 10> playerStack;

Hostage hostages[NUMBER_OF_HOSTAGES];
Dormitory dormitories[NUMBER_OF_DORMITORIES];
Tank tanks[NUMBER_OF_TANKS];
Bullet playerBullets[NUMBER_OF_PLAYER_BULLETS];
Bullet tankBullets[NUMBER_OF_TANK_BULLETS];
BulletExplosion playerBulletExplosion;
BulletExplosion tankBulletExplosion;

uint8_t dead = 0;
uint8_t safe = 0;
uint8_t inHelicopter = 0;
uint8_t sortieNumber = 0;

GameState gameState = GameState::Initialise;

void resetSortie() {

  heli.stance = 1;
  heli.xDelta = 0;
  heli.yDelta = 0;
  heli.xInc = DELTA_X_DO_NOTHING;
  heli.xPos = 0;
  heli.yPos = 40;
  heli.prevTurn = PREV_TURN_FROM_LEFT; 
  heli.hits = 0; 
  heli.countDown = 0;

  tankBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;
  playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;
  backgroundXOffset = 0;
  playerStack.clear();

}

void resetGame() {

  resetSortie();

  sortieNumber = 1;

  dormitories[0] = { DormitoryState::Open,    DORMITORY_SPACING };
  dormitories[1] = { DormitoryState::Intact,  (DORMITORY_SPACING * 2) };
  dormitories[2] = { DormitoryState::Intact,  (DORMITORY_SPACING * 3) };
  dormitories[3] = { DormitoryState::Intact,  (DORMITORY_SPACING * 4) };
/* SJH
  tanks[0] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (1 * TANK_SPACING) + random(100, 600), true};
  tanks[1] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (2 * TANK_SPACING) + random(100, 600), true};
  tanks[2] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (3 * TANK_SPACING) + random(100, 600), true};
  tanks[3] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (4 * TANK_SPACING) + random(100, 600), true};
  tanks[4] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (5 * TANK_SPACING) + random(100, 600), true};
  tanks[5] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (6 * TANK_SPACING) + random(100, 600), true};
 */  

  tanks[0] = {TankState::Stationary, TurrentDirection::Left_Low, 5, (1 * TANK_SPACING) + random(100, 600), true};
  tanks[1] = {TankState::Dead1, TurrentDirection::Left_Low, 5, (2 * TANK_SPACING) + random(100, 600), true};
  tanks[2] = {TankState::Dead1, TurrentDirection::Left_Low, 5, (3 * TANK_SPACING) + random(100, 600), true};
  tanks[3] = {TankState::Dead1, TurrentDirection::Left_Low, 5, (4 * TANK_SPACING) + random(100, 600), true};
  tanks[4] = {TankState::Dead1, TurrentDirection::Left_Low, 5, (5 * TANK_SPACING) + random(100, 600), true};
  tanks[5] = {TankState::Dead1, TurrentDirection::Left_Low, 5, (6 * TANK_SPACING) + random(100, 600), true};

  for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {

      hostages[i] = { (i < 1 /*SJH 6*/ ? HostageStance::Leaving_Dorm : HostageStance::In_Dorm), ((i % 16) * 15) + 10, dormitories[i / 16].xPos };

  }

  for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
     
      playerBullets[i] = { BULLET_INACTIVE_X_VALUE, 0, 0, 0, 0 };

  }

  for (int i = 0; i < NUMBER_OF_TANK_BULLETS; i++) {
     
      tankBullets[i] = { BULLET_INACTIVE_X_VALUE, 0, 0, 0, 0 };

  }

  gameState = GameState::Introduction;

}



// -- Setup -------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.setFrameRate(25);
  arduboy.initRandomSeed();

  resetGame();
  gameState = GameState::Initialise;
  
}


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

void introduction() {

  arduboy.drawCompressedMirror(16, 21, choplifter, WHITE, false);
  if (arduboy.justPressed(A_BUTTON)) { gameState = GameState::Sortie; }

}

void gameOver() {

  arduboy.drawCompressedMirror(45, 21, the_end, WHITE, false);
  drawScoreBoard();
  if (arduboy.justPressed(A_BUTTON)) { gameState = GameState::Initialise; }

}


// -- Play Choplifter ! -------------------------------------------------------------------

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


  // Update bullets ..

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




  // Update bullets ..

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
  
  if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 4) && heli.yDelta == -4) { heli.yDelta = -2; }
  if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 2) && heli.yDelta == -2) { heli.yDelta = -1; }
  if (heli.yPos < (HELICOPTER_MAXIMUM_HEIGHT + 1) && heli.yDelta == -1) { heli.yDelta = 0; }

  if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 4) && heli.yDelta == 4)  { heli.yDelta = 2; }
  if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 2) && heli.yDelta == 2)  { heli.yDelta = 1; }
  if (heli.yPos > (HELICOPTER_MINIMUM_HEIGHT - 1) && heli.yDelta == 1)  { heli.yDelta = 0; }
  
}


void bulletHit(Bullet *bullet, BulletExplosion *explosion, bool playerBullet) {


  // Check to see if a tank bullet hit the helicopter (it can be at any height) ..
  
  {
    Rect hitZone = {50, 0, 0, 0};

    if (!playerBullet) {

      switch (absT(heli.stance)) {

        case 1 ... 3:
        case 13 ... 17:
        case 20:
    
          hitZone.x = 50;
          hitZone.y = heli.yPos + 5;
          hitZone.width = 28;
          hitZone.height = 12;

          break;

        case 4 ... 6:
    
          hitZone.x = 50;
          hitZone.y = heli.yPos + 7;
          hitZone.width = 28;
          hitZone.height = 12;

          break;

        case 7 ... 12:
        case 18 ... 19:

          hitZone.x = 57;
          hitZone.y = heli.yPos + 5;
          hitZone.width = 14;
          hitZone.height = 13;

          break;

      }

      Point pt = { 64 + bullet->xPos - heli.xPos, bullet->yPos };

      if (arduboy.collide(pt, hitZone)) {

        explosion->xPos = bullet->xPos;
        explosion->yPos = hitZone.y - 4;
        explosion->explosionType = ExplosionType::Both_Medium;
        bullet->xPos = BULLET_INACTIVE_X_VALUE;

        heli.hits++;

        if (heli.hits == TANK_BULLET_NUMBER_OF_HITS) {

          explosion->explosionType = ExplosionType::Large_1;
          heli.countDown++;


          dead = dead + inHelicopter;
          inHelicopter = 0;

          for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {

            Hostage *hostage = &hostages[i];
            if (hostage->stance == HostageStance::In_Helicopter) hostage->stance = HostageStance::Dead;

          }

        }

      }

    }

  }



  // Check to see if we hit anything ..

  switch (bullet->yPos) {

    case 40 ... 46:               // Hitting a dormitory?

      for (uint8_t i = 0; i < NUMBER_OF_DORMITORIES; i++) {

        Dormitory *dormitory = &dormitories[i];
        uint16_t diff = dormitory->xPos - bullet->xPos;

        if (dormitory->state == DormitoryState::Intact && diff < 16) {

            dormitory->numberOfHits++;
            
            explosion->xPos = bullet->xPos;
            explosion->yPos = 36 + (diff / 3);
            explosion->explosionType = ExplosionType::Medium;

            if (dormitory->numberOfHits == DORMITORY_HITS_REQUIRED) {

            dormitory->state = DormitoryState::Open;
            explosion->xPos = bullet->xPos;
            explosion->yPos = 36 + (diff / 3);
            explosion->explosionType = ExplosionType::Large_1;


            // Release the hostages ..

            uint8_t count = 0;

            for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {

              Hostage *hostage = &hostages[i];

              if (hostage->stance == HostageStance::In_Dorm) {
                
                hostage->stance = HostageStance::Leaving_Dorm;
                hostage->xPos = dormitory->xPos + 16;
                hostage->countDown = (count * 15) + 10;
                count++;

                if (count == DORMITORY_HOSTAGE_CAPACITY) { break; }

              }

            }

            } 

            bullet->xPos = BULLET_INACTIVE_X_VALUE;

        }

      }

      break;

    case 47 ... 52:               // Hitting a hostage or the tank?
      {
        bool hit = false;

        for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {

          Hostage *hostage = &hostages[i];
          uint16_t diff = hostage->xPos - bullet->xPos;

          if (hostage->stance >= HostageStance::Running_Left_1 && 
              hostage->stance <= HostageStance::Waving_22 && 
              diff < 3) {

            explosion->xPos = bullet->xPos;
            explosion->yPos = 52;
            explosion->explosionType = ExplosionType::Medium;
            bullet->xPos = BULLET_INACTIVE_X_VALUE;

            hostage->stance = HostageStance::Dying_2;
            dead++;
            hit = true;
            break;

          }

        }

        if (!hit && playerBullet) {

          for (uint8_t i = 0; i < NUMBER_OF_TANKS; i++) {

            Tank *tank = &tanks[i];
            uint16_t diff = tank->xPos - bullet->xPos;

//SJH                if (bullet->startYPos > TANK_BULLET_MIN_Y_VALUE && diff < 16) {  // Bullets that hit tank must be fired from down low ..
            if (diff < 16) {  

              explosion->xPos = bullet->xPos;
              explosion->yPos = (diff < 8 ? 46 : 51);
              explosion->explosionType = ExplosionType::Medium;
              bullet->xPos = BULLET_INACTIVE_X_VALUE;

              tank->numberOfHits++;

              if (tank->numberOfHits == TANK_BULLET_NUMBER_OF_HITS) {

                explosion->yPos = 50;
                explosion->explosionType = ExplosionType::Large_1;
                tank->state = TankState::Dead3;

              }

            }

          }

        }

      }
      break;

    case 61 ... 74:               //  Hitting the ground ..
      explosion->xPos = bullet->xPos;
      explosion->yPos = 60;
      explosion->explosionType = ExplosionType::Small;
      bullet->xPos = BULLET_INACTIVE_X_VALUE;
      break;

  }

}
