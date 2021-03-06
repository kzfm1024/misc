/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

変位マッピング1
OpenGL+GLSL
*/
#include <windows.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../myGlsl.h"
#include "../../imageLoadSave.h"

//関数のプロトタイプ宣言
void init();
void idle();
void display();
void draw0();
void draw1();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void CalcShadowMat(int objNo,float* mat);
void drawShadow();
void makeTexture();
void setNormalMap(int no);
void makeNormalMap(Bitmap *bmp);
void drawLight();
//バンプマッピング用プリミティブ
void drawDispPlate(float size, int nDiv, int nRepeatS, int nRepeatT);
void drawDispCube(float size, int nDiv, int nRepeatS, int nRepeatT);
void drawBumpSphere(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT);
void drawBumpTorus(double radius1, double radius2, int nSide, int nRing, int nRepeatS, int nRepeatT);

//affine変換用変数
float pos[][3] = { //位置（平行移動）
	{ 0.75, 1.0, 0.0},//obj0
	{-0.75, 1.0, 0.0} //obj1
};

float pos0[][3] = {//初期値
	{ 0.75, 1.0, 0.0},//obj0
	{-0.75, 1.0, 0.0} //obj1
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
	{ 0.0, 0.0, 0.0},  //obj0
	{ 0.0, 0.0, 0.0}   //obj1
};

//光源
float lightPos[] =  {10.0, 15.0, 10.0, 1.0};//光源位置
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
	7.0, 30.0, 20.0,//dist, theta, phi
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
//ﾜｲﾔｰﾌﾚｰﾑ<-->ｿﾘｯﾄﾞ切り替えフラグ
bool flagWireframe = false;//追加
//影のフラグ
bool flagShadow = false;
//オブジェクト識別番号
int objNo = 0;
//回転アニメーション
float ang = 0.0;
float dang = 0.1;//角度増分
//texture
#define TEX_WIDTH 256
#define TEX_HEIGHT 256
GLubyte normalMap[TEX_HEIGHT][TEX_WIDTH][3];
GLuint texName[2];
//シェーダプログラム名
GLuint shaderProg;

float inv = 1.0;//凹凸反転
float fGrad = 5.0;//勾配強調
float fDisp = 0.05;//変位量調整ファクタ

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
  glutCreateWindow("変位マッピン1(SL_Displacement1)");
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
  initGlsl(&shaderProg, "displacement1.vert", "displacement1.frag");
  //イベント処理ループに入る
  glutMainLoop();
	//リソースの解放
	glDeleteTextures(2, texName);
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

  //テクスチャー作成
  glGenTextures(2, texName);//テクスチャオブジェクトの名前付け
	makeTexture();
}

void makeTexture()
{
  Bitmap* bm0 = new Bitmap();
//  loadBitmap(bm0, "../../bmp256/char256-1.bmp");
  loadBitmap(bm0, "../../bmp256/disk4-256.bmp");
  makeNormalMap(bm0);
  setNormalMap(0);
  //obj1用
  Bitmap* bm1 = new Bitmap();
//  loadBitmap(bm1, "../../bmp256/char256-2.bmp");
  loadBitmap(bm1, "../../bmp256/disk4-256.bmp");
  makeNormalMap(bm1);
  setNormalMap(1);
}

void makeNormalMap(Bitmap *bmp)
{//法線マップ用配列作成
  int i, j, ip, jp;
  float r, g;

  if(bmp->bi.Width != TEX_WIDTH || bmp->bi.Height != TEX_HEIGHT)
  {
		printf("TEX_WIDTH,TEX_HEIGHTとファイルサイズを一致させてください\n");
		return;
  }
  //BMPファイルから高さマップをロード
  for(j = 0; j < TEX_HEIGHT; j++)
  {
		for(i = 0; i < TEX_WIDTH; i++)
		{
      //高度マップを取得
			if(bmp->bi.BitCount <= 24)
			normalMap[j][i][2] = (bmp->pixel[i+j * TEX_WIDTH]).r;
			else//32bit color
			normalMap[j][i][2] = (bmp->pixelA[i+j * TEX_WIDTH]).r;
		}
  }
	if(bmp->bi.BitCount <= 24) free(bmp->pixel);
	else free(bmp->pixelA);

  //高さマップから法線マップを作成
  for(j = 0; j < TEX_HEIGHT; j++)
  {
		jp = j+1;
		if(jp >= TEX_HEIGHT) jp = 0;
		for(i = 0; i < TEX_WIDTH; i++)
		{
			ip = i+1;
			if(ip >= TEX_WIDTH) ip = 0;
			float deltaS = inv * (normalMap[j][ip][2] - normalMap[j][i][2]);
			float deltaT = inv * (normalMap[jp][i][2] - normalMap[j][i][2]);
			r = 127.5 + deltaS * fGrad;
			if(r < 0.0) r = 0.0;
			if(r > 255.0) r = 255.0;
			g = 127.5 + deltaT * fGrad;
			if(g < 0.0) g = 0.0;
			if(g > 255.0) g = 255.0;
			normalMap[j][i][0] = (GLubyte)r;
			normalMap[j][i][1] = (GLubyte)g;
			if(inv == 1.0) //反転なし
				normalMap[j][i][2] = (GLubyte)(normalMap[j][i][2]);
			else//if(inv == -1.0)//反転あり
				normalMap[j][i][2] = (GLubyte)(256 - normalMap[j][i][2]);
		}
  }
}

