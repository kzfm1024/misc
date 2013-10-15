#!/usr/bin/env python
# -*- coding: utf-8 -*-

from OpenGL.GL import *

def box(x, y, z):
    vertex = [
        [[ -x, -y, -z ], [  x, -y, -z ], [  x, -y,  z ], [ -x, -y,  z ]],
        [[ -x, -y, -z ], [ -x,  y, -z ], [  x,  y, -z ], [  x, -y, -z ]],
        [[  x, -y, -z ], [  x,  y, -z ], [  x,  y,  z ], [  x, -y,  z ]],
        [[  x, -y,  z ], [  x,  y,  z ], [ -x,  y,  z ], [ -x, -y,  z ]],
        [[ -x, -y,  z ], [ -x,  y,  z ], [ -x,  y, -z ], [ -x, -y, -z ]],
        [[ -x,  y,  z ], [  x,  y,  z ], [  x,  y, -z ], [ -x,  y, -z ]]]

    texcoord = [
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]],
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]],
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]],
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]],
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]],
        [[ 0.0, 0.0 ], [ 0.0, 1.0 ], [ 1.0, 1.0 ], [ 1.0, 0.0 ]]]
  
    normal = [
        [  0.0, -1.0,  0.0 ],
        [  0.0,  0.0, -1.0 ],
        [  1.0,  0.0,  0.0 ],
        [  0.0,  0.0,  1.0 ],
        [ -1.0,  0.0,  0.0 ],
        [  0.0,  1.0,  0.0 ]]

    glBegin(GL_QUADS)
    for j in range(6):
        glNormal3dv(normal[j])
        for i in range(4):
            glTexCoord3dv(vertex[j][i])
            glVertex3dv(vertex[j][i])
    glEnd()
