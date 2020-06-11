#ifndef __CRL_H__
#define __CRL_H__

// Module master settings
#define CRL_FREQ  10000000U // TIM6 frequency setting, pixel trigger
#define CRL_RES_X 640U       //
#define CRL_RES_Y 480U       //
#define CRL_RES_Z 4U         // Effective grayscale resolution of image. 

#define CRL_LINE_LEN    CRL_RES_Z*CRL_RES_X
#define CRL_LINE_N      CRL_RES_Y
#define CRL_BFR_SIZE    CRL_LINE_LEN*CRL_LINE_N 

//

void crl_start(void);
void fill_bfr(size_t L);
#endif // !__CRL_H__