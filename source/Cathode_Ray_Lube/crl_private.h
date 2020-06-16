#ifndef __CRL_PRIVATE_H__
#define __CRL_PRIVATE_H__

#include "hal.h"
#include "portab.h"
#include "crl.h"

#define CRL_LOW_X   CRL_X_MARGIN
#define CRL_HIGH_X  4096U - CRL_X_MARGIN
#define CRL_LOW_Y   CRL_Y_MARGIN
#define CRL_HIGH_Y  4096U - CRL_Y_MARGIN

#define CRL_VSTEPX (CRL_HIGH_X-CRL_LOW_X)/CRL_RES_X
#define CRL_VSTEPY (CRL_HIGH_Y-CRL_LOW_Y)/CRL_RES_Y

/**
 * callback at end of line
 *
 **/

static void line_end(DACDriver *dacp);


static const DACConfig X_dac1cfg1 = {
    .init = 2047U,
    .datamode = DAC_DHRM_12BIT_RIGHT,
    .cr = 0};


static const DACConfig Y_dac1cfg2 = {
    .init = 2047U,
    .datamode = DAC_DHRM_12BIT_RIGHT,
    .cr = 0};


static const DACConversionGroup dacgrpcfg1 = {
    .num_channels = 1U,
    .end_cb = line_end,
    .error_cb = NULL,
    .trigger = DAC_TRG(PORTAB_DAC_TRIG)};

static const GPTConfig gpt6cfg1 = {
    .frequency = CRL_FREQ,
    .callback = NULL,
    .cr2 = TIM_CR2_MMS_1, /* MMS = 010 = TRGO on Update Event.    */
    .dier = 0U};

#endif // !__CRL_PRIVATE_H__