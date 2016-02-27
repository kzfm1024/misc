/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

OpenGL+GLSL
FFT�ɂ��C�Ɖ_
�R�x�n�`�Ƒ��z��ǉ�
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../midpoint.h"
#include "terrain.h"
#include "../../fft.h"
#include "../../imageLoadSave.h"

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void keyboard(GLubyte key, int x, int y);
void special(int key, int x, int y);
void resize(int w, int h);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void setCamera();
void setLight();
void makeTerrainImage(Bitmap *bmp);
void setTerrainTexture();
void makeCloudImage();
void setCloudTexture();
void drawWave();
void drawCloud();
void drawSun();
void makeSeaCloudData();

//����
float lightPos[]  = {0.0, 13.0, -50.0, 1.0};//�����ʒu
float lightPos0[] = {0.0, 13.0, -50.0, 1.0};//�����ʒu(�����l�j

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
View view = { 
	0.0, 0.0, 0.0,//pos(���ݒ�j
  0.0, 8.0, 0.0,//cnt 
	20.0, 10.0, 0.0,//dist, theta, phi
	30.0, 1.0, 300.0//fovY,nearZ, farZ
};
View view0 = view;

//Window�̃T�C�Y
int width = 500;
int height = 500;
//�A�t�B���ϊ�
enum SELECT_KEY {LIGHT};
SELECT_KEY sKey = LIGHT;
//�}�E�X����
int xStart, yStart;
bool flagMouse = false;
//�w���v�L�[�t���O
bool flagHelp = false;
//�����ʒu�ύX�t���O
bool flagLight = false;//�ǉ�
//�o�ߎ���
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
//terrainø����p
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][3];
GLuint texName[2];
//�V�F�[�_�v���O������
GLuint shaderProg;

//cloud texture
#define texWidthC  128
#define texHeightC 128
GLubyte texCloud[texHeightC][texWidthC][4];
//�C�ʁE�_�f�[�^
#define nMesh 128 //�ő啪�����i���ӓ����j
float sizeX = 36.0;
float sizeY = 80.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//�C�ʃf�[�^
float sr[nMesh*nMesh], si[nMesh*nMesh];
float tblSin[nMesh], tblCos[nMesh];//sin,cosð���
float amp[nMesh * nMesh];
float phiSea[nMesh * nMesh];//�C�ʂ̈ʑ�
float phiCloud[nMesh * nMesh];//�_�̈ʑ�
float beta0 = 3.0;//fBm�̎w����
float amp0 = 0.2;//��{�g�̐U��
float deltaWave = 0.2;
float deltaCloud = 0.05;
int K1 = 1 ;//k�̒᎟������
int K2 = 20;//k�̍���������
int L1 = 2; //l�̒᎟������
int L2 = 20;//l�̍���������
//FFT�C���X�^���X���쐬
CFFT fft = CFFT();

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
  glutCreateWindow("�V�[��1(SL_Scene1)");
  //�E�B���h�E�̃T�C�Y�ύX
  glutReshapeFunc(resize);
  //�\��
  glutDisplayFunc(display);
  //�L�[�{�[�h�̗��p
  glutKeyboardFunc(keyboard);
  //���L�[�Ȃǂ̓���L�[���p
  glutSpecialFunc(special);
  //�}�E�X����
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  //�ĕ`��
  glutIdleFunc(idle);
  //�����ݒ�
  init();
	initGlsl(&shaderProg, "terrain.vert", "terrain.frag");
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
	//���\�[�X�̉��
	glDeleteTextures(2, texName);
	glDeleteProgram(shaderProg);
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

  setCamera();//���_�����߂�
  setLight(); //�����ݒ�@
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("�}�E�X/�L�[����̐����ɂ�'h'�L�[���v�b�V�� \n");

  glGenTextures(2, texName);//�e�N�X�`���I�u�W�F�N�g�̖��O�t��
	makeSeaCloudData();

	makeTerrainData();
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/altgrad1.bmp");
  makeTerrainImage(bm0);
  setTerrainTexture();

	//�v�����Ԃ̃N���A
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
  lastTime = timeGetTime();
}

