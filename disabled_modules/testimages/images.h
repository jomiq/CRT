#ifndef __IMAGES_H__
#define __IMAGES_H__

#define N_IMAGES 4
#include "crysis.h"
#include "test1.h"
#include "test2.h"
#include "test3.h"


static uint8_t *images[N_IMAGES] = {
    crysis,
    test1,
    test2,
    test3
};

#endif //__IMAGES_H__
