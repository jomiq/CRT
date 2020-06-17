from PIL import Image

image = Image.open("test.png")
pixels = image.load()

for y in range(240):
    for x in range(0, 319, 2):
        if x == 319:
            nl = ", \n"
        else:
            nl = ", "
        print(f'0x{pixels[x,y]:X}{pixels[x+1,y]:X}', end=nl)