void setNormalMap(int No)
{
  glBindTexture(GL_TEXTURE_2D, texName[No]);//テクスチャをバインドする

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,normalMap);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);//GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);//LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);//LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);//バインドを解除
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

  if(flagWireframe)//'w'でwireframeとsolid model切り替え
  {
	  glPolygonMode(GL_FRONT,GL_LINE);
	  glPolygonMode(GL_BACK,GL_POINT);
  }
  else glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texName[1]);

  // シェーダプログラムの適用 
  glUseProgram(shaderProg);

	GLint dispLoc = glGetUniformLocation(shaderProg, "fDisp");
	glUniform1f(dispLoc, fDisp);
	GLint invLoc = glGetUniformLocation(shaderProg, "inv");
	glUniform1f(invLoc, inv);
  GLint texNormalLoc = glGetUniformLocation(shaderProg, "normalMap");
  glUniform1i(texNormalLoc, 0);//GL_TEXTURE0を適用
  draw0(); 
  glUniform1i(texNormalLoc, 1);//GL_TEXTURE1を適用
  draw1();
  // シェーダプログラムの適用を解除	
  glUseProgram(0);
  
  drawFloor(10.0, 10.0, 10, 10);
  //影
  drawShadow();

	drawLight();

  //回転角度更新
  ang += dang;
  if(ang >= 360.0)
  {
		time2 = timeGetTime();
		//１回転当たりの描画時間
		drawTime = (time2 - time1) * 0.001;
		frame = (360.0 / dang) / drawTime;
		printf("フレーム数 = %4.4f[fps] \n", frame);
		//フレームあたりの描画時間
		drawTime = 1.0 / frame;
		printf("描画時間 = %4.4f[spf] \n", drawTime);		
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
		printf("[F1]キー：凹凸反転 \n");
		printf("[F2]キー：勾配強調 \n");
		printf("[F3]キー：頂点座標変位量調整 \n");
		printf("[Page Up]キー：回転角度+0.1 \n");
		printf("[Page Dn]キー：回転角度-0.1 \n");
		flagHelp = false;
  }
  //終了
  glutSwapBuffers();
}

