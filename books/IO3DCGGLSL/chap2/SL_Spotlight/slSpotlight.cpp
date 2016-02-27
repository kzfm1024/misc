/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�X�|�b�g���C�g
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include <math.h>
#define M_PI 3.14159265358979

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void draw0();
void draw1();
void drawFloor(float widthX, float widthZ, int nx, int nz);
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,int lightN, float* mat);
void drawShadow(int lightN);
void drawLight();
void drawSpotlight();

//affine�ϊ��p�ϐ�
float pos[][3] = { //�ʒu�i���s�ړ��j
	{ 0.7, 1.0, 0.0}, //obj0
	{-0.7, 1.0, 0.0}  //obj1
};

float pos0[][3] = {//�����l
	{ 0.7, 1.0, 0.0}, //obj0
	{-0.7, 1.0, 0.0}  //obj1
};

float scale[][3] = {//�T�C�Y�i�X�P�[�����O�j
	{ 1.0, 1.0, 1.0 },//obj0
	{ 1.0, 1.0, 1.0 } //obj1
};
float scale0[][3] = {//�����l
	{ 1.0, 1.0, 1.0 },//obj0
	{ 1.0, 1.0, 1.0 } //obj1
};

float angle[][3] = {//�p���i��]�j
	{ 0.0, 0.0, 0.0}, //obj0
	{ 0.0, 0.0, 0.0}  //obj1
};
float angle0[][3] = {//�����l
	{ 0.0, 0.0, 0.0}, //obj0
	{ 0.0, 0.0, 0.0}  //obj1
};

//����
float lightPos[2][4] = { {0.0, 5.0, 0.0, 1.0} , {2.0, 5.0, 0.0, 1.0} }; //�����ʒu
float lightPos0[2][4] = { {0.0, 5.0, 0.0, 1.0}, {2.0, 5.0, 0.0, 1.0} };//�����ʒu(�����l�j
float lightDiffuse[2][4] = { {0.5, 0.5, 0.5, 1.0}, {1.0, 0.0, 0.0, 1.0} }; //�g�U��
float lightSpecular[2][4] = { {1.0, 1.0, 1.0, 1.0}, {1.0, 0.0, 0.0, 1.0} }; //���ʌ�
float constAtt = 1.0;    //LIGHT0
float linearAtt = 0.0;   //LIGHT0
float quadraAtt = 0.0;   //LIGHT0
int AttMode = 0;         //LIGHT0
float	spotExp = 0.0;     //LIGHT1
float spotCutoff = 40.0; //LIGHT1
float spotDir[3] ;       //LIGHT1
//�e�̃}�e���A��
float shadowDiffuse[] =  {0.0,0.0,0.0,0.3};//�e�̊g�U��
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
//�����l
View view = { 
	0.0, 0.0, 0.0,//pos(���ݒ�j
  0.0, 0.0, 0.0,//cnt 
	20.0, 30.0, 20.0,//dist, theta, phi
	30.0, 1.0, 100.0//fovY,nearZ, farZ
};
View view0 = view;


//Window�̃T�C�Y
int width = 500;
int height = 500;
//�A�t�B���ϊ�
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE, LIGHT0, LIGHT1};
SELECT_KEY sKey = TRANSLATE;
//�}�E�X����
int xStart, yStart;
bool flagMouse = false;
//�w���v�L�[�t���O
bool flagHelp = false;
//�����ʒu�ύX�t���O
bool flagLight = false;//�ǉ�
//ܲ԰�ڰ�<-->�د�ސ؂�ւ��t���O
bool flagWireframe = false;//�ǉ�
//�e�̃t���O
bool flagShadow = false;
//�I�u�W�F�N�g���ʔԍ�
int objNo = 0;
//��]�A�j���[�V����
float ang = 0.0;
float dang = 0.1;//�p�x����
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
  glutCreateWindow("��߯�ײ�(SL_Spotlight)");
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
  initGlsl(&shaderProg, "spotlight.vert", "spotlight.frag");
	
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
	//���\�[�X�̉��
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

  setCamera(); //���_�����߂�
  setLight();  //�����ݒ�@
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("�}�E�X/�L�[����̐����ɂ�'h'�L�[���v�b�V�� \n");
}

