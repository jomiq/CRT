from __future__ import division
import os, sys
from PIL import Image, ImageOps


def gr8(px):
    n = 3
    try:
        n = len(px)
    except:
        return int(px/0x24)

    if n == 2:
        return int( int(px[0] + px[1]) // 0x2 // 0x24)

    if n >= 3:
        return int( int(px[0] + px[1] + px[2]) // 0x3 // 0x24)

def main(img:Image.Image):
    w = 320
    h = 240
    
    if img.size != (w, h):
        img = ImageOps.fit(img, (w, h))

    px = img.load()

    for y in range(h):
        if y
        for x in range(0, w-1, 2):
            if x+2 == w:
                nl = ",\n"
                if y == h-1:
                    nl = "\n"
            else:
                nl = ", "
            
            outfile.write(f'0x{gr8(px[x,y]):X}{gr8(px[x+1,y]):X}{nl}')



if __name__ == "__main__":
    images = []
    for filename in sys.argv[1:]:
        img = None
        try:
            img = Image.open('./input/' + filename)
        except:
            print(f"can't open file {filename}")

        if img is not None:
            images.append( (filename.split('.',1)[0], img) )


    headerfile = open('./output/images.h', mode='w')
    headerfile.write('#ifndef __IMAGES_H__\n')
    headerfile.write('#define __IMAGES_H__\n\n')
    headerfile.write(f'#define N_IMAGES {len(images)}\n')
    
    
    
    for name, image in images:
        print(f'process: {name}\n')
        outfile = open('./output/' + name + '.h', mode='w')
        
        outfile.write(f'#ifndef __{name.upper()}_H__\n')
        outfile.write(f'#define __{name.upper()}_H__\n\n')
        outfile.write(f'static uint8_t {name}[320U*240U/2U] ')
        outfile.write('= {\n')
        
        main(image)
        
        outfile.write('};\n\n')

        outfile.write(f'#endif //__{name.upper()}_H__\n')    
        outfile.close()

        headerfile.write(f'#include "{name}.h"\n')

    headerfile.write('\n\n')
    headerfile.write('static uint8_t *images[N_IMAGES] = {\n')
    
    n = 0
    for name, image in images:
        n = n+1
        headerfile.write(f'    {name}')
        if n != len(images):
            headerfile.write(',\n')

    headerfile.write('\n};\n\n')
    headerfile.write('#endif //__IMAGES_H__\n')

        




            