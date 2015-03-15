/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

2�����}�`�i�O�p�`�j��`��
OpenGL + GLSL
*/
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"

//�֐��̃v���g�^�C�v�錾
void init();
void display();
void drawTriangle();
//�V�F�[�_�v���O������
GLuint shaderProg;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  //�\�����[�h
  glutInitDisplayMode(GLUT_RGBA);
  //�\���E�B���h�E�̃T�C�Y
  glutInitWindowSize(500, 500);
  //����̈ʒu
  glutInitWindowPosition(100, 100);
  //�E�B���h�E�쐬
  glutCreateWindow("�O�p�`�`��(SL_Triangle)");
  //�\��
  glutDisplayFunc(display);
  //�����ݒ�
  init();
  initGlsl(&shaderProg, "triangle.vert");
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
	//���\�[�X�̉��
 	glDeleteProgram(shaderProg);
  return 0;
}

void init(void)
{
  //�w�i�F
  glClearColor(0.2, 0.2, 0.2, 1.0);
}

void display(void)
{
  //�J���[�o�b�t�@�̃N���A
  glClear(GL_COLOR_BUFFER_BIT);
  //�`��
  // �V�F�[�_�v���O�����̓K�p 
  glUseProgram(shaderProg);
  drawTriangle();
  // �V�F�[�_�v���O�����̓K�p������	
  glUseProgram(0);
  //�I��
  glFlush();
}

void drawTriangle()
{
  //����
  glLineWidth(10.0);
  //���[�h
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
  //�O�p�`�̕`��
  glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.75, -0.75);//�Ԃ̒��_
	glColor3f(0.0, 1.0, 0.0); glVertex2f( 0.75, -0.75);//�΂̒��_
	glColor3f(0.0, 0.0, 1.0); glVertex2f( 0.0 ,  0.75);//�̒��_
  glEnd();
}