void display(void)
{
  //���Ԍv��
  static double time1, time2, drawTime, frame;
  if(ang <= 0.001) time1 = timeGetTime();

	//�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();//���_��ς���Ƃ��͂��̈ʒu�ɕK�v
  if(cos(M_PI * view.theta /180.0) >= 0.0)//�J�����p90�x���ޭ������޸�ِؑ�
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  if(flagWireframe)//'w'��wireframe��solid model�؂�ւ�
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	setLight();

  glUseProgram(shaderProg);
  //�`��
  draw0(); draw1();
  drawFloor(20.0, 20.0, 20, 20);
  glUseProgram(0);
  //�e
  drawShadow(0);//����0�ɑ΂���e
  drawShadow(1);//����1�ɑ΂���e
	//�����`��
	drawLight();
	drawSpotlight();

  //��]�p�x�X�V
  ang += dang;
  if(ang >= 360.0)
  {
		time2 = timeGetTime();
		//�P��]������̕`�掞��
		drawTime = (time2 - time1) * 0.001;
		frame = (360.0 / dang) / drawTime;
		printf("�t���[���� = %4.4f[fps] \n", frame);
		//�t���[��������̕`�掞��
		drawTime = 1.0 / frame;
		printf("�`�掞�� = %4.4f[spf] \n", drawTime);		
		ang = 0.0;
  }

	if(flagHelp)
  {
		printf("���L�[�ɂ��A�t�B���ϊ�/�����ړ� \n");
		printf("�@��,���Fx�� \n");
		printf("�@��,���Fy�� \n");
		printf("�@[Shift]�{��,���Fz�� \n");
		printf("�@'r'����������:��] \n");
		printf("�@'t'����������:���s�ړ� \n");
		printf("�@'s'����������:����ݸ� \n");
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
		printf("[Page Up]�L�[�F��]�p�x+0.1 \n");
		printf("[Page Dn]�L�[�F��]�p�x-0.1 \n");
		printf("[F1]:�����̌������[�h�؂肩�� \n");
		printf("[F2]�L�[:��߯�ײĎw������ \n");
		printf("[F3]�L�[:��߯�ײĂ̶�ĵ̊p�x���� \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void draw0(void)
{
  float ambient[] = { 0.1, 0.3, 0.3, 1.0};
  float diffuse[] = { 0.2, 0.7, 0.7, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

	if(flagShadow) 
  {
	  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
  }
  else
  {	
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100);
  }

  glPushMatrix();
	glTranslatef(pos[0][0], pos[0][1], pos[0][2]);
  glRotatef(angle[0][2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[0][1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0][0] + ang, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0][0], scale[0][1], scale[0][2]);
	glutSolidSphere(0.5, 20, 20);
  glPopMatrix();
}

void draw1(void)
{
  float ambient[] = { 0.2, 0.1, 0.1, 1.0};
  float diffuse[] = { 0.8, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

	if(flagShadow) 
  {
	  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
  }
  else
  {	
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100);
  }

  glPushMatrix();
	glTranslatef(pos[1][0], pos[1][1], pos[1][2]);
  glRotatef(angle[1][2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1][1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[1][0] + ang, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[1][0], scale[1][1], scale[1][2]);
//glutSolidCube(1.0);  //�I�u�W�F�N�g�`��
	glutSolidTorus(0.2, 0.5, 10, 10);
  glPopMatrix();
}

void drawFloor(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor�P��������̕�
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.7, 0.7, 0.6, 1.0f}, { 0.3f, 0.4, 0.4, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3d(0.0, 1.0, 0.0);
  glPushMatrix();
  for (j = 0; j < nz; j++) {
    float z1 = -widthZ / 2.0 + wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = -widthX / 2.0 + wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glBegin(GL_QUADS);
      glVertex3d(x1, 0.0, z1);
      glVertex3d(x1, 0.0, z2);
      glVertex3d(x2, 0.0, z2);
      glVertex3d(x2, 0.0, z1);
		  glEnd();
    }
  }
  glPopMatrix();
}

void drawLight(void)
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
	glTranslatef(lightPos[0][0], lightPos[0][1], lightPos[0][2]);
	glutSolidSphere(0.1, 10, 10);
  glPopMatrix();
	glEnable(GL_LIGHTING);
}

void drawSpotlight(void)
{
	float length = 0.5;//�X�|�b�g���C�g�̒���
	float d = sqrt(spotDir[0]*spotDir[0] + spotDir[1]*spotDir[1] + spotDir[2]*spotDir[2]);
	for(int i = 0; i < 3; i++) spotDir[i] /= d;//���K��
	//�X�|�b�g���C�g�̐�[(LIGHT1�����j
	float x = lightPos[1][0] + spotDir[0] * length;
	float y = lightPos[1][1] + spotDir[1] * length;
	float z = lightPos[1][2] + spotDir[2] * length;

	glDisable(GL_LIGHTING);
	glColor3f(lightDiffuse[1][0], lightDiffuse[1][1],lightDiffuse[1][2]);
	glLineWidth(5.0);
  glPushMatrix();
	glBegin(GL_LINES);
		glVertex3f(lightPos[1][0], lightPos[1][1], lightPos[1][2]); 
		glVertex3f(x, y, z); 
	glEnd();

  glPopMatrix();
	glEnable(GL_LIGHTING);
	glLineWidth(1.0);
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

void setLight()
{
	//LIGHT0
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse[0]);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightDiffuse[0]);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular[0]);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos[0]);
	//LIGHT1
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse[1]);
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightDiffuse[1]);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular[1]);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPos[1]);

	//�������[�h�ύX��LIGHT0����
	if(AttMode == 0)
	{
		constAtt = 1.0; linearAtt = 0.0; quadraAtt = 0.0;
	}
	else if(AttMode == 1) 
	{
		constAtt = 0.0; linearAtt = 1.0 / lightPos0[0][1]; quadraAtt = 0.0;
	}
	else
	{
		constAtt = 0.0; linearAtt = 0.0; quadraAtt = 1.0 / (lightPos0[0][1]*lightPos0[0][1]);
	}

	//LIGHT1�̓X�|�b�g���C�g(�X�|�b�g���C�g�͏�Ƀ��[���h��Ԃ̌��_�������j
	for(int i = 0; i < 3; i++) spotDir[i] = -lightPos[1][i];

	//LIGHT0
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, constAtt);
  glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAtt);
  glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, quadraAtt);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDir);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, spotExp);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, spotCutoff);
}

