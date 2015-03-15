/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

OpenGL+GLSL
FFT�ɂ��C
���_�ψʖ@�ɂ��_�iparticle)
�R�x�n�`�Ƒ��z
���ˊ��}�b�s���O
���˃I�u�W�F�N�g�ɉ_��ǉ�
�D�Ƃ��̍q�Ղ�ǉ�
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
#include "cloud.h"
#include "particle.h"
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
void setTerrainTexture(int n);
void makeParticleImage(Bitmap *bm);
void setParticleTexture(int n);
void drawParticle(float dt);
void drawWave();
void drawSun();
void makeSeaData();
void setCubeMap(int n);
void drawShip();
void addTrackWave();


//����
float lightPos[]  = {0.0, 50.0, -50.0, 1.0};//�����ʒu
float lightPos0[] = {0.0, 50.0, -50.0, 1.0};//�����ʒu(�����l�j

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
  0.0, 3.0, 0.0,//cnt 
	30.0, 0.0, 0.0,//dist, theta, phi
	80.0, 1.0, 120.0//fovY,nearZ, farZ
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
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][4];
GLuint texName[3];
//�V�F�[�_�v���O������
GLuint shaderProg1;
GLuint shaderProg2;

//�C�ʃf�[�^
#define nMesh 128 //�ő啪�����i���ӓ����j
float sizeX = 60.0;//16.0;
float sizeY = 60.0;//16.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//�C�ʃf�[�^
float sr[nMesh*nMesh], si[nMesh*nMesh];
float tblSin[nMesh], tblCos[nMesh];//sin,cosð���
float amp[nMesh * nMesh];
float phiSea[nMesh * nMesh];//�C�ʂ̈ʑ�
float beta0 = 2.5;//fBm�̎w����
float amp0 = 0.05;//��{�g�̐U��
float deltaWave = 0.2;
int K1 = 5 ;//k�̒᎟������
int K2 = 20;//k�̍���������
int L1 = 20; //l�̒᎟������
int L2 = 30;//l�̍���������
//FFT�C���X�^���X���쐬
CFFT fft = CFFT();
//���ˊ��}�b�s���O�p
#define texRefWidth  128
#define texRefHeight 128
//�e�N�X�`���[�\����
struct Target{
	GLenum name;//�^�[�Q�b�g��
	GLint x, y;//�����摜�̈ʒu
	GLdouble cx, cy, cz;//�����_
	GLdouble ux, uy, uz;//�A�b�v�x�N�g��
};

struct Target target[] = {
  { //�E
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    0, texRefHeight,
    1.0, 0.0, 0.0,
    0.0, -1.0, 0.0,
  },
  { 
	 //��
	 GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    0, 0,
    -1.0, 0.0, 0.0,
    0.0, -1.0, 0.0,
  },
  { //�V��
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    texRefWidth, texRefHeight,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  },
  { 
		//��
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    texRefWidth,0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
  },
  { //��O
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    2*texRefWidth, texRefHeight,
    0.0, 0.0, 1.0,
    0.0, -1.0, 0.0,
  },
  { //��
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    2*texRefWidth, 0,
    0.0, 0.0, -1.0,
    0.0, -1.0, 0.0,
  }
};
float reflectivity0 = 0.5;//�������˗�
float posShip0[] = { -10.0, 0.3, 20.0 };//�D��xyz���W
float posShip[3];
float speedShip = 0.3;// 0.1;//�D�̃X�s�[�h
//particle
CParticle p[NUM_PARTICLE];

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
  glutCreateWindow("�_2(SL_Cloud2)");
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
	initGlsl(&shaderProg1, "terrain.vert", "terrain.frag");
	initGlsl(&shaderProg2, "reflection.vert", "reflection.frag");
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
	//���\�[�X�̉��
	glDeleteTextures(3, texName);
	glDeleteProgram(shaderProg1);
	glDeleteProgram(shaderProg2);
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
  glClearColor(0.5, 0.4, 0.2, 1.0);

  setCamera();//���_�����߂�
  setLight(); //�����ݒ�@
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("�}�E�X/�L�[����̐����ɂ�'h'�L�[���v�b�V�� \n");

  glGenTextures(3, texName);//�e�N�X�`���I�u�W�F�N�g�̖��O�t��
	makeSeaData();

	makeTerrainData();
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/altgrad4.bmp");
  makeTerrainImage(bm0);
  setTerrainTexture(0);
  //���ˊ��}�b�s���O�p�e�N�X�`���̐ݒ�
  setCubeMap(1);

	//particle�pø���
  Bitmap* bm1 = new Bitmap();
  loadBitmap(bm1, "../../bmp/cloud.bmp");
  makeParticleImage(bm1);
  setParticleTexture(2);
	//�_���q�̔���
	makeCloudData();//cloud.h
  for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
	countP = 0.0;

	//���Ԍv��
  lastTime = timeGetTime();
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
}