void makeSeaCloudData()
{
	//�C�ʁA�_�f�[�^�̍쐬�iFFT)
	fft = CFFT(K1, K2, L1, L2, beta0, amp0, -1, nMesh);
  //sin,cosð���
  fft.makeTable(tblSin, tblCos);
  //�U���Ə����ʑ�
  fft.makeAmpPhase(amp, phiSea);
  //�_�̏����ʑ��͊C�ʂƓ���
  for(int l = 0; l < nMesh; l++)
	 for(int k = 0; k < nMesh; k++) phiCloud[k+l*nMesh] = phiSea[k+l*nMesh];
}
//----------------------------------------------------------------
void makeCloudImage()
{
  int i, j;
  float c;

  for(j = 0; j < texHeightC; j++)
		for(i = 0; i < texWidthC; i++)
		{
			if(sr[i + j * texWidthC] > 0.0)
			{//cloud
				if(sr[i + j * texWidthC] > 1.0) sr[i + j * texWidthC] = 1.0;
				c = 1.0 - sr[i + j * texWidthC];
				if(c < 0.0) c = 0.0;
				texCloud[j][i][0] = (GLubyte) (250.0 * c);//red
				texCloud[j][i][1] = (GLubyte) (150.0 * c);//green
				texCloud[j][i][2] = (GLubyte) (100.0 * c);//blue
				texCloud[j][i][3] = (GLubyte) 200.0;//alpha(�������j
			}
			else
			{//sky
				texCloud[j][i][0] = (GLubyte) 150;//red
				texCloud[j][i][1] = (GLubyte) 50; //green
				texCloud[j][i][2] = (GLubyte) 250;//blue
				texCloud[j][i][3] = (GLubyte) 0.0;//alpha�i�����j
			}
		}
}

void setCloudTexture()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texName[1]);//�e�N�X�`�����o�C���h����
  //�e�N�X�`���̊��蓖�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texWidthC,texHeightC,0,GL_RGBA,GL_UNSIGNED_BYTE,texCloud);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //�F�̒���
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
}

