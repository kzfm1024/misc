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

def rotate_xyz(rx, ry, rz):
    R = rotate_x(rx) * rotate_y(ry) * rotate_z(rz)
    return R

def rotate_z(rad):
    C = math.cos(rad)
    S = math.sin(rad)
    Rz = np.matrix([[C, -S, 0],
                    [S,  C, 0],
                    [0,  0, 1]])
    return Rz

def rotate_quoternion(q1, q2, q3, q4):
    q1_2 = q1 * q1
    q2_2 = q2 * q2
    q3_2 = q3 * q3
    q4_2 = q4 * q4
    q1q2 = q1 * q2
    q1q3 = q1 * q3
    q1q4 = q1 * q4
    q2q3 = q1 * q2
    q2q4 = q1 * q4
    q3q4 = q3 * q4
    Rq = np.matrix([[q1_2 - q2_2 - q3_2 + q4_2, 2.0 * (q1q2 - q3q4), 2.0 * (q1q3 - q2q4)],
                    [2.0 * (q1q2 + q3q4), - q1_2 + q2_2 + q3_2 + q4_2, 2.0 * (q2q3 - q1q4)],
                    [2.0 * (q1q3 - q2q4), 2.0 * (q2q3 + q1q4), - q1_2 - q2_2 + q3_2 + q4_2]])
    return Rq

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

    R = rotate_quoternion(0, 0, 0, 1)
    print('R=' + str(R))
    
if __name__ == '__main__':
    main()
