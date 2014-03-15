#!/usr/bin/env python

from PIL import Image
import numpy as np

def imresize(im,sz):
    """ change image array size with PIL """
    pil_im = Image.fromarray(uint8(im))
    return np.array(pil_im.resize(sz))

def histeq(im, nbr_bins=256):
    imhist, bins = np.histogram(im.flatten(),nbr_bins,normed=True)
    cdf = imhist.cumsum()
    cdf = 255 * cdf / cdf[-1]

    im2 = np.interp(im.flatten(),bins[:-1],cdf)

    return im.reshape(im.shape), cdf