void makeSeaData()
{
	//�C�ʁA�_�f�[�^�̍쐬�iFFT)
	fft = CFFT(K1, K2, L1, L2, beta0, amp0, -1, nMesh);
  //sin,cosð���
  fft.makeTable(tblSin, tblCos);
  //�U���Ə����ʑ�
  fft.makeAmpPhase(amp, phiSea);
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

void setTerrainTexture(int n)
{
  glBindTexture(GL_TEXTURE_2D, texName[n]);//�e�N�X�`�����o�C���h����
  //�e�N�X�`���̎w�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void setCubeMap(int n)
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[n]);//�e�N�X�`�����o�C���h����
  for(int i = 0; i < 6; i++)
	glTexImage2D(target[i].name,0,GL_RGB,texRefWidth,texRefHeight,0,GL_RGB,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//�o�C���h������
}

void makeParticleImage(Bitmap *bmp)
{
  int i, j, ii, jj;
  int nx = TEX_WIDTH;
  int ny = TEX_HEIGHT;
  float pWidth = (float)bmp->bi.Width;
  float pHeight = (float)bmp->bi.Height;

  for(j = 0; j < ny; j++)
  {
    jj = j * (int)(pHeight / (float)ny);
		for(i = 0; i < nx; i++)
    {
      ii = i * (int)(pWidth / (float)nx);
			if(bmp->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bmp->pixel[ii+jj*bmp->bi.Width]).r;
				texImage[j][i][1] = (bmp->pixel[ii+jj*bmp->bi.Width]).g;
				texImage[j][i][2] = (bmp->pixel[ii+jj*bmp->bi.Width]).b;
				//���𓧖���
				if( texImage[j][i][0] == 0 && texImage[j][i][1] == 0 &&  texImage[j][i][2] == 0)
				texImage[j][i][3] = 0; 
				else texImage[j][i][3] = 255;

			}
			else//32bit color
			{
				texImage[j][i][0] = (bmp->pixelA[ii+jj*bmp->bi.Width]).r;
				texImage[j][i][1] = (bmp->pixelA[ii+jj*bmp->bi.Width]).g;
				texImage[j][i][2] = (bmp->pixelA[ii+jj*bmp->bi.Width]).b;
				//���𓧖���
				if( texImage[j][i][0] == 0 && texImage[j][i][1] == 0 &&  texImage[j][i][2] == 0)
				texImage[j][i][3] = 0; 
				else texImage[j][i][3] = 255;
			}
    }
  }
}

