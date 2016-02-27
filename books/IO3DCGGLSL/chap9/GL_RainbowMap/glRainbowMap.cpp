/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

虹色マップ
[Shift] + 's'で保存

*/
//作成するtexture imageのサイズ
#define TEX_WIDTH 440
#define TEX_HEIGHT 100
//保存ファイル名
#define FILENAME "file0.bmp"

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "../../imageLoadSave.h"

//関数のプロトタイプ宣言
void init();
void idle();
void makeColorData();
void display();
void keyboard(unsigned char key, int x, int y);

//Windowのサイズ
int width = TEX_WIDTH+100;
int height = TEX_HEIGHT+100;
//texture image
GLubyte image[TEX_HEIGHT * TEX_WIDTH][3];

Bitmap* bm = new Bitmap();
//描画の初期波長
float lambda0 = 360.0;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //表示モード
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  //表示ウィンドウのサイズ
  glutInitWindowSize(width, height);
  //左上の位置
  glutInitWindowPosition(100, 100);
  //ウィンドウ作成
  glutCreateWindow("虹色マップ作成(GL_RainbowMap)");
  //キーボードの利用
  glutKeyboardFunc(keyboard);
  //矢印キーなどの特殊キー利用
//  glutSpecialFunc(special);
  //表示
  glutDisplayFunc(display);

  //再描画
  glutIdleFunc(idle);

  //初期設定
  init();
  //イベント処理ループに入る
  glutMainLoop();
  return 0;
}

void idle(void)
{
  //再描画
  glutPostRedisplay();
}

void init(void)
{
 //背景色
  glClearColor(1.0, 1.0, 1.0, 1.0);
//	bm->imageType = COLOR24;//imageLoadSave.hで定義
}

void makeColorData()
{
  int i, j;
	float red = 0.0, green = 0.0, blue = 0.0, x;
	float red1, red2;
	float ultraviolet = 380.0;
	float infrared = 780.0;
	//中心
	float b0 = 0.2;//460nm;
	float g0 = 0.4;//540nm;
	float r0 = 0.7;//660nm
	float m0 = 0.08;//412
	//幅
	float wR = 0.3;//0.25;//R,G
	float wB = 0.2;//0.175;//B
	float wV = 0.08;//Violet

	for(i = 0; i < TEX_WIDTH; i++)
	{
		int ii = i + lambda0;
		x = ((float)(ii) - ultraviolet) /(infrared - ultraviolet);//正規化波長
		//blue
		blue = 255.0 * (1.0 - (x - b0)*(x - b0) / (wB * wB)) ;
		if(blue < 0.0) blue = 0.0;
		//green
		if(x < g0) green = 255.0 * (1.0 - (x - g0)*(x - g0) / (wB * wB));
		else       green = 255.0 * (1.0 - (x - g0)*(x - g0) / (wR * wR));
		if(green < 0.0) green = 0.0;
		//red
		red1 = 255.0 * (1.0 - (x - r0)*(x - r0) / (wR * wR));
		if(red1 < 0.0) red1 = 0.0;
		//magenta
		red2 = 170.0 * (1.0 - (x - m0)*(x - m0) / (wV * wV));
		if(red2 < 0.0) red2 = 0.0;
		red = red1 + red2;
		if(blue > 255.0) {printf("blue=%f x= %f\n", blue, x);
		blue = 255.0;}
		if(red > 255.0) red = 255.0;

		for(j = 0; j < TEX_HEIGHT; j++)
		{
			image[j * TEX_WIDTH + i][0] = red;
			image[j * TEX_WIDTH + i][1] = green;
			image[j * TEX_WIDTH + i][2] = blue;
		}
	}
}

void display()
{
  //カラーバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT );
	makeColorData();

	glLineWidth(1.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex2f(-0.9, -0.7);
		glVertex2f(0.9, -0.7);
	glEnd();
	//400nm
	glBegin(GL_LINES);
		glVertex2f(-0.6666, -0.7);
		glVertex2f(-0.6666, -0.5);
	glEnd();
	//500nm
	glBegin(GL_LINES);
		glVertex2f(-0.29629, -0.7);
		glVertex2f(-0.29629, -0.5);
	glEnd();
	//600nm
	glBegin(GL_LINES);
		glVertex2f( 0.074074, -0.7);
		glVertex2f( 0.074074, -0.5);
	glEnd();
	//700nm
	glBegin(GL_LINES);
		glVertex2f( 0.44444, -0.7);
		glVertex2f( 0.44444, -0.5);
	glEnd();
	//700nm
	glBegin(GL_LINES);
		glVertex2f( 0.81481, -0.7);
		glVertex2f( 0.81481, -0.5);
	glEnd();

  //描画
	glRasterPos2f(-0.8, -0.5);
	glDrawPixels(TEX_WIDTH,TEX_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);
	//

  //終了
	//glFlush();
  glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
  switch((unsigned char)key)
  {
  case 27://Esc
	  exit(0);
	  break;
  case 'S'://imageの保存([Shift] + 's')
		saveBitmap24(bm, FILENAME, TEX_WIDTH, TEX_HEIGHT, image); 
	  break;

  default:
	  break;
  }
}

