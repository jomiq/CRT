#include "hal.h"
#include "crl_private.h"
#include "crl.h"


dacsample_t LINE_BFR[2][CRL_LINE_LEN];

size_t  LINE = 0;
uint8_t CNT = 0;

void crl_start(void){
    /* System specific configuration */
    portab_setup();
    for(size_t i = 0; i < CRL_LINE_LEN; i++)
    {
        LINE_BFR[0][i] = (dacsample_t) 0;
        LINE_BFR[1][i] = (dacsample_t) 0;
    }

    /* Starting GPT6 driver.*/
    gptStart(&GPTD6, &gpt6cfg1);

    /* Start DAC */
    dacStart(&DACD1, &X_dac1cfg1);
    dacStart(&DACD2, &Y_dac1cfg2);


    /* Starting a continuous conversion.*/
    LINE = 0;
    fill_bfr(0);
    dacStartConversion(&DACD1, &dacgrpcfg1, (dacsample_t *)LINE_BFR[0], CRL_LINE_LEN/((size_t)2));
    gptStartContinuous(&GPTD6, 2U);
    /* GO */

}

void fill_bfr(size_t L){
    if(L%2 == 0)
    {
        for(size_t i = 0; i < CRL_LINE_LEN; i++)
        {
            LINE_BFR[0][i] = (dacsample_t) i*CRL_VSTEPX;
            //LINE_BFR[0][i] = (dacsample_t) 0;
        }
    }
    else
    {
        for(size_t i = 0; i < CRL_LINE_LEN; i++)
        {
            LINE_BFR[1][i] = (dacsample_t) (CRL_LINE_LEN - i -1)*CRL_VSTEPX;
            //LINE_BFR[1][i] = (dacsample_t) 4095;
        }
    }
};


static void line_end(DACDriver *dacp){

    //dacPutChannelX(&DACD1, 1, (dacsample_t) 0);

    dacStopConversion(dacp);
    dacPutChannelX(&DACD2, 2, (dacsample_t) LINE*CRL_VSTEPY);
    dacStartConversion(dacp, &dacgrpcfg1, (dacsample_t *)LINE_BFR[LINE%2U], CRL_LINE_LEN/((size_t)2));
    
    LINE++;
    if(LINE >= CRL_LINE_N)
    {
        palToggleLine(PORTAB_LINE_FRAMESYNC);
        LINE = 0;
    }
    //palClearLine(PORTAB_LINE_LINESYNC);
    palSetLine(PORTAB_LINE_LINESYNC);
    fill_bfr(LINE);
    palClearLine(PORTAB_LINE_LINESYNC);
}