/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

色空間
固定シェーダー（OpenGLアプリケーション）
*/
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
//#define M_PI 3.14159265358979

//関数のプロトタイプ宣言
void init();
void idle();
void display();
void draw();
void drawColorSpace();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);

//affine変換用変数
float pos[] = { 0.0, 0.0, 0.0 };    //object中心のxyz座標
float scale[] = { 1.0, 1.0, 1.0 };  //大きさ(倍率)
float angle[] = { 0.0, -30.0, 0.0 };//回転角度
//初期値
float pos0[] = { 0.0, 0.0, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, -30.0, 0.0 };

//カメラと視体積
struct View
{
  //カメラ
	float pos[3];//位置（視点）
	float cnt[3];//注視点
  float dist;  //注視点から視点までの距離
  float theta; //仰角（水平面との偏角）
  float phi;   //方位角
  //視体積
  float fovY;  //視野角
  float nearZ; //前方クリップ面(近平面)
  float farZ;  //後方クリップ面(遠平面)
};
//初期値
View view = { 
	0.0, 2.0, 4.0,//pos
  0.0, 0.0, 0.0,//cnt 
	4.0, 30.0, 20.0,//dist, theta, phi
	30.0, 1.0, 100.0//fovY,nearZ, farZ
};

//Windowのサイズ
int width = 500;
int height = 500;
//アフィン変換
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE};
SELECT_KEY sKey = TRANSLATE;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //表示モード
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  //表示ウィンドウのサイズ
  glutInitWindowSize(width, height);
  //左上の位置
  glutInitWindowPosition(100, 100);
  //ウィンドウ作成
  glutCreateWindow("色空間(GL_ColorSpace)");
  //ウィンドウのサイズ変更
  glutReshapeFunc(resize);
  //表示
  glutDisplayFunc(display);
  //キーボードの利用
  glutKeyboardFunc(keyboard);
  //矢印キーなどの特殊キー利用
  glutSpecialFunc(special);
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
  glClearColor(0.2, 0.2, 0.3, 1.0);
}

void display(void)
{
  //カラーバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT);

  //描画
  draw();
  //終了
  glutSwapBuffers();
}

void draw(void)
{
  //モード
  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_POINT);//GL_LINEにするとすべて直線表示
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[0], 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0], scale[1], scale[2]);
  drawColorSpace();  //描画オブジェクト
  glPopMatrix();
}

void drawColorSpace()
{
  float p[8][3] = 
  { {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
	{0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}
  };
  float c[8][3] = //各頂点の色
  { {1.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {1.0,1.0, 0.0},
	{0.0, 1.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0}
  };

  glBegin(GL_QUADS);
	glColor3fv(c[0]); glVertex3fv(p[0]); 
	glColor3fv(c[1]); glVertex3fv(p[1]);
	glColor3fv(c[2]); glVertex3fv(p[2]); 
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[0]); glVertex3fv(p[0]);
	glColor3fv(c[3]); glVertex3fv(p[3]);
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[4]); glVertex3fv(p[4]);

	glColor3fv(c[0]); glVertex3fv(p[0]); 
	glColor3fv(c[4]); glVertex3fv(p[4]);
	glColor3fv(c[5]); glVertex3fv(p[5]); 
	glColor3fv(c[1]); glVertex3fv(p[1]);

	glColor3fv(c[1]); glVertex3fv(p[1]); 
	glColor3fv(c[5]); glVertex3fv(p[5]);
	glColor3fv(c[6]); glVertex3fv(p[6]); 
	glColor3fv(c[2]); glVertex3fv(p[2]);

	glColor3fv(c[2]); glVertex3fv(p[2]); 
	glColor3fv(c[6]); glVertex3fv(p[6]);
	glColor3fv(c[7]); glVertex3fv(p[7]); 
	glColor3fv(c[3]); glVertex3fv(p[3]);

	glColor3fv(c[4]); glVertex3fv(p[4]); 
	glColor3fv(c[7]); glVertex3fv(p[7]);
	glColor3fv(c[6]); glVertex3fv(p[6]); 
	glColor3fv(c[5]); glVertex3fv(p[5]);
  glEnd();
}

void resize(int w, int h)
{
  //ビューポート変換
  glViewport(0, 0, w, h);
  //プロジェクション行列の指定
  glMatrixMode(GL_PROJECTION);
  //プロジェクション行列の初期化
  glLoadIdentity();
  //透視投影行列の設定（投影変換）
  gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
  glMatrixMode(GL_MODELVIEW);
  //モデルビュー行列の初期化
  glLoadIdentity();
  //視点を設定（ビューイング変換）
  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  //表示ウィンドウのサイズ
  width = w;
  height = h;
}


void keyboard(unsigned char key, int x, int y)
{
  switch(key)
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
  case 'R'://reset（初期値に戻す)
	  for(int i = 0; i < 3; i++)
		{
			pos[i] = pos0[i];
			scale[i] = scale0[i];
			angle[i] = angle0[i];
		}
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
	  case GLUT_KEY_RIGHT://x方向
		  pos[0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://-x方向
		  pos[0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) pos[1] += 0.1;//y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] -= 0.1; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) pos[1] -= 0.1;//-y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[2] += 0.1; //-z方向
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == ROTATE)
  {
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x軸回転
		  angle[0] += 10.0;
		  break;
	  case GLUT_KEY_LEFT://-x軸回転
		  angle[0] -= 10.0;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) angle[1] += 10;//y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] += 10; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) angle[1] -= 10;//-y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[2] -= 10; //-z方向
		  break;
	  default:
		  break;
	  }
  }
  else if(sKey == SCALE)
  {
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x方向拡大
		  scale[0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://x方向縮小
		  scale[0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) scale[1] += 0.1;//y方向拡大
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] += 0.1; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) scale[1] -= 0.1;//y方向縮小
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[2] -= 0.1; //z方向
		  break;
	  default:
		  break;
	  }
  }
}

