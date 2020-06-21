#include "hal.h"
#include "crl.h"
#include "crl_private.h"

uint8_t * IMG_BFR;
dacsample_t LINE_BFR[2][CRL_LINE_LEN];


size_t  line = 0;
uint8_t free_line = 0;

void crl_start(uint8_t * img){
    /* Point to the buffer */
    IMG_BFR = img;
    
    /* System specific configuration */
    portab_setup();

    /* Starting GPT6 driver.*/
    gptStart(&GPTD6, &gpt6cfg1);

    /* Start DAC */
    dacStart(&DACD1, &X_dac1cfg1);
    dacStart(&DACD2, &Y_dac1cfg2);


    /* Starting a continuous conversion.*/
    line = 0;
    render_line(line);
    dacPutChannelX(&DACD2, 1, (dacsample_t) line*CRL_VSTEPY);
    gptStartContinuous(&GPTD6, 2U);
    dacStartConversion(&DACD1, &dacgrpcfg1, (dacsample_t *)LINE_BFR[free_line], CRL_LINE_LEN*2U);    

    /* GO */

}

/**
 * This rather complicated thing is basically rendering the image one line at a 
 * time into the inactive line buffer. There are @CRL_PPB pixels per byte and we 
 * need to shift out the 2-bit pixel values.
 *   
*/
#ifdef CRL_ZMODE
void render_line(size_t L){

#ifdef CRL_DEBUG
    palClearLine(PORTAB_LINE_LINESYNC);
#endif // DEBUG

    size_t n;           // Line buffer index
    size_t linestart;   // Image index
    dacsample_t value;  // Output value
    uint8_t pixel;      // Pixel value [3/4-bit]
    
    if(free_line){
        n = 0;
        value = CRL_LOW_X;
        linestart = L * CRL_RES_X/CRL_PPB;

        for(size_t b = linestart; b < linestart + CRL_RES_X/CRL_PPB; b++){
            for(uint8_t i = 0; i < 8; i = i+4U){
                pixel = (IMG_BFR[b] >> i) & CRL_MASK;
                if(pixel){
                    while(pixel){
                        LINE_BFR[free_line][n++] = value;
                        pixel--;
                    }
                }
                value += CRL_VSTEPX;
            }
        }
        while (n < CRL_LINE_LEN)
        {
            LINE_BFR[free_line][n++] = (dacsample_t)4095U;
        }
    }else{
        /* right-to-left */
        n = 0;
        value = CRL_HIGH_X;
        linestart = (L+1) * CRL_RES_X/CRL_PPB;
        for(size_t b = linestart; b > linestart - CRL_RES_X/CRL_PPB; b--){
            pixel = IMG_BFR[b] & CRL_MASK;
            if(pixel){
                while(pixel){
                    LINE_BFR[free_line][n++] = value;
                    pixel--;
                }
            }
            value -= CRL_VSTEPX;
            pixel = (IMG_BFR[b] >> 4)  & CRL_MASK;
            if(pixel){
                while(pixel){
                    LINE_BFR[free_line][n++] = value;
                    pixel--;
                }
            }
            value -= CRL_VSTEPX;
        }
        while(n < CRL_LINE_LEN)
        {
            LINE_BFR[free_line][n++] = (dacsample_t)0U;
        }
    }

#ifdef CRL_DEBUG
    palClearLine(PORTAB_LINE_LINESYNC);
#endif // DEBUG

}
#else
void render_line(size_t L){

}
#endif

static void line_end(DACDriver *dacp){

    dacStopConversion(&DACD1);
    
    dacPutChannelX(&DACD2, 1, (dacsample_t) line*CRL_VSTEPY);
    dacStartConversion(dacp, &dacgrpcfg1, (dacsample_t *)LINE_BFR[free_line], CRL_LINE_LEN*2U);
    palClearLine(PORTAB_LINE_FRAMESYNC);
    line++;
    if(line >= CRL_RES_Y)
    {
        palSetLine(PORTAB_LINE_FRAMESYNC);
        line = 0;
    }
    free_line = !free_line;   
    render_line(line);

}