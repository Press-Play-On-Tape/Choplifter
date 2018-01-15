#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"

void render(uint8_t sortieNumber) {


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

  if (sortieNumber == 0) {

    for (uint8_t i = 0; i < NUMBER_OF_DORMITORIES; i++) {

      Dormitory *dormitory = &dormitories[i];

      if ((dormitory->xPos > backgroundX - 144) && (dormitory->xPos < backgroundX + 144)) {

        if (dormitory->state == DORMITORY_STATE_INTACT) {

          arduboy.drawCompressedMirror(backgroundX - dormitory->xPos + 64 - 15, 40, dormitory_01_mask, BLACK, false);
          arduboy.drawCompressedMirror(backgroundX - dormitory->xPos + 64 - 15, 40, dormitory_01, WHITE, false);

        }
        else {

          arduboy.drawCompressedMirror(backgroundX - dormitory->xPos + 64 - 15, 40, dormitory_02_mask, BLACK, false);
          arduboy.drawCompressedMirror(backgroundX - dormitory->xPos + 64 - 15, 40, dormitory_02, WHITE, false);
          
        }

      }

    }


    // Draw hostages ..

    for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {
      
      Hostage *hostage = &hostages[i];

      if ((hostage->xPos > backgroundX - 133) && (hostage->xPos < backgroundX + 133)) {

        switch (hostage->stance) {

          case HOSTAGE_RUNNING_LEFT_1 ... HOSTAGE_RUNNING_LEFT_4:
            arduboy.drawCompressedMirror(backgroundX - hostage->xPos + 64 - 3, 48, hostage_images[hostage->stance - 1], WHITE, false);
            break;

          case HOSTAGE_RUNNING_RIGHT_1 ... HOSTAGE_RUNNING_RIGHT_4:
            arduboy.drawCompressedMirror(backgroundX - hostage->xPos + 64 - 3, 48, hostage_images[hostage->stance - 5], WHITE, true);
            break;

          case HOSTAGE_WAVING_11 ... HOSTAGE_WAVING_12:
            arduboy.drawCompressedMirror(backgroundX - hostage->xPos + 64 - 3, 48, hostage_05, WHITE, false);
            break;

          case HOSTAGE_WAVING_21 ... HOSTAGE_WAVING_22:
            arduboy.drawCompressedMirror(backgroundX - hostage->xPos + 64 - 3, 48, hostage_06, WHITE, false);
            break;

          case HOSTAGE_DYING_1:
          case HOSTAGE_DYING_2:
            arduboy.drawCompressedMirror(backgroundX - hostage->xPos + 64 - 3, 48, hostage_07, WHITE, false);
            hostage->stance--;
            break;

        }

      }

    }


    // Draw helicopter ..

    drawHelicopter(45, y, image);


    // Draw tank ..

    for (int i = 0; i < NUMBER_OF_TANKS; i++) {

      Tank *tank = &tanks[i];

      if (tank->state > TANK_STATE_DEAD_1 && (tank->xPos > backgroundX - 144) && (tank->xPos < backgroundX + 144)) {

        arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_00_mask, BLACK, false);
        arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, (tank->track ? tank_00 : tank_01), WHITE, false);

        switch (tank->turrentDirection) {

          case TANK_TURRENT_DIR_LEFT_LOW:
            arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_turrent_00, WHITE, true);
            break;

          case TANK_TURRENT_DIR_LEFT_MID:
            arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_turrent_01, WHITE, true);
            break;

          case TANK_TURRENT_DIR_UPRIGHT:
            arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_turrent_02, WHITE, false);
            break;

          case TANK_TURRENT_DIR_RIGHT_MID:
            arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_turrent_01, WHITE, false);
            break;

          case TANK_TURRENT_DIR_RIGHT_LOW:
            arduboy.drawCompressedMirror(backgroundX - tank->xPos + 64 - 15, 47, tank_turrent_00, WHITE, false);
            break;
          

        }

        if (tank->state == TANK_STATE_MOVE_LEFT || tank->state == TANK_STATE_MOVE_RIGHT) tank->track = !tank->track;

        if (tank->state <= TANK_STATE_DEAD_3) {tank->state--; }

      }

    }



    // Draw player bullets ..

    for (int i = 0; i < NUMBER_OF_PLAYER_BULLETS; i++) {

      Bullet *bullet = &playerBullets[i];

      if (bullet->xPos != BULLET_INACTIVE_X_VALUE) {
  
        if ((bullet->xPos > backgroundX - 144) && (bullet->xPos < backgroundX + 144)) {

          arduboy.fillRect(backgroundX - bullet->xPos + 64 - 1, bullet->yPos, 2, 2, WHITE);

        }
    
      }

    }


    // Draw explosion ..

    if (playerBulletExplosion.xPos != BULLET_INACTIVE_X_VALUE) {

        switch (playerBulletExplosion.explosionType) {

          case EXPLOSION_SML:
          case EXPLOSION_MED:

            if ((playerBulletExplosion.xPos > backgroundX - 144) && (playerBulletExplosion.xPos < backgroundX + 144)) {

              arduboy.drawCompressedMirror(backgroundX - playerBulletExplosion.xPos + 64 - 4, playerBulletExplosion.yPos, explosions[playerBulletExplosion.explosionType - 1], WHITE, false);
              playerBulletExplosion.explosionType--;
              if (playerBulletExplosion.explosionType == 0) playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;

            }

            break;

          case EXPLOSION_LRG_1 ... EXPLOSION_LRG_4:

            if ((playerBulletExplosion.xPos > backgroundX - 144) && (playerBulletExplosion.xPos < backgroundX + 144)) {

              arduboy.drawCompressedMirror(backgroundX - playerBulletExplosion.xPos + 64 - 12, playerBulletExplosion.yPos, explosions_masks[playerBulletExplosion.explosionType - 1], BLACK, false);
              arduboy.drawCompressedMirror(backgroundX - playerBulletExplosion.xPos + 64 - 12, playerBulletExplosion.yPos, explosions[playerBulletExplosion.explosionType - 1], WHITE, false);

              playerBulletExplosion.explosionType++;
              if (playerBulletExplosion.explosionType == EXPLOSION_LRG_4 + 1) playerBulletExplosion.xPos = BULLET_INACTIVE_X_VALUE;

            }

            break;

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

    arduboy.drawCompressedMirror(-1, 0, digit_Cross, WHITE, false);
    arduboy.drawCompressedMirror(4, 0, digits[dead / 10], WHITE, false);
    arduboy.drawCompressedMirror(9, 0, digits[dead % 10], WHITE, false);
    
    arduboy.drawCompressedMirror(14, 0, digit_Diamond, WHITE, false);
    arduboy.drawCompressedMirror(21, 0, digits[inHeli / 10], WHITE, false);
    arduboy.drawCompressedMirror(26, 0, digits[inHeli % 10], WHITE, false);
    
    arduboy.drawCompressedMirror(31, 0, digit_Heart, WHITE, false);
    arduboy.drawCompressedMirror(38, 0, digits[safe / 10], WHITE, false);
    arduboy.drawCompressedMirror(43, 0, digits[safe % 10], WHITE, false);

  }
  else {
        
    switch (sortieNumber) {

      case 1:
        arduboy.drawCompressedMirror(38, 25, sortie_01, WHITE, false);
        break;

      case 2:
        arduboy.drawCompressedMirror(38, 25, sortie_02, WHITE, false);
        break;

      case 3:
        arduboy.drawCompressedMirror(38, 25, sortie_03, WHITE, false);
        break;
    
    }

  }

}

