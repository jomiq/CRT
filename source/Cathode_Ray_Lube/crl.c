#include "hal.h"
#include "crl_private.h"
#include "crl.h"

uint8_t * IMG_BFR;
dacsample_t LINE_BFR0[CRL_LINE_LEN];
dacsample_t LINE_BFR1[CRL_LINE_LEN];


size_t  line = 0;
dacsample_t *free_line = LINE_BFR0;
dacsample_t *active_line = LINE_BFR1;

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
    dacStartConversion(&DACD1, &dacgrpcfg1, (dacsample_t *)free_line, CRL_LINE_LEN);    

    /* GO */

}

/**
 * This rather complicated thing is basically rendering the image one line at a 
 * time into the inactive line buffer. There are four bytes per pixel and we 
 * need to shift out the 2-bit pixel values.
 *   
*/

#define MASK 0b1111U
void render_line(size_t L){
    /*for(size_t i = 0; i < CRL_LINE_LEN; i++){
        free_line[i] = (dacsample_t)4U*i;
    }
    */
    size_t n;
    size_t linestart = L * CRL_RES_X/2U;
    dacsample_t value;

    n = 0;
    value = CRL_LOW_X;
    uint8_t j;
    for(size_t b = linestart; b < linestart + CRL_RES_X/2U; b++){
        for(uint8_t i = 0; i < 8; i = i+4U){
            j = (IMG_BFR[b] >> i)  & MASK;
            if(j){
                while(j){
                    free_line[n++] = value;
                    j--;
                }
            }
            value += CRL_VSTEPX;
        }
    }
    while (n < CRL_LINE_LEN)
    {
        free_line[n++] = 4095U;
    }
}
#undef MASK

static void line_end(DACDriver *dacp){

    dacsample_t * tmp = free_line;
    free_line = active_line;
    active_line = tmp;   
    dacStopConversion(&DACD1);
    
    //dacPutChannelX(&DACD1, 0, (dacsample_t) 0U);
    
    dacPutChannelX(&DACD2, 1, (dacsample_t) line*CRL_VSTEPY);
    dacStartConversion(dacp, &dacgrpcfg1, (dacsample_t *)active_line, CRL_LINE_LEN);
    palClearLine(PORTAB_LINE_FRAMESYNC);
    line++;
    if(line >= CRL_LINE_N)
    {
        palSetLine(PORTAB_LINE_FRAMESYNC);
        line = 0;
    }
    render_line(line);

}