void keyboard(unsigned char key, int x, int y)
{
  switch((unsigned char)key)
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
		for(int j = 0; j < 2; j++)
			for(int i = 0; i < 3; i++)
			{
				pos[j][i] = pos0[j][i]; scale[j][i] = scale0[j][i]; angle[j][i] = angle0[j][i];
			}
			for(int i = 0; i < 3; i++) 
			{
				lightPos[0][i] = lightPos0[0][i];
				lightPos[1][i] = lightPos0[1][i];
			}
	  view = view0;
	  setCamera();
	  break;
  case 'l':
		sKey = LIGHT0;
	  break;
  case 'L':
		sKey = LIGHT1;
	  break;
  case 'w':
	  if(flagWireframe) flagWireframe = false;
	  else if(!flagWireframe) flagWireframe = true;
	  break;
  case 'h':
	  if(flagHelp) flagHelp = false;
	  else flagHelp = true;
	  break;
  case '0':
	  objNo = 0;
	  break;
  case '1':
	  objNo = 1;
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
		  pos[objNo][0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://-x����
		  pos[objNo][0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) pos[objNo][1] += 0.1;//y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[objNo][2] -= 0.1; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) pos[objNo][1] -= 0.1;//-y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[objNo][2] += 0.1; //-z����
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
		  angle[objNo][0] += 10.0;
		  break;
	  case GLUT_KEY_LEFT://-x����]
		  angle[objNo][0] -= 10.0;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) angle[objNo][1] += 10;//y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[objNo][2] += 10; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) angle[objNo][1] -= 10;//-y����
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[objNo][2] -= 10; //-z����
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
		  scale[objNo][0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://x�����k��
		  scale[objNo][0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) scale[objNo][1] += 0.1;//y�����g��
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[objNo][2] += 0.1; //z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) scale[objNo][1] -= 0.1;//y�����k��
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[objNo][2] -= 0.1; //z����
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == LIGHT0)
  {
		float d = 0.2;
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x����
		  lightPos[0][0] += d;
		  break;
	  case GLUT_KEY_LEFT:
		  lightPos[0][0] -= d;//-x����
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) lightPos[0][1] += d;//y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[0][2] -= d;//-z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[0][1] -= d;//-y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[0][2] += d;//z����
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == LIGHT1)//spotlight
  {
		float d = 0.2;
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x����
		  lightPos[1][0] += d;
		  break;
	  case GLUT_KEY_LEFT:
		  lightPos[1][0] -= d;//-x����
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) lightPos[1][1] += d;//y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[1][2] -= d;//-z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1][1] -= d;//-y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[1][2] += d;//z����
		  break;
	  default:
		  break;
	  }
	}

	if(key == GLUT_KEY_PAGE_UP) 
	{
		dang += 0.1;
  }	
	if(key == GLUT_KEY_PAGE_DOWN) 
	{
		dang -= 0.1;
  }	

	if(key == GLUT_KEY_F1) 
	{
		AttMode += 1; 
		if(AttMode > 2) AttMode = 0;
		printf("AttMode = %d \n", AttMode);
	}
	if(key == GLUT_KEY_F2) 
	{
		if(glutGetModifiers() == 0) spotExp += 1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) spotExp -= 1;
		if(spotExp < 0.0) spotExp = 0.0;
		printf("spotExp = %f \n", spotExp);
  }	
	if(key == GLUT_KEY_F3) 
	{
		if(glutGetModifiers() == 0) spotCutoff += 1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) spotCutoff -= 1;
		if(spotCutoff < 0.0) spotCutoff = 0.0;
		if(spotCutoff > 90.0) spotCutoff = 90.0;
		printf("spotCutoff = %f \n", spotCutoff);
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
  printf("view.posX=%f, view.posY=%f, view.posZ=%f \n", view.pos[0], view.pos[1], view.pos[2]);
  printf("cntX=%f, cntY=%f, cntZ=%f \n", view.cnt[0], view.cnt[1], view.cnt[2]);
  printf("phi=%f, theta=%f, fovY=%f \n", view.phi, view.theta, view.fovY);
	resize(width, height);
}

//����2�͉e�`��ɕK�v
//------------------------------------------------------------------------
void CalcShadowMat(int ID, int lightN, float* mat)
{
  float ex, ey, ez;//�����̕���
  float a, b, c, d;//���̖ʂ̃p�����[�^
  float s; //object���S��������܂ł̋���
  float x, y, z;

  x = lightPos[lightN][0] - pos[ID][0];
  y = lightPos[lightN][1] - pos[ID][1];
  z = lightPos[lightN][2] - pos[ID][2];

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
void drawShadow(int lightN)
{
  //�v�Z�͕��s�����ɑ΂���e
  //��޼ު�Ă��ƂɌ����̕�����ς��A�^���I�ɓ_�����ɑ΂���e�����
  float mat[16]; //�e�s��̗v�f

  flagShadow = true;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	  glDepthMask(GL_FALSE);
  //�e�̕`��
  CalcShadowMat(0, lightN, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  draw0();//��޼ު�Ă̕`��
  glPopMatrix();
  CalcShadowMat(1, lightN, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  draw1();//��޼ު�Ă̕`��
  glPopMatrix();

  //�e�̕`��I��
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  flagShadow = false;
}