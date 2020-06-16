#include "hal.h"
#include "crl_private.h"
#include "crl.h"

uint8_t * IMG_BFR;
dacsample_t LINE_BFR[2][CRL_LINE_LEN];

size_t  LINE = 0;
uint8_t CNT = 0;
uint8_t ODD = FALSE;

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
    LINE = 0;
    ODD = FALSE;
    render_line(0);
    line_end(&DACD1);
    gptStartContinuous(&GPTD6, 2U);
    /* GO */

}

// void fill_bfr(size_t L){
//     size_t line_p = L * CRL_LINE_LEN;
//     if(ODD) // 
//     {
//         for(size_t i = 0; i < CRL_LINE_LEN; i++)
//         {
//             LINE_BFR[0][i] = (dacsample_t) IMG_BFR[line_p + i];
//             line_p++;
//             //LINE_BFR[0][i] = (dacsample_t) 0;
//         }
//     }
//     else
//     {
//         for(size_t i = 0; i < CRL_LINE_LEN; i++)
//         {
//             LINE_BFR[1][i] = (dacsample_t) IMG_BFR[line_p];
//             line_p++;
//             //LINE_BFR[1][i] = (dacsample_t) 4095;
//         }
//     }
//     ODD = !ODD;
// };

/**
 * This rather complicated thing is basically rendering the image one line at a 
 * time into the inactive line buffer. There are four bytes per pixel and we 
 * need to shift out the 2-bit pixel values.
 *   
*/

#define MASK 0b11U
void render_line(size_t L){
    
    size_t n;
    size_t linestart = L * CRL_RES_X/CRL_RES_Z;
    dacsample_t value;

    n = 0;
    value = CRL_LOW_X;

    uint8_t j;
    for(size_t b = linestart; b < linestart + CRL_RES_X/CRL_RES_Z; b++){
        for(uint8_t i = 0; i < 8; i = i+2U){
            j = (IMG_BFR[b] >> i)  & MASK;
            if(j){
                while(j){
                    LINE_BFR[!ODD][n++] = value;
                    j--;
                }
            }
            value += CRL_VSTEPX;
        }
    }
    while (n < CRL_LINE_LEN)
    {
        LINE_BFR[!ODD][n++] = 4095U;
    }

    ODD = !ODD;
}
#undef MASK

void fill_bfr(size_t L){
    size_t line_p = L * CRL_LINE_LEN;
    if(ODD) // 
    {
        for(size_t i = 0; i < CRL_LINE_LEN; i++)
        {
            LINE_BFR[0][i] = (dacsample_t) IMG_BFR[line_p + i];
            line_p++;
            //LINE_BFR[0][i] = (dacsample_t) 0;
        }
    }
    else
    {
        for(size_t i = 0; i < CRL_LINE_LEN; i++)
        {
            LINE_BFR[1][i] = (dacsample_t) IMG_BFR[line_p];
            line_p++;
            //LINE_BFR[1][i] = (dacsample_t) 4095;
        }
    }
    ODD = !ODD;
};


static void line_end(DACDriver *dacp){

    //dacPutChannelX(&DACD1, 1, (dacsample_t) 0);

    dacStopConversion(dacp);
    palClearLine(PORTAB_LINE_FRAMESYNC);
    dacPutChannelX(&DACD2, 2, (dacsample_t) LINE*CRL_VSTEPY);
    dacStartConversion(dacp, &dacgrpcfg1, (dacsample_t *)LINE_BFR[ODD], CRL_LINE_LEN/((size_t)2));
    
    if(LINE >= CRL_LINE_N)
    {
        palSetLine(PORTAB_LINE_FRAMESYNC);
        LINE = 0;
    }
    //palClearLine(PORTAB_LINE_LINESYNC);
    LINE++;
    render_line(LINE);

    palToggleLine(PORTAB_LINE_LINESYNC);
}