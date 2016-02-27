/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�F���
�Œ�V�F�[�_�[�iOpenGL�A�v���P�[�V�����j
*/
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
//#define M_PI 3.14159265358979

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void draw();
void drawColorSpace();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

//affine�ϊ��p�ϐ�
float pos[] = { 0.0, 0.0, 0.0 };    //object���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };  //�傫��(�{��)
float angle[] = { 0.0, -30.0, 0.0 };//��]�p�x
//�����l
float pos0[] = { 0.0, 0.0, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, -30.0, 0.0 };

//�J�����Ǝ��̐�
struct View
{
  //�J����
	float pos[3];//�ʒu�i���_�j
	float cnt[3];//�����_
  float dist;  //�����_���王�_�܂ł̋���
  float theta; //�p�i�����ʂƂ̕Ίp�j
  float phi;   //���ʊp
  //���̐�
  float fovY;  //����p
  float nearZ; //�O���N���b�v��(�ߕ���)
  float farZ;  //����N���b�v��(������)
};
//�����l
View view = { 
	0.0, 2.0, 4.0,//pos
  0.0, 0.0, 0.0,//cnt 
	4.0, 30.0, 20.0,//dist, theta, phi
	30.0, 1.0, 100.0//fovY,nearZ, farZ
};

//Window�̃T�C�Y
int width = 500;
int height = 500;
//�A�t�B���ϊ�
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE};
SELECT_KEY sKey = TRANSLATE;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //�\�����[�h
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  //�\���E�B���h�E�̃T�C�Y
  glutInitWindowSize(width, height);
  //����̈ʒu
  glutInitWindowPosition(100, 100);
  //�E�B���h�E�쐬
  glutCreateWindow("�F���(GL_ColorSpace)");
  //�E�B���h�E�̃T�C�Y�ύX
  glutReshapeFunc(resize);
  //�\��
  glutDisplayFunc(display);
  //�L�[�{�[�h�̗��p
  glutKeyboardFunc(keyboard);
  //���L�[�Ȃǂ̓���L�[���p
  glutSpecialFunc(special);
  //�ĕ`��
  glutIdleFunc(idle);
  //�����ݒ�
  init();
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
  return 0;
}

void idle(void)
{
  //�ĕ`��
  glutPostRedisplay();
}

void init(void)
{
 //�w�i�F
  glClearColor(0.2, 0.2, 0.3, 1.0);
}

void display(void)
{
  //�J���[�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT);

  //�`��
  draw();
  //�I��
  glutSwapBuffers();
}

void draw(void)
{
  //���[�h
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_POINT);//GL_LINE�ɂ���Ƃ��ׂĒ����\��
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0], 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0], scale[1], scale[2]);
  drawColorSpace();  //�`��I�u�W�F�N�g
  glPopMatrix();
}

void drawColorSpace()
{
  float p[8][3] = 
  { {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}
  };
  float c[8][3] = //�e���_�̐F
  { {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {1.0,1.0, 0.0},
	{0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}
  };

  glBegin(GL_QUADS);
	glColor3fv(c[0]); glVertex3fv(p[0]); 
	glColor3fv(c[1]); glVertex3fv(p[1]);
	glColor3fv(c[2]); glVertex3fv(p[2]); 
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[0]); glVertex3fv(p[0]);
	glColor3fv(c[3]); glVertex3fv(p[3]);
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[4]); glVertex3fv(p[4]);

	glColor3fv(c[0]); glVertex3fv(p[0]); 
	glColor3fv(c[4]); glVertex3fv(p[4]);
	glColor3fv(c[5]); glVertex3fv(p[5]); 
	glColor3fv(c[1]); glVertex3fv(p[1]);

	glColor3fv(c[1]); glVertex3fv(p[1]); 
	glColor3fv(c[5]); glVertex3fv(p[5]);
	glColor3fv(c[6]); glVertex3fv(p[6]); 
	glColor3fv(c[2]); glVertex3fv(p[2]);

	glColor3fv(c[2]); glVertex3fv(p[2]); 
	glColor3fv(c[6]); glVertex3fv(p[6]);
	glColor3fv(c[7]); glVertex3fv(p[7]); 
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[4]); glVertex3fv(p[4]); 
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[6]); glVertex3fv(p[6]); 
	glColor3fv(c[5]); glVertex3fv(p[5]);
  glEnd();
}

void resize(int w, int h)
{
  //�r���[�|�[�g�ϊ�
  glViewport(0, 0, w, h);
  //�v���W�F�N�V�����s��̎w��
  glMatrixMode(GL_PROJECTION);
  //�v���W�F�N�V�����s��̏�����
  glLoadIdentity();
  //�������e�s��̐ݒ�i���e�ϊ��j
  gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
  glMatrixMode(GL_MODELVIEW);
  //���f���r���[�s��̏�����
  glLoadIdentity();
  //���_��ݒ�i�r���[�C���O�ϊ��j
  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  //�\���E�B���h�E�̃T�C�Y
  width = w;
  height = h;
}


void keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
  case 27://Esc
	  exit(0);
	  break;
  case 'r':
	  sKey = ROTATE;
	  break;
  case 's':
	  sKey = SCALE;
	  break;
  case 't':
	  sKey = TRANSLATE;
	  break;
  case 'R'://reset�i�����l�ɖ߂�)
	  for(int i = 0; i < 3; i++)
		{
			pos[i] = pos0[i];
			scale[i] = scale0[i];
			angle[i] = angle0[i];
		}
	  break;
  default:
	  break;
  }
}

void special(int key, int x, int y)
{
  if(sKey == TRANSLATE)
  {
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x����
		  pos[0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://-x����
		  pos[0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) pos[1] += 0.1;//y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] -= 0.1; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) pos[1] -= 0.1;//-y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] += 0.1; //-z����
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == ROTATE)
  {
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x����]
		  angle[0] += 10.0;
		  break;
	  case GLUT_KEY_LEFT://-x����]
		  angle[0] -= 10.0;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) angle[1] += 10;//y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] += 10; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) angle[1] -= 10;//-y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] -= 10; //-z����
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == SCALE)
  {
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x�����g��
		  scale[0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://x�����k��
		  scale[0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) scale[1] += 0.1;//y�����g��
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] += 0.1; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) scale[1] -= 0.1;//y�����k��
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] -= 0.1; //z����
		  break;
	  default:
		  break;
	  }
  }
}

