/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

���_�ψʖ@�ɂ��2�����e�N�X�`���̍쐬
*/
//�쐬����texture image�̃T�C�Y
#define TEX_WIDTH 256
#define TEX_HEIGHT 256
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
double getNormalRandom(double sigma0);
void makeTextureData();
void makeImageData();
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void display();

//����������
int numDivX0 = 2;
int numDivY0 = 2;

float data[(TEX_WIDTH+1)][(TEX_HEIGHT+1)];//�Z�x�f�[�^
float h[(TEX_WIDTH+1)][(TEX_HEIGHT+1)];//�����l,���Ԓl
double beta = 2.0;//fBm�w��
double sigma0 = 0.3;//�W���΍������l
int seed = 0;//�����̼�āi�l��ς���ƈقȂ����p�^�[�����쐬�j�

//Window�̃T�C�Y
int width = TEX_WIDTH;
int height = TEX_HEIGHT;
//texture image
GLubyte image[TEX_HEIGHT * TEX_WIDTH][3];
//�������l
float thUpper = 0.8;
float thLower = 0.5;
//�o�ߎ���
float curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
bool flagHelp = false;

Bitmap* bm = new Bitmap();

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
  glutCreateWindow("ø����쐬(GL_TexMaker2D)");
  //�L�[�{�[�h�̗��p
  glutKeyboardFunc(keyboard);
  //���L�[�Ȃǂ̓���L�[���p
  glutSpecialFunc(special);
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
  glClearColor(0.0, 0.0, 0.0, 1.0);

	printf("[s] + [Shift]�L�[�FFILENAME�փZ�[�u \n");
	printf("function key�ɂ��p�����[�^�̒��� \n");
	printf(" [F1]�L�[�F�n�[�X�g�w��hurst�̒��� \n");
	printf(" [F2]�L�[�F�W���΍�sigma�̒��� \n");
	printf(" [F3]�L�[�F�����n��seed�̕ύX \n");
}

void makeTextureData()
{//���_�ψʖ@�ɂ�鍂�x�f�[�^�쐬
  int i, j, ii, jj;
	int numDivX = numDivX0;
	int numDivY = numDivY0;
	if(((float)numDivX0 / (float)numDivY0) != ((float)TEX_WIDTH / (float)TEX_HEIGHT))
	{
		printf("�������̐ݒ肪�Ԉ���Ă��܂�\n");
		return;
	}

  srand(seed);
	double hurst = (beta-1.0) / 2.0;

  //�������x�f�[�^
  for(j = 0; j <= numDivY; j++)
		for(i = 0; i <= numDivX; i++) 
		{
			if(i == 0 || i == numDivX || j == 0 || j == numDivY)
				h[i][j] = -0.3;
			else h[i][j] = 0.7;
		}

	double sigma = sigma0 ;

	while(numDivX <= TEX_WIDTH / 2)
  {
		sigma /= pow(2.0, hurst);
		numDivX *= 2;//��������2�{
		numDivY *= 2;
	  	
		for(jj = 0; jj <= numDivY; jj++)
		{
			for(ii = 0 ; ii <= numDivX; ii++)
			{
				j = jj / 2;
				i = ii / 2;
				if(j*2 == jj && i*2 == ii) //jj=even, ii=even
				{
					data[ii][jj] = h[i][j];//�m��l
				}

				else if(j*2 == jj && i*2 != ii)//jj=even, ii=odd
				{
					data[ii][jj] = (h[i][j] + h[i+1][j]) / 2.0 + getNormalRandom(sigma);
				}
				else if(j*2 != jj && i*2 == ii)//jj=od, ii=even
				{
					data[ii][jj] = (h[i][j] + h[i][j+1]) / 2.0 + getNormalRandom(sigma);
				}
				else //jj=odd, ii=odd
				{
					data[ii][jj] = (h[i][j] + h[i+1][j] + h[i][j+1] + h[i+1][j+1]) / 4.0 
						+ getNormalRandom(sigma);
				}
			}
		}
		for(j = 0; j <= numDivY; j++) 
			for(i = 0; i <= numDivX; i++) h[i][j] = data[i][j];
	}
	for(j = 0; j <= numDivY; j++) 
		for(i = 0; i <= numDivX; i++) 
		{
			if( data[i][j] >= 1.0) data[i][j] = 1.0;
			if( data[i][j] <= 0.0) data[i][j] = 0.0;
		}
}

double getNormalRandom(double sigma)
{//���K����
  double ran = 0.0;
  for(int i=0; i < 12; i++) ran += (double)rand() / (double)RAND_MAX;
  
  ran -= 6.0;
  ran *= sigma;
  return ran;
}

void makeImageData()
{
  int i, j;

  for(j = 0; j < TEX_HEIGHT; j++)
  {
		for(i = 0; i < TEX_WIDTH; i++)
    {
			if(data[i][j] > thUpper)
			{
				image[j * TEX_WIDTH + i][0] = 255;
				image[j * TEX_WIDTH + i][1] = 255;
				image[j * TEX_WIDTH + i][2] = (GLubyte)(255.0 * (data[i][j] - thUpper) / (1.0 - thUpper));
			}
			else if(data[i][j] > thLower)
			{
				image[j * TEX_WIDTH + i][0] = 255;
				image[j * TEX_WIDTH + i][1] = (GLubyte)(255.0 * (data[i][j] - thLower) / (thUpper - thLower));
				image[j * TEX_WIDTH + i][2] = 0;
			}
			else
			{
				image[j * TEX_WIDTH + i][0] = (GLubyte)(255.0 * data[i][j] / thLower);
				image[j * TEX_WIDTH + i][1] = 0;
				image[j * TEX_WIDTH + i][2] = 0;
			}
		}
  }
}

void display()
{
  //�J���[�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT );
	//���_�ψʖ@�ɂ��texture�f�[�^�̍쐬
	makeTextureData();
	makeImageData();

  //�`��
	glDrawPixels(TEX_WIDTH,	TEX_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);

  //�I��
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

void special(int key, int x, int y)
{
  //hurst��ς���
  if(key == GLUT_KEY_F1) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta -= 0.1;
	  else beta += 0.1;
		printf("hurst = %f \n", beta);
  }
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) sigma0 -= 0.01;
	  else sigma0 += 0.01;
		printf("sigma0 = %f \n", sigma0);
  }
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) seed -= 1;
	  else seed += 1;
		printf("seed = %d \n", seed);
  }
}