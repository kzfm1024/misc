/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

�L���[�u�}�b�s���O�ɂ��
�t���l������2�i�J���[�}�b�v��ǉ�)
OpenGL+GLSL
*/
#include <stdio.h>
//#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../room2.h"
#include "../../myGlsl.h"
#include "../../imageLoadSave.h"
#include <time.h> // for clock_gettime()

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
void makeTexImage(Bitmap *bm);
void setTexture();
void initFramebuffer();

//affine�ϊ��p�ϐ�
float pos[] =   { 0.0, 1.0, -1.0 };  //object���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
float angle[] = { 0.0, 0.0, 0.0 };//��]�p�x
//�����l
float pos0[] =   { 0.0, 1.0, -1.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, 0.0, 0.0 };
//����
float lightPos[] = {5.0, 5.0, 5.0, 1.0}; //�����ʒu
float lightPos0[] = {5.0, 5.0, 5.0, 1.0};//�����ʒu(�����l)

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
	60.0, 0.1, 10.0//fovY,nearZ, farZ
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
//�A�j���[�V����
#include "../../myTexPet.h"
CmyTexPet pet ;
float curTime, lastTime, elapseTime;
int frameCount = 0;
int fps = 0;
int actionMode = 0;
float transparency0 = 0.3;//�����x
float reflectivity0 = 0.3;//�������˗�
float nRatio0 = 2.0;

//�����摜��ø����p
#define TEX_WIDTH2  128
#define TEX_HEIGHT2 128
GLubyte texImage[TEX_HEIGHT2][TEX_WIDTH2][4];
//�L���[�u�}�b�v�p
#define TEX_WIDTH  512
#define TEX_HEIGHT 512
//�V�F�[�_�v���O������
GLuint shaderProg;
//FBO�֘A
GLuint	texName[2];//texture object
GLuint	rbName;//renderbuffer
GLuint	fbName;//framebuffer
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
  glutCreateWindow("�t���l������2(SL_Fresnel2)");
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
	glewInit();
  init();
	initFramebuffer();
  initGlsl(&shaderProg, "Fresnel2.vert", "Fresnel2.frag");
	
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
	glDeleteTextures(2, texName);
	glDeleteRenderbuffersEXT(1, &rbName);
	glDeleteFramebuffersEXT(1, &fbName);
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
  glGenTextures(2, texName);//�e�N�X�`���I�u�W�F�N�g�̖��O�t��
	//CubeMap
  setCubeMap();
	//ColorMap
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/check1.bmp");
  makeTexImage(bm0);
  setTexture();
  //pet robot �A�j���[�V����
  pet.unitTime = 0.015;//�P�t���[��������̎��ԁi�ŏ��͉��̒l�j
  //�t���[���f�[�^���쐬
  pet.numFrame0 = 0;
  //�t���[���f�[�^���쐬
  pet.numFrame0 = 0;
  frameCount = 0;
  if(actionMode == 0)
  {
	  pet.walk(1.5, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.0, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.5, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.0, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
  }
  else
  {
	  pet.turn(90.0, 2.0);
	  pet.stand(5.0);
	  pet.swingTail(5, 2.0);
	  pet.wait(3.0);
	  pet.initPose(5.0);
	  pet.turn(-90.0, 2.0);
  }

  printf("numFrame0=%d \n", pet.numFrame0);
}

void setCubeMap()
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[0]);
   //�e�N�X�`���I�u�W�F�N�g�̍쐬
  for(int i = 0; i < 6; i++)
	glTexImage2D(target[i].name,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void initFramebuffer( void )
{
	//renderbuffer
	glGenRenderbuffersEXT( 1, &rbName );
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rbName );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEX_WIDTH, TEX_HEIGHT );
  //framebuffer
	glGenFramebuffersEXT( 1, &fbName );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbName );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, rbName );

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}


