/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

2次元図形（三角形）を描く
固定シェーダー
*/

#include <GL/glut.h>

//関数のプロトタイプ宣言
void init();
void display();
void drawTriangle();

int main(int argc, char** argv)
{
  glutInit(&argc, argv);

  //表示モード
  glutInitDisplayMode(GLUT_RGBA);
  //表示ウィンドウのサイズ
  glutInitWindowSize(500, 500);
  //左上の位置
  glutInitWindowPosition(100, 100);
  //ウィンドウ作成
  glutCreateWindow("GL_Triangle");
  //表示
  glutDisplayFunc(display);
  //初期設定
  init();
  //イベント処理ループに入る
  glutMainLoop();
  return 0;
}

void init(void)
{
  //背景色
  glClearColor(0.2, 0.2, 0.2, 1.0);
}

void display(void)
{
  //カラーバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT);
  //描画
  drawTriangle();
  //終了
  glFlush();
}

void drawTriangle()
{
  //線幅
  glLineWidth(10.0);
  //モード
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  //三角形の描画
  glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.75, -0.75);//赤の頂点
	glColor3f(0.0, 1.0, 0.0); glVertex2f( 0.75, -0.75);//緑の頂点
	glColor3f(0.0, 0.0, 1.0); glVertex2f( 0.0 ,  0.75);//青の頂点
  glEnd();
}
