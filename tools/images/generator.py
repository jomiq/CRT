from __future__ import division
import os, sys
from PIL import Image


def gr8(pixel):
    if len(pixel) >= 3:
        return int(int((pixel[0] + pixel[1] + pixel[2])/3)/int(32))
    else:
        return int(pixel/28)

def main(input_img:Image.Image):
    px = input_img.load()
    w = 10
    h = 240
    for y in range(h):
        for x in range(0, w-1, 2):
            if x+2 == w:
                nl = ",\n"
                if y == h-1:
                    nl = "\n"
            else:
                nl = ", "
            
            outfile.write(f'0x{gr8(px[x,y]):X}{gr8(px[x+1,y]):X}{nl}')



if __name__ == "__main__":
    filename = sys.argv[1]
    try:
        image = Image.open(filename + '.png')
    except:
        print(f"can't open file {filename}")
        exit()
    
    pixels = image.load()

    outfile = open(filename+ '.h', mode='w')
    outfile.write(f'#ifndef __{filename}_H__\n')
    outfile.write(f'#define __{filename}_H__\n\n')
    outfile.write(f'static uint8_t {filename}[320U*240U/2U] ')
    outfile.write('= {\n')
    
    main(image)
    
    outfile.write('};\n\n')

    outfile.write(f'#endif //__{filename}_H__\n')    
    outfile.close()