void draw0(void)
{
  float ambient[] = { 0.3, 0.3, 0.3, 1.0};
  float diffuse[] = { 0.7, 0.7, 0.7, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

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
  //オブジェクト形状
  if(!flagShadow)
  {
//  drawDispPlate(1.0, 100, 1, 1);
//	drawDispCube(1.0, 100, 1, 1);
		drawBumpTorus(0.25, 0.45, 200, 200, 16, 4);
  }
  else
  {//影表示のときはTextureを付けない
// drawPlate(1.0);
// drawCube(1.0);
		drawTorus(0.25, 0.45, 10, 10);
  }

  glPopMatrix();
}

void draw1(void)
{
  float ambient[] = { 0.1, 0.3, 0.4, 1.0};
  float diffuse[] = { 0.2, 0.6, 0.6, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

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
  //オブジェクト形状
  if(!flagShadow)
  {
	  drawBumpSphere(0.7, 200, 200, 8, 4);
//	drawDispCube(1.0, 100, 1, 1);
  }
  else
  {
	  drawSphere(0.5, 10, 10);
//	drawCube(1.0);
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
  case 'w':
	  if(flagWireframe) flagWireframe = false;
	  else if(!flagWireframe) flagWireframe = true;
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
		if(inv == 1.0) inv = -1.0; 
		else inv = 1.0;
		makeTexture();
  }	
  if(key == GLUT_KEY_F2) 
  {
		if(glutGetModifiers() == 0) fGrad += 1.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fGrad -= 1.0;
		if(fGrad < 1.0) fGrad = 1.0;
		printf("fGrad = %f \n", fGrad);
		makeTexture();
  }
  if(key == GLUT_KEY_F3) 
  {
		if(glutGetModifiers() == 0) fDisp += 0.01;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fDisp -= 0.01;
		if(fDisp < 0.0) fDisp = 0.0;
		printf("fDisp = %f \n", fDisp);
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

//------------------------------------------------------------------------------
//ﾊﾞﾝﾌﾟﾏｯﾋﾟﾝｸﾞ(PlateとCubeは変位ﾏｯﾋﾟﾝｸﾞ用に変更）
void drawDispPlate(float size, int nDiv, int nRepeatS, int nRepeatT)
{//x-y平面
	float tnt[3];
	float s, t;
	float q[4][3];//平面上の分割頂点
	float s0, s1, t0, t1, d0, d1;
	int i, j;
	float sz = 0.5 * size;
	static float p[4][3] = //z:上方向
	{ 
		{ sz, sz, 0.0}, {-sz, sz, 0.0}, 
		{-sz,-sz, 0.0}, { sz,-sz, 0.0}
	};

	s = (float)nRepeatS;
	t = (float)nRepeatT;
  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");
  glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0, 0.0, 1.0); //z方向の法線
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z座標
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[0][2];
		//y座標
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;
    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x座標
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawDispCube(float size, int nDiv, int nRepeatS, int nRepeatT)
{
	float sz, s, t, tnt[3];
	float q[4][3];//平面上の分割頂点
	float s0, s1, t0, t1, d0, d1;
	int i, j;

	sz = 0.5 * size;
  float p[8][3] =
  { { sz, sz, sz}, {-sz, sz, sz}, {-sz,-sz, sz}, 
		{ sz,-sz, sz}, { sz, sz,-sz}, {-sz, sz,-sz},
		{-sz,-sz,-sz}, { sz,-sz,-sz}
  };

	s = (float)nRepeatS;
	t = (float)nRepeatT;
  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

  glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);

	//top(z軸が鉛直軸のときz方向）
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,1.0f);
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z座標
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[0][2];
		//y座標
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;
    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x座標
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//x方向(正面）
	tnt[0] = 0.0; tnt[1] = 0.0; tnt[2] =-1.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(1.0f,0.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//x座標
		q[0][0] = q[1][0] = q[2][0] = q[3][0] = p[0][0];
		//y座標
		q[0][1] = q[1][1] = p[3][1] + (p[0][1] - p[3][1]) * d0;
		q[2][1] = q[3][1] = p[3][1] + (p[0][1] - p[3][1]) * d1;

    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//z座標
			q[0][2] = q[3][2] = p[3][2] + (p[7][2] - p[3][2]) * d0;
			q[1][2] = q[2][2] = p[3][2] + (p[7][2] - p[3][2]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//y方向
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,1.0f,0.0f);
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//y座標
		q[0][1] = q[1][1] = q[2][1] = q[3][1] = p[0][1];
		//z座標
		q[0][2] = q[1][2] = p[0][2] + (p[4][2] - p[0][2]) * d0;
		q[2][2] = q[3][2] = p[0][2] + (p[4][2] - p[0][2]) * d1;
    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x座標
			q[0][0] = q[3][0] = p[1][0] + (p[0][0] - p[1][0]) * d0;
			q[1][0] = q[2][0] = p[1][0] + (p[0][0] - p[1][0]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-x方向
	tnt[0] = 0.0; tnt[1] = 0.0; tnt[2] = 1.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(-1.0f,0.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//x座標
		q[0][0] = q[1][0] = q[2][0] = q[3][0] = p[2][0];
		//y座標
		q[0][1] = q[1][1] = p[2][1] + (p[1][1] - p[2][1]) * d0;
		q[2][1] = q[3][1] = p[2][1] + (p[1][1] - p[2][1]) * d1;

    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//z座標
			q[0][2] = q[3][2] = p[5][2] + (p[1][2] - p[5][2]) * d0;
			q[1][2] = q[2][2] = p[5][2] + (p[1][2] - p[5][2]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-y方向
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,-1.0f,0.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//y座標
		q[0][1] = q[1][1] = q[2][1] = q[3][1] = p[2][1];
		//z座標
		q[0][2] = q[1][2] = p[6][2] + (p[2][2] - p[6][2]) * d0;
		q[2][2] = q[3][2] = p[6][2] + (p[2][2] - p[6][2]) * d1;
    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x座標
			q[0][0] = q[3][0] = p[2][0] + (p[3][0] - p[2][0]) * d0;
			q[1][0] = q[2][0] = p[2][0] + (p[3][0] - p[2][0]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
	//-z方向
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,-1.0f); 
	for(j = 0; j < nDiv; j++)
	{
		d0 = (float)j / (float)nDiv;
		d1 = (float)(j+1) / (float)nDiv;
		//z座標
		q[0][2] = q[1][2] = q[2][2] = q[3][2] = p[4][2];
		//y座標
		q[0][1] = q[1][1] = p[5][1] + (p[6][1] - p[5][1]) * d0;
		q[2][1] = q[3][1] = p[5][1] + (p[6][1] - p[5][1]) * d1;
    //t ﾃｸｽﾁｬ座標
		t0 = t * d0; t1 = t * d1;
		for(i = 0; i < nDiv; i++)
		{
			d0 = (float)i / (float)nDiv;
			d1 = (float)(i+1) / (float)nDiv;
			//x座標
			q[0][0] = q[3][0] = p[5][0] + (p[4][0] - p[5][0]) * d0;
			q[1][0] = q[2][0] = p[5][0] + (p[4][0] - p[5][0]) * d1;
			//s ﾃｸｽﾁｬ座標
			s0 = s * d0; s1 = s * d1;

			glTexCoord2f( s0, t0); glVertex3fv(q[0]);
			glTexCoord2f( s1, t0); glVertex3fv(q[1]);
			glTexCoord2f( s1, t1); glVertex3fv(q[2]);
			glTexCoord2f( s0, t1); glVertex3fv(q[3]);
		}
	}
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

//球のバンプマッピング用
void drawBumpSphere(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT)
{
	int i, j;
	double s, t0, t1, r0, r1, th0, th1, phi;
	double p[2][3], tnt[3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

	for(j = 0; j < nStack; j++)
	{
		//j=0は北極点(x=0, y=0, z=radius)
		//2つのt座標
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//これらの天頂角
		th0 = M_PI * t0;
		th1 = M_PI * t1;
		//x-y平面に投影した半径
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//頂点z座標
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);
		//接線ベクトルのｚ成分
		tnt[2] = 0.0;

		//北極点を1とするt座標
		t0 = (1.0 - t0) * nRepeatT;
		t1 = (1.0 - t1) * nRepeatT;

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s座標
			s = (double)i / (double)nSlice;
			phi = -M_PI + 2.0 * M_PI * s;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = r0 * cos(phi);//x座標
			p[0][1] = r0 * sin(phi);//y座標
			p[1][0] = r1 * cos(phi);//x座標
			p[1][1] = r1 * sin(phi);//y座標
			//接線ベクトルのxy成分
			tnt[0] =-sin(phi);
			tnt[1] = cos(phi);

			s *= nRepeatS;

			glVertexAttrib3dv(tangentLoc, tnt);
			glTexCoord2d(s, t0);//テクスチャ座標
			glNormal3dv(p[0]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[0]);//頂点座標

			glTexCoord2d(s, t1);//テクスチャ座標
			glNormal3dv(p[1]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}
}

void drawBumpTorus(double radius1, double radius2, 
									int nSide, int nRing, int nRepeatS, int nRepeatT)
{	
	//radius1:円環断面半径
	//radius2:円環の中心軸半径
	//nSide:円環断面における表面分割点数
	//nRing:円環中心軸に沿った分割点数

	int i, j;
	double rr, zz, z0, z1;
	double s0, s1, t, phi0, phi1, theta;
	double p[2][3], tnt[2][3];

  GLuint tangentLoc = glGetAttribLocation(shaderProg, "tangent");

	for(i = 0; i < nRing; i++)
	{
		//i=0は基本断面(x=radius2を中心とする円, y=0）
		//2つのｓ座標
		s0 = (double)i / (double)nRing;
		s1 = (double)(i+1) / (double)nRing;
		phi0 = 2.0 * M_PI * s0;
		phi1 = 2.0 * M_PI * s1;
		//接線ベクトルのxyｚ成分

		tnt[0][0] = - sin(phi0);
		tnt[0][1] =   cos(phi0);
		tnt[0][2] =   0.0;
		tnt[1][0] = - sin(phi1);
		tnt[1][1] =   cos(phi1);
		tnt[1][2] =   0.0;

		//s座標
		s0 *= (double)nRepeatS;
		s1 *= (double)nRepeatS;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			//t座標
			t = (double)j / (double)nSide;
			theta = -M_PI + 2.0 * M_PI * t;
			rr = radius2 + radius1 * cos(theta);//z軸からの距離
			zz = radius1 * sin(theta);
			//頂点のxyz座標(j=0を内側xy平面)
			p[0][0] = rr * cos(phi0);//x座標
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x座標
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z  

		  t *= (double)nRepeatT;

		glVertexAttrib3dv(tangentLoc, tnt[0]);
			glTexCoord2d(s0, t);//テクスチャ座標
			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//頂点座標

		glVertexAttrib3dv(tangentLoc, tnt[1]);
			glTexCoord2d(s1, t);//テクスチャ座標
			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

