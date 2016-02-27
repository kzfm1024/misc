/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�W���͗l1
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "wave.h"

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
void makeTexImage();
void setTexture();
void drawFloor0(float widthX, float widthZ, int nx, int nz);
void setTextureMatrix();
void drawLight();

//����
float lightPos[] = {0.0, 10.0, 0.0, 1.0};//�����ʒu
float lightPos0[] = {0.0, 10.0, 0.0, 1.0};//�����ʒu(�����l)
//�e�̃}�e���A��
float shadowDiffuse[] =  {0.0,0.0,0.0,0.1};//�e�̊g�U��
float shadowSpecular[] = {0.0,0.0,0.0,1.0};//�e�̋��ʌ�

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
  0.0, 1.0, 0.0,//cnt 
	15.0, 20.0, 0.0,//dist, theta, phi
	50.0, 1.0, 100.0//fovY,nearZ, farZ
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
//ܲ԰�ڰ�<-->�د�ސ؂�ւ��t���O
bool flagWireframe = false;
//�w���v�L�[�t���O
bool flagHelp = false;
//�����ʒu�ύX�t���O
bool flagLight = false;//�ǉ�
//�e�̃t���O
bool flagShadow = false;
//�I�u�W�F�N�g���ʔԍ�
int objNo = 0;
//ø�����摜
#define TEX_WIDTH 128
#define TEX_HEIGHT 128
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][3];
//�o�ߎ���
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
float amp = 1.0;
//�A�j���[�V����
#include "slFish1.h"
CFish1 fish1 ;
#include "slFish2.h"
CFish2 fish2 ;

GLfloat fov = 60.0;//�������e�}�b�s���O�̎���p
//�V�F�[�_�v���O������
GLuint shaderProg;


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
  glutCreateWindow("�W���͗l1(SL_Caustics1)");
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
  initGlsl(&shaderProg, "projection.vert", "projection.frag");
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
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

	//���Ԋ֌W
  lastTime = timeGetTime();
  fps = 0;
  elapseTime1 = 0.0;//1sec�Ԉȓ��̌o�ߎ���
  elapseTime2 = 0.0; //init()��̑��o�ߎ���

	makeWavePlane(amp, elapseTime2);
	makeTexImage();
	setTexture();
}

void makeTexImage()
{
  int i, j;
	float v, a;

  for(j = 0; j < TEX_HEIGHT; j++)
		for(i = 0; i < TEX_WIDTH; i++)
    {
			v = data[j * (nMesh + 1) + i];
			if(kindWave == 0) a = 180.0 + 75.0 * v ;
			else              a = 250.0 - 100.0 * v*v  ;
			if(a >= 255.0) a = 255.0;
			if(a < 120) a = 120;
 			texImage[j][i][0] = texImage[j][i][1] = texImage[j][i][2] = (GLubyte)a;
    }
}

