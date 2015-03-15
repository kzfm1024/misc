/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

集光模様1
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "wave.h"

//関数のプロトタイプ宣言
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
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
void makeTexImage();
void setTexture();
void drawFloor0(float widthX, float widthZ, int nx, int nz);
void setTextureMatrix();
void drawLight();

//光源
float lightPos[] = {0.0, 10.0, 0.0, 1.0};//光源位置
float lightPos0[] = {0.0, 10.0, 0.0, 1.0};//光源位置(初期値)
//影のマテリアル
float shadowDiffuse[] =  {0.0,0.0,0.0,0.1};//影の拡散光
float shadowSpecular[] = {0.0,0.0,0.0,1.0};//影の鏡面光

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
View view = { 
	0.0, 0.0, 0.0,//pos(仮設定）
  0.0, 1.0, 0.0,//cnt 
	15.0, 20.0, 0.0,//dist, theta, phi
	50.0, 1.0, 100.0//fovY,nearZ, farZ
};
View view0 = view;

//Windowのサイズ
int width = 500;
int height = 500;
//アフィン変換
enum SELECT_KEY {LIGHT};
SELECT_KEY sKey = LIGHT;
//マウス操作
int xStart, yStart;
bool flagMouse = false;
//ﾜｲﾔｰﾌﾚｰﾑ<-->ｿﾘｯﾄﾞ切り替えフラグ
bool flagWireframe = false;
//ヘルプキーフラグ
bool flagHelp = false;
//光源位置変更フラグ
bool flagLight = false;//追加
//影のフラグ
bool flagShadow = false;
//オブジェクト識別番号
int objNo = 0;
//ﾃｸｽﾁｬｰ画像
#define TEX_WIDTH 128
#define TEX_HEIGHT 128
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][3];
//経過時間
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
float amp = 1.0;
//アニメーション
#include "slFish1.h"
CFish1 fish1 ;
#include "slFish2.h"
CFish2 fish2 ;

GLfloat fov = 60.0;//透視投影マッピングの視野角
//シェーダプログラム名
GLuint shaderProg;


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
  glutCreateWindow("集光模様1(SL_Caustics1)");
  //ウィンドウのサイズ変更
  glutReshapeFunc(resize);
  //表示
  glutDisplayFunc(display);
  //キーボードの利用
  glutKeyboardFunc(keyboard);
  //矢印キーなどの特殊キー利用
  glutSpecialFunc(special);
  //マウス操作
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  //再描画
  glutIdleFunc(idle);
  //初期設定
  init();
  initGlsl(&shaderProg, "projection.vert", "projection.frag");
  //イベント処理ループに入る
  glutMainLoop();
	glDeleteProgram(shaderProg);
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

  setCamera();//視点を求める
  setLight(); //光源設定　
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("マウス/キー操作の説明には'h'キーをプッシュ \n");

	//時間関係
  lastTime = timeGetTime();
  fps = 0;
  elapseTime1 = 0.0;//1sec間以内の経過時間
  elapseTime2 = 0.0; //init()後の総経過時間

	makeWavePlane(amp, elapseTime2);
	makeTexImage();
	setTexture();
}

void makeTexImage()
{
  int i, j;
	float v, a;

  for(j = 0; j < TEX_HEIGHT; j++)
		for(i = 0; i < TEX_WIDTH; i++)
    {
			v = data[j * (nMesh + 1) + i];
			if(kindWave == 0) a = 180.0 + 75.0 * v ;
			else              a = 250.0 - 100.0 * v*v  ;
			if(a >= 255.0) a = 255.0;
			if(a < 120) a = 120;
 			texImage[j][i][0] = texImage[j][i][1] = texImage[j][i][2] = (GLubyte)a;
    }
}