//�摜�t�@�C������C���[�W�f�[�^���쐬
void makeTerrainImage(Bitmap *bm)
{
  int i, j, ii, jj;

  if(bm->bi.Width < TEX_WIDTH) printf("�摜�t�@�C���̻���>=TEX_WIDTH�Ƃ��邱��\n");

  for(j = 0; j < TEX_HEIGHT; j++)
  {
		jj = (int)((float)j * (float)bm->bi.Height / (float)TEX_HEIGHT);
		for(i = 0; i < TEX_WIDTH; i++)
    {
      ii = (int)((float)i * (float)bm->bi.Width / (float)TEX_WIDTH) ;
			if(bm->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bm->pixel[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixel[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixel[ii+jj*bm->bi.Width]).b;
			}
			else
			{
				texImage[j][i][0] = (bm->pixelA[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixelA[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixelA[ii+jj*bm->bi.Width]).b;
			}
    }
  }
	if(bm->bi.BitCount <= 24) free(bm->pixel);
	else free(bm->pixelA);
}

void setTerrainTexture()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texName[0]);//�e�N�X�`�����o�C���h����
  //�e�N�X�`���̎w�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,texImage);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void display(void)
{
  curTime = timeGetTime();
  float dt = (float)(curTime - lastTime) * 0.001;//sec�ɕϊ�
  elapseTime1 += dt;
  elapseTime2 += dt;
  fps ++;
  if(elapseTime1 >= 1.0)
  {
		printf("frame per sec = %d \n", fps);
		elapseTime1 = 0.0;
		fps = 0;
  }
	lastTime = curTime;

  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();//���_��ς���Ƃ��͂��̈ʒu�ɕK�v
  if(cos(M_PI * view.theta /180.0) >= 0.0)//�J�����p90�x���ޭ������޸�ِؑ�
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  //�`��
	glUseProgram(shaderProg);
  GLint texSampler = glGetUniformLocation(shaderProg, "tex");
  glUniform1i(texSampler, 0);
  drawTerrain();
  glUseProgram(0);

  //���������̂�����Ƃ��͎���2�s���K�v
  glCullFace(GL_BACK);   //���ʂ��폜
  glEnable(GL_CULL_FACE);//���ʍ폜��L���ɂ���
	drawSun();
	drawWave();
  //ø����A���������̂�����Ƃ�
  glDepthMask(GL_FALSE); //���߽�ޯ̧���������݋֎~
  glEnable(GL_BLEND);//��̧�����ިݸނ�L���ɂ���
  glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//�F�����W�������߂�
  //�������`��
//  glActiveTexture(GL_TEXTURE1);
//  glBindTexture(GL_TEXTURE_2D, texName[1]);
  drawCloud();
  //ø����A���������̂�����Ƃ�
  glDepthMask(GL_TRUE); //���߽�ޯ̧�̏������݂�����
  glDisable(GL_BLEND);

  if(flagHelp)
  {
		printf("���L�[�ɂ��A�t�B���ϊ�/�����ړ� \n");
		printf("�@��,���Fx�� \n");
		printf("�@��,���Fy�� \n");
		printf("�@[Shift]�{��,���Fz�� \n");
		printf("�@'l'����������A�����ʒu�̈ړ��� \n");
		printf("�}�E�X����Ŏ��_�ύX�� \n");
		printf("  dolly�F�����t�߂����{�^���N���b�N�ŋ߂Â��C�E�{�^���N���b�N�ŉ������� \n");
		printf("  pan�F��������щE�����E�{�^���N���b�N�Œ����_�����E�ɕω����� \n");
		printf("  tilt�F�^�エ��ѐ^�����E�{�^���N���b�N�Œ����_���㉺�ɕω����� \n");
		printf("  tumble�F���E�Ƀh���b�O����Ǝ��_�����E�ɕω����� \n");
		printf("  crane�F�㉺�Ƀh���b�O����Ǝ��_���㉺�ɕω����� \n");
		printf("  zoom�F�������E�{�^���N���b�N�ŃY�[���C�� \n");
		printf("        �E�����E�{�^���N���b�N�ŃY�[���A�E�g \n");
		printf("[Shift]+'r'�Ń��Z�b�g \n");
		printf("[F1]:k1, [F2]:k2, [F3]:L1, [F4]:L2�̒��� \n");
		printf("[F5]:beta, [F6]:amp0, [F7]:deltaWave, [F8]:deltaCloud �̒���\n");
		printf(" \n");
		flagHelp = false;
  }

  //�I��
  glutSwapBuffers();
}

void drawWave()
{
  float ambient[] = { 0.1f, 0.2f, 0.5f, 0.5f};
  float diffuse[] = { 0.2f, 0.4f, 1.0f, 0.5f};
  float specular[]= { 0.8f, 0.8f, 0.8f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,120);

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);//x����]
  //�ʑ���ω�
  for(int l = L1; l < L2; l++)
  for(int k = K1; k < K2 ; k++) phiSea[k + l* nMesh] += deltaWave;

  fft.makeFourier(amp, phiSea, sr, si);
  fft.ifft2d(sr, si, tblSin, tblCos);
  int i, j;
  for(j = 0; j < nMesh; j++) 
  {
	  for(i = 0; i < nMesh; i++) data[j * (nMesh+1) + i] = sr[j * nMesh + i] * (1.0-(float)j/(float)nMesh);
	  data[j * (nMesh+1) + nMesh] = sr[j * nMesh];//i=nMesh��i=0�Ɠ����Ƃ���
  }
  //j=nMesh�̂Ƃ��Aj=0�Ɠ����Ƃ���
  for(i = 0; i <= nMesh; i++) data[nMesh * (nMesh + 1) + i] = 0.0;//data[i];

  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

void drawCloud()
{
  float ambient[] = { 0.6f, 0.5f, 0.4f, 1.0f};
  float diffuse[] = { 0.9f, 0.9f, 0.6f, 1.0f} ;
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  //�ʑ���ω�
  for(int l = L1; l < L2; l++)
  for(int k = K1; k < K2 ; k++) phiCloud[k + l* nMesh] += deltaCloud;//0.02;
  fft.makeFourier(amp, phiCloud, sr, si);
  fft.ifft2d(sr, si, tblSin, tblCos);
  makeCloudImage();
  setCloudTexture();

  glPushMatrix();
  glTranslatef(0.0, 7.0, -sizeY / 2.0);
  glRotatef(30.0, 1.0, 0.0, 0.0);//x����]
  glScalef(sizeX, 25.0, 1.0);
  //�I�u�W�F�N�g�`��
  drawTexPlate(1.0, 1, 1);
  glPopMatrix();
}

void drawSun()
{
  float diffuse[] = { 1.0f, 0.2f, 0.0f, 1.0f};
  float ambient[] = { 1.0f, 0.1f, 0.0f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 0.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,1);

  glPushMatrix();
  glTranslatef(lightPos[0], lightPos[1] , lightPos[2] - 10.0);
  drawSphere(5.0, 20, 20);
  glPopMatrix();
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);//
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
  glMatrixMode(GL_MODELVIEW);
  width = w;
  height = h;
}

void setLight()
{
  float lightAmbient[] = {1.0f, 0.5f, 0.5f, 1.0f}; //����
  float lightDiffuse[] = {1.0f, 0.5f, 0.5f, 1.0f}; //�g�U��
  float lightSpecular[] = {1.0, 0.5f, 0.5f, 1.0f}; //���ʌ�
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
}

void keyboard(GLubyte key, int x, int y)
{
  switch((GLubyte)key)
  {
  case 27://Esc
	  exit(0);
	  break;
  case 'R'://reset
	  //pos[0] = 0.0; pos[1] = 0.0; pos[2] = 0.0;
	  //scale[0] = scale[1] = scale[2] = 1.0;
	  //angle[0] = angle[1] = angle[2] = 0.0;
	  for(int i = 0; i < 3; i++) lightPos[i] = lightPos0[i];
	  view = view0;
	  setCamera();
	  break;
  case 'l':
		sKey = LIGHT;
	  break;
  case 'h':
	  if(flagHelp) flagHelp = false;
	  else flagHelp = true;
	  break;

  default:
	  break;
  }
}

void special(int key, int x, int y)
{
  if(sKey == LIGHT)
  {
		float d = 1.0;
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x����
		  lightPos[0] += d;
		  break;
	  case GLUT_KEY_LEFT:
		  lightPos[0] -= d;//-x����
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) lightPos[1] += d;//y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] -= d;//-z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1] -= d;//-y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] += d;//z����
		  break;
	  default:
		  break;
	  }
  }

  //K1
  if(key == GLUT_KEY_F1) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K1 -= 1;
	  else K1 += 1;
	  if(K1 <= 1) K1 = 1;
	  if(K1 >= K2) K1 = K2 -1;
		printf("K1 = %d \n", K1);
	  makeSeaCloudData();
  }
  //K2
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K2 -= 1;
	  else K2 += 1;
	  if(K2 <= K1) K2 = K1 + 1;
	  if(K2 >= nMesh/2) K2 = nMesh/2 - 1;
		printf("K2 = %d \n", K2);
	  makeSeaCloudData();
  }
  //L1
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L1 -= 1;
	  else L1 += 1;
	  if(L1 <= 1) L1 = 1;
	  if(L1 >= L2) L1 = L2 -1;
		printf("L1 = %d \n", L1);
	  makeSeaCloudData();
  }
  //L2
  if(key == GLUT_KEY_F4) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L2 -= 1;
	  else L2 += 1;
	  if(L2 <= L1) L2 = L1 + 1;
	  if(L2 >= nMesh/2) L2 = nMesh/2 - 1;
		printf("L2 = %d \n", L2);
	  makeSeaCloudData();
  }
	//beta0
  if(key == GLUT_KEY_F5) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta0 -= 0.1;
	  else beta0 += 0.1;
		printf("beta0 = %f \n", beta0);
	  makeSeaCloudData();
  }

  //amp��ς���
  if(key == GLUT_KEY_F6) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp0 -= 0.01;
	  else amp0 += 0.01;
		printf("amp0 = %f \n", amp0);
	  makeSeaCloudData();
  }
  //���ʔg�ʑ��V�t�g
  if(key == GLUT_KEY_F7) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaWave -= 0.01;
	  else deltaWave += 0.01;
		printf("deltaWave = %f \n", deltaWave);
	  makeSeaCloudData();
  }
  //�_�ʑ��V�t�g
  if(key == GLUT_KEY_F8) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaCloud -= 0.001;
	  else deltaCloud += 0.001;
		printf("deltaCloud = %f \n", deltaCloud);
	  makeSeaCloudData();
  }

  else
  {
  }
}

