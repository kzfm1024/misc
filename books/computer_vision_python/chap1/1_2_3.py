from PIL import Image
from pylab import *

im = array(Image.open('../data/baboon.jpg'))
imshow(im)
print 'click 3 points.'
x = ginput(3)
print 'clicked x coord:', x
show()
