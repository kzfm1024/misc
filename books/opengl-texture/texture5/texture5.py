#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

# 三角関数等を利用するならばコメントアウトを解除する。
#import math
from optparse import OptionParser

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

# NumPy
#from numpy import array

# PIL
#from Image import open as imageopen

from trackball import *
from box import box

lightpos = [ 0.0, 0.0, 1.0, 0.0 ] # 位置
lightcol = [ 1.0, 1.0, 1.0, 1.0 ] # 直接光強度
lightamb = [ 0.1, 0.1, 0.1, 1.0 ] # 環境光強度

TEXWIDTH = 256
TEXHEIGHT = 256
FRAMES = 360.0
g_frame = 0.0

def init():
    f = open('tire.raw', 'rb')
    texture = f.read()
    f.close()

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture)
    #gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, TEXWIDTH, TEXHEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, texture)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP)

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)

    #blend = [ 0.0, 1.0, 0.0, 1.0 ]
    #glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blend)

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR)
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR)
    glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR)
    glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR)

    glAlphaFunc(GL_GREATER, 0.5)

    glClearColor(0.3, 0.3, 1.0, 0.0)
    glEnable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
  
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol)
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol)
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb)

def scene():
    color = [ 1.0, 1.0, 1.0, 1.0 ]
    #color = [ 1.0, 0.0, 0.0, 1.0 ]
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color)

    #glEnable(GL_ALPHA_TEST)
    glEnable(GL_TEXTURE_2D)

    glEnable(GL_TEXTURE_GEN_S)
    glEnable(GL_TEXTURE_GEN_T)
    glEnable(GL_TEXTURE_GEN_R)
    glEnable(GL_TEXTURE_GEN_Q)

    glPushMatrix()
    glTranslated(-1.2, 0.0, 0.0)
    box(1.0, 1.0, 1.0)
    glPopMatrix()
    glPushMatrix()
    glTranslated(1.2, 0.0, 0.0)
    box(1.0, 1.0, 1.0)
    glPopMatrix()

    glDisable(GL_TEXTURE_GEN_S)
    glDisable(GL_TEXTURE_GEN_T)
    glDisable(GL_TEXTURE_GEN_R)
    glDisable(GL_TEXTURE_GEN_Q)

    glDisable(GL_TEXTURE_2D)
    #glDisable(GL_ALPHA_TEST)

def display():
    genfunc = [
        [ 1.0, 0.0, 0.0, 0.0 ],
        [ 0.0, 1.0, 0.0, 0.0 ],
        [ 0.0, 0.0, 1.0, 0.0 ],
        [ 0.0, 0.0, 0.0, 1.0 ]]

    global g_frame
    t = g_frame / FRAMES

    g_frame += 1.0
    if g_frame >= FRAMES:
        g_frame = 0.0

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos)
    
    glTranslated(0.0, 0.0, -5.0) # 視点の移動（物体の方を奥に移動）
    glMultMatrixd(trackballRotation()) # トラックボール処理による回転

    model = glGetDoublev(GL_MODELVIEW_MATRIX)
    proj = glGetDoublev(GL_PROJECTION_MATRIX)

    glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0])
    glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1])
    glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2])
    glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3])

    glMatrixMode(GL_TEXTURE)
    glLoadIdentity();
    glTranslated(0.5, 0.5, 0.0)
    glRotated(t * 360.0, 0.0, 0.0, 1.0)
    glScaled(0.5, 0.5, 1.0)
    glMultMatrixd(proj)
    glMultMatrixd(model)

    glMatrixMode(GL_MODELVIEW)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    scene()
    glutSwapBuffers()

################################################################################
# callback

def resize(w, h):
    trackballRegion(w, h)
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60.0, float(w)/float(h), 1.0, 100.0)

def keyboard(key, x, y):
    if key == '\033' or key == 'q' or key == 'Q': # ESC or q or Q
        sys.exit()
    else:
        print(key)

def idle():
    glutPostRedisplay()
    pass

def mouse(button, state, x, y):
    if button == GLUT_LEFT_BUTTON:
        if state == GLUT_DOWN:
            trackballStart(x, y)
        elif state == GLUT_UP:
            trackballStop(x, y)

def motion(x, y):
    trackballMotion(x, y)
    pass

################################################################################
# main

def main(options, args):
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE)
    #glutInitWindowSize(320, 240)
    #glutInitWindowPosition(0, 0)
    glutCreateWindow(sys.argv[0])
    glutDisplayFunc(display)
    glutReshapeFunc(resize)
    glutIdleFunc(idle)
    glutMouseFunc(mouse)
    glutMotionFunc(motion)
    glutKeyboardFunc(keyboard)
    init()
    glutMainLoop()

if __name__ == '__main__':
    parser = OptionParser(__doc__)

    options, args = parser.parse_args()
    #if len(args) < 1:
    #    print >>sys.stderr, parser.print_help()
    #    sys.exit(1)

    main(options, args)