//�ȉ��̂R�̊֐��̓}�E�X����ɂ�鎋�_�̕ύX�ɕK�v
void mouse(int button, int state, int x, int y)
{
  double pp = M_PI / 180.0;

  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
	  xStart = x; yStart = y;
	  flagMouse = true;
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist -= 0.5; //�߂Â�
	  }
  }
  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist += 0.5; //��������
	  }
	  else if(( x < width/4 || x > 3*width/4) && (y > height/4 && y < 3*height/4))//pan
	  {
		  if(x < width/4 ) view.phi -= 1.0;
		  else view.phi += 1.0;
		  view.cnt[2] = view.pos[2] - view.dist * cos(pp * view.phi) * cos(pp * view.theta);
		  view.cnt[0] = view.pos[0] - view.dist * sin(pp * view.phi) * cos(pp * view.theta);
	  }
	  else if((x > width/4 && x < 3*width/4) && (y < height/4 || y > 3*height/4))//tilt
	  {
		  if( y < height/4)
			  view.theta += 1.0; 
		  else
			  view.theta -= 1.0;

		  view.cnt[2] = view.pos[2] - view.dist * cos(pp * view.theta) * cos(pp * view.phi);
		  view.cnt[0] = view.pos[0] - view.dist * cos(pp * view.theta) * sin(pp * view.phi);
		  view.cnt[1] = view.pos[1] - view.dist * sin(pp * view.theta);
	  }
	  else if(x < width/8 && y > 7*height/8) view.fovY -= 1.0;//zoom in
	  else if(x > 7*width/8 && y > 7*height/8) view.fovY += 1.0;//zoom out
  }
  else flagMouse = false;
  if(state == GLUT_DOWN) setCamera();
}

void motion(int x, int y)
{
  if(!flagMouse) return;
  if(cos(M_PI * view.theta /180.0) >= 0.0)
	view.phi -= 0.5 * (float)(x - xStart) ;//tumble
  else
	view.phi += 0.5 * (float)(x - xStart) ;//tumble

  view.theta += 0.5 * (float)(y - yStart) ;//crane

  setCamera();
  xStart = x;
  yStart = y;
}

void setCamera()
{
  double pp = M_PI / 180.0;
  view.pos[2] = view.cnt[2] + view.dist * cos(pp * view.theta) * cos(pp * view.phi);//z
  view.pos[0] = view.cnt[0] + view.dist * cos(pp * view.theta) * sin(pp * view.phi);//x
  view.pos[1] = view.cnt[1] + view.dist * sin(pp * view.theta);//y
//  printf("view.posX=%f, view.posY=%f, view.posZ=%f \n", view.pos[0], view.pos[1], view.pos[2]);
//  printf("cntX=%f, cntY=%f, cntZ=%f \n", view.cnt[0], view.cnt[1], view.cnt[2]);
//  printf("phi=%f, theta=%f, fovY=%f \n", view.phi, view.theta, view.fovY);
	resize(width, height);
}
