import Image

f = open('tire.raw', 'rb')
rgb = f.read()
f.close()

img1 = Image.fromstring('RGB', (256, 256), rgb)
img2 = Image.new('RGBA', (256, 256), (0, 0, 0, 0))

for y in range(256):
    for x in range(256):
        r, g, b = img1.getpixel((x, y))
        if r == 72 or g == 72 or b == 72:
            img2.putpixel((x, y), (0, 0, 0, 0))
        else:
            img2.putpixel((x, y), (r, g, b, 255))

rgba = img2.tostring()

f = open('tire2.raw', 'wb')
f.write(rgba)
f.close()

 
