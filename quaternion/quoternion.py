#!/usr/bin/env python
# -*- coding: utf-8 -*-

import math
import numpy as np

def rotate_x(rad):
    C = math.cos(rad)
    S = math.sin(rad)
    Rx = np.matrix([[1, 0,  0],
                    [0, C, -S],
                    [0, S,  C]])
    return Rx

def rotate_y(rad):
    C = math.cos(rad)
    S = math.sin(rad)
    Ry = np.matrix([[C, 0, -S],
                    [0, 1,  0],
                    [S, 0,  C]])
    return Ry

def rotate_z(rad):
    C = math.cos(rad)
    S = math.sin(rad)
    Rz = np.matrix([[C, -S, 0],
                    [S,  C, 0],
                    [0,  0, 1]])
    return Rz

def rotate_xyz(rx, ry, rz):
    R = rotate_x(rx) * rotate_y(ry) * rotate_z(rz)
    return R

def rotate_quoternion(q):
    x2 = q[1] * q[1] * 2.0
    y2 = q[2] * q[2] * 2.0
    z2 = q[3] * q[3] * 2.0
    xy = q[1] * q[2] * 2.0
    yz = q[2] * q[3] * 2.0
    zx = q[3] * q[1] * 2.0
    xw = q[1] * q[0] * 2.0
    yw = q[2] * q[0] * 2.0
    zw = q[3] * q[0] * 2.0

    Rq = np.matrix([[ 1.0 - y2 - z2,       xy + zw,       zx - yw ],
                    [       xy - zw, 1.0 - z2 - x2,       yz + xw ],
                    [       zx + yw,       yz - xw, 1.0 - x2 - y2 ]])
    return Rq

def qmul(p, q):
    r = [ 0.0, 0.0, 0.0, 0.0 ]
    r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3]
    r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2]
    r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1]
    r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0]
    return r

def qrot(q):
    r = [ 1.0, 0.0, 0.0, 0.0,
          0.0, 1.0, 0.0, 0.0,
          0.0, 0.0, 1.0, 0.0,
          0.0, 0.0, 0.0, 1.0 ]

    x2 = q[1] * q[1] * 2.0
    y2 = q[2] * q[2] * 2.0
    z2 = q[3] * q[3] * 2.0
    xy = q[1] * q[2] * 2.0
    yz = q[2] * q[3] * 2.0
    zx = q[3] * q[1] * 2.0
    xw = q[1] * q[0] * 2.0
    yw = q[2] * q[0] * 2.0
    zw = q[3] * q[0] * 2.0
  
    r[ 0] = 1.0 - y2 - z2
    r[ 1] = xy + zw
    r[ 2] = zx - yw
    r[ 3] = 0.0
    r[ 4] = xy - zw
    r[ 5] = 1.0 - z2 - x2
    r[ 6] = yz + xw
    r[ 7] = 0.0
    r[ 8] = zx + yw
    r[ 9] = yz - xw
    r[10] = 1.0 - x2 - y2
    r[11] = 0.0
    r[12] = 0.0
    r[13] = 0.0
    r[14] = 0.0
    r[15] = 1.0
    return r


def main():
    a = np.array((1,1,0))
    R = rotate_x(math.radians(90))
    b = np.dot(R, a)
    print('a=' + str(a))
    print('R=' + str(R))
    print('b=' + str(b))

    R = rotate_xyz(math.radians(90), 0, 0)
    b = np.dot(R, a)
    print('R=' + str(R))
    print('b=' + str(b))

    q = np.array((0, 0, 0, 1))
    R = rotate_quoternion(q)
    print('R=' + str(R))

if __name__ == '__main__':
    main()
