/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�ψʃ}�b�s���O1
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../myGlsl.h"
#include "../../imageLoadSave.h"

//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void draw0();
void draw1();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
void makeTexture();
void setNormalMap(int no);
void makeNormalMap(Bitmap *bmp);
void drawLight();
//�o���v�}�b�s���O�p�v���~�e�B�u
void drawDispPlate(float size, int nDiv, int nRepeatS, int nRepeatT);
void drawDispCube(float size, int nDiv, int nRepeatS, int nRepeatT);
void drawBumpSphere(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT);
void drawBumpTorus(double radius1, double radius2, int nSide, int nRing, int nRepeatS, int nRepeatT);

//affine�ϊ��p�ϐ�
float pos[][3] = { //�ʒu�i���s�ړ��j
	{ 0.75, 1.0, 0.0},//obj0
	{-0.75, 1.0, 0.0} //obj1
};

float pos0[][3] = {//�����l
	{ 0.75, 1.0, 0.0},//obj0
	{-0.75, 1.0, 0.0} //obj1
};

float scale[][3] = {//�T�C�Y�i�X�P�[�����O�j
	{ 1.0, 1.0, 1.0 },//obj0
	{ 1.0, 1.0, 1.0 } //obj1
};
float scale0[][3] = {//�����l
	{ 1.0, 1.0, 1.0 }, //obj0
	{ 1.0, 1.0, 1.0 }  //obj1
};

float angle[][3] = {//�p���i��]�j
	{ 0.0, 0.0, 0.0}, //obj0
	{ 0.0, 0.0, 0.0}  //obj1
};
float angle0[][3] = {//�����l
	{ 0.0, 0.0, 0.0},  //obj0
	{ 0.0, 0.0, 0.0}   //obj1
};

//����
float lightPos[] =  {10.0, 15.0, 10.0, 1.0};//�����ʒu
float lightPos0[] = {10.0, 15.0, 10.0, 1.0};//�����ʒu(�����l)
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
	7.0, 30.0, 20.0,//dist, theta, phi
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
float dang = 0.1;//�p�x����
//texture
#define TEX_WIDTH 256
#define TEX_HEIGHT 256
GLubyte normalMap[TEX_HEIGHT][TEX_WIDTH][3];
GLuint texName[2];
//�V�F�[�_�v���O������
GLuint shaderProg;

float inv = 1.0;//���ʔ��]
float fGrad = 5.0;//���z����
float fDisp = 0.05;//�ψʗʒ����t�@�N�^

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
  glutCreateWindow("�ψʃ}�b�s��1(SL_Displacement1)");
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
  initGlsl(&shaderProg, "displacement1.vert", "displacement1.frag");
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

  //�e�N�X�`���[�쐬
  glGenTextures(2, texName);//�e�N�X�`���I�u�W�F�N�g�̖��O�t��
	makeTexture();
}

void makeTexture()
{
  Bitmap* bm0 = new Bitmap();
//  loadBitmap(bm0, "../../bmp256/char256-1.bmp");
  loadBitmap(bm0, "../../bmp256/disk4-256.bmp");
  makeNormalMap(bm0);
  setNormalMap(0);
  //obj1�p
  Bitmap* bm1 = new Bitmap();
//  loadBitmap(bm1, "../../bmp256/char256-2.bmp");
  loadBitmap(bm1, "../../bmp256/disk4-256.bmp");
  makeNormalMap(bm1);
  setNormalMap(1);
}

