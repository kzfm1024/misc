/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

雑音関数の3Dテクスチャマッピング
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "../../noise2.h"

//関数のプロトタイプ宣言
void init();
void idle();
void display();
void draw0();
void draw1();
void drawFloor0(float widthX, float widthZ, int nx, int nz);
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
void setTexture3D();
void makeTexImage3D();

//affine変換用変数
float pos[][3] = { //位置（平行移動）
	{ 0.7, 1.0, 0.0}, //obj0
	{-0.7, 1.0, 0.0}  //obj1
};

float pos0[][3] = {//初期値
	{ 0.7, 1.0, 0.0},//obj0
	{-0.7, 1.0, 0.0} //obj1
};

float scale[][3] = {//サイズ（スケーリング）
	{ 1.0, 1.0, 1.0 },//obj0
	{ 1.0, 1.0, 1.0 } //obj1
};
float scale0[][3] = {//初期値
	{ 1.0, 1.0, 1.0 }, //obj0
	{ 1.0, 1.0, 1.0 }  //obj1
};

float angle[][3] = {//姿勢（回転）
	{ 0.0, 0.0, 0.0}, //obj0
	{ 0.0, 0.0, 0.0}  //obj1
};
float angle0[][3] = {//初期値
	{ 0.0, 0.0, 0.0}, //obj0
	{ 0.0, 0.0, 0.0}  //obj1
};

//光源
float lightPos[] = {10.0, 15.0, 10.0, 1.0};//光源位置
float lightPos0[] = {10.0, 15.0, 10.0, 1.0};//光源位置(初期値)
//影のマテリアル
float shadowDiffuse[] =  {0.0,0.0,0.0,0.3};//影の拡散光
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
	6.0, 30.0, 20.0,//dist, theta, phi
	30.0, 1.0, 100.0//fovY,nearZ, farZ
};
View view0 = view;

//Windowのサイズ
int width = 500;
int height = 500;
//アフィン変換
enum SELECT_KEY {ROTATE, SCALE, TRANSLATE, LIGHT};
SELECT_KEY sKey = TRANSLATE;
//マウス操作
int xStart, yStart;
bool flagMouse = false;
//ヘルプキーフラグ
bool flagHelp = false;
//光源位置変更フラグ
bool flagLight = false;//追加
//影のフラグ
bool flagShadow = false;
//オブジェクト識別番号
int objNo = 0;
//回転アニメーション
float ang = 0.0;
float dang = 0.1;//角度増分

//ﾃｸｽﾁｬ用
GLubyte texImage[NOISE_SIZE][NOISE_SIZE][NOISE_SIZE][4];
float scaleTex = 0.5;
float thUpper = 0.9;
float thLower = 0.3;
float amp = 1.0;
enum Coord { OBJECT, VIEW};
Coord coord = OBJECT;
//シェーダプログラム名
GLuint shaderProg;

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  //表示モード
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  //表示ウィンドウのサイズ
  glutInitWindowSize(width, height);
  //左上の位置
  glutInitWindowPosition(100, 100);
  //ウィンドウ作成
  glutCreateWindow("雑音3D2B(SL_Noise3D2B)");
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
	glewInit();
  init();
  initGlsl(&shaderProg, "noise3D2B.vert", "noise3D2B.frag");
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
  printf("ノイズ関数を計算しています \n");
  //３DテクスチャーのImageをあらかじめ作成
  makeTexImage3D();
  setTexture3D();
}

void setTexture3D()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage3D(GL_TEXTURE_3D,0,GL_RGBA,NOISE_SIZE,NOISE_SIZE,NOISE_SIZE,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_WRAP_R,GL_REPEAT);

  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_3D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_3D, 0);
}

void makeTexImage3D()
{
  CNoise3 noise3;
  int i, j, k;
  	
  for (k = 0; k < NOISE_SIZE; k++) 
  {
		for (j = 0; j < NOISE_SIZE; j++) 
		{
			for (i = 0; i < NOISE_SIZE; i++) 
			{
				noise3.calc(i, j, k);
				texImage[k][j][i][0] = (GLubyte)((1.0 + noise3.val[0]) * 127.5);
				texImage[k][j][i][1] = (GLubyte)((1.0 + noise3.val[1]) * 127.5);
				texImage[k][j][i][2] = (GLubyte)((1.0 + noise3.val[2]) * 127.5);
				texImage[k][j][i][3] = (GLubyte)((1.0 + noise3.val[3]) * 127.5);
			}
		}
  } 
}

