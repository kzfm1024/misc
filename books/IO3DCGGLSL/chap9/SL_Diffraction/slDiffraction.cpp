/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

��܊i�q�ɂ�銱��
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
void drawFloor0(float widthX, float widthZ, int nx, int nz);
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
//�ٕ������˗p�v���~�e�B�u
void drawAnisoCube(float s);
void drawAnisoSphere(float radius, int nSlice, int nStack);
void drawAnisoCylinder(float rBottom, float rTop, float height, int nSlice, int nStack, int nRing);
void drawAnisoTorus(float radius1, float radius2, int nSide, int nRing);

//affine�ϊ��p�ϐ�
float pos[][3] = { //�ʒu�i���s�ړ��j
	{ 1.0, 1.0, 0.0}, //obj0
	{-1.0, 1.0, 0.0}  //obj1
};

float pos0[][3] = {//�����l
	{ 1.0, 1.0, 0.0}, //obj0
	{-1.0, 1.0, 0.0}  //obj1
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
float lightPos[] =  {0.0, 5.0, 0.0, 1.0};//�����ʒu
float lightPos0[] = {0.0, 5.0, 0.0, 1.0};//�����ʒu(�����l)
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
View view = { 
	0.0, 0.0, 0.0,//pos(���ݒ�j
  0.0, 1.0, 0.0,//cnt 
	9.0, 20.0, 20.0,//dist, theta, phi
	30.0, 1.0, 100.0//fovY,nearZ, farZ
};
View view0 = view;

//Window�̃T�C�Y
int width = 500;
int height = 500;
//�A�t�B���ϊ�
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE, LIGHT};
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
float dang = 0.0;//�p�x����
//�e���W��
float roughX = 0.1;
float roughY = 0.5;
//�i�q�Ԋu
float gratingSpace = 1700.0;
//�ڐ��x�N�g���؂�ւ�
int changeTangent = 0;
//�����W��
float blendFactor = 0.8;
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
  glutCreateWindow("���(SL_Diffraction)");
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
  initGlsl(&shaderProg, "diffraction.vert", "diffraction.frag");
	
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

  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  if(flagWireframe)//'w'��wireframe��solid model�؂�ւ�
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

  glUseProgram(shaderProg);
  GLint spaceLoc = glGetUniformLocation(shaderProg, "GratingSpace");
	glUniform1f(spaceLoc, gratingSpace);
	GLint changeLoc = glGetUniformLocation(shaderProg, "changeTangent");
	glUniform1i(changeLoc, changeTangent);
  GLint roughXLoc = glGetUniformLocation(shaderProg, "roughX");
	glUniform1f(roughXLoc, roughX);
  GLint roughYLoc = glGetUniformLocation(shaderProg, "roughY");
	glUniform1f(roughYLoc, roughY);
  GLint blendLoc = glGetUniformLocation(shaderProg, "blendFactor");
	glUniform1f(blendLoc, blendFactor);
 
  //�`��
  draw0(); draw1();
  glUseProgram(0);
  drawFloor0(10.0, 10.0, 10, 10);
  //�e
  drawShadow();

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
		printf("[F1]�L�[�F�i�q�ԊugratingSpace���� \n");
		printf("[F2]�L�[�F�ڐ��x�N�g���؂�ւ�changeTangent \n");
		printf("[F3]�L�[�F�e���W��rough���� \n");
		printf("[Page Up]�L�[�F��]�p�x+0.1 \n");
		printf("[Page Dn]�L�[�F��]�p�x-0.1 \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void draw0(void)
{
	float s ;

	//Gold	
	s =0.74; //0.61;//1.627
  float ambient[] = { 0.24725*s, 0.1995*s, 0.0745*s, 1.0};
  float diffuse[] = { 0.75164*s, 0.60648*s, 0.22648*s, 1.0};
  float specular[]= { 0.62828*s, 0.555802*s, 0.366065*s, 1.0};
/*
	//chrome
	s = 0.84;
  float ambient[] = { 0.25*s, 0.25*s, 0.25*s, 1.0};
  float diffuse[] = { 0.4*s, 0.4*s, 0.4*s, 1.0};
  float specular[]= { 0.774597*s, 0.774597*s, 0.774597*s, 1.0};

	//brass
	s = 0.57;
  float ambient[] = { 0.329412*s, 0.223529*s, 0.027451*s, 1.0};
  float diffuse[] = { 0.780392*s, 0.568627*s, 0.113725*s, 1.0};
  float specular[]= { 0.992157*s, 0.941176*s, 0.807843*s, 1.0};
*/

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
	  glMaterialf(GL_FRONT,GL_SHININESS,50.0);
  }

  glPushMatrix();
	glTranslatef(pos[0][0], pos[0][1], pos[0][2]);
  glRotatef(angle[0][2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[0][1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0][0] - 90.0 + ang, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0][0], scale[0][1], scale[0][2]);
	drawAnisoCube(1.0);
//	drawAnisoCylinder(0.7, 0.7, 0.2, 30, 5, 10);
//	glutSolidSphere(0.8, 20, 20);
//	glutSolidTeapot(0.8);
  glPopMatrix();
}