void setParticleTexture(int n)
{
//  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texName[n]);//�e�N�X�`�����o�C���h����
 //�e�N�X�`���̊��蓖�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //�F�̒����i���̐ݒ�j
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    // �A���t�@�e�X�g�̔��ʊ֐� 
  glAlphaFunc(GL_GREATER, 0.1);

  //point sprite�̻��ނ���������
  float atten[] = {0.0, 0.0, 0.01};
  glPointParameterfv =
    (PFNGLPOINTPARAMETERFVPROC)wglGetProcAddress("glPointParameterfv");
  glPointParameterf =
    (PFNGLPOINTPARAMETERFPROC)wglGetProcAddress("glPointParameterf");
  glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, atten);
  glPointParameterf(GL_POINT_SIZE_MAX, 10000.0);

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

  //step1
  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // �����ϊ��s��̐ݒ� (texture�쐬�j
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.0, 1.0, 80.0);//����p��90��
  
  // ���f���r���[�ϊ��s��̐ݒ� 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GLint texSampler = glGetUniformLocation(shaderProg1, "tex");
  glUniform1i(texSampler, 0);//TEXTURE0
  
  //���f���_���猩���V�[�����e�N�X�`���ɗ��p 
  for (int i = 0; i < 6; ++i) 
  {
	  glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texName[0]);
    // �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� 
    glViewport(target[i].x, target[i].y, texRefWidth, texRefHeight);
		// ���f���_���猩������̂������_�����O 
		glPushMatrix();
		gluLookAt(view.pos[0], -view.pos[1], view.pos[2], 
			view.pos[0]+target[i].cx, -view.pos[1]+target[i].cy, view.pos[2]+target[i].cz, 
			target[i].ux, target[i].uy, target[i].uz);
		//�C�ʈȊO�������_�����O
		glUseProgram(shaderProg1);
		drawTerrain();
		glUseProgram(0);
		drawSun();
		drawShip();
    //�_��particle
		glBindTexture(GL_TEXTURE_2D, texName[2]);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		drawParticle(dt);
		glDisable(GL_BLEND);

		glPopMatrix();  
		// �����_�����O�����摜���e�N�X�`���������ɓ]�� 
		glCopyTexSubImage2D(target[i].name, 0, 0, 0, target[i].x, target[i].y, texRefWidth, texRefHeight);
  }

  //step2:���ۂ̃I�u�W�F�N�g�������_�����O
	resize(width, height);
  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(cos(M_PI * view.theta /180.0) >= 0.0)//�J�����p90�x���ޭ������޸�ِؑ�
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  //�`��
	glUseProgram(shaderProg1);
  drawTerrain();
  glUseProgram(0);

  //���������̂�����Ƃ��͎���2�s���K�v
  glCullFace(GL_BACK);   //���ʂ��폜
  glEnable(GL_CULL_FACE);//���ʍ폜��L���ɂ���
	drawSun();
	drawShip();

	//step3
	//���ˊ��}�b�s���O
  glUseProgram(shaderProg2);
  GLint refLoc = glGetUniformLocation(shaderProg2, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg2, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[1]);
  GLint texLoc = glGetUniformLocation(shaderProg2, "texCube");
  glUniform1i(texLoc, 0);//GL_TEXTURE1��K�p

  drawWave(); //�C�ʕ`��
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);
	
	//�_��particle
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	drawParticle(dt);
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
		printf("[F5]:beta, [F6]:amp0, [F7]:deltaWave �̒���\n");
		printf("[F8]:���ʔ��˗�reflectivity����\n");
		printf("[F9]:x�������̕���accelX����\n");
		printf("[F10]:�o�C�A�Xbias����\n");
		printf("[F11]:�_�쐬���̗����n��̃V�[�hseedC����\n");

		printf(" \n");
		flagHelp = false;
  }

  //�I��
  glutSwapBuffers();
}