void display(void)
{
 //時間計測
  static double time1, time2, drawTime, frame;
  if(ang <= 0.001) time1 = timeGetTime();

  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();//視点を変えるときはこの位置に必要
  if(cos(M_PI * view.theta /180.0) >= 0.0)//カメラ仰角90度でﾋﾞｭｰｱｯﾌﾟﾍﾞｸﾄﾙ切替
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //光源設定//'l'を押した後光源位置可変
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glActiveTexture(GL_TEXTURE0);
  // シェーダプログラムの適用 
  glUseProgram(shaderProg);
	GLint scaleLoc = glGetUniformLocation(shaderProg, "scale");
	glUniform1f(scaleLoc, scaleTex);
	GLint thUpperLoc = glGetUniformLocation(shaderProg, "thUpper");
	glUniform1f(thUpperLoc, thUpper);
	GLint thLowerLoc = glGetUniformLocation(shaderProg, "thLower");
	glUniform1f(thLowerLoc, thLower);
	GLint ampLoc = glGetUniformLocation(shaderProg, "amp");
	glUniform1f(ampLoc, amp);
	GLint coordLoc = glGetUniformLocation(shaderProg, "coord");
	if(coord == OBJECT) glUniform1i(coordLoc, 0);
	else glUniform1i(coordLoc, 1);
  GLint texLoc = glGetUniformLocation(shaderProg, "smpl3D");
  glUniform1i(texLoc, 0);//GL_TEXTURE0を適用
  draw0(); 
  draw1();
  // シェーダプログラムの適用を解除	
  glUseProgram(0);
  drawFloor0(10.0, 10.0, 10, 10);
  //影
  drawShadow();

  //回転角度更新
  ang += dang;
  if(ang >= 360.0)
  {
	time2 = timeGetTime();
	drawTime = (time2 - time1) * 0.001;//１回転当たりの描画時間
	printf("描画時間 = %4.4f[s] \n", drawTime);
	frame = (360.0 / dang) / drawTime;
	printf("フレーム数 = %4.4f[fps] \n", frame);
	ang = 0.0;
  }

  if(flagHelp)
  {
	printf("矢印キーによるアフィン変換/光源移動 \n");
	printf("　→,←：x軸 \n");
	printf("　↑,↓：y軸 \n");
	printf("　[Shift]＋↑,↓：z軸 \n");
	printf("　'r'を押した後:回転 \n");
	printf("　't'を押した後:平行移動 \n");
	printf("　's'を押した後:ｽｹｰﾘﾝｸﾞ \n");
	printf("　'l'を押した後、光源位置の移動可 \n");
	printf("マウス操作で視点変更可 \n");
	printf("  dolly：中央付近を左ボタンクリックで近づき，右ボタンクリックで遠ざかる \n");
	printf("  pan：左横および右横を右ボタンクリックで注視点が左右に変化する \n");
	printf("  tilt：真上および真下を右ボタンクリックで注視点が上下に変化する \n");
	printf("  tumble：左右にドラッグすると視点が左右に変化する \n");
	printf("  crane：上下にドラッグすると視点が上下に変化する \n");
	printf("  zoom：左下を右ボタンクリックでズームイン \n");
	printf("        右下を右ボタンクリックでズームアウト \n");
	printf("[Shift]+'r'でリセット \n");
	printf("[F1]キー：テクスチャの倍率調整 \n");
	printf("[F2]キー：座標系の切り替え \n");
	printf("[F3]キー：上方しきい値調整 \n");
	printf("[F4]キー：下方しきい値調整 \n");
	printf("[F5]キー：ノイズ振幅調整 \n");
	flagHelp = false;
  }
  //終了
  glutSwapBuffers();
}

void draw0(void)
{
  float ambient[] = { 0.4, 0.4, 0.1, 1.0};
  float diffuse[] = { 0.8, 0.8, 0.2, 1.0};
  float specular[]= { 0.9, 0.9, 0.9, 1.0};

	if(flagShadow) 
  {
	  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
  }
  else
  {	
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100);
  }

  glPushMatrix();
	glTranslatef(pos[0][0], pos[0][1], pos[0][2]);
  glRotatef(angle[0][2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[0][1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[0][0] + ang, 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0][0], scale[0][1], scale[0][2]);
//	glutSolidCube(1.0);
//	glutSolidSphere(0.5, 20, 20);
	glutSolidTorus(0.2, 0.5, 10, 10);
  glPopMatrix();
}