void setTexture()
{
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,texImage);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void display(void)
{
 	//時間計測
  curTime = timeGetTime();
  float dt = (float)(curTime - lastTime) * 0.001;//secに変換
  elapseTime1 += dt;
  elapseTime2 += dt;
  fps ++;
		printf("elapseTime2 = %f \n", elapseTime2);
  if(elapseTime1 >= 1.0)
  {
		printf("frame per sec = %d \n", fps);
		elapseTime1 = 0.0;
		fps = 0;
  }
  lastTime = curTime;

	//波データを作成し、投影マップを設定
	if(kindWave == 0) makeWaveCircle(amp, elapseTime2);
	else if(kindWave == 1) makeWavePlane(amp, elapseTime2);
	else makeWaveMix(amp, elapseTime2);
	makeTexImage();
	setTexture();

  if(flagWireframe)//'w'でwireframeとsolid model切り替え
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	resize(width, height);
	//カラーバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(cos(M_PI * view.theta /180.0) > 0.0)//カメラ仰角90度でﾋﾞｭｰｱｯﾌﾟﾍﾞｸﾄﾙ切替
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //光源設定//'l'を押した後光源位置可変
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  //fragment shaderのユニフォーム変数インデックスを取得
  GLint texLoc = glGetUniformLocation(shaderProg, "smplCaustics");
  glUniform1i(texLoc, 0);//GL_TEXTURE0を適用
  // シェーダプログラムの適用 
  glUseProgram(shaderProg);

  //描画
	fish1.motion1(elapseTime2);
	fish2.motion1(elapseTime2);
	drawFloor0(10.0, 10.0, 10, 10);

  // シェーダプログラムの適用を解除	
  glUseProgram(0);

	//ﾃｸｽﾁｬ、半透明物体があるとき
	glDepthMask(GL_FALSE); //ﾃﾞﾌﾟｽﾊﾞｯﾌｧを書き込み禁止
	glEnable(GL_BLEND);//ｱﾙﾌｧﾌﾞﾚﾝﾃﾞｨﾝｸﾞを有効にする
	glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//色混合係数を決める

	//半透明描画
	drawWave();
	//ﾃｸｽﾁｬ、半透明物体があるとき
	glDepthMask(GL_TRUE); //ﾃﾞﾌﾟｽﾊﾞｯﾌｧの書き込みを許可
	glDisable(GL_BLEND);
	
  //影
  drawShadow();

	drawLight();

  if(flagHelp)
  {
		printf("矢印キーによる光源/投影中心移動 \n");
		printf("　→,←：x軸 \n");
		printf("　↑,↓：y軸 \n");
		printf("　[Shift]＋↑,↓：z軸 \n");
		printf("　'l'を押した後、光源位置の移動可 \n");
		printf("'w'でﾜｲﾔｰﾌﾚｰﾑとｿﾘｯﾄﾞﾓﾃﾞﾙ切り替え \n");
		printf("マウス操作で視点変更可 \n");
		printf("  dolly：中央付近を左ボタンクリックで近づき，右ボタンクリックで遠ざかる \n");
		printf("  pan：左横および右横を右ボタンクリックで注視点が左右に変化する \n");
		printf("  tilt：真上および真下を右ボタンクリックで注視点が上下に変化する \n");
		printf("  tumble：左右にドラッグすると視点が左右に変化する \n");
		printf("  crane：上下にドラッグすると視点が上下に変化する \n");
		printf("  zoom：左下を右ボタンクリックでズームイン \n");
		printf("        右下を右ボタンクリックでズームアウト \n");
		printf("[Shift]+'r'でリセット \n");
		printf("[F1]キー：投影マッピングの視野角fovの増減 \n");
		printf("[F2]キー：波の振幅amp \n");
		printf("[F3]キー：波の種類（円形波と平面波）の切り替え \n");
		printf("[F4]キー：円形波の個数切り替え \n");
		flagHelp = false;
  }
  //終了
  glutSwapBuffers();
}

void setTextureMatrix()
{
 //テクスチャ変換行列を設定する 
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  //正規化座標の [-1,1] の範囲をテクスチャ座標の範囲 [0,1] に変換 
  glTranslatef(0.5, 0.5, 0.5);
  glScalef(0.5, 0.5, 0.5);
  gluPerspective(fov, 1.0, 0.1, 50.0);
	//投影中心=光源位置
	gluLookAt(lightPos[0], lightPos[1], lightPos[2], 0.0, waveH, 0.0, 0.0, 0.0, -1.0);
}

