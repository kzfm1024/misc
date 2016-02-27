/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

���F�}�b�v
[Shift] + 's'�ŕۑ�

*/
//�쐬����texture image�̃T�C�Y
#define TEX_WIDTH 440
#define TEX_HEIGHT 100
//�ۑ��t�@�C����
#define FILENAME "file0.bmp"

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../imageLoadSave.h"

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void makeColorData();
void display();
void keyboard(unsigned char key, int x, int y);

//Window�̃T�C�Y
int width = TEX_WIDTH+100;
int height = TEX_HEIGHT+100;
//texture image
GLubyte image[TEX_HEIGHT * TEX_WIDTH][3];

Bitmap* bm = new Bitmap();
//�`��̏����g��
float lambda0 = 360.0;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //�\�����[�h
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  //�\���E�B���h�E�̃T�C�Y
  glutInitWindowSize(width, height);
  //����̈ʒu
  glutInitWindowPosition(100, 100);
  //�E�B���h�E�쐬
  glutCreateWindow("���F�}�b�v�쐬(GL_RainbowMap)");
  //�L�[�{�[�h�̗��p
  glutKeyboardFunc(keyboard);
  //���L�[�Ȃǂ̓���L�[���p
//  glutSpecialFunc(special);
  //�\��
  glutDisplayFunc(display);

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
  glClearColor(1.0, 1.0, 1.0, 1.0);
//	bm->imageType = COLOR24;//imageLoadSave.h�Œ�`
}

void makeColorData()
{
  int i, j;
	float red = 0.0, green = 0.0, blue = 0.0, x;
	float red1, red2;
	float ultraviolet = 380.0;
	float infrared = 780.0;
	//���S
	float b0 = 0.2;//460nm;
	float g0 = 0.4;//540nm;
	float r0 = 0.7;//660nm
	float m0 = 0.08;//412
	//��
	float wR = 0.3;//0.25;//R,G
	float wB = 0.2;//0.175;//B
	float wV = 0.08;//Violet

	for(i = 0; i < TEX_WIDTH; i++)
	{
		int ii = i + lambda0;
		x = ((float)(ii) - ultraviolet) /(infrared - ultraviolet);//���K���g��
		//blue
		blue = 255.0 * (1.0 - (x - b0)*(x - b0) / (wB * wB)) ;
		if(blue < 0.0) blue = 0.0;
		//green
		if(x < g0) green = 255.0 * (1.0 - (x - g0)*(x - g0) / (wB * wB));
		else       green = 255.0 * (1.0 - (x - g0)*(x - g0) / (wR * wR));
		if(green < 0.0) green = 0.0;
		//red
		red1 = 255.0 * (1.0 - (x - r0)*(x - r0) / (wR * wR));
		if(red1 < 0.0) red1 = 0.0;
		//magenta
		red2 = 170.0 * (1.0 - (x - m0)*(x - m0) / (wV * wV));
		if(red2 < 0.0) red2 = 0.0;
		red = red1 + red2;
		if(blue > 255.0) {printf("blue=%f x= %f\n", blue, x);
		blue = 255.0;}
		if(red > 255.0) red = 255.0;

		for(j = 0; j < TEX_HEIGHT; j++)
		{
			image[j * TEX_WIDTH + i][0] = red;
			image[j * TEX_WIDTH + i][1] = green;
			image[j * TEX_WIDTH + i][2] = blue;
		}
	}
}

void display()
{
  //�J���[�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT );
	makeColorData();

	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex2f(-0.9, -0.7);
		glVertex2f(0.9, -0.7);
	glEnd();
	//400nm
	glBegin(GL_LINES);
		glVertex2f(-0.6666, -0.7);
		glVertex2f(-0.6666, -0.5);
	glEnd();
	//500nm
	glBegin(GL_LINES);
		glVertex2f(-0.29629, -0.7);
		glVertex2f(-0.29629, -0.5);
	glEnd();
	//600nm
	glBegin(GL_LINES);
		glVertex2f( 0.074074, -0.7);
		glVertex2f( 0.074074, -0.5);
	glEnd();
	//700nm
	glBegin(GL_LINES);
		glVertex2f( 0.44444, -0.7);
		glVertex2f( 0.44444, -0.5);
	glEnd();
	//700nm
	glBegin(GL_LINES);
		glVertex2f( 0.81481, -0.7);
		glVertex2f( 0.81481, -0.5);
	glEnd();

  //�`��
	glRasterPos2f(-0.8, -0.5);
	glDrawPixels(TEX_WIDTH,TEX_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
	//

  //�I��
	//glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch((unsigned char)key)
  {
  case 27://Esc
	  exit(0);
	  break;
  case 'S'://image�̕ۑ�([Shift] + 's')
		saveBitmap24(bm, FILENAME, TEX_WIDTH, TEX_HEIGHT, image); 
	  break;

  default:
	  break;
  }
}