void drawWave()
{
  float ambient[] = { 0.1f, 0.1f, 0.5f, 1.0f};
  float diffuse[] = { 0.2f, 0.2f, 1.0f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

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

	addTrackWave();
  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

void addTrackWave()
{
	//�D�̍q�Ս쐬�ix���������̂Ƃ������j
	//�D�̑��x > ���ʂ̔g�̑��x�̂Ƃ��h�b�v���[�g
	int i, ii, j;
	float x, y, r, xx, phase;
	float a0 = 0.1;//�~�`�g�̐U��
	float period = 5.0;
	float lambda = 1.0;
	float velocity = lambda / period ;//���ʔg�̑��x
	//�D�̈ʒu(�g���j
	float sX = posShip[0]+1.0;
	float sY = -posShip[2];

	if(speedShip == 0.0) return;
	for(j = 0; j <= nMesh; j++)
	{
		y = (float)(-nMesh / 2 + j) * meshY - sY;
		for(i = 0; i <= nMesh; i++)
		{
			x = (float)(-nMesh / 2 + i) * meshX - sX;
			r = sqrt(x * x + y * y);
			phase = 2.0 * M_PI * (elapseTime2 / period - r / lambda);
			if(phase < 0.0) continue;
			xx = speedShip * r / velocity;
			ii = i - floor( xx / meshX + 0.5);
			if(ii < 0 || ii > nMesh) continue;
			if( r < 0.5) r = 0.5;
			data[j * (nMesh+1) + ii] += a0 * sin(phase) / r;//sqrt(r);
		}
	}
}

void drawSun()
{
  float ambient[] = { 1.0f, 0.5f, 0.2f, 1.0f};
  float diffuse[] = { 1.0f, 0.5f, 0.2f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,1);

  glPushMatrix();
  glTranslatef(lightPos[0], lightPos[1] + 10 , lightPos[2] - 15.0);
  drawSphere(10.0, 20, 20);
  glPopMatrix();
}

void drawShip()
{
  float ambient[] = { 0.5f, 0.5f, 0.6f, 1.0f};
  float diffuse[] = { 0.8f, 0.8f, 0.9f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

	for(int i= 0; i < 3; i++) posShip[i] = posShip0[i];
	posShip[0] = posShip0[0] + speedShip * elapseTime2;//�������̑D�̑��x
  glPushMatrix();
	glTranslatef(posShip[0], posShip[1], posShip[2]);
	glRotatef(90.0, 1.0, 0.0, 0.);
	glScalef(1.0, 0.5, 0.5);
	drawHemiSuper(1.0, 12, 6, 0.8, 0.5);
	drawCube(1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.5);
	drawCylinder1(0.2, 0.2, 1.0, 5, 2);//����
	glPopMatrix();
  glPopMatrix();
}

void drawParticle(float dt)
{
  int i;

  glEnable(GL_POINT_SPRITE);//�|�C���g�X�v���C�g�L��
	glEnable(GL_TEXTURE_2D);  //�e�N�X�`���}�b�s���O�L��
  glEnable(GL_ALPHA_TEST);


	for(i = 0; i < num0; i++) p[countP + i].create(elapseTime2);//���q�̐���
	countP += num0;
  if(countP + num0 >= NUM_PARTICLE) countP = 0;

  for(i = 0; i < NUM_PARTICLE; i++)
  {
		//���q�̈ʒu�ɑ�������Terrain�̊i�q���WcoordI,coordJ�����߂�
		if(p[i].vPosition.x <= - sizeX / 2.0 || p[i].vPosition.x >= sizeX / 2.0
		|| p[i].vPosition.z <= - sizeY / 2.0 || p[i].vPosition.z >= sizeY / 2.0) 
		{
			p[i].inside = false;//�̈�O
		}
		else
		{
			p[i].inside = true;
			p[i].heightT = getHeightTerrain(p[i].vPosition.x, p[i].vPosition.z);
		}
		p[i].show();
		p[i].update(elapseTime2, dt);
		p[i].vAccel.x = accelX;//[F4]�L�[�ŕύX
	}

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_POINT_SPRITE);
  glDisable(GL_TEXTURE_2D);
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  width = w;
  height = h;
}

void setLight()
{
  float lightAmbient[] = {0.5f, 0.3f, 0.2f, 1.0f}; //����
  float lightDiffuse[] = {1.0f, 0.5f, 0.2f, 1.0f}; //�g�U��
  float lightSpecular[] = {1.0, 0.5f, 0.2f, 1.0f}; //���ʌ�
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
	  makeSeaData();
  }
  //K2
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K2 -= 1;
	  else K2 += 1;
	  if(K2 <= K1) K2 = K1 + 1;
	  if(K2 >= nMesh/2) K2 = nMesh/2 - 1;
		printf("K2 = %d \n", K2);
	  makeSeaData();
  }
  //L1
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L1 -= 1;
	  else L1 += 1;
	  if(L1 <= 1) L1 = 1;
	  if(L1 >= L2) L1 = L2 -1;
		printf("L1 = %d \n", L1);
	  makeSeaData();
  }
  //L2
  if(key == GLUT_KEY_F4) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L2 -= 1;
	  else L2 += 1;
	  if(L2 <= L1) L2 = L1 + 1;
	  if(L2 >= nMesh/2) L2 = nMesh/2 - 1;
		printf("L2 = %d \n", L2);
	  makeSeaData();
  }
	//beta0
  if(key == GLUT_KEY_F5) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta0 -= 0.1;
	  else beta0 += 0.1;
		printf("beta0 = %f \n", beta0);
	  makeSeaData();
  }

  //amp��ς���
  if(key == GLUT_KEY_F6) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp0 -= 0.01;
	  else amp0 += 0.01;
		if(amp0 < 0.0) amp0 = 0.0;
		printf("amp0 = %f \n", amp0);
	  makeSeaData();
  }
  //���ʔg�ʑ��V�t�g
  if(key == GLUT_KEY_F7) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaWave -= 0.01;
	  else deltaWave += 0.01;
		printf("deltaWave = %f \n", deltaWave);
	  makeSeaData();
  }
  if(key == GLUT_KEY_F8) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) reflectivity0 -= 0.1;
	  else reflectivity0 += 0.1;
		if(reflectivity0 < 0.0) reflectivity0 = 0.0;
		if(reflectivity0 > 1.0) reflectivity0 = 1.0;
		printf("reflectivity0 = %f \n", reflectivity0);
  }  //x����������
  if(key == GLUT_KEY_F9) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) accelX -= 0.01;
	  else accelX += 0.01;
		printf("accelX = %f \n", accelX);
  }

  if(key == GLUT_KEY_F10) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) bias -= 0.1;
	  else bias += 0.1;
		if(bias < 0.0) bias = 0.0;
		if(bias > 1.0) bias = 1.0;
		printf("bias = %f \n", bias);
		makeCloudData();
		for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
		countP = 0.0;//numParticle;
  }
  if(key == GLUT_KEY_F11) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) seedC -= 1;
	  else seedC += 1;
		printf("seedC = %d \n", seedC);
		makeCloudData();
		for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
		countP = 0.0;//numParticle;
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