//�摜�t�@�C������C���[�W�f�[�^���쐬
void makeTexImage(Bitmap *bm)
{
  int i, j, ii, jj;

  if(bm->bi.Width < TEX_WIDTH2) printf("�摜�t�@�C���̻���>=TEX_WIDTH�Ƃ��邱��\n");

  for(j = 0; j < TEX_HEIGHT2; j++)
  {
		jj = (int)((float)j * (float)bm->bi.Height / (float)TEX_HEIGHT2);
		for(i = 0; i < TEX_WIDTH2; i++)
    {
      ii = (int)((float)i * (float)bm->bi.Width / (float)TEX_WIDTH2) ;
			if(bm->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bm->pixel[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixel[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixel[ii+jj*bm->bi.Width]).b;
				texImage[j][i][3] = 255;
			}
			else
			{
				texImage[j][i][0] = (bm->pixelA[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixelA[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixelA[ii+jj*bm->bi.Width]).b;
				texImage[j][i][3] = (bm->pixelA[ii+jj*bm->bi.Width]).a;
			}
    }
  }
	if(bm->bi.BitCount <= 24) free(bm->pixel);
	else free(bm->pixelA);
}

void setTexture()
{
   //�e�N�X�`���I�u�W�F�N�g�̍쐬
  glBindTexture(GL_TEXTURE_2D, texName[1]);
  //�e�N�X�`���̎w�� 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH2,TEX_HEIGHT2,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //�e�N�X�`���̌J��Ԃ����@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//GL_CLAMP);
  //�e�N�X�`�����g��E�k��������@�̎w�� 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

static double timeGetTime()
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	double msec = (double)tp.tv_sec * 1000.0 + (double)tp.tv_nsec / 1000.0;
	printf("%s: msec %f\n", __FUNCTION__, msec);
	return msec;
}

void display(void)
{
	//step1:���}�b�v�̍쐬
	//�I�u�W�F�N�g���S���猩���U�����̃V�[�����쐬���A�e�N�X�`���������ɓ]��

	//framebuffer object ��L����
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbName );

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
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              target[i].name, texName[0], 0);
    glViewport(0, 0, TEX_WIDTH, TEX_HEIGHT);
		//�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// �I�u�W�F�N�g���S���猩������̂������_�����O 
		glPushMatrix();
		gluLookAt(pos[0], pos[1], pos[2], 
		    pos[0]+target[i].cx, pos[1]+target[i].cy, pos[2]+target[i].cz,
				target[i].ux, target[i].uy, target[i].uz);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		//3D��Ԃ̕`��
		drawRoom(10.0, 10.0, 5.0);
		glPopMatrix();
  }
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

  //step2:�ʏ�̕`�惋�[�`���ɖ߂�
  //�J���[�o�b�t�@,�f�v�X�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resize(width, height);  
  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);
  GLint tranLoc = glGetUniformLocation(shaderProg, "transparency");
  glUniform1f(tranLoc, transparency0);
  GLint refLoc = glGetUniformLocation(shaderProg, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  GLint ratioLoc = glGetUniformLocation(shaderProg, "nRatio");
  glUniform1f(ratioLoc, nRatio0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  //�����ݒ�//'l'��������������ʒu��
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//texture unit �� texture object �̊֘A�t��
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texName[1]);
	//�e�N�X�`�����j�b�g���V�F�[�_���̃T���v���Ɋ֘A�t����
  GLint samplerLoc0 = glGetUniformLocation(shaderProg, "envSampler");
  glUniform1i(samplerLoc0, 0);//GL_TEXTURE0��K�p
  GLint samplerLoc1 = glGetUniformLocation(shaderProg, "texSampler");
  glUniform1i(samplerLoc1, 1);//GL_TEXTURE1��K�p
  //�`��
  draw(); 
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);

  drawRoom(10.0, 10.0, 5.0);//room.h�ɂ���

  curTime = timeGetTime();
  //1�R�}�̏��v����
  float dt = (float)(curTime - lastTime) * 0.001;//sec
  lastTime = curTime;
  elapseTime += dt;
  fps ++;
  if(elapseTime >= 1.0)
  {
  	printf("frame per sec = %d , dt = %f\n", fps, dt);
	  elapseTime = 0.0;
	  fps = 0;
  }
  frameCount++;

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
		printf("[F1]�L�[�F�A�j���[�V�����X�^�[�g \n");
		printf("[F2]�L�[�F���ߗ��̑��� \n");
		printf("[F3]�L�[�F���ܗ��̑��� \n");
		printf("[F4]�L�[�F�������˗��̑��� \n");
		flagHelp = false;
  }
  //�I��
  glutSwapBuffers();
}

void draw(void)
{
  float ambient[] = { 0.3, 0.3, 0.3, 1.0};
  float diffuse[] = { 0.7, 0.7, 0.7, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0], 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0], scale[1], scale[2]);
//  pet.draw(frameCount);
  drawTexSphere2(1.0, 30, 30, 1, 1);
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
		actionMode += 1; 
		if(actionMode > 1) actionMode = 0;
		printf("actionMode = %d \n", actionMode);
	  init();//�A�j���[�V�����X�^�[�g
	}	
	if(key == GLUT_KEY_F2) 
	{
	  if(glutGetModifiers() == 0) transparency0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	transparency0 -= 0.1;  
		if(transparency0 < 0.0) transparency0 = 0.0;
		if(transparency0 > 1.0) transparency0 = 1.0;
		printf("�����x= %f \n", transparency0);
	}	
	if(key == GLUT_KEY_F3) 
	{
		if(glutGetModifiers() == 0) nRatio0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	nRatio0 -= 0.1;
		printf("����ܗ�= %f \n", nRatio0);		
	}	
	if(key == GLUT_KEY_F4) 
	{
	  if(glutGetModifiers() == 0) reflectivity0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	reflectivity0 -= 0.1;  
		if(reflectivity0 < 0.0) reflectivity0 = 0.0;
		if(reflectivity0 > 1.0) reflectivity0 = 1.0;
		printf("�������˗�= %f \n", reflectivity0);
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
//  printf("view.posX=%f, view.posY=%f, view.posZ=%f \n", view.pos[0], view.pos[1], view.pos[2]);
//  printf("cntX=%f, cntY=%f, cntZ=%f \n", view.cnt[0], view.cnt[1], view.cnt[2]);
//  printf("phi=%f, theta=%f, fovY=%f \n", view.phi, view.theta, view.fovY);
	resize(width, height);
}

