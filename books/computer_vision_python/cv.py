import cv2

im = cv2.imread('data/baboon.jpg')
h,w = im.shape[:2]
print h, w
print im.shape
