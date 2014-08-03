from PIL import Image
import numpy as np
from pylab import *
#import matplotlib.pyplot as plt

im = array(Image.open('../data/baboon.jpg'))
im2 = 255 - im
imshow(im2)
show()
