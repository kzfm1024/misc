/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

OpenGL+GLSL
FFTにより海と雲
山岳地形と太陽
反射環境マッピング
*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myGlsl.h"
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../midpoint.h"
#include "terrain.h"
#include "../../fft.h"
#include "../../imageLoadSave.h"

//関数のプロトタイプ宣言
void init();
void idle();
void display();
void keyboard(GLubyte key, int x, int y);
void special(int key, int x, int y);
void resize(int w, int h);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void setCamera();
void setLight();
void makeTerrainImage(Bitmap *bmp);
void setTerrainTexture();
void makeCloudImage();
void setCloudTexture();
void drawWave();
void drawCloud();
void drawSun();
void makeSeaCloudData();
void setCubeMap();

//光源
float lightPos[]  = {0.0, 15.0, -20.0, 1.0};//光源位置
float lightPos0[] = {0.0, 15.0, -20.0, 1.0};//光源位置(初期値）

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
  0.0, 3.0, 0.0,//cnt 
	13.0, 5.0, 0.0,//dist, theta, phi
	80.0, 1.0, 100.0//fovY,nearZ, farZ
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
//ヘルプキーフラグ
bool flagHelp = false;
//光源位置変更フラグ
bool flagLight = false;//追加
//経過時間
double curTime, lastTime, elapseTime1, elapseTime2;
int fps = 0;//frame per sec
//terrainﾃｸｽﾁｬ用
#define TEX_WIDTH 64
#define TEX_HEIGHT 64
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][3];
GLuint texName[3];
//シェーダプログラム名
GLuint shaderProg1;
GLuint shaderProg2;
//cloud texture
#define texWidthC  128
#define texHeightC 128
GLubyte texCloud[texHeightC][texWidthC][4];
//海面・雲データ
#define nMesh 128 //最大分割数（両辺同じ）
float sizeX = 40.0;//16.0;
float sizeY = 25.0;//16.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//海面データ
float sr[nMesh*nMesh], si[nMesh*nMesh];
float tblSin[nMesh], tblCos[nMesh];//sin,cosﾃｰﾌﾞﾙ
float amp[nMesh * nMesh];
float phiSea[nMesh * nMesh];//海面の位相
float phiCloud[nMesh * nMesh];//雲の位相
float beta0 = 2.5;//fBmの指数β
float amp0 = 0.2;//基本波の振幅
float deltaWave = 0.2;
float deltaCloud = 0.05;
int K1 = 1 ;//kの低次側次数
int K2 = 20;//kの高次側次数
int L1 = 2; //lの低次側次数
int L2 = 10;//lの高次側次数
//FFTインスタンスを作成
CFFT fft = CFFT();
//反射環境マッピング用
#define texRefWidth  128
#define texRefHeight 128
//テクスチャー構造体
struct Target{
	GLenum name;//ターゲット名
	GLint x, y;//部分画像の位置
	GLdouble cx, cy, cz;//注視点
	GLdouble ux, uy, uz;//アップベクトル
};

struct Target target[] = {
  { //右
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    0, texRefHeight,
    1.0, 0.0, 0.0,
    0.0, -1.0, 0.0,
  },
  { 
	 //左
	 GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    0, 0,
    -1.0, 0.0, 0.0,
    0.0, -1.0, 0.0,
  },
  { //天井
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    texRefWidth, texRefHeight,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  },
  { 
		//床
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    texRefWidth,0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
  },
  { //手前
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    2*texRefWidth, texRefHeight,
    0.0, 0.0, 1.0,
    0.0, -1.0, 0.0,
  },
  { //奥
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    2*texRefWidth, 0,
    0.0, 0.0, -1.0,
    0.0, -1.0, 0.0,
  }
};
float reflectivity0 = 0.8;//垂直反射率
float posShip[] = { -10.0, 0.3, 5.0 };//船のxyz座標
float speedShip = 0.3;// 0.1;//船のスピード

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
  glutCreateWindow("シーン2(SL_Scene2)");
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
	initGlsl(&shaderProg1, "terrain.vert", "terrain.frag");
	initGlsl(&shaderProg2, "reflection.vert", "reflection.frag");
  //イベント処理ループに入る
  glutMainLoop();
	//リソースの解放
	glDeleteTextures(3, texName);
	glDeleteProgram(shaderProg1);
	glDeleteProgram(shaderProg2);
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
  glClearColor(0.3, 0.2, 0.2, 1.0);

  setCamera();//視点を求める
  setLight(); //光源設定　
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("マウス/キー操作の説明には'h'キーをプッシュ \n");

  glGenTextures(2, texName);//テクスチャオブジェクトの名前付け
	makeSeaCloudData();

	makeTerrainData();
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/altgrad1.bmp");
  makeTerrainImage(bm0);
  setTerrainTexture();
  //反射環境マッピング用テクスチャの設定
  setCubeMap();
	//計測時間のクリア
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
  lastTime = timeGetTime();
}