void makeNormalMap(Bitmap *bmp)
{//�@���}�b�v�p�z��쐬
  int i, j, ip, jp;
  float r, g;

  if(bmp->bi.Width != TEX_WIDTH || bmp->bi.Height != TEX_HEIGHT)
  {
		printf("TEX_WIDTH,TEX_HEIGHT�ƃt�@�C���T�C�Y����v�����Ă�������\n");
		return;
  }
  //BMP�t�@�C�����獂���}�b�v�����[�h
  for(j = 0; j < TEX_HEIGHT; j++)
  {
		for(i = 0; i < TEX_WIDTH; i++)
		{
      //���x�}�b�v���擾
			if(bmp->bi.BitCount <= 24)
			normalMap[j][i][2] = (bmp->pixel[i+j * TEX_WIDTH]).r;
			else//32bit color
			normalMap[j][i][2] = (bmp->pixelA[i+j * TEX_WIDTH]).r;
		}
  }
	if(bmp->bi.BitCount <= 24) free(bmp->pixel);
	else free(bmp->pixelA);

  //�����}�b�v����@���}�b�v���쐬
  for(j = 0; j < TEX_HEIGHT; j++)
  {
		jp = j+1;
		if(jp >= TEX_HEIGHT) jp = 0;
		for(i = 0; i < TEX_WIDTH; i++)
		{
			ip = i+1;
			if(ip >= TEX_WIDTH) ip = 0;
			float deltaS = inv * (normalMap[j][ip][2] - normalMap[j][i][2]);
			float deltaT = inv * (normalMap[jp][i][2] - normalMap[j][i][2]);
			r = 127.5 + deltaS * fGrad;
			if(r < 0.0) r = 0.0;
			if(r > 255.0) r = 255.0;
			g = 127.5 + deltaT * fGrad;
			if(g < 0.0) g = 0.0;
			if(g > 255.0) g = 255.0;
			normalMap[j][i][0] = (GLubyte)r;
			normalMap[j][i][1] = (GLubyte)g;
			if(inv == 1.0) //���]�Ȃ�
				normalMap[j][i][2] = (GLubyte)(normalMap[j][i][2]);
			else//if(inv == -1.0)//���]����
				normalMap[j][i][2] = (GLubyte)(256 - normalMap[j][i][2]);
		}
  }
}