void draw1(void)
{
  float ambient[] = { 0.2, 0.4, 0.4, 1.0};
  float diffuse[] = { 0.4, 0.8, 0.8, 1.0};
  float specular[]= { 0.8, 0.8, 0.8, 1.0};

	if(flagShadow) 
  {
	  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
  }
  else
  {	
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100);
  }

  glPushMatrix();
	glTranslatef(pos[1][0], pos[1][1], pos[1][2]);
  glRotatef(angle[1][2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[1][1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[1][0] + ang, 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[1][0], scale[1][1], scale[1][2]);
//glutSolidCube(1.0);  //オブジェクト形状
//	glutSolidSphere(0.5, 20, 20);
//	glutSolidTorus(0.2, 0.5, 10, 10);
//	glutSolidTeapot(0.7);

  //回転体1
  static float cs1[][2] = {
	  {0.0, 0.5}, {0.3, 0.6}, {0.4, 1.0}, {0.5, 1.0}, {0.4, 0.6},
	  {0.1, 0.4}, {0.1, 0.1}, {0.5, 0.1}, {0.5, 0.0}, {0.0, 0.05}
  };
  drawRevolution1(cs1, 10, 20);

  glPopMatrix();
}

void drawFloor0(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor１枚当たりの幅
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.7, 0.7, 0.6, 1.0f}, { 0.3f, 0.4, 0.4, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3d(0.0, 1.0, 0.0);
  glPushMatrix();
  for (j = 0; j < nz; j++) {
    float z1 = -widthZ / 2.0 + wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = -widthX / 2.0 + wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glBegin(GL_QUADS);
      glVertex3d(x1, 0.0, z1);
      glVertex3d(x1, 0.0, z2);
      glVertex3d(x2, 0.0, z2);
      glVertex3d(x2, 0.0, z1);
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
		for(int j = 0; j < 2; j++)
			for(int i = 0; i < 3; i++)
			{
				pos[j][i] = pos0[j][i]; scale[j][i] = scale0[j][i]; angle[j][i] = angle0[j][i];
			}
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
  case '0':
	  objNo = 0;
	  break;
  case '1':
	  objNo = 1;
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
		  pos[objNo][0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://-x方向
		  pos[objNo][0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) pos[objNo][1] += 0.1;//y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[objNo][2] -= 0.1; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) pos[objNo][1] -= 0.1;//-y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) pos[objNo][2] += 0.1; //-z方向
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
		  angle[objNo][0] += 10.0;
		  break;
	  case GLUT_KEY_LEFT://-x軸回転
		  angle[objNo][0] -= 10.0;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) angle[objNo][1] += 10;//y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[objNo][2] += 10; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) angle[objNo][1] -= 10;//-y方向
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) angle[objNo][2] -= 10; //-z方向
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
		  scale[objNo][0] += 0.1;
		  break;
	  case GLUT_KEY_LEFT://x方向縮小
		  scale[objNo][0] -= 0.1;
		  break;
	  case GLUT_KEY_UP:
		  if(glutGetModifiers() == 0) scale[objNo][1] += 0.1;//y方向拡大
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[objNo][2] += 0.1; //z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) scale[objNo][1] -= 0.1;//y方向縮小
		  else if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scale[objNo][2] -= 0.1; //z方向
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
	  if(glutGetModifiers() == 0) scaleTex += 0.01;//-y方向
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) scaleTex -= 0.01;//z方向
		printf("scaleTex = %f \n", scaleTex);
//		init();
	}  
	if(key == GLUT_KEY_F2)
	{
		if(coord == OBJECT) coord = VIEW;
		else coord = OBJECT;
	}
	if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == 0) thUpper += 0.02;
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) thUpper -= 0.02;
		if(thUpper > 1.0) thUpper = 1.0;
		if(thUpper < thLower) thUpper = thLower;
		printf("thUpper = %f \n", thUpper);
	}  
	if(key == GLUT_KEY_F4) 
  {
	  if(glutGetModifiers() == 0) thLower += 0.02;
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) thLower -= 0.02;
		if(thLower > thUpper) thLower = thUpper;
		if(thUpper < 0.0) thLower = 0.0;
		printf("thLower = %f \n", thLower);
	}  
	if(key == GLUT_KEY_F5) 
  {
	  if(glutGetModifiers() == 0) amp += 0.02;
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp -= 0.02;
		if(amp < 0.0) amp = 0.0;
		printf("amp = %f \n", amp);
	}  
	if(key == GLUT_KEY_PAGE_UP) 
	{
		dang += 0.1;
  }	
	if(key == GLUT_KEY_PAGE_DOWN) 
	{
		dang -= 0.1;
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
	resize(width, height);
}

//次の2つは影描画に必要
//------------------------------------------------------------------------
void CalcShadowMat(int ID, float* mat)
{
  float ex, ey, ez;//光源の方向
  float a, b, c, d;//床の面のパラメータ
  float s; //object中心から光源までの距離
  float x, y, z;

  x = lightPos[0] - pos[ID][0];
  y = lightPos[1] - pos[ID][1];
  z = lightPos[2] - pos[ID][2];

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
  CalcShadowMat(0, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//マトリクスの乗算
	  draw0();//ｵﾌﾞｼﾞｪｸﾄの描画
  glPopMatrix();
  CalcShadowMat(1, mat);
  glPushMatrix();
	  glMultMatrixf(mat);//マトリクスの乗算
	  draw1();//ｵﾌﾞｼﾞｪｸﾄの描画
  glPopMatrix();

  //影の描画終了
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  flagShadow = false;
}


