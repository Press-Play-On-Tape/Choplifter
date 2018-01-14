#include "src/utils/Arduboy2Ext.h"
#include "Enums.h"
#include "Images.h"
#include "src/utils/Stack.h"

void hostageMovements() {


  // Update hostages ..

  uint8_t randomTopLimit = 0;

  for (uint8_t i = 0; i < NUMBER_OF_HOSTAGES; i++) {

    Hostage *hostage = &hostages[i];


    // Collect hostages ..

    if (inHeli < HELICOPTER_HOSTAGE_CAPACITY &&
        hostage->stance != HOSTAGE_DEAD && hostage->stance <= HOSTAGE_LEAVING_DORM &&
        backgroundX > 50 && y >= 40 && absT(hostage->xPos - backgroundX) < 5
        ) {    

      inHeli++;
      hostage->stance = HOSTAGE_IN_HELICOPTER;
      hostage->countDown = (14 * inHeli);

    }

    // Deposit hostages at base ..

    else if (hostage->stance == HOSTAGE_IN_HELICOPTER && 
             y >= 40 &&                                                                                               
             ((image == 1 && backgroundX < 24 && backgroundX > -4) || 
              (image == -1 && backgroundX < 22 && backgroundX > 0) || 
              (image == 10 && backgroundX < 22 && backgroundX > -12))) {                

      hostage->countDown--;

      if (hostage->countDown == 0) {

        hostage->stance = HOSTAGE_RUNNING_RIGHT_1;
        hostage->countDown = 255;
        hostage->xPos = backgroundX;
        inHeli--;

      }

    }

    // Hostages are safe ..
    
    else if (hostage->stance != HOSTAGE_SAFE && hostage->xPos < -40) {                                                                                   

        hostage->stance = HOSTAGE_SAFE;
        safe++;

    }

    // Hostages run wild!

    else {                                                                                                            

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
          if (hostage->countDown == 0) {
            randomTopLimit = HOSTAGE_RUNNING_RIGHT_4;
          }
          break;

      }


      if (hostage->countDown == 0) {

        if (absT(hostage->xPos - backgroundX) < 70) {

          if (random(0, 15) < 13) {

            hostage->stance = (hostage->xPos > backgroundX ? (hostage->xPos > HOSTAGE_FAR_RIGHT_POS ? HOSTAGE_RUNNING_RIGHT_1 : HOSTAGE_RUNNING_LEFT_1) : HOSTAGE_RUNNING_LEFT_1);
            hostage->countDown = random(5, 15);

          }
          else {

            hostage->stance = HOSTAGE_WAVING_11;
            hostage->countDown = random(2, 5);

          }

        }
        else {

          if (hostage->xPos > HOSTAGE_FAR_RIGHT_POS) { randomTopLimit = HOSTAGE_RUNNING_LEFT_4; }  // Prevent hostage from crossing fence ..      

          hostage->stance = random(HOSTAGE_RUNNING_LEFT_1, randomTopLimit + 1);

          switch (hostage->stance) {

            case HOSTAGE_RUNNING_LEFT_1 ... HOSTAGE_RUNNING_RIGHT_4:
              hostage->countDown = random(5, 15);
              break;

            case HOSTAGE_WAVING_11 ... HOSTAGE_WAVING_22:
              hostage->countDown = random(2, 5);
              break;


          }

        }

      }

    }

  }

}