void draw1(void)
{
	float s = 1.0;

	//Silver
	s = 1.0;
  float ambient[] = { 0.19225*s, 0.19225*s, 0.19225*s, 1.0};
  float diffuse[] = { 0.50754*s, 0.50745*s, 0.50754*s, 1.0};
  float specular[]= { 0.508273*s, 0.508273*s, 0.508273*s, 1.0};
/*
	//Copper
	s = 1.0;
  float ambient[] = { 0.19125*s, 0.0735*s, 0.0225*s, 1.0};
	float diffuse[] = { 0.7038*s, 0.27048*s, 0.0828*s, 1.0};
  float specular[]= { 0.256777*s, 0.137622*s, 0.086014*s, 1.0};

	//bronze
	s = 0.9;
  float ambient[] = { 0.2125*s, 0.1275*s, 0.054*s, 1.0};
	float diffuse[] = { 0.714*s, 0.4284*s, 0.18144*s, 1.0};
  float specular[]= { 0.393548*s, 0.271906*s, 0.166721*s, 1.0};
*/
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
	  glMaterialf(GL_FRONT,GL_SHININESS,100.0);
  }

  glPushMatrix();
	glTranslatef(pos[1][0], pos[1][1], pos[1][2]);
  glRotatef(angle[1][2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1][1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[1][0] - 90.0 + ang, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[1][0], scale[1][1], scale[1][2]);
	drawAnisoSphere(0.5, 30, 30);
//  drawAnisoCube(1.0);  //�I�u�W�F�N�g�`��
//	drawAnisoTorus(0.2, 0.6, 30, 30);
  glPopMatrix();
}

void drawFloor0(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor�P��������̕�
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.5, 0.5, 0.4, 1.0f}, { 0.2f, 0.3, 0.3, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.2, 0.2, 0.2, 1.0};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
//  glMaterialf(GL_FRONT,GL_SHININESS,10);

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
  float lightAmbient0[] = {1.0, 1.0, 1.0, 1.0}; //����
