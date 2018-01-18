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
uint8_t hits = 0;

int8_t backgroundXOffset = 0;
int16_t backgroundX = 0;

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
uint8_t inHeli = 0;

GameState gameState = GameState::Initialise;
uint8_t sortieNumber = 0;

void resetGame() {

  sortieNumber = 1;
  deltaX = 0;
  deltaY = 0;
  backgroundXOffset = 0;
  backgroundX = 0;

  playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;

  dormitories[0] = { DORMITORY_STATE_OPEN,    DORMITORY_SPACING };
  dormitories[1] = { DORMITORY_STATE_INTACT,  (DORMITORY_SPACING * 2) };
  dormitories[2] = { DORMITORY_STATE_INTACT,  (DORMITORY_SPACING * 3) };
  dormitories[3] = { DORMITORY_STATE_INTACT,  (DORMITORY_SPACING * 4) };
/* SJH
  tanks[0] = {TANK_STATE_STATIONARY, 0, 5, (1 * TANK_SPACING) + random(100, 600), true};
  tanks[1] = {TANK_STATE_STATIONARY, 0, 5, (2 * TANK_SPACING) + random(100, 600), true};
  tanks[2] = {TANK_STATE_STATIONARY, 0, 5, (3 * TANK_SPACING) + random(100, 600), true};
  tanks[3] = {TANK_STATE_STATIONARY, 0, 5, (4 * TANK_SPACING) + random(100, 600), true};
  tanks[4] = {TANK_STATE_STATIONARY, 0, 5, (5 * TANK_SPACING) + random(100, 600), true};
  tanks[5] = {TANK_STATE_STATIONARY, 0, 5, (6 * TANK_SPACING) + random(100, 600), true};
*/  
  tanks[0] = {TANK_STATE_STATIONARY, 0, 5, (1 * TANK_SPACING) + random(100, 600), true};
  tanks[1] = {TANK_STATE_DEAD_1, 0, 5, (2 * TANK_SPACING) + random(100, 600), true};
  tanks[2] = {TANK_STATE_DEAD_1, 0, 5, (3 * TANK_SPACING) + random(100, 600), true};
  tanks[3] = {TANK_STATE_DEAD_1, 0, 5, (4 * TANK_SPACING) + random(100, 600), true};
  tanks[4] = {TANK_STATE_DEAD_1, 0, 5, (5 * TANK_SPACING) + random(100, 600), true};
  tanks[5] = {TANK_STATE_DEAD_1, 0, 5, (6 * TANK_SPACING) + random(100, 600), true};

  for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {
     
      hostages[i] = { (i < 1 /*SJH 6*/ ? HOSTAGE_LEAVING_DORM : HOSTAGE_IN_DORM), ((i % 16) * 15) + 10, dormitories[i / 16].xPos };

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

  gameState = GameState::Sortie;

}

void gameOver() {

  arduboy.drawCompressedMirror(45, 21, the_end, WHITE, false);
  if (arduboy.justPressed(A_BUTTON)) { gameState = GameState::Initialise; }

}


// -- Play Choplifter ! -------------------------------------------------------------------

void play() {


  // Handle player actions ..

  if (playerStack.isEmpty()) { playerMovements(); }

  if (arduboy.justPressed(A_BUTTON) && (absT(image) <= 12 || absT(image) >=20)) {

    switch (absT(image)) {

      case 1 ... 3:
      case 13 ... 17:

        for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
          
          Bullet *bullet = &playerBullets[i];

          if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

            bullet->xPos = backgroundX + (image < 0 ? -13 : 13);
            bullet->yPos = y + 15;
            bullet->yDelta = BULLET_SHOOT_HORIZONTAL;
            bullet->xDelta = (image < 0 ? 4 : -4);
            bullet->startYPos = bullet->yPos;
            break;

          }

        }

        break;

      default:

        for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {
          
          Bullet *bullet = &playerBullets[i];

          if (bullet->xPos == BULLET_INACTIVE_X_VALUE) {

            bullet->xPos = backgroundX;
            bullet->yPos = y + 19;
            bullet->yDelta = 1;
            bullet->xDelta = deltaX * 2;
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
      image = playerStack.pop();
    }

  }

  if (arduboy.everyXFrames(4)) {
  
    if (incX == DELTA_X_INCREASE)     { deltaX = calcSpeed(deltaX, true); }
    if (incX == DELTA_X_DECREASE)     { deltaX = calcSpeed(deltaX, false); }
    if (deltaX == 0)                  { incX = DELTA_X_DO_NOTHING; }
  
  }


  // Change the helicopter's position ..
 
  y = y + deltaY;
  backgroundXOffset = backgroundXOffset - deltaX;
  backgroundX = backgroundX - deltaX;
  if (backgroundXOffset < -64) backgroundXOffset = backgroundXOffset + 64;
  if (backgroundXOffset > 0) backgroundXOffset = backgroundXOffset - 64;


  // Did we just land on hostages?

  if (y - deltaY < 38 && y > 38) {  // Check to see if we just landed.


    for (int i = 0; i < NUMBER_OF_HOSTAGES; i++) {

      Hostage *hostage = &hostages[i];
      int16_t diff = absT(hostage->xPos - backgroundX);

      if (hostage->stance >= HOSTAGE_RUNNING_LEFT_1 && hostage->stance <= HOSTAGE_WAVING_22) {

        switch (absT(image)) {

          case 1 ... 6:
          case 13 ... 17:
          case 20:
      
            if (diff < 7) {

              hostage->stance = HOSTAGE_DYING_2;
              dead++;

            }

            break;

          case 7 ... 12:
          case 18 ... 19:

            if (diff < 4) {

              hostages->stance = HOSTAGE_DYING_2;
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
      if (absT(bullet->xPos - backgroundX) > 70) { bullet->xPos = BULLET_INACTIVE_X_VALUE; }

      if (bullet->yDelta != BULLET_SHOOT_HORIZONTAL) {
        bullet->yPos = bullet->yPos + bullet->yDelta;
        bullet->yDelta = calcSpeed(bullet->yDelta, true);
      }



      // Check to see if we hit anything ..

      bulletHit(bullet, &playerBulletExplosion, true);

//       switch (bullet->yPos) {

//         case 40 ... 46:               // Hitting a dormitory?

//           for (uint8_t i = 0; i < NUMBER_OF_DORMITORIES; i++) {

//             Dormitory *dormitory = &dormitories[i];

//             if (dormitory->state == DORMITORY_STATE_INTACT && absT(dormitory->xPos - bullet->xPos) < 16) {

//                dormitory->numberOfHits++;
               
//                playerBulletExplosion.xPos = bullet->xPos;
//                playerBulletExplosion.yPos = 36 + (absT(dormitory->xPos - bullet->xPos) / 3);
//                playerBulletExplosion.explosionType = EXPLOSION_MED;

//                if (dormitory->numberOfHits == DORMITORY_HITS_REQUIRED) {

//                 dormitory->state = DORMITORY_STATE_OPEN;
//                 playerBulletExplosion.xPos = bullet->xPos;
//                 playerBulletExplosion.yPos = 36 + (absT(dormitory->xPos - bullet->xPos) / 3);
//                 playerBulletExplosion.explosionType = EXPLOSION_LRG_1;


//                 // Release the hostages ..

//                 uint8_t count = 0;

//                 for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {

//                   Hostage *hostage = &hostages[i];

//                   if (hostage->stance == HOSTAGE_IN_DORM) {
                   
//                     hostage->stance = HOSTAGE_LEAVING_DORM;
//                     hostage->xPos = dormitory->xPos + 16;
//                     hostage->countDown = (count * 15) + 10;
//                     count++;

//                     if (count == DORMITORY_HOSTAGE_CAPACITY) { break; }

//                   }

//                 }

//                } 

//                bullet->xPos = BULLET_INACTIVE_X_VALUE;

//             }

//           }

//           break;

//         case 47 ... 52:               // Hitting a hostage or the tank?
//           {
//             bool hit = false;

//             for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {
  
//               Hostage *hostage = &hostages[i];

//               if (hostage->stance >= HOSTAGE_RUNNING_LEFT_1 && 
//                   hostage->stance <= HOSTAGE_WAVING_22 && 
//                   absT(hostage->xPos - bullet->xPos) < 3) {

//                 playerBulletExplosion.xPos = bullet->xPos;
//                 playerBulletExplosion.yPos = 52;
//                 playerBulletExplosion.explosionType = EXPLOSION_MED;
//                 bullet->xPos = BULLET_INACTIVE_X_VALUE;

//                 hostage->stance = HOSTAGE_DYING_2;
//                 dead++;
//                 hit = true;
//                 break;

//               }

//             }

//             if (!hit) {

//               for (uint8_t i = 0; i < NUMBER_OF_TANKS; i++) {

//                 Tank *tank = &tanks[i];

// //SJH                if (bullet->startYPos > TANK_BULLET_MIN_Y_VALUE && absT(tank->xPos - bullet->xPos) < 16) {  // Bullets that hit tank must be fired from down low ..
//                 if (absT(tank->xPos - bullet->xPos) < 16) {  

//                   playerBulletExplosion.xPos = bullet->xPos;
//                   playerBulletExplosion.yPos = (absT(tank->xPos - bullet->xPos) < 8 ? 46 : 51);
//                   playerBulletExplosion.explosionType = EXPLOSION_MED;
//                   bullet->xPos = BULLET_INACTIVE_X_VALUE;

//                   tank->numberOfHits++;

//                   if (tank->numberOfHits == TANK_BULLET_NUMBER_OF_HITS) {

//                     playerBulletExplosion.yPos = 50;
//                     playerBulletExplosion.explosionType = EXPLOSION_LRG_1;
//                     tank->state = TANK_STATE_DEAD_3;

//                   }

//                 }

//               }

//             }

//           }
//           break;

//         case 61 ... 74:               //  Hitting the ground ..
//           playerBulletExplosion.xPos = bullet->xPos;
//           playerBulletExplosion.yPos = 60;
//           playerBulletExplosion.explosionType = EXPLOSION_SML;
//           bullet->xPos = BULLET_INACTIVE_X_VALUE;
//           break;

//       }

    }

  }




  // Update bullets ..

  for (int i = 0; i < NUMBER_OF_TANK_BULLETS; i++) {

    Bullet *bullet = &tankBullets[i];

    if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {


      // Update bullet position ..

      bullet->xPos = bullet->xPos - bullet->xDelta;
      if (absT(bullet->xPos - backgroundX) > 70) { bullet->xPos = BULLET_INACTIVE_X_VALUE; }

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


  
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 4) && deltaY == -4) { deltaY = -2; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 2) && deltaY == -2) { deltaY = -1; }
  if (y < (HELICOPTER_MAXIMUM_HEIGHT + 1) && deltaY == -1) { deltaY = 0; }

  if (y > (HELICOPTER_MINIMUM_HEIGHT - 4) && deltaY == 4)  { deltaY = 2; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 2) && deltaY == 2)  { deltaY = 1; }
  if (y > (HELICOPTER_MINIMUM_HEIGHT - 1) && deltaY == 1)  { deltaY = 0; }

  
}


void bulletHit(Bullet *bullet, BulletExplosion *explosion, bool playerBullet) {


  // Check to see if a tank bullet hit the helicopter (it can be at any height) ..
  
  {
    Rect hitZone = {50, 0, 0, 0};

    if (!playerBullet) {

      switch (absT(image)) {

        case 1 ... 3:
        case 13 ... 17:
        case 20:
    
          hitZone.x = 50;
          hitZone.y = y + 5;
          hitZone.width = 28;
          hitZone.height = 12;

          break;

        case 4 ... 6:
    
          hitZone.x = 50;
          hitZone.y = y + 7;
          hitZone.width = 28;
          hitZone.height = 12;

          break;

        case 7 ... 12:
        case 18 ... 19:

          hitZone.x = 57;
          hitZone.y = y + 5;
          hitZone.width = 14;
          hitZone.height = 13;

          break;

      }

      Point pt = { 64 + bullet->xPos - backgroundX, bullet->yPos };
//     arduboy.setCursor(1, 10);
//     arduboy.print(pt.x);
//     arduboy.print(",");
//     arduboy.print(pt.y);
// arduboy.drawRect(hitZone.x, hitZone.y, hitZone.width, hitZone.height);

      if (arduboy.collide(pt, hitZone)) {
    // arduboy.setCursor(1, 20);
    // arduboy.print(hits);

        explosion->xPos = bullet->xPos;
        explosion->yPos = hitZone.y - 4;
        explosion->explosionType = EXPLOSION_BOTH_MED;
        bullet->xPos = BULLET_INACTIVE_X_VALUE;

        hits++;

        if (hits == TANK_BULLET_NUMBER_OF_HITS) {

          explosion->yPos = 50;
          explosion->explosionType = EXPLOSION_LRG_1;
          //tank->state = TANK_STATE_DEAD_3;

        }

      }

    }

  }



  // Check to see if we hit anything ..

  switch (bullet->yPos) {

    case 40 ... 46:               // Hitting a dormitory?

      for (uint8_t i = 0; i < NUMBER_OF_DORMITORIES; i++) {

        Dormitory *dormitory = &dormitories[i];

        if (dormitory->state == DORMITORY_STATE_INTACT && absT(dormitory->xPos - bullet->xPos) < 16) {

            dormitory->numberOfHits++;
            
            explosion->xPos = bullet->xPos;
            explosion->yPos = 36 + (absT(dormitory->xPos - bullet->xPos) / 3);
            explosion->explosionType = EXPLOSION_MED;

            if (dormitory->numberOfHits == DORMITORY_HITS_REQUIRED) {

            dormitory->state = DORMITORY_STATE_OPEN;
            explosion->xPos = bullet->xPos;
            explosion->yPos = 36 + (absT(dormitory->xPos - bullet->xPos) / 3);
            explosion->explosionType = EXPLOSION_LRG_1;


            // Release the hostages ..

            uint8_t count = 0;

            for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {

              Hostage *hostage = &hostages[i];

              if (hostage->stance == HOSTAGE_IN_DORM) {
                
                hostage->stance = HOSTAGE_LEAVING_DORM;
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

          if (hostage->stance >= HOSTAGE_RUNNING_LEFT_1 && 
              hostage->stance <= HOSTAGE_WAVING_22 && 
              absT(hostage->xPos - bullet->xPos) < 3) {

            explosion->xPos = bullet->xPos;
            explosion->yPos = 52;
            explosion->explosionType = EXPLOSION_MED;
            bullet->xPos = BULLET_INACTIVE_X_VALUE;

            hostage->stance = HOSTAGE_DYING_2;
            dead++;
            hit = true;
            break;

          }

        }

        if (!hit && playerBullet) {

          for (uint8_t i = 0; i < NUMBER_OF_TANKS; i++) {

            Tank *tank = &tanks[i];

//SJH                if (bullet->startYPos > TANK_BULLET_MIN_Y_VALUE && absT(tank->xPos - bullet->xPos) < 16) {  // Bullets that hit tank must be fired from down low ..
            if (absT(tank->xPos - bullet->xPos) < 16) {  

              explosion->xPos = bullet->xPos;
              explosion->yPos = (absT(tank->xPos - bullet->xPos) < 8 ? 46 : 51);
              explosion->explosionType = EXPLOSION_MED;
              bullet->xPos = BULLET_INACTIVE_X_VALUE;

              tank->numberOfHits++;

              if (tank->numberOfHits == TANK_BULLET_NUMBER_OF_HITS) {

                explosion->yPos = 50;
                explosion->explosionType = EXPLOSION_LRG_1;
                tank->state = TANK_STATE_DEAD_3;

              }

            }

          }

        }

      }
      break;

    case 61 ... 74:               //  Hitting the ground ..
      explosion->xPos = bullet->xPos;
      explosion->yPos = 60;
      explosion->explosionType = EXPLOSION_SML;
      bullet->xPos = BULLET_INACTIVE_X_VALUE;
      break;

  }

}
