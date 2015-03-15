/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

中点変位法による2次元テクスチャの作成
*/
//作成するtexture imageのサイズ
#define TEX_WIDTH 256
#define TEX_HEIGHT 256
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
double getNormalRandom(double sigma0);
void makeTextureData();
void makeImageData();
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void display();

//初期分割数
int numDivX0 = 2;
int numDivY0 = 2;

float data[(TEX_WIDTH+1)][(TEX_HEIGHT+1)];//濃度データ
float h[(TEX_WIDTH+1)][(TEX_HEIGHT+1)];//初期値,中間値
double beta = 2.0;//fBm指数
double sigma0 = 0.3;//標準偏差初期値
int seed = 0;//乱数のｼｰﾄ（値を変えると異なったパターンが作成）ﾞ

//Windowのサイズ
int width = TEX_WIDTH;
int height = TEX_HEIGHT;
//texture image
GLubyte image[TEX_HEIGHT * TEX_WIDTH][3];
//しきい値
float thUpper = 0.8;
float thLower = 0.5;
//経過時間
float curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
bool flagHelp = false;

Bitmap* bm = new Bitmap();

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
  glutCreateWindow("ﾃｸｽﾁｬ作成(GL_TexMaker2D)");
  //キーボードの利用
  glutKeyboardFunc(keyboard);
  //矢印キーなどの特殊キー利用
  glutSpecialFunc(special);
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
  glClearColor(0.0, 0.0, 0.0, 1.0);

	printf("[s] + [Shift]キー：FILENAMEへセーブ \n");
	printf("function keyによるパラメータの調整 \n");
	printf(" [F1]キー：ハースト指数hurstの調整 \n");
	printf(" [F2]キー：標準偏差sigmaの調整 \n");
	printf(" [F3]キー：乱数系列seedの変更 \n");
}

void makeTextureData()
{//中点変位法による高度データ作成
  int i, j, ii, jj;
	int numDivX = numDivX0;
	int numDivY = numDivY0;
	if(((float)numDivX0 / (float)numDivY0) != ((float)TEX_WIDTH / (float)TEX_HEIGHT))
	{
		printf("分割数の設定が間違っています\n");
		return;
	}

  srand(seed);
	double hurst = (beta-1.0) / 2.0;

  //初期高度データ
  for(j = 0; j <= numDivY; j++)
		for(i = 0; i <= numDivX; i++) 
		{
			if(i == 0 || i == numDivX || j == 0 || j == numDivY)
				h[i][j] = -0.3;
			else h[i][j] = 0.7;
		}

	double sigma = sigma0 ;

	while(numDivX <= TEX_WIDTH / 2)
  {
		sigma /= pow(2.0, hurst);
		numDivX *= 2;//分割数を2倍
		numDivY *= 2;
	  	
		for(jj = 0; jj <= numDivY; jj++)
		{
			for(ii = 0 ; ii <= numDivX; ii++)
			{
				j = jj / 2;
				i = ii / 2;
				if(j*2 == jj && i*2 == ii) //jj=even, ii=even
				{
					data[ii][jj] = h[i][j];//確定値
				}

				else if(j*2 == jj && i*2 != ii)//jj=even, ii=odd
				{
					data[ii][jj] = (h[i][j] + h[i+1][j]) / 2.0 + getNormalRandom(sigma);
				}
				else if(j*2 != jj && i*2 == ii)//jj=od, ii=even
				{
					data[ii][jj] = (h[i][j] + h[i][j+1]) / 2.0 + getNormalRandom(sigma);
				}
				else //jj=odd, ii=odd
				{
					data[ii][jj] = (h[i][j] + h[i+1][j] + h[i][j+1] + h[i+1][j+1]) / 4.0 
						+ getNormalRandom(sigma);
				}
			}
		}
		for(j = 0; j <= numDivY; j++) 
			for(i = 0; i <= numDivX; i++) h[i][j] = data[i][j];
	}
	for(j = 0; j <= numDivY; j++) 
		for(i = 0; i <= numDivX; i++) 
		{
			if( data[i][j] >= 1.0) data[i][j] = 1.0;
			if( data[i][j] <= 0.0) data[i][j] = 0.0;
		}
}

double getNormalRandom(double sigma)
{//正規乱数
  double ran = 0.0;
  for(int i=0; i < 12; i++) ran += (double)rand() / (double)RAND_MAX;
  
  ran -= 6.0;
  ran *= sigma;
  return ran;
}

void makeImageData()
{
  int i, j;

  for(j = 0; j < TEX_HEIGHT; j++)
  {
		for(i = 0; i < TEX_WIDTH; i++)
    {
			if(data[i][j] > thUpper)
			{
				image[j * TEX_WIDTH + i][0] = 255;
				image[j * TEX_WIDTH + i][1] = 255;
				image[j * TEX_WIDTH + i][2] = (GLubyte)(255.0 * (data[i][j] - thUpper) / (1.0 - thUpper));
			}
			else if(data[i][j] > thLower)
			{
				image[j * TEX_WIDTH + i][0] = 255;
				image[j * TEX_WIDTH + i][1] = (GLubyte)(255.0 * (data[i][j] - thLower) / (thUpper - thLower));
				image[j * TEX_WIDTH + i][2] = 0;
			}
			else
			{
				image[j * TEX_WIDTH + i][0] = (GLubyte)(255.0 * data[i][j] / thLower);
				image[j * TEX_WIDTH + i][1] = 0;
				image[j * TEX_WIDTH + i][2] = 0;
			}
		}
  }
}

void display()
{
  //カラーバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT );
	//中点変位法によるtextureデータの作成
	makeTextureData();
	makeImageData();

  //描画
	glDrawPixels(TEX_WIDTH,	TEX_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, image);

  //終了
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

void special(int key, int x, int y)
{
  //hurstを変える
  if(key == GLUT_KEY_F1) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta -= 0.1;
	  else beta += 0.1;
		printf("hurst = %f \n", beta);
  }
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) sigma0 -= 0.01;
	  else sigma0 += 0.01;
		printf("sigma0 = %f \n", sigma0);
  }
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) seed -= 1;
	  else seed += 1;
		printf("seed = %d \n", seed);
  }
}