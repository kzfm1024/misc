#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

/*
** ����
*/
static const GLfloat lightpos[] = { 0.0, 0.0, 1.0, 0.0 }; /* ���֡����� */
static const GLfloat lightcol[] = { 1.0, 1.0, 1.0, 1.0 }; /* ľ�ܸ����� */
static const GLfloat lightamb[] = { 0.1, 0.1, 0.1, 1.0 }; /* �Ķ������� */

/*
** �ƥ�������
*/
#define TEXWIDTH  256                      /* �ƥ���������������� */
#define TEXHEIGHT 256                      /* �ƥ�������ι⤵���� */
static const char texture1[] = "tire.raw"; /* �ƥ�������ե�����̾ */
static double genfunc[][4] = {
    { 1.0, 0.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 0.0, 1.0 },
};

/*
** �����
*/
static void init(void)
{
  /* �ƥ���������ɤ߹��ߤ˻Ȥ����� */
  GLubyte texture[TEXHEIGHT][TEXWIDTH][4];
  FILE *fp;
  
  /* �ƥ�������������ɤ߹��� */
  if ((fp = fopen(texture1, "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    fclose(fp);
  }
  else {
    perror(texture1);
  }
  
  /* �ƥ�����������ϥХ���ñ�̤˵ͤ���ޤ�Ƥ��� */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  /* �ƥ�������γ������ */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
  /* �ƥ����������硦�̾�������ˡ�λ��� */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  /* �ƥ�������η����֤���ˡ�λ��� */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  /* �ƥ�������Ķ� */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  
#if 0
  /* ���礹�뿧������ */
  static const GLfloat blend[] = { 0.0, 1.0, 0.0, 1.0 };
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, blend);
#endif

  /* ĺ���Υ��֥������ȶ��֤ˤ������ɸ�ͤ�ƥ��������ɸ�˻Ȥ� */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);

#if 0
  /* ĺ���Υ��֥������ȶ��֤ˤ������ɸ�ͤ�ƥ��������ɸ�˻Ȥ� */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

  /* �ƥ��������ɸ�����ؿ������� */
  glTexGendv(GL_S, GL_OBJECT_PLANE, genfunc[0]);
  glTexGendv(GL_T, GL_OBJECT_PLANE, genfunc[1]);
  glTexGendv(GL_R, GL_OBJECT_PLANE, genfunc[2]);
  glTexGendv(GL_Q, GL_OBJECT_PLANE, genfunc[3]);
#endif

  /* ����ե��ƥ��Ȥ�Ƚ�̴ؿ� */
  glAlphaFunc(GL_GREATER, 0.5);
  
  /* ������� */
  glClearColor(0.3, 0.3, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  /* �����ν������ */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
}

/* Ȣ�������ؿ������ */
#include "box.h"

/*
** �����������
*/
static void scene(void)
{
  static const GLfloat color[] = { 1.0, 1.0, 1.0, 1.0 };  /* ��� (��) */
  
  /* ��������� */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
  /* ����ե��ƥ��ȳ��� */
  //glEnable(GL_ALPHA_TEST);
  
  /* �ƥ�������ޥåԥ󥰳��� */
  glEnable(GL_TEXTURE_2D);
  
  /* �ƥ��������ɸ�μ�ư������ͭ���ˤ��� */
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glEnable(GL_TEXTURE_GEN_Q);

  /* Ȣ������ */
  glPushMatrix();
  glTranslated(-1.2, 0.0, 0.0);
  box(1.0, 1.0, 1.0);
  glPopMatrix();
  glPushMatrix();
  glTranslated(1.2, 0.0, 0.0);  
  box(1.0, 1.0, 1.0);
  glPopMatrix();
  
  /* �ƥ��������ɸ�μ�ư������̵���ˤ��� */
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisable(GL_TEXTURE_GEN_Q);

  /* �ƥ�������ޥåԥ󥰽�λ */
  glDisable(GL_TEXTURE_2D);
  
  /* ����ե��ƥ��Ƚ�λ */
  //glDisable(GL_ALPHA_TEST);
}


/****************************
** GLUT �Υ�����Хå��ؿ� **
****************************/

/* �ȥ�å��ܡ�������Ѵؿ������ */
#include "trackball.h"

/* ���˥᡼�����Υ������� */
#define FRAMES 360

static void display(void)
{
  /* �ե졼����򥫥���Ȥ��ƻ��֤Ȥ��ƻȤ� */
  static int frame = 0;                      /* �ե졼����������������� */
  double t = (double)frame / (double)FRAMES; /* ���֤ȤȤ�� 0��1 ���Ѳ� */
  
  if (++frame >= FRAMES) frame = 0;
  
#if 0
  /* �ƥ��������������� */
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslated(0.5, 0.5, 0.0);
  glRotated(t * 360.0, 0.0, 0.0, 1.0);
  glScaled(0.5, 0.5, 1.0);
  gluPerspective(60.0, 1.0, 1.0, 100.0);
  gluLookAt(0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  /* �ȥ�å��ܡ�������ˤ���ž */
  glMultMatrixd(trackballRotation());
#endif
  
  /* ��ǥ�ӥ塼�Ѵ���������� */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* �����ΰ��֤����� */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* �����ΰ�ư��ʪ�Τ�������˰�ư��*/
  glTranslated(0.0, 0.0, -5.0);
  
#if 1
  /* �ȥ�å��ܡ�������ˤ���ž */
  glMultMatrixd(trackballRotation());
#endif

  GLdouble model[16], proj[16];
  glGetDoublev(GL_MODELVIEW_MATRIX, model);
  glGetDoublev(GL_PROJECTION_MATRIX, proj);

  glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
  glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
  glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
  glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

  /* �ƥ��������������� */
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslated(0.5, 0.5, 0.0);
  glRotated(t * 360.0, 0.0, 0.0, 1.0);
  glScaled(0.5, 0.5, 1.0);
  //gluPerspective(60.0, 1.0, 1.0, 100.0);
  //gluLookAt(0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  glMultMatrixd(proj);
  glMultMatrixd(model);

  /* ��ǥ�ӥ塼�Ѵ������������᤹ */
  glMatrixMode(GL_MODELVIEW);
  
  /* ���̥��ꥢ */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* ����������� */
  scene();
  
  /* ���֥�Хåե���� */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* �ȥ�å��ܡ��뤹���ϰ� */
  trackballRegion(w, h);
  
  /* ������ɥ����Τ�ӥ塼�ݡ��Ȥˤ��� */
  glViewport(0, 0, w, h);
  
  /* Ʃ���Ѵ�����λ��� */
  glMatrixMode(GL_PROJECTION);
  
  /* Ʃ���Ѵ�����ν���� */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);
}

static void idle(void)
{
  /* ���̤������ؤ� */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* �ȥ�å��ܡ��볫�� */
      trackballStart(x, y);
      break;
    case GLUT_UP:
      /* �ȥ�å��ܡ������ */
      trackballStop(x, y);
      break;
    default:
      break;
    }
    break;
    default:
      break;
  }
}

static void motion(int x, int y)
{
  /* �ȥ�å��ܡ����ư */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC �� q �� Q �򥿥��פ����齪λ */
    exit(0);
  default:
    break;
  }
}

/*
** �ᥤ��ץ����
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