//  float lightAmbient0[] = {1.0, 1.0, 1.0, 1.0}; //����
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
		for(int i = 0; i < 3; i++) lightPos[i] = lightPos0[i];
	  view = view0;
	  setCamera();
	  break;
  case 'l':
	  sKey = LIGHT;
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
  else if(sKey == LIGHT)
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
		if(glutGetModifiers() == 0) gratingSpace += 100.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) gratingSpace -= 100.0;
		printf("�i�q�Ԋu = %f \n", gratingSpace);
  }	
	if(key == GLUT_KEY_F2) 
	{
		changeTangent += 1;
		if(changeTangent == 2) changeTangent = 0;
		printf("changeTangent = %d \n", changeTangent);
  }	
	if(key == GLUT_KEY_F3) 
	{
		if(glutGetModifiers() == 0) roughX += 0.02;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) roughX -= 0.02;
		if(roughX < 0.02) roughX = 0.02;
		printf("roughX = %f \n", roughX);
  }	
	if(key == GLUT_KEY_F4) 
	{
		if(glutGetModifiers() == 0) roughY += 0.02;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) roughY -= 0.02;
		if(roughY < 0.02) roughY = 0.02;
		printf("roughY = %f \n", roughY);
  }	
	if(key == GLUT_KEY_F5) 
	{
		if(glutGetModifiers() == 0) blendFactor += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) blendFactor -= 0.1;
		if(blendFactor < 0.0) blendFactor = 0.0;
		if(blendFactor > 1.0) blendFactor = 1.0;
		printf("blendFactor = %f \n", blendFactor);
  }

	if(key == GLUT_KEY_PAGE_UP) 
	{
		dang += 0.1;
  }	
	if(key == GLUT_KEY_PAGE_DOWN) 
	{
		dang -= 0.1;
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

//����2�͉e�`��ɕK�v
//------------------------------------------------------------------------
void CalcShadowMat(int ID, float* mat)
{
  float ex, ey, ez;//�����̕���
  float a, b, c, d;//���̖ʂ̃p�����[�^
  float s; //object���S��������܂ł̋���
  float x, y, z;

  x = lightPos[0] - pos[ID][0];
  y = lightPos[1] - pos[ID][1];
  z = lightPos[2] - pos[ID][2];

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
  CalcShadowMat(0, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  draw0();//��޼ު�Ă̕`��
  glPopMatrix();
  CalcShadowMat(1, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//�}�g���N�X�̏�Z
	  draw1();//��޼ު�Ă̕`��
  glPopMatrix();

  //�e�̕`��I��
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  flagShadow = false;
}


//------------------------------------------------------------------------------
//�ٕ������˗p

void drawAnisoCube(float size)
{
	float sz, tnt[3];
	sz = 0.5 * size;
  float p[8][3] =
  { { sz, sz, sz}, {-sz, sz, sz}, {-sz,-sz, sz}, 
		{ sz,-sz, sz}, { sz, sz,-sz}, {-sz, sz,-sz},
		{-sz,-sz,-sz}, { sz,-sz,-sz}
  };

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
	//top(z�����������̂Ƃ�z�����j
	tnt[0] = 0.0;tnt[1] = 1.0;tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,1.0f); 
	glVertex3fv(p[2]);
	glVertex3fv(p[3]);
	glVertex3fv(p[0]);
	glVertex3fv(p[1]);
	//x����(���ʁj
	tnt[0] = 0.0; tnt[1] = 1.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(1.0f,0.0f,0.0f); 
	glVertex3fv(p[0]);
	glVertex3fv(p[3]);
	glVertex3fv(p[7]);
	glVertex3fv(p[4]);
	//y����
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,1.0f,0.0f);
	glVertex3fv(p[1]);
	glVertex3fv(p[0]);
	glVertex3fv(p[4]);
	glVertex3fv(p[5]);
	//-x����
	tnt[0] = 0.0; tnt[1] = 1.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(-1.0f,0.0f,0.0f); 
	glVertex3fv(p[6]);
	glVertex3fv(p[2]);
	glVertex3fv(p[1]);
	glVertex3fv(p[5]);
	//-y����
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,-1.0f,0.0f); 
	glVertex3fv(p[2]);
	glVertex3fv(p[6]);
	glVertex3fv(p[7]);
	glVertex3fv(p[3]);
	//-z����
	tnt[0] = -1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,-1.0f); 
	glVertex3fv(p[7]);
	glVertex3fv(p[6]);
	glVertex3fv(p[5]);
	glVertex3fv(p[4]);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

