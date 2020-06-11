#ifndef __DAC1_H__

void dac1_begin(void);
static void end_cb1(DACDriver *dacp);
static void error_cb1(DACDriver *dacp, dacerror_t err);

#endif // !__DAC1_H__