from PIL import Image
import numpy as np

im = np.array(Image.open('../data/baboon.jpg'))
print im.shape, im.dtype

im = np.array(Image.open('../data/baboon.jpg').convert('L'), 'f')
print im.shape, im.dtype