void drawFloor0(float widthX, float widthZ, int nx, int nz)
{
  setTextureMatrix();
  int i, j;
  //Floor１枚当たりの幅
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.7, 0.7, 0.6, 1.0}, { 0.3f, 0.4, 0.4, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
  glNormal3f(0.0, 1.0, 0.0);
  glPushMatrix();
  for (j = 0; j < nz; j++) 
  {
    float z1 = -widthZ / 2.0 + wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = -widthX / 2.0 + wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glBegin(GL_QUADS);
      glVertex3f(x1, 0.0, z1);
      glVertex3f(x1, 0.0, z2);
      glVertex3f(x2, 0.0, z2);
      glVertex3f(x2, 0.0, z1);
			glEnd();
    }
  }
  glPopMatrix();
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
  //表示ウィンドウのサイズ
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
  float lightAmbient0[] = {0.5, 0.5, 0.5, 1.0}; //環境光
  float lightDiffuse0[] = {1.0, 1.0, 1.0, 1.0}; //拡散光
  float lightSpecular0[] = {1.0, 1.0, 1.0, 1.0};//鏡面光
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
	case 'w':
		if(flagWireframe) flagWireframe = false;
		else if(!flagWireframe) flagWireframe = true;
		break;
  case 'R'://reset（初期値に戻す)
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
  if(sKey == LIGHT)
  {
		float d = 1.0;
	  switch( key )
	  {
	  case GLUT_KEY_RIGHT://x方向
		  lightPos[0] += d;
		  break;
	  case GLUT_KEY_LEFT:
		  lightPos[0] -= d;//-x方向
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) lightPos[1] += d;//y方向
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] -= d;//z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1] -= d;//-y方向
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] += d;//-z方向
		  break;
	  default:
		  break;
	  }
  }
	if(key == GLUT_KEY_F1)
	{
		if(glutGetModifiers() == 0) fov += 1.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fov -= 1.0;
		if(fov < 5.0) fov = 5.0;
		printf("fov = %f \n", fov);
	}
	if(key == GLUT_KEY_F2)
	{
		if(glutGetModifiers() == 0) amp += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp -= 0.1;
		if(amp < 0.0) amp = 0.0;
		printf("amp = %f \n", amp);
	}
	if(key == GLUT_KEY_F3)
	{
		kindWave ++;
		if(kindWave == 3) kindWave = 0;
		printf("kindWave = %d \n", kindWave);
	}
	if(key == GLUT_KEY_F4)
	{
		numWave ++;
		if(numWave >= 4) numWave = 1;
		printf("numWave = %d \n", numWave);
	}
}

//以下の３個の関数はマウス操作による視点の変更に必要
void mouse(int button, int state, int x, int y)
{
  double pp = M_PI / 180.0;

  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
	  xStart = x; yStart = y;
	  flagMouse = true;
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist -= 0.5; //近づく
	  }
  }
  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist += 0.5; //遠ざかる
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
}


//次の2つは影描画に必要
//------------------------------------------------------------------------
void CalcShadowMat(int , float* mat)
{
  float ex, ey, ez;//光源の方向
  float a, b, c, d;//床の面のパラメータ
  float s; //object中心から光源までの距離
  float x, y, z;

	if(objNo == 1)
	{
		x = lightPos[0] - fish1.pos[0];
		y = lightPos[1] - fish1.pos[1];
		z = lightPos[2] - fish1.pos[2];
	}
	else
	{
		x = lightPos[0] - fish2.pos[0];
		y = lightPos[1] - fish2.pos[1];
		z = lightPos[2] - fish2.pos[2];
	}

  //光源の方向ベクトル
  s = sqrt(x * x + y * y + z * z);
  ex = x / s;
  ey = y / s;
  ez = z / s;

  //フロアの方向ベクトル(ｙ方向）
  a = 0.0;
  b = 1.0;
  c = 0.0;
  d = -0.001; //フロアと影の干渉を防ぐため
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
  //計算は平行光線に対する影
  //ｵﾌﾞｼﾞｪｸﾄごとに光源の方向を変え、疑似的に点光源に対する影を作る
  float mat[16]; //影行列の要素

  flagShadow = true;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	  glDepthMask(GL_FALSE);
  //影の描画
  CalcShadowMat(1, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//マトリクスの乗算
	  fish1.draw();//ｵﾌﾞｼﾞｪｸﾄの描画
  glPopMatrix();
  CalcShadowMat(2, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//マトリクスの乗算
	  fish2.draw();//ｵﾌﾞｼﾞｪｸﾄの描画
  glPopMatrix();

  //影の描画終了
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  flagShadow = false;
}