void setTexture()
{
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,texImage);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void display(void)
{
 	//���Ԍv��
  curTime = timeGetTime();
  float dt = (float)(curTime - lastTime) * 0.001;//sec�ɕϊ�
  elapseTime1 += dt;
  elapseTime2 += dt;
  fps ++;
		printf("elapseTime2 = %f \n", elapseTime2);
  if(elapseTime1 >= 1.0)
  {
		printf("frame per sec = %d \n", fps);
		elapseTime1 = 0.0;
		fps = 0;
  }
  lastTime = curTime;

	//�g�f�[�^���쐬���A���e�}�b�v��ݒ�
	if(kindWave == 0) makeWaveCircle(amp, elapseTime2);
	else if(kindWave == 1) makeWavePlane(amp, elapseTime2);
	else makeWaveMix(amp, elapseTime2);
	makeTexImage();
	setTexture();

  if(flagWireframe)//'w'��wireframe��solid model�؂�ւ�
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	resize(width, height);
	//�J���[�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(cos(M_PI * view.theta /180.0) > 0.0)//�J�����p90�x���ޭ������޸�ِؑ�
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  //fragment shader�̃��j�t�H�[���ϐ��C���f�b�N�X���擾
  GLint texLoc = glGetUniformLocation(shaderProg, "smplCaustics");
  glUniform1i(texLoc, 0);//GL_TEXTURE0��K�p
  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);

  //�`��
	fish1.motion1(elapseTime2);
	fish2.motion1(elapseTime2);
	drawFloor0(10.0, 10.0, 10, 10);

  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);

	//ø����A���������̂�����Ƃ�
	glDepthMask(GL_FALSE); //���߽�ޯ̧���������݋֎~
	glEnable(GL_BLEND);//��̧�����ިݸނ�L���ɂ���
	glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//�F�����W�������߂�

	//�������`��
	drawWave();
	//ø����A���������̂�����Ƃ�
	glDepthMask(GL_TRUE); //���߽�ޯ̧�̏������݂�����
	glDisable(GL_BLEND);
	
  //�e
  drawShadow();

	drawLight();

  if(flagHelp)
  {
		printf("���L�[�ɂ�����/���e���S�ړ� \n");
		printf("�@��,���Fx�� \n");
		printf("�@��,���Fy�� \n");
		printf("�@[Shift]�{��,���Fz�� \n");
		printf("�@'l'����������A�����ʒu�̈ړ��� \n");
		printf("'w'��ܲ԰�ڰтƿد�����ِ؂�ւ� \n");
		printf("�}�E�X����Ŏ��_�ύX�� \n");
		printf("  dolly�F�����t�߂����{�^���N���b�N�ŋ߂Â��C�E�{�^���N���b�N�ŉ������� \n");
		printf("  pan�F��������щE�����E�{�^���N���b�N�Œ����_�����E�ɕω����� \n");
		printf("  tilt�F�^�エ��ѐ^�����E�{�^���N���b�N�Œ����_���㉺�ɕω����� \n");
		printf("  tumble�F���E�Ƀh���b�O����Ǝ��_�����E�ɕω����� \n");
		printf("  crane�F�㉺�Ƀh���b�O����Ǝ��_���㉺�ɕω����� \n");
		printf("  zoom�F�������E�{�^���N���b�N�ŃY�[���C�� \n");
		printf("        �E�����E�{�^���N���b�N�ŃY�[���A�E�g \n");
		printf("[Shift]+'r'�Ń��Z�b�g \n");
		printf("[F1]�L�[�F���e�}�b�s���O�̎���pfov�̑��� \n");
		printf("[F2]�L�[�F�g�̐U��amp \n");
		printf("[F3]�L�[�F�g�̎�ށi�~�`�g�ƕ��ʔg�j�̐؂�ւ� \n");
		printf("[F4]�L�[�F�~�`�g�̌��؂�ւ� \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void setTextureMatrix()
{
 //�e�N�X�`���ϊ��s���ݒ肷�� 
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  //���K�����W�� [-1,1] �͈̔͂��e�N�X�`�����W�͈̔� [0,1] �ɕϊ� 
  glTranslatef(0.5, 0.5, 0.5);
  glScalef(0.5, 0.5, 0.5);
  gluPerspective(fov, 1.0, 0.1, 50.0);
	//���e���S=�����ʒu
	gluLookAt(lightPos[0], lightPos[1], lightPos[2], 0.0, waveH, 0.0, 0.0, 0.0, -1.0);
}

void drawFloor0(float widthX, float widthZ, int nx, int nz)
{
  setTextureMatrix();
  int i, j;
  //Floor�P��������̕�
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.7, 0.7, 0.6, 1.0}, { 0.3f, 0.4, 0.4, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  //�ʏ�̃��f���r���[�ϊ��ɖ߂� 
  glMatrixMode(GL_MODELVIEW);
  glNormal3f(0.0, 1.0, 0.0);
  glPushMatrix();
  for (j = 0; j < nz; j++) 
  {
    float z1 = -widthZ / 2.0 + wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = -widthX / 2.0 + wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glBegin(GL_QUADS);
      glVertex3f(x1, 0.0, z1);
      glVertex3f(x1, 0.0, z2);
      glVertex3f(x2, 0.0, z2);
      glVertex3f(x2, 0.0, z1);
			glEnd();
    }
  }
  glPopMatrix();
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
  //�\���E�B���h�E�̃T�C�Y
  width = w;
  height = h;
}

void drawLight(void)
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	glutSolidSphere(0.1, 10, 10);
  glPopMatrix();
	glEnable(GL_LIGHTING);
}

