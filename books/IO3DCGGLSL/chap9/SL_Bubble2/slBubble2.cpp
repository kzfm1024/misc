/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�V���{����2
�L���[�u�}�b�s���O�ɂ�锽�ˊ��}�b�s���O��
��������ǉ�
OpenGL+GLSL
*/
#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../room2.h"
#include "../../myGlsl.h"
//�֐��̃v���g�^�C�v�錾
void init();
void idle();
void display();
void draw();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void setCubeMap();

//affine�ϊ��p�ϐ�
float pos[] = { 0.0, 1.0, 0.0 };  //object���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
float angle[] = { 0.0, 0.0, 0.0 };//��]�p�x
//�����l
float pos0[] = { 0.0, 1.0, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, 0.0, 0.0 };
//����
float lightPos[] = {0.0, 6.0, 0.0, 1.0}; //�����ʒu
float lightPos0[] = {0.0, 6.0, 0.0, 1.0};//�����ʒu(�����l)

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
  0.0, 1.2, 0.0,//cnt 
	3.0, 10.0, 0.0,//dist, theta, phi
	60.0, 0.1, 20.0//fovY,nearZ, farZ
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
//��]�A�j���[�V����
float ang = 0.0;
float dang = 0.0;
//�o�ߎ���
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec

//�i�q�Ԋu
float filmSpace = 800.0;
//�����̋��ܗ�
float refract = 1.3;
//�A���t�@�l����
float alpha = 0.5;

//ø����p
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
//�V�F�[�_�v���O������
GLuint shaderProg;

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
    0, TEX_HEIGHT,
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
    TEX_WIDTH, TEX_HEIGHT,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  },
  { 
		//��
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TEX_WIDTH,0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
  },
  { //��O
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    2*TEX_WIDTH, TEX_HEIGHT,
    0.0, 0.0, 1.0,
    0.0, -1.0, 0.0,
  },
  { //��
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    2*TEX_WIDTH, 0,
    0.0, 0.0, -1.0,
    0.0, -1.0, 0.0,
  }
};

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //�\�����[�h
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  //�\���E�B���h�E�̃T�C�Y
  glutInitWindowSize(width, height);
  //����̈ʒu
  glutInitWindowPosition(100, 100);
  //�E�B���h�E�쐬
  glutCreateWindow("�V���{����2(SL_Bubble2)");
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
  initGlsl(&shaderProg, "bubble2.vert", "bubble2.frag");
	
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

  //�e�N�X�`���̐ݒ�
  setCubeMap();

	//���Ԍv��
  lastTime = timeGetTime();
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
}

