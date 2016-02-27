/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

����
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "../../midpoint.h"
#include "terrain.h"
#include "particle.h"
#include "../../imageLoadSave.h"

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
void makeTerrainImage(Bitmap *bm);
void setTerrainTexture();
void drawParticle(float dt);

//����
float lightPos[] =  {10.0, 15.0, 10.0, 1.0};//�����ʒu
float lightPos0[] = {10.0, 15.0, 10.0, 1.0};//�����ʒu(�����l)

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
  0.0, 18.0, 0.0,//cnt 
	90.0, 30.0, 20.0,//dist, theta, phi
	30.0, 1.0, 200.0//fovY,nearZ, farZ
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
//texture
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][4];
//�V�F�[�_�v���O������
GLuint shaderProg;

//�o�ߎ���
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
//particle
CParticle p[NUM_PARTICLE];
float light = 0.3;

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
  glutCreateWindow("����(SL_Eruption)");
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

  setCamera();//���_�����߂�
  setLight(); //�����ݒ�@
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("�}�E�X/�L�[����̐����ɂ�'h'�L�[���v�b�V�� \n");

  //Terrain�f�[�^�̍쐬
	makeTerrainData();
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/altgrad4.bmp");
  makeTerrainImage(bm0);
  setTerrainTexture();

	//���Ԍv��
  lastTime = timeGetTime();
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
	countP = 0.0;

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
  //�e�N�X�`���̎w�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
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
	setLight();

  //�`��
  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);
	//�e�N�X�`�����j�b�g���V�F�[�_���̃T���v���Ɋ֘A�t��,�����_�����O
  //fragment shader�̃��j�t�H�[���ϐ�tex�̃C���f�b�N�X���擾
  GLint texLoc = glGetUniformLocation(shaderProg, "tex");
  glUniform1i(texLoc, 0);//GL_TEXTURE0��K�p
  drawTerrain(); 
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);
	//particle
	drawParticle(dt);

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
		printf("�}�E�X����Ŏ��_�ύX�� \n");
		printf("  dolly�F�����t�߂����{�^���N���b�N�ŋ߂Â��C�E�{�^���N���b�N�ŉ������� \n");
		printf("  pan�F��������щE�����E�{�^���N���b�N�Œ����_�����E�ɕω����� \n");
		printf("  tilt�F�^�エ��ѐ^�����E�{�^���N���b�N�Œ����_���㉺�ɕω����� \n");
		printf("  tumble�F���E�Ƀh���b�O����Ǝ��_�����E�ɕω����� \n");
		printf("  crane�F�㉺�Ƀh���b�O����Ǝ��_���㉺�ɕω����� \n");
		printf("  zoom�F�������E�{�^���N���b�N�ŃY�[���C�� \n");
		printf("        �E�����E�{�^���N���b�N�ŃY�[���A�E�g \n");
		printf("[Shift]+'r'�Ń��Z�b�g \n");
		printf("[F1]�L�[�F���̒��� \n");
		printf("[F2]�L�[�F�Ђ̒��� \n");
		printf("[F3]�L�[�Fseed�̕ύX \n");
		printf("[F4]�L�[�Fnum0�̕ύX \n");
		printf("[F5]�L�[�Fx�����̕���accelX�ύX \n");
		printf("[F6]�L�[�F����GL_LIGHT0�̖��邳�ύX \n");

		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void drawParticle(float dt)
{
  int i;
  glDepthMask(GL_FALSE); //���߽�ޯ̧���������݋֎~
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	for(i = 0; i < num0; i++) p[countP + i].create(elapseTime2);//���q�̐���
	countP += num0;
  if(countP + num0 >= NUM_PARTICLE) countP = 0;

  for(i = 0; i < NUM_PARTICLE; i++) 
  {
		//���q�̈ʒu�ɑ�������Terrain�̍��������߂�
		if(p[i].vPosition.x <= - sizeX * sc / 2.0 || p[i].vPosition.x >= sizeX * sc / 2.0) continue;
		if(p[i].vPosition.z <= - sizeY * sc / 2.0 || p[i].vPosition.z >= sizeY * sc / 2.0) continue;
		p[i].heightT = getHeightTerrain(p[i].vPosition.x, p[i].vPosition.z);//terrain.h�ɂ���
		p[i].show(elapseTime2);
		p[i].update(elapseTime2, dt);
		p[i].vAccel.x = accelX;//[F4]�L�[�ŕύX
	}
	glDisable(GL_BLEND);
  glDepthMask(GL_TRUE); //���߽�ޯ̧�̏������݂�����
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
  float lightAmbient0[] = {light, light, light, 1.0}; //����
  float lightDiffuse0[] = {light, light, light, 1.0}; //�g�U��
  float lightSpecular0[] = {0.2, 0.2, 0.2, 1.0}; //���ʌ�
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//���΂ɂ��Ɩ�
	float lightPos1[] = {0.0, hCrater + 5.0, 0.0, 1.0};
  float lightAmbient1[] = {0.0, 0.0, 0.0, 1.0}; 
  float lightDiffuse1[] = {0.8, 0.5, 0.0, 1.0}; 
  float lightSpecular1[] = {0.2, 0.2, 0.2, 1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
	//back
  glClearColor(light/2.0, light/2.0, light/2.0, 1.0);
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
	  pos[0] = 0.0; pos[1] = 0.0; pos[2] = 0.0;
	  scale[0] = scale[1] = scale[2] = 1.0;
	  angle[0] = angle[1] = angle[2] = 0.0;
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
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] -= d;//-z����
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1] -= d;//-y����
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] += d;//z����
		  break;
	  default:
		  break;
	  }
printf("lightPos[1]=%f \n", lightPos[1]);
	
  }

  //beta��ς���
  if(key == GLUT_KEY_F1) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta -= 0.1;
	  else beta += 0.1;
		printf("beta = %f \n", beta);
	  makeTerrainData();
  }
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) sigma0 -= 0.1;
	  else sigma0 += 0.1;
		printf("sigma0 = %f \n", sigma0);
	  makeTerrainData();
  }
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) seed -= 1;
	  else seed += 1;
		printf("seed = %d \n", seed);
	  makeTerrainData();
  }
  //num0
  if(key == GLUT_KEY_F4) 
  {
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) num0 -= 10; 
		else num0 += 10; 
		if(num0 < 10) num0 = 10;
		if(num0 > 500) num0 = 500;
		printf("num0 = %d \n", num0);
  }
  //accelX(���́j
  if(key == GLUT_KEY_F5) 
  {
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) accelX -= 1.0; 
		else accelX += 1.0; 
		printf("accelX = %f \n", accelX);
  }
  if(key == GLUT_KEY_F6)
  {
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) light -= 0.05;
		else light += 0.05;
		if(light > 1.0) light = 1.0;
		if(light < 0.0) light = 0.0;
		printf(" light = %f \n", light);
		setLight();
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