void setLight()
{
  float lightAmbient0[] = {0.5, 0.5, 0.5, 1.0}; //����
  float lightDiffuse0[] = {1.0, 1.0, 1.0, 1.0}; //�g�U��
  float lightSpecular0[] = {1.0, 1.0, 1.0, 1.0};//���ʌ�
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
}
void keyboard(unsigned char key, int x, int y)
{
  switch((unsigned char)key)
  {
  case 27://Esc
	  exit(0);
	  break;
	case 'w':
		if(flagWireframe) flagWireframe = false;
		else if(!flagWireframe) flagWireframe = true;
		break;
  case 'R'://reset�i�����l�ɖ߂�)
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
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] -= d;//z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1] -= d;//-y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] += d;//-z����
		  break;
	  default:
		  break;
	  }
  }
	if(key == GLUT_KEY_F1)
	{
		if(glutGetModifiers() == 0) fov += 1.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fov -= 1.0;
		if(fov < 5.0) fov = 5.0;
		printf("fov = %f \n", fov);
	}
	if(key == GLUT_KEY_F2)
	{
		if(glutGetModifiers() == 0) amp += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp -= 0.1;
		if(amp < 0.0) amp = 0.0;
		printf("amp = %f \n", amp);
	}
	if(key == GLUT_KEY_F3)
	{
		kindWave ++;
		if(kindWave == 3) kindWave = 0;
		printf("kindWave = %d \n", kindWave);
	}
	if(key == GLUT_KEY_F4)
	{
		numWave ++;
		if(numWave >= 4) numWave = 1;
		printf("numWave = %d \n", numWave);
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
}


//����2�͉e�`��ɕK�v
//------------------------------------------------------------------------
void CalcShadowMat(int , float* mat)
{
  float ex, ey, ez;//�����̕���
  float a, b, c, d;//���̖ʂ̃p�����[�^
  float s; //object���S��������܂ł̋���
  float x, y, z;

	if(objNo == 1)
	{
		x = lightPos[0] - fish1.pos[0];
		y = lightPos[1] - fish1.pos[1];
		z = lightPos[2] - fish1.pos[2];
	}
	else
	{
		x = lightPos[0] - fish2.pos[0];
		y = lightPos[1] - fish2.pos[1];
		z = lightPos[2] - fish2.pos[2];
	}

  //�����̕����x�N�g��
  s = sqrt(x * x + y * y + z * z);
  ex = x / s;
  ey = y / s;
  ez = z / s;

  //�t���A�̕����x�N�g��(�������j
  a = 0.0;
  b = 1.0;
  c = 0.0;
  d = -0.001; //�t���A�Ɖe�̊���h������
	//shadow matrix
  mat[0] = b * ey + c * ez;
  mat[1] = -a * ey;
  mat[2] = -a * ez;
  mat[3] = 0.0;
  mat[4] = -b * ex;
  mat[5] = a * ex + c * ez;
  mat[6] = -b * ez;
  mat[7] = 0.0;
  mat[8] = -c * ex;
  mat[9] = -c * ey;
  mat[10] = a * ex + b * ey;
  mat[11] = 0.0;
  mat[12] = -d * ex;
  mat[13] = -d * ey;
  mat[14] = -d * ez;
  mat[15] = a * ex + b * ey + c * ez;
}

//-----------------------------------------------------------------------------
void drawShadow()
{
  //�v�Z�͕��s�����ɑ΂���e
  //��޼ު�Ă��ƂɌ����̕�����ς��A�^���I�ɓ_�����ɑ΂���e�����
  float mat[16]; //�e�s��̗v�f

  flagShadow = true;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	  glDepthMask(GL_FALSE);
  //�e�̕`��
  CalcShadowMat(1, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  fish1.draw();//��޼ު�Ă̕`��
  glPopMatrix();
  CalcShadowMat(2, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  fish2.draw();//��޼ު�Ă̕`��
  glPopMatrix();

  //�e�̕`��I��
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  flagShadow = false;
}