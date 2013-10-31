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

from sphere import *
from trackball import *

lightpos = [ 0.0, 0.0, 1.0, 0.0 ] # 位置
lightcol = [ 1.0, 1.0, 1.0, 1.0 ] # 直接光強度
lightamb = [ 0.1, 0.1, 0.1, 1.0 ] # 環境光強度

FOVY = 66.0 # defined in MultiCameraRenderer.ini
MCR_USER_VIEW_NEAR_PLANE = 0.01
MCR_USER_VIEW_FAR_PLANE  = 1000.0

RADIUS = 2.0
SLICES = 100
STACKS = 50

g_vertex_array = sphere_vertex_array(RADIUS, SLICES, STACKS)
g_index_array = sphere_index_array(SLICES, STACKS)
g_texcoord_array = sphere_texcoord_array(SLICES, STACKS)

def init(filename):
    img = Image.open(filename)
    texwidth, texheight = img.size
    texture = img.tostring() # 画像サイズが 3840x1920 だと IOError が発生

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST)

    glClearColor(0.6, 0.6, 0.6, 0.0)
    glEnable(GL_DEPTH_TEST)
    glDisable(GL_CULL_FACE)
  
    glEnable(GL_LIGHTING)
    glEnable(GL_LIGHT0)
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol)
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol)
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb)

def scene():
    global RADIUS
    global SLICES
    global STACKS
    global g_vertex_array
    global g_index_array
    global g_texcoord_array

    color = [ 1.0, 1.0, 1.0, 1.0 ]
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color)

    glEnable(GL_TEXTURE_2D)

    glEnableClientState(GL_TEXTURE_COORD_ARRAY)
    glEnableClientState(GL_VERTEX_ARRAY)
    glTexCoordPointer(3, GL_FLOAT, 0, g_texcoord_array.tostring())
    glVertexPointer(3, GL_FLOAT, 0, g_vertex_array.tostring())
    glDrawElements(GL_TRIANGLES, len(g_index_array), GL_UNSIGNED_INT, g_index_array.tostring())
    glDisableClientState(GL_TEXTURE_COORD_ARRAY)
    glDisableClientState(GL_VERTEX_ARRAY)

    #glutWireSphere(RADIUS, SLICES, STACKS) # FIXME

    glDisable(GL_TEXTURE_2D)

def display():
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos)
    
    #glTranslated(0.0, 0.0, -5.0) # 視点の移動（物体の方を奥に移動）
    glMultMatrixd(trackballRotation()) # トラックボール処理による回転
  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
    glutSwapBuffers()

################################################################################

def resize(w, h):
    global FOVY
    global MCR_USER_VIEW_NEAR_PLANE
    global MCR_USER_VIEW_FAR_PLANE    
    trackballRegion(w, h)
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(FOVY, float(w)/float(h), MCR_USER_VIEW_NEAR_PLANE, MCR_USER_VIEW_FAR_PLANE)

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

def main(options, args):
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE)
    glutInitWindowSize(1280, 720)
    glutInitWindowPosition(0, 0)
    glutCreateWindow(sys.argv[0])
    glutDisplayFunc(display)
    glutReshapeFunc(resize)
    glutIdleFunc(idle)
    glutMouseFunc(mouse)
    glutMotionFunc(motion)
    glutKeyboardFunc(keyboard)
    init(sys.argv[1])
    glutMainLoop()

if __name__ == '__main__':
    parser = OptionParser(__doc__)

    options, args = parser.parse_args()
    #if len(args) < 1:
    #    print >>sys.stderr, parser.print_help()
    #    sys.exit(1)

    main(options, args)
