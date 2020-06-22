/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <stdlib.h>
#include "ch.h"
#include "hal.h"
#include "portab.h"

#include "crl.h"
#include "include/images.h"


uint8_t img[CRL_RES_X*CRL_RES_Y/2U];

/* 
 * Application entry point.
 */
int main(void) {
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  uint8_t N = 0;

  halInit();
  chSysInit();

  /**
   * Fill the buffer with some nonsense.
   * 
   * 
  
  for(size_t j = 0; j < CRL_RES_Y; j++){
    for(size_t i = 0; i < CRL_RES_X/CRL_RES_Z; i++){
      img[j*CRL_RES_X + i] = 0x00U;
    }
  }*/
  for(size_t j = 0; j < CRL_RES_X*CRL_RES_Y/2U; j++){
    //img[j] = testimage[j];
    img[j] = 0x77U;
  }
  crl_init();
  crl_start(img);

  /*
   * Activates the serial driver 3 using the driver default configuration.
   */
  //sdStart(&SD3, NULL);
  /*
   * Creates the blinky thread.
   */

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  while (true) {
    //chThdSleepMilliseconds(500);
    //sdWrite(&SD3, (uint8_t*) READY_MSG, READY_MSG_N);
    if (palReadLine(PORTAB_LINE_BUTTON) == PORTAB_BUTTON_PRESSED) {
      palSetLine(PORTAB_LINE_LED1);
      N++;
      if(N == N_IMAGES) {
        palClearLine(PORTAB_LINE_LED1);
        N = 0;
      }
      
      crl_stop();
      
      for(size_t j = 0; j < CRL_RES_X*CRL_RES_Y/2U; j++){
        img[j] = images[N][j];
        //img[j] = 0x77U;
      }
      crl_start(img);
    }
    chThdSleepMilliseconds(500);
  }
}