void setNormalMap(int No)
{
  glBindTexture(GL_TEXTURE_2D, texName[No]);//�e�N�X�`�����o�C���h����

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,normalMap);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);//LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);//�o�C���h������
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


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texName[1]);

  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);

	GLint dispLoc = glGetUniformLocation(shaderProg, "fDisp");
	glUniform1f(dispLoc, fDisp);
	GLint invLoc = glGetUniformLocation(shaderProg, "inv");
	glUniform1f(invLoc, inv);
  GLint texNormalLoc = glGetUniformLocation(shaderProg, "normalMap");
  glUniform1i(texNormalLoc, 0);//GL_TEXTURE0��K�p
  draw0(); 
  glUniform1i(texNormalLoc, 1);//GL_TEXTURE1��K�p
  draw1();
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);
  
  drawFloor(10.0, 10.0, 10, 10);
  //�e
  drawShadow();

	drawLight();

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
		printf("[F1]�L�[�F���ʔ��] \n");
		printf("[F2]�L�[�F���z���� \n");
		printf("[F3]�L�[�F���_���W�ψʗʒ��� \n");
		printf("[Page Up]�L�[�F��]�p�x+0.1 \n");
		printf("[Page Dn]�L�[�F��]�p�x-0.1 \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void draw0(void)
{
  float ambient[] = { 0.3, 0.3, 0.3, 1.0};
  float diffuse[] = { 0.7, 0.7, 0.7, 1.0};
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
  //�I�u�W�F�N�g�`��
  if(!flagShadow)
  {
//  drawDispPlate(1.0, 100, 1, 1);
//	drawDispCube(1.0, 100, 1, 1);
		drawBumpTorus(0.25, 0.45, 200, 200, 16, 4);
  }
  else
  {//�e�\���̂Ƃ���Texture��t���Ȃ�
// drawPlate(1.0);
// drawCube(1.0);
		drawTorus(0.25, 0.45, 10, 10);
  }

  glPopMatrix();
}

void draw1(void)
{
  float ambient[] = { 0.1, 0.3, 0.4, 1.0};
  float diffuse[] = { 0.2, 0.6, 0.6, 1.0};
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
  //�I�u�W�F�N�g�`��
  if(!flagShadow)
  {
	  drawBumpSphere(0.7, 200, 200, 8, 4);
//	drawDispCube(1.0, 100, 1, 1);
  }
  else
  {
	  drawSphere(0.5, 10, 10);
//	drawCube(1.0);
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
		if(inv == 1.0) inv = -1.0; 
		else inv = 1.0;
		makeTexture();
  }	
  if(key == GLUT_KEY_F2) 
  {
		if(glutGetModifiers() == 0) fGrad += 1.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fGrad -= 1.0;
		if(fGrad < 1.0) fGrad = 1.0;
		printf("fGrad = %f \n", fGrad);
		makeTexture();
  }
  if(key == GLUT_KEY_F3) 
  {
		if(glutGetModifiers() == 0) fDisp += 0.01;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fDisp -= 0.01;
		if(fDisp < 0.0) fDisp = 0.0;
		printf("fDisp = %f \n", fDisp);
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
//�����ϯ��ݸ�(Plate��Cube�͕ψ�ϯ��ݸޗp�ɕύX�j
void drawDispPlate(float size, int nDiv, int nRepeatS, int nRepeatT)
{//x-y����
	float tnt[3];
	float s, t;
	float q[4][3];//���ʏ�̕������_
	float s0, s1, t0, t1, d0, d1;
	int i, j;
	float sz = 0.5 * size;
	static float p[4][3] = //z:�����
	{ 
		{ sz, sz, 0.0}, {-sz, sz, 0.0}, 
		{-sz,-sz, 0.0}, { sz,-sz, 0.0}
	};

	s = (float)nRepeatS;
	t = (float)nRepeatT;
  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");
  glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0, 0.0, 1.0); //z�����̖@��
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z���W
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[0][2];
		//y���W
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;
    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x���W
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawDispCube(float size, int nDiv, int nRepeatS, int nRepeatT)
{
	float sz, s, t, tnt[3];
	float q[4][3];//���ʏ�̕������_
	float s0, s1, t0, t1, d0, d1;
	int i, j;

	sz = 0.5 * size;
  float p[8][3] =
  { { sz, sz, sz}, {-sz, sz, sz}, {-sz,-sz, sz}, 
		{ sz,-sz, sz}, { sz, sz,-sz}, {-sz, sz,-sz},
		{-sz,-sz,-sz}, { sz,-sz,-sz}
  };

	s = (float)nRepeatS;
	t = (float)nRepeatT;
  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);

	//top(z�����������̂Ƃ�z�����j
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,1.0f);
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z���W
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[0][2];
		//y���W
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;
    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x���W
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//x����(���ʁj
	tnt[0] = 0.0; tnt[1] = 0.0; tnt[2] =-1.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(1.0f,0.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//x���W
		q[0][0] = q[1][0] = q[2][0] = q[3][0] = p[0][0];
		//y���W
		q[0][1] = q[1][1] = p[3][1] + (p[0][1] - p[3][1]) * d0;
		q[2][1] = q[3][1] = p[3][1] + (p[0][1] - p[3][1]) * d1;

    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//z���W
			q[0][2] = q[3][2] = p[3][2] + (p[7][2] - p[3][2]) * d0;
			q[1][2] = q[2][2] = p[3][2] + (p[7][2] - p[3][2]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//y����
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,1.0f,0.0f);
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//y���W
		q[0][1] = q[1][1] = q[2][1] = q[3][1] = p[0][1];
		//z���W
		q[0][2] = q[1][2] = p[0][2] + (p[4][2] - p[0][2]) * d0;
		q[2][2] = q[3][2] = p[0][2] + (p[4][2] - p[0][2]) * d1;
    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x���W
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-x����
	tnt[0] = 0.0; tnt[1] = 0.0; tnt[2] = 1.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(-1.0f,0.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//x���W
		q[0][0] = q[1][0] = q[2][0] = q[3][0] = p[2][0];
		//y���W
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;

    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//z���W
			q[0][2] = q[3][2] = p[5][2] + (p[1][2] - p[5][2]) * d0;
			q[1][2] = q[2][2] = p[5][2] + (p[1][2] - p[5][2]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-y����
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,-1.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//y���W
		q[0][1] = q[1][1] = q[2][1] = q[3][1] = p[2][1];
		//z���W
		q[0][2] = q[1][2] = p[6][2] + (p[2][2] - p[6][2]) * d0;
		q[2][2] = q[3][2] = p[6][2] + (p[2][2] - p[6][2]) * d1;
    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x���W
			q[0][0] = q[3][0] = p[2][0] + (p[3][0] - p[2][0]) * d0;
			q[1][0] = q[2][0] = p[2][0] + (p[3][0] - p[2][0]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-z����
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,-1.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z���W
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[4][2];
		//y���W
		q[0][1] = q[1][1] = p[5][1] + (p[6][1] - p[5][1]) * d0;
		q[2][1] = q[3][1] = p[5][1] + (p[6][1] - p[5][1]) * d1;
    //t ø������W
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x���W
			q[0][0] = q[3][0] = p[5][0] + (p[4][0] - p[5][0]) * d0;
			q[1][0] = q[2][0] = p[5][0] + (p[4][0] - p[5][0]) * d1;
			//s ø������W
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

//���̃o���v�}�b�s���O�p
void drawBumpSphere(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT)
{
	int i, j;
	double s, t0, t1, r0, r1, th0, th1, phi;
	double p[2][3], tnt[3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

	for(j = 0; j < nStack; j++)
	{
		//j=0�͖k�ɓ_(x=0, y=0, z=radius)
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

		//�k�ɓ_��1�Ƃ���t���W
		t0 = (1.0 - t0) * nRepeatT;
		t1 = (1.0 - t1) * nRepeatT;

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s���W
			s = (double)i / (double)nSlice;
			phi = -M_PI + 2.0 * M_PI * s;
			//���_��xy���W(i=0��^���)
			p[0][0] = r0 * cos(phi);//x���W
			p[0][1] = r0 * sin(phi);//y���W
			p[1][0] = r1 * cos(phi);//x���W
			p[1][1] = r1 * sin(phi);//y���W
			//�ڐ��x�N�g����xy����
			tnt[0] =-sin(phi);
			tnt[1] = cos(phi);

			s *= nRepeatS;

			glVertexAttrib3dv(tangentLoc, tnt);
			glTexCoord2d(s, t0);//�e�N�X�`�����W
			glNormal3dv(p[0]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[0]);//���_���W

			glTexCoord2d(s, t1);//�e�N�X�`�����W
			glNormal3dv(p[1]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

void drawBumpTorus(double radius1, double radius2, 
									int nSide, int nRing, int nRepeatS, int nRepeatT)
{	
	//radius1:�~�f�ʔ��a
	//radius2:�~�̒��S�����a
	//nSide:�~�f�ʂɂ�����\�ʕ����_��
	//nRing:�~���S���ɉ����������_��

	int i, j;
	double rr, zz, z0, z1;
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

		//s���W
		s0 *= (double)nRepeatS;
		s1 *= (double)nRepeatS;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			//t���W
			t = (double)j / (double)nSide;
			theta = -M_PI + 2.0 * M_PI * t;
			rr = radius2 + radius1 * cos(theta);//z������̋���
			zz = radius1 * sin(theta);
			//���_��xyz���W(j=0�����xy����)
			p[0][0] = rr * cos(phi0);//x���W
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x���W
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z  

		  t *= (double)nRepeatT;

		glVertexAttrib3dv(tangentLoc, tnt[0]);
			glTexCoord2d(s0, t);//�e�N�X�`�����W
			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//���_���W

		glVertexAttrib3dv(tangentLoc, tnt[1]);
			glTexCoord2d(s1, t);//�e�N�X�`�����W
			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

