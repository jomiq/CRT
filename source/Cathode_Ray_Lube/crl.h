/**
 * crl.h
 * Public interface to the Cathode Ray Lube module.
 * 
 * **/

#ifndef __CRL_H__
#define __CRL_H__


// Module master settings

#define CRL_3BIT    // 8-color mode, 22 fps
//#define CRL_4BIT  // 16-color, 11 fps
#define CRL_ZMODE 1
#define CRL_DEBUG 0

#define CRL_RES_X 320U       //
#define CRL_RES_Y 240U       //

// Limits
#define CRL_X_MARGIN    (dacsample_t)800U
#define CRL_Y_MARGIN    (dacsample_t)0U

// Clock
#define CRL_FREQ  100000000U  // TIM6 frequency setting. This is the pixel clock.

// Iterface

extern volatile uint8_t CRL_RUNNING;
void crl_init(void);
void crl_start(uint8_t * img);
void crl_stop(void);


#endif // !__CRL_H__