/**
 * crl.h
 * Public interface to the Cathode Ray Lube module.
 * 
 * **/

#ifndef __CRL_H__
#define __CRL_H__


// Module master settings

#define CRL_3BIT    // 8-color mode
//#define CRL_4BIT  // 16-color
#define CRL_ZMODE 1
#define CRL_DEBUG 1

#define CRL_RES_X 320U       //
#define CRL_RES_Y 240U       //

// Limits
#define CRL_X_MARGIN    (dacsample_t)0U
#define CRL_Y_MARGIN    (dacsample_t)0U

// Clock
#define CRL_FREQ  100000000U  // TIM6 frequency setting. This is the pixel clock.

void crl_start(uint8_t * img);

#endif // !__CRL_H__