void setCubeMap()
{
  for(int i = 0; i < 6; i++)
	  glTexImage2D(target[i].name,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
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

	//step1:���}�b�v�̍쐬
	//�I�u�W�F�N�g���S���猩���U�����̃V�[�����쐬���A�e�N�X�`���������ɓ]��
  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // �����ϊ��s��̐ݒ� (texture�쐬�j
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.0, 0.1, 30.0);//����p��90��
  
  // ���f���r���[�ϊ��s��̐ݒ� 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // �I�u�W�F�N�g���猩���V�[�����e�N�X�`���ɗ��p 
  for (int i = 0; i < 6; ++i) 
  {
    // �r���[�|�[�g���e�N�X�`���̃T�C�Y�ɐݒ肷�� 
    glViewport(target[i].x, target[i].y, TEX_WIDTH, TEX_HEIGHT);
		// �I�u�W�F�N�g���S���猩������̂������_�����O 
		glPushMatrix();
		gluLookAt(pos[0], pos[1], pos[2], 
		    pos[0]+target[i].cx, pos[1]+target[i].cy, pos[2]+target[i].cz,
				target[i].ux, target[i].uy, target[i].uz);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		//3D��Ԃ̕`��
		drawRoom(10.0, 10.0, 5.0);
		glPopMatrix();
	    
		// �����_�����O�����摜���e�N�X�`���������ɓ]�� 
		glCopyTexSubImage2D(target[i].name, 0, 0, 0, target[i].x, target[i].y, TEX_WIDTH, TEX_HEIGHT);
  }

  //step2:�ʏ�̕`�惋�[�`���ɖ߂�
  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resize(width, height);  
  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  if(flagWireframe)//'w'��wireframe��solid model�؂�ւ�
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

//	glCullFace(GL_BACK);
//	glEnable(GL_CULL_FACE);

	//step3:�s�������̂��ɕ`��
  drawRoom(10.0, 10.0, 5.0);//room2.h�ɂ���

  //step4:
  glDepthMask(GL_FALSE); //���߽�ޯ̧���������݋֎~
  glEnable(GL_BLEND);//��̧�����ިݸނ�L���ɂ���
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//�F�����W�������߂�
  //step5:�������`��
  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);
  GLint spaceLoc = glGetUniformLocation(shaderProg, "filmSpace");
	glUniform1f(spaceLoc, filmSpace);
  GLint refractLoc = glGetUniformLocation(shaderProg, "refract");
	glUniform1f(refractLoc, refract);
  GLint alphaLoc = glGetUniformLocation(shaderProg, "alpha");
	glUniform1f(alphaLoc, alpha);
  GLint timeLoc = glGetUniformLocation(shaderProg, "time");
	glUniform1f(timeLoc, elapseTime2);

  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);
  //fragment shader�̃��j�t�H�[���ϐ�tex�̃C���f�b�N�X���擾
  GLint samplerLoc = glGetUniformLocation(shaderProg, "envSampler");
  glUniform1i(samplerLoc, 0);//GL_TEXTURE0��K�p
  //�`��
  draw(); 
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);

  //step6:
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);

  //��]�p�x�X�V
  ang += dang;
  if(ang >= 360.0) ang = 0.0;

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
		printf("[F1]�L�[�F�����̌���filmSpace�̑��� \n");
		printf("[F2]�L�[�F�����̋��ܗ�refract�̑��� \n");
		printf("[F3]�L�[�F�A���t�@�lalpha�̑��� \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void draw(void)
{
  glMaterialf(GL_FRONT,GL_SHININESS,50);

  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0] + ang, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0], scale[1], scale[2]);
//  pet.draw(0);//frameCount);
//  glutSolidSphere(1.0, 30, 30);
//  glutSolidTeapot(1.0);
	drawSphere(0.5, 20, 20);
//	glutSolidTorus(0.2, 0.6, 20, 20);
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

  if(cos(M_PI * view.theta /180.0) >= 0.0)//�J�����p90�x���ޭ������޸�ِؑ�
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);
 
 //�\���E�B���h�E�̃T�C�Y
  width = w;
  height = h;
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
		for(int i = 0; i < 3; i++)
		{
			pos[i] = pos0[i]; scale[i] = scale0[i]; angle[i] = angle0[i];
			lightPos[i] = lightPos0[i];
		}
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
		if(glutGetModifiers() == 0) filmSpace += 10.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) filmSpace -= 10.0;
		if(filmSpace < 0.0) filmSpace = 0.0;
		printf("�����̌��� = %f \n", filmSpace);
  }	
	if(key == GLUT_KEY_F2) 
	{
		if(glutGetModifiers() == 0) refract += 0.02;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) refract -= 0.02;
		if(refract < 1.0) refract = 1.0;
		printf("refract = %f \n", refract);
  }		
	if(key == GLUT_KEY_F3) 
	{
		if(glutGetModifiers() == 0) alpha += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) alpha -= 0.1;
		if(alpha > 1.0) alpha = 1.0;
		if(alpha < 0.0) alpha = 0.0;
		printf("alpha = %f \n", alpha);
  }		
	if(key == GLUT_KEY_PAGE_UP) 
	{
		dang += 0.1;
  }	
	if(key == GLUT_KEY_PAGE_DOWN) 
	{
		dang -= 0.1;
  }	}

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
		  view.dist -= 0.1; //�߂Â�
	  }
  }
  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist += 0.1; //��������
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
  //printf("view.posX=%f, view.posY=%f, view.posZ=%f \n", view.pos[0], view.pos[1], view.pos[2]);
  //printf("cntX=%f, cntY=%f, cntZ=%f \n", view.cnt[0], view.cnt[1], view.cnt[2]);
  //printf("phi=%f, theta=%f, fovY=%f \n", view.phi, view.theta, view.fovY);
	resize(width, height);
}

