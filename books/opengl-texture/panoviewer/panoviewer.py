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
import Image

from trackball import *

lightpos = [ 0.0, 0.0, 1.0, 0.0 ] # 位置
lightcol = [ 1.0, 1.0, 1.0, 1.0 ] # 直接光強度
lightamb = [ 0.1, 0.1, 0.1, 1.0 ] # 環境光強度

TEXWIDTH = 960
TEXHEIGHT = 480

def init():
    img = Image.open('pano.jpg')
    texture = img.tostring()

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXWIDTH, TEXHEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

    #glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE)
    #glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)
    #glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND)
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE)

    glAlphaFunc(GL_GREATER, 0.5)

    glClearColor(0.6, 0.6, 0.6, 0.0)
    glEnable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
  
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol)
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol)
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb)

def scene():
    color = [ 1.0, 1.0, 1.0, 1.0 ]
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color)

    glEnable(GL_ALPHA_TEST)
    glEnable(GL_TEXTURE_2D)

    """
    glNormal3d(0.0, 0.0, 1.0)
    glBegin(GL_QUADS)
    glTexCoord2d(0.0, 1.0)
    glVertex3d(-1.6, -0.9,  0.0)
    glTexCoord2d(1.0, 1.0)
    glVertex3d( 1.6, -0.9,  0.0)
    glTexCoord2d(1.0, 0.0)
    glVertex3d( 1.6,  0.9,  0.0)
    glTexCoord2d(0.0, 0.0)
    glVertex3d(-1.6,  0.9,  0.0)
    glEnd()
    """

    #glutSolidSphere(1, 50, 50)
    glutWireSphere(2, 100, 100)

    glDisable(GL_TEXTURE_2D)
    glDisable(GL_ALPHA_TEST)

def display():
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos)
    
    #glTranslated(0.0, 0.0, -3.0) # 視点の移動（物体の方を奥に移動）
    #glTranslated(0.0, 0.0, -1.0) # 視点の移動（物体の方を奥に移動）
    glMultMatrixd(trackballRotation()) # トラックボール処理による回転
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
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
    #glutInitWindowSize(960, 480)
    glutInitWindowSize(800, 800)
    glutInitWindowPosition(0, 0)
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
