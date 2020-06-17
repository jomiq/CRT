#ifndef __CRL_H__
#define __CRL_H__

// Module master settings
#define CRL_FREQ  50000000U  // TIM6 frequency setting. This is the pixel clock.
#define CRL_RES_X 320U       //
#define CRL_RES_Y 240U       //
#define CRL_RES_Z 16U        // Effective grayscale resolution of image. 

// Limits
#define CRL_X_MARGIN    512U
#define CRL_Y_MARGIN    256U


// Derive these and include here for external use?
#define CRL_LINE_LEN    CRL_RES_Z*CRL_RES_X
#define CRL_LINE_N      CRL_RES_Y


void crl_start(uint8_t * img);
void fill_bfr(size_t L);
void render_line(size_t L);

#endif // !__CRL_H__