void makeSeaCloudData()
{
	//海面、雲データの作成（FFT)
	fft = CFFT(K1, K2, L1, L2, beta0, amp0, -1, nMesh);
  //sin,cosﾃｰﾌﾞﾙ
  fft.makeTable(tblSin, tblCos);
  //振幅と初期位相
  fft.makeAmpPhase(amp, phiSea);
  //雲の初期位相は海面と同じ
  for(int l = 0; l < nMesh; l++)
	 for(int k = 0; k < nMesh; k++) phiCloud[k+l*nMesh] = phiSea[k+l*nMesh];
}
//----------------------------------------------------------------
void makeCloudImage()
{
  int i, j;
  float c;

  for(j = 0; j < texHeightC; j++)
		for(i = 0; i < texWidthC; i++)
		{
			if(sr[i + j * texWidthC] > 0.0)
			{//cloud
				if(sr[i + j * texWidthC] > 1.0) sr[i + j * texWidthC] = 1.0;
				c = 1.0 - sr[i + j * texWidthC];
				if(c < 0.0) c = 0.0;
				texCloud[j][i][0] = (GLubyte) (200.0 * c);//red
				texCloud[j][i][1] = (GLubyte) (150.0 * c);//green
				texCloud[j][i][2] = (GLubyte) (100.0 * c);//blue
				texCloud[j][i][3] = (GLubyte) 100.0;      //alpha
			}
			else
			{//sky
				texCloud[j][i][0] = (GLubyte) 150;//red
				texCloud[j][i][1] = (GLubyte) 120;//green
				texCloud[j][i][2] = (GLubyte) 200;//blue
				texCloud[j][i][3] = (GLubyte) 0;  //alpha
			}
		}
}

void setCloudTexture()
{
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texName[2]);//テクスチャをバインドする
  //テクスチャの割り当て 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,texWidthC,texHeightC,0,GL_RGBA,GL_UNSIGNED_BYTE,texCloud);
  //テクスチャの繰り返し方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //テクスチャを拡大・縮小する方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //色の調整
  glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
  glBindTexture(GL_TEXTURE_2D, 0);
}

