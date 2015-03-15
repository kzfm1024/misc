/*
Microsoft Visual Studio .NET2005 Standard Edition
�v���W�F�N�g�̎�ށFVisualC++�v���W�F�N�g
�e���v���[�g�FWin32�R���\�[���v���W�F�N�g
�A�v���P�[�V�����̎�ށF�R���\�[���A�v���P�[�V����
�ǉ��̃I�v�V�����F��̃v���W�F�N�g

2�����}�`�i�O�p�`�j��`��
�Œ�V�F�[�_�[
*/

#include <GL/glut.h>

//�֐��̃v���g�^�C�v�錾
void init();
void display();
void drawTriangle();

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
  glutCreateWindow("GL_Triangle");
  //�\��
  glutDisplayFunc(display);
  //�����ݒ�
  init();
  //�C�x���g�������[�v�ɓ���
  glutMainLoop();
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
  drawTriangle();
  //�I��
  glFlush();
}

void drawTriangle()
{
  //����
  glLineWidth(10.0);
  //���[�h
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //�O�p�`�̕`��
  glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.75, -0.75);//�Ԃ̒��_
	glColor3f(0.0, 1.0, 0.0); glVertex2f( 0.75, -0.75);//�΂̒��_
	glColor3f(0.0, 0.0, 1.0); glVertex2f( 0.0 ,  0.75);//�̒��_
  glEnd();
}
