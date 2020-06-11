#include "ch.h"
#include "hal.h"

#include "blink.h"

THD_WORKING_AREA(waThread1, 128);
THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(200);
    palSetLine(LINE_LED2);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED2);
    chThdSleepMilliseconds(200);
    palSetLine(LINE_LED3);
    chThdSleepMilliseconds(200);
    palClearLine(LINE_LED3);
    chThdSleepMilliseconds(200);

    
  }
}