//���̃o���v�}�b�s���O�p
void drawAnisoSphere(float radius, int nSlice, int nStack)
{
	int i, j;
	double s, t0, t1, r0, r1, th0, th1, phi;
	double p[2][3], tnt[3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");
	
	for(j = 0; j < nStack; j++)
	{
		//2��t���W
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//�����̓V���p
		th0 = M_PI * t0;
		th1 = M_PI * t1;
		//x-y���ʂɓ��e�������a
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//���_z���W
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);
		//�ڐ��x�N�g���̂�����
		tnt[2] = 0.0;
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = -M_PI + 2.0 * M_PI * (double)i / (double)nSlice;
			p[0][0] = r0 * cos(phi);//x���W
			p[0][1] = r0 * sin(phi);//y���W
			p[1][0] = r1 * cos(phi);//x���W
			p[1][1] = r1 * sin(phi);//y���W
			//�ڐ��x�N�g����xy����
			tnt[0] =-sin(phi);
			tnt[1] = cos(phi);
			glVertexAttrib3dv(tangentLoc, tnt);
			glNormal3dv(p[0]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[0]);//���_���W
			glNormal3dv(p[1]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

void drawAnisoCylinder(float rBottom, float rTop, float height, int nSlice, int nStack, int nRing)
{
	//�~��(rBottom=rTop))�A�~����A�~��(rTop = 0.0)
	//rBottom:���ꔼ�a, rTop:��ꔼ�a
	//nSlice--xy�f�ʕ�����
	//nStack---������������
	//���̂̒��S�͉���Ə��̒���

	int i, j;
	double z0, z1, r0, r1;
	double theta;
	double theta0 = 2.0*M_PI/(double)nSlice;
	double p[2][3], n[3], tnt[3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

	//���(Top)
	//���S�ȊO
	for(j = 0; j < nRing; j++)
	{
		glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0, 0.0, 1.0);
		for(i = 0; i <= nSlice; i++) 
		{ 
			theta = theta0 * (double)i;
			r0 = rTop * (float)j / (float)nRing;
			r1 = rTop * (float)(j+1) / (float)nRing;
			p[0][0] = r0 * cos(theta); //x����
			p[0][1] = r0 * sin(theta); //y����
			p[0][2] = height/2.0;      //z����
			p[1][0] = r1 * cos(theta); //x����
			p[1][1] = r1 * sin(theta); //y����
			p[1][2] = height/2.0;      //z����
			tnt[0] = -sin(theta); tnt[1] = cos(theta); tnt[2] = 0.0;
			glVertexAttrib3dv(tangentLoc, tnt);
			glVertex3dv(p[0]);
			glVertex3dv(p[1]);
		}
		glEnd();
	}

	//����(Bottom)
	for(j = 0; j < nRing; j++)
	{
		glBegin(GL_QUAD_STRIP);
		glNormal3f(0.0, 0.0, -1.0);
		for(i = 0; i <= nSlice; i++) 
		{ 
			theta = theta0 * (double)(nSlice - i);
			r0 = rBottom * (float)j / (float)nRing;
			r1 = rBottom * (float)(j+1) / (float)nRing;
			p[0][0] = r0 * cos(theta); //x����
			p[0][1] = r0 * sin(theta); //y����
			p[0][2] = -height/2.0;      //z����
			p[1][0] = r1 * cos(theta); //x����
			p[1][1] = r1 * sin(theta); //y����
			p[1][2] = -height/2.0;      //z����
			tnt[0] = -sin(theta); tnt[1] = cos(theta); tnt[2] = 0.0;
			glVertexAttrib3dv(tangentLoc, tnt);
			glVertex3dv(p[0]);
			glVertex3dv(p[1]);
		}
		glEnd();
	}

	double t0, t1, phi;
	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);

	//����
	for(j = 0; j < nStack; j++)
	{
		//j=0�͏��(x=0, y=0, z=height/2)
		//2��t���W
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//��ʂ���̍���
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//���a
		r0 = rBottom + (rTop - rBottom) * z0 / height;
		r1 = rBottom + (rTop - rBottom) * z1 / height;

		//���_z���W
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			theta = theta0 * (double)i;
			//���_��xy���W
			p[0][0] = r0 * cos(theta);//x���W
			p[0][1] = r0 * sin(theta);//y���W
			p[1][0] = r1 * cos(theta);//x���W
			p[1][1] = r1 * sin(theta);//y���W
			//�@���x�N�g��
			n[0] = nxy * cos(theta);
			n[1] = nxy * sin(theta);
			n[2] = nz;
			tnt[0] = -sin(theta); tnt[1] = cos(theta); tnt[2] = 0.0;
			glVertexAttrib3dv(tangentLoc, tnt);

			glNormal3dv(n);//�@���x�N�g��
			glVertex3dv(p[0]);//���_���W
			glVertex3dv(p[1]);//���_���W
		}

		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}


void drawAnisoTorus(float radius1, float radius2, int nSide, int nRing)
{	
	//radius1:�~�f�ʔ��a
	//radius2:�~�̒��S�����a
	//nSide:�~�f�ʂɂ�����\�ʕ����_��
	//nRing:�~���S���ɉ����������_��

	int i, j;
	double rr, zz;
	double s0, s1, t, phi0, phi1, theta;
	double p[2][3], tnt[2][3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

	for(i = 0; i < nRing; i++)
	{
		//i=0�͊�{�f��(x=radius2�𒆐S�Ƃ���~, y=0�j
		//2�̂����W
		s0 = (double)i / (double)nRing;
		s1 = (double)(i+1) / (double)nRing;
		phi0 = 2.0 * M_PI * s0;
		phi1 = 2.0 * M_PI * s1;
		//�ڐ��x�N�g����xy������
		tnt[0][0] = - sin(phi0);
		tnt[0][1] =   cos(phi0);
		tnt[0][2] =   0.0;
		tnt[1][0] = - sin(phi1);
		tnt[1][1] =   cos(phi1);
		tnt[1][2] =   0.0;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			//t���W
			t = (double)j / (double)nSide;
			theta = 2.0 * M_PI * t;
//			theta = -M_PI + 2.0 * M_PI * t;
			rr = radius2 + radius1 * cos(theta);//z������̋���
			zz = radius1 * sin(theta);
			//���_��xyz���W(j=0�����xy����)
			p[0][0] = rr * cos(phi0);//x���W
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x���W
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z  

		  glVertexAttrib3dv(tangentLoc, tnt[0]);
			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//���_���W

		  glVertexAttrib3dv(tangentLoc, tnt[1]);
			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}
