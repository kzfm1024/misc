from PIL import Image
from pylab import *

im = array(Image.open('../data/baboon.jpg').convert('L'))
figure()
gray()
contour(im, origin='image')
axis('equal')
axis('off')
figure()
hist(im.flatten(), 128)
show()