//画像ファイルからイメージデータを作成
void makeTerrainImage(Bitmap *bm)
{
  int i, j, ii, jj;

  if(bm->bi.Width < TEX_WIDTH) printf("画像ファイルのｻｲｽﾞ>=TEX_WIDTHとすること\n");

  for(j = 0; j < TEX_HEIGHT; j++)
  {
		jj = (int)((float)j * (float)bm->bi.Height / (float)TEX_HEIGHT);
		for(i = 0; i < TEX_WIDTH; i++)
    {
      ii = (int)((float)i * (float)bm->bi.Width / (float)TEX_WIDTH) ;
			if(bm->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bm->pixel[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixel[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixel[ii+jj*bm->bi.Width]).b;
			}
			else
			{
				texImage[j][i][0] = (bm->pixelA[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixelA[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixelA[ii+jj*bm->bi.Width]).b;
			}
    }
  }
	if(bm->bi.BitCount <= 24) free(bm->pixel);
	else free(bm->pixelA);
}

void setTerrainTexture()
{
  glBindTexture(GL_TEXTURE_2D, texName[0]);//テクスチャをバインドする
  //テクスチャの指定 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,texImage);
  //テクスチャの繰り返し方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //テクスチャを拡大・縮小する方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void setCubeMap()
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[1]);//テクスチャをバインドする
  for(int i = 0; i < 6; i++)
	glTexImage2D(target[i].name,0,GL_RGB,texRefWidth,texRefHeight,0,GL_RGB,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//バインドを解除
}

void display(void)
{
  curTime = timeGetTime();
  float dt = (float)(curTime - lastTime) * 0.001;//secに変換
  elapseTime1 += dt;
  elapseTime2 += dt;
  fps ++;
  if(elapseTime1 >= 1.0)
  {
		printf("frame per sec = %d \n", fps);
		elapseTime1 = 0.0;
		fps = 0;
  }
	lastTime = curTime;

  //step1
  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 透視変換行列の設定 (texture作成）
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.0, 1.0, 80.0);//視野角を90°
  
  // モデルビュー変換行列の設定 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

	//terrain shaderの準備
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  GLint texSampler = glGetUniformLocation(shaderProg1, "tex");
  glUniform1i(texSampler, 0);//TEXTURE0
  
  //鏡映視点から見たシーンをテクスチャに利用 
  for (int i = 0; i < 6; ++i) 
  {
    // ビューポートをテクスチャのサイズに設定する 
    glViewport(target[i].x, target[i].y, texRefWidth, texRefHeight);
		// 鏡映視点から見えるものをレンダリング 
		glPushMatrix();
		gluLookAt(view.pos[0], -view.pos[1], view.pos[2], 
			view.pos[0]+target[i].cx, -view.pos[1]+target[i].cy, view.pos[2]+target[i].cz, 
			target[i].ux, target[i].uy, target[i].uz);
		//海面以外をレンダリング
		glUseProgram(shaderProg1);
		drawTerrain();
		glUseProgram(0);
		drawSun();
		glPopMatrix();  
		// レンダリングした画像をテクスチャメモリに転送 
		glCopyTexSubImage2D(target[i].name, 0, 0, 0, target[i].x, target[i].y, texRefWidth, texRefHeight);
  }

  //step2:実際のオブジェクトをレンダリング
	resize(width, height);
  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if(cos(M_PI * view.theta /180.0) >= 0.0)//カメラ仰角90度でﾋﾞｭｰｱｯﾌﾟﾍﾞｸﾄﾙ切替
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);

  //光源設定//'l'を押した後光源位置可変
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  //描画
	glUseProgram(shaderProg1);
  drawTerrain();
  glUseProgram(0);

  //半透明物体があるときは次の2行が必要
  glCullFace(GL_BACK);   //裏面を削除
  glEnable(GL_CULL_FACE);//裏面削除を有効にする
	drawSun();
  //ﾃｸｽﾁｬ、半透明物体があるとき
  glDepthMask(GL_FALSE); //ﾃﾞﾌﾟｽﾊﾞｯﾌｧを書き込み禁止
  glEnable(GL_BLEND);//ｱﾙﾌｧﾌﾞﾚﾝﾃﾞｨﾝｸﾞを有効にする
  glBlendFunc(GL_DST_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//色混合係数を決める
  //半透明描画
  drawCloud();
  //ﾃｸｽﾁｬ、半透明物体があるとき
  glDepthMask(GL_TRUE); //ﾃﾞﾌﾟｽﾊﾞｯﾌｧの書き込みを許可
  glDisable(GL_BLEND);

	//step3
	//反射環境マッピング
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[1]);
  glUseProgram(shaderProg2);
  GLint refLoc = glGetUniformLocation(shaderProg2, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg2, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  //fragment shaderのユニフォーム変数texのインデックスを取得
  GLint texLoc = glGetUniformLocation(shaderProg2, "texCube");
  glUniform1i(texLoc, 0);//GL_TEXTURE0を適用

  drawWave(); //海面描画
  // シェーダプログラムの適用を解除	
  glUseProgram(0);


  if(flagHelp)
  {
		printf("矢印キーによるアフィン変換/光源移動 \n");
		printf("　→,←：x軸 \n");
		printf("　↑,↓：y軸 \n");
		printf("　[Shift]＋↑,↓：z軸 \n");
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
		printf("[F1]:k1, [F2]:k2, [F3]:L1, [F4]:L2の調整 \n");
		printf("[F5]:beta, [F6]:amp0, [F7]:deltaWave, [F8]:deltaCloud の調整\n");
		printf("[F9]:水面反射率reflectivity調整\n");
		printf(" \n");
		flagHelp = false;
  }

  //終了
  glutSwapBuffers();
}

void drawWave()
{
  float ambient[] = { 0.6f, 0.6f, 0.6f, 1.0f};
  float diffuse[] = { 0.8f, 0.8f, 1.0f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);//x軸回転
  //位相を変化
  for(int l = L1; l < L2; l++)
  for(int k = K1; k < K2 ; k++) phiSea[k + l* nMesh] += deltaWave;

  fft.makeFourier(amp, phiSea, sr, si);
  fft.ifft2d(sr, si, tblSin, tblCos);
  int i, j;
  for(j = 0; j < nMesh; j++) 
  {
	  for(i = 0; i < nMesh; i++) data[j * (nMesh+1) + i] = sr[j * nMesh + i] * (1.0-(float)j/(float)nMesh);
	  data[j * (nMesh+1) + nMesh] = sr[j * nMesh];//i=nMeshはi=0と同じとする
  }
  //j=nMeshのとき、j=0と同じとする
  for(i = 0; i <= nMesh; i++) data[nMesh * (nMesh + 1) + i] = 0.0;//data[i];

  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

void drawCloud()
{
  float ambient[] = { 0.6f, 0.5f, 0.4f, 1.0f};
  float diffuse[] = { 0.9f, 0.9f, 0.6f, 1.0f} ;
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  //位相を変化
  for(int l = L1; l < L2; l++)
  for(int k = K1; k < K2 ; k++) phiCloud[k + l* nMesh] += deltaCloud;
  fft.makeFourier(amp, phiCloud, sr, si);
  fft.ifft2d(sr, si, tblSin, tblCos);
  makeCloudImage();
  setCloudTexture();

  glPushMatrix();
  glTranslatef(0.0, 15.0, -sizeY / 2.0-8.0);
  glRotatef(0.0, 1.0, 0.0, 0.0);//x軸回転
  glScalef(sizeX+20, 30.0, 1.0);
  //オブジェクト形状
  drawTexPlate(1.0, 1, 1);
  glPopMatrix();
}

void drawSun()
{
  float diffuse[] = { 1.0f, 0.5f, 0.4f, 1.0f};
  float ambient[] = { 1.0f, 0.3f, 0.2f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glPushMatrix();
  glTranslatef(lightPos[0], lightPos[1] , lightPos[2] - 5.0);
  drawSphere(4.0, 20, 20);
  glPopMatrix();
}

void resize(int w, int h)
{
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(view.fovY, (double)w/(double)h, view.nearZ, view.farZ);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  width = w;
  height = h;
}

void setLight()
{
  float lightAmbient[] = {1.0f, 0.5f, 0.2f, 1.0f}; //環境光
  float lightDiffuse[] = {1.0f, 0.5f, 0.2f, 1.0f}; //拡散光
  float lightSpecular[] = {1.0, 0.5f, 0.2f, 1.0f}; //鏡面光
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
}

void keyboard(GLubyte key, int x, int y)
{
  switch((GLubyte)key)
  {
  case 27://Esc
	  exit(0);
	  break;
  case 'R'://reset
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
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] -= d;//-z方向
		  break;
	  case GLUT_KEY_DOWN:
		  if(glutGetModifiers() == 0) lightPos[1] -= d;//-y方向
		  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) lightPos[2] += d;//z方向
		  break;
	  default:
		  break;
	  }
  }

  //K1
  if(key == GLUT_KEY_F1) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K1 -= 1;
	  else K1 += 1;
	  if(K1 <= 1) K1 = 1;
	  if(K1 >= K2) K1 = K2 -1;
		printf("K1 = %d \n", K1);
	  makeSeaCloudData();
  }
  //K2
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K2 -= 1;
	  else K2 += 1;
	  if(K2 <= K1) K2 = K1 + 1;
	  if(K2 >= nMesh/2) K2 = nMesh/2 - 1;
		printf("K2 = %d \n", K2);
	  makeSeaCloudData();
  }
  //L1
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L1 -= 1;
	  else L1 += 1;
	  if(L1 <= 1) L1 = 1;
	  if(L1 >= L2) L1 = L2 -1;
		printf("L1 = %d \n", L1);
	  makeSeaCloudData();
  }
  //L2
  if(key == GLUT_KEY_F4) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L2 -= 1;
	  else L2 += 1;
	  if(L2 <= L1) L2 = L1 + 1;
	  if(L2 >= nMesh/2) L2 = nMesh/2 - 1;
		printf("L2 = %d \n", L2);
	  makeSeaCloudData();
  }
	//beta0
  if(key == GLUT_KEY_F5) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta0 -= 0.1;
	  else beta0 += 0.1;
		printf("beta0 = %f \n", beta0);
	  makeSeaCloudData();
  }

  //ampを変える
  if(key == GLUT_KEY_F6) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp0 -= 0.01;
	  else amp0 += 0.01;
		if(amp0 < 0.0) amp0 = 0.0;
		printf("amp0 = %f \n", amp0);
	  makeSeaCloudData();
  }
  //水面波位相シフト
  if(key == GLUT_KEY_F7) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaWave -= 0.01;
	  else deltaWave += 0.01;
		printf("deltaWave = %f \n", deltaWave);
	  makeSeaCloudData();
  }
  //雲位相シフト
  if(key == GLUT_KEY_F8) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaCloud -= 0.001;
	  else deltaCloud += 0.001;
		printf("deltaCloud = %f \n", deltaCloud);
	  makeSeaCloudData();
  }
  if(key == GLUT_KEY_F9) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) reflectivity0 -= 0.1;
	  else reflectivity0 += 0.1;
		if(reflectivity0 < 0.0) reflectivity0 = 0.0;
		if(reflectivity0 > 1.0) reflectivity0 = 1.0;
		printf("reflectivity0 = %f \n", reflectivity0);
  }

  else
  {
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
