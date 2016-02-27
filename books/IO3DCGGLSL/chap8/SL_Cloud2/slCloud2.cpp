/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

OpenGL+GLSL
FFTにより海
中点変位法により雲（particle)
山岳地形と太陽
反射環境マッピング
反射オブジェクトに雲を追加
船とその航跡を追加
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
#include "cloud.h"
#include "particle.h"
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
void setTerrainTexture(int n);
void makeParticleImage(Bitmap *bm);
void setParticleTexture(int n);
void drawParticle(float dt);
void drawWave();
void drawSun();
void makeSeaData();
void setCubeMap(int n);
void drawShip();
void addTrackWave();


//光源
float lightPos[]  = {0.0, 50.0, -50.0, 1.0};//光源位置
float lightPos0[] = {0.0, 50.0, -50.0, 1.0};//光源位置(初期値）

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
	30.0, 0.0, 0.0,//dist, theta, phi
	80.0, 1.0, 120.0//fovY,nearZ, farZ
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
GLubyte texImage[TEX_HEIGHT][TEX_WIDTH][4];
GLuint texName[3];
//シェーダプログラム名
GLuint shaderProg1;
GLuint shaderProg2;

//海面データ
#define nMesh 128 //最大分割数（両辺同じ）
float sizeX = 60.0;//16.0;
float sizeY = 60.0;//16.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//海面データ
float sr[nMesh*nMesh], si[nMesh*nMesh];
float tblSin[nMesh], tblCos[nMesh];//sin,cosﾃｰﾌﾞﾙ
float amp[nMesh * nMesh];
float phiSea[nMesh * nMesh];//海面の位相
float beta0 = 2.5;//fBmの指数β
float amp0 = 0.05;//基本波の振幅
float deltaWave = 0.2;
int K1 = 5 ;//kの低次側次数
int K2 = 20;//kの高次側次数
int L1 = 20; //lの低次側次数
int L2 = 30;//lの高次側次数
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
float reflectivity0 = 0.5;//垂直反射率
float posShip0[] = { -10.0, 0.3, 20.0 };//船のxyz座標
float posShip[3];
float speedShip = 0.3;// 0.1;//船のスピード
//particle
CParticle p[NUM_PARTICLE];

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
  glutCreateWindow("雲2(SL_Cloud2)");
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
  glClearColor(0.5, 0.4, 0.2, 1.0);

  setCamera();//視点を求める
  setLight(); //光源設定　
	glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  printf("マウス/キー操作の説明には'h'キーをプッシュ \n");

  glGenTextures(3, texName);//テクスチャオブジェクトの名前付け
	makeSeaData();

	makeTerrainData();
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/altgrad4.bmp");
  makeTerrainImage(bm0);
  setTerrainTexture(0);
  //反射環境マッピング用テクスチャの設定
  setCubeMap(1);

	//particle用ﾃｸｽﾁｬ
  Bitmap* bm1 = new Bitmap();
  loadBitmap(bm1, "../../bmp/cloud.bmp");
  makeParticleImage(bm1);
  setParticleTexture(2);
	//雲粒子の発生
	makeCloudData();//cloud.h
  for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
	countP = 0.0;

	//時間計測
  lastTime = timeGetTime();
  elapseTime1 = 0.0;
  elapseTime2 = 0.0;
}

void makeSeaData()
{
	//海面、雲データの作成（FFT)
	fft = CFFT(K1, K2, L1, L2, beta0, amp0, -1, nMesh);
  //sin,cosﾃｰﾌﾞﾙ
  fft.makeTable(tblSin, tblCos);
  //振幅と初期位相
  fft.makeAmpPhase(amp, phiSea);
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

void setTerrainTexture(int n)
{
  glBindTexture(GL_TEXTURE_2D, texName[n]);//テクスチャをバインドする
  //テクスチャの指定 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //テクスチャの繰り返し方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //テクスチャを拡大・縮小する方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void setCubeMap(int n)
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[n]);//テクスチャをバインドする
  for(int i = 0; i < 6; i++)
	glTexImage2D(target[i].name,0,GL_RGB,texRefWidth,texRefHeight,0,GL_RGB,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);//バインドを解除
}

void makeParticleImage(Bitmap *bmp)
{
  int i, j, ii, jj;
  int nx = TEX_WIDTH;
  int ny = TEX_HEIGHT;
  float pWidth = (float)bmp->bi.Width;
  float pHeight = (float)bmp->bi.Height;

  for(j = 0; j < ny; j++)
  {
    jj = j * (int)(pHeight / (float)ny);
		for(i = 0; i < nx; i++)
    {
      ii = i * (int)(pWidth / (float)nx);
			if(bmp->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bmp->pixel[ii+jj*bmp->bi.Width]).r;
				texImage[j][i][1] = (bmp->pixel[ii+jj*bmp->bi.Width]).g;
				texImage[j][i][2] = (bmp->pixel[ii+jj*bmp->bi.Width]).b;
				//黒を透明に
				if( texImage[j][i][0] == 0 && texImage[j][i][1] == 0 &&  texImage[j][i][2] == 0)
				texImage[j][i][3] = 0; 
				else texImage[j][i][3] = 255;

			}
			else//32bit color
			{
				texImage[j][i][0] = (bmp->pixelA[ii+jj*bmp->bi.Width]).r;
				texImage[j][i][1] = (bmp->pixelA[ii+jj*bmp->bi.Width]).g;
				texImage[j][i][2] = (bmp->pixelA[ii+jj*bmp->bi.Width]).b;
				//黒を透明に
				if( texImage[j][i][0] == 0 && texImage[j][i][1] == 0 &&  texImage[j][i][2] == 0)
				texImage[j][i][3] = 0; 
				else texImage[j][i][3] = 255;
			}
    }
  }
}

void setParticleTexture(int n)
{
//  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texName[n]);//テクスチャをバインドする
 //テクスチャの割り当て 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //テクスチャの繰り返し方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
  //テクスチャを拡大・縮小する方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //色の調整（環境の設定）
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    // アルファテストの判別関数 
  glAlphaFunc(GL_GREATER, 0.1);

  //point spriteのｻｲｽﾞを自動調整
  float atten[] = {0.0, 0.0, 0.01};
  glPointParameterfv =
    (PFNGLPOINTPARAMETERFVPROC)wglGetProcAddress("glPointParameterfv");
  glPointParameterf =
    (PFNGLPOINTPARAMETERFPROC)wglGetProcAddress("glPointParameterf");
  glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, atten);
  glPointParameterf(GL_POINT_SIZE_MAX, 10000.0);

  glBindTexture(GL_TEXTURE_2D, 0);
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

  GLint texSampler = glGetUniformLocation(shaderProg1, "tex");
  glUniform1i(texSampler, 0);//TEXTURE0
  
  //鏡映視点から見たシーンをテクスチャに利用 
  for (int i = 0; i < 6; ++i) 
  {
	  glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texName[0]);
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
		drawShip();
    //雲のparticle
		glBindTexture(GL_TEXTURE_2D, texName[2]);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		drawParticle(dt);
		glDisable(GL_BLEND);

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

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texName[0]);
  //描画
	glUseProgram(shaderProg1);
  drawTerrain();
  glUseProgram(0);

  //半透明物体があるときは次の2行が必要
  glCullFace(GL_BACK);   //裏面を削除
  glEnable(GL_CULL_FACE);//裏面削除を有効にする
	drawSun();
	drawShip();

	//step3
	//反射環境マッピング
  glUseProgram(shaderProg2);
  GLint refLoc = glGetUniformLocation(shaderProg2, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg2, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[1]);
  GLint texLoc = glGetUniformLocation(shaderProg2, "texCube");
  glUniform1i(texLoc, 0);//GL_TEXTURE1を適用

  drawWave(); //海面描画
  // シェーダプログラムの適用を解除	
  glUseProgram(0);
	
	//雲のparticle
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	drawParticle(dt);
	glDisable(GL_BLEND);


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
		printf("[F5]:beta, [F6]:amp0, [F7]:deltaWave の調整\n");
		printf("[F8]:水面反射率reflectivity調整\n");
		printf("[F9]:x軸方向の風力accelX調整\n");
		printf("[F10]:バイアスbias調整\n");
		printf("[F11]:雲作成時の乱数系列のシードseedC調整\n");

		printf(" \n");
		flagHelp = false;
  }

  //終了
  glutSwapBuffers();
}

void drawWave()
{
  float ambient[] = { 0.1f, 0.1f, 0.5f, 1.0f};
  float diffuse[] = { 0.2f, 0.2f, 1.0f, 1.0f};
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

	addTrackWave();
  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

void addTrackWave()
{
	//船の航跡作成（x軸正方向のときだけ）
	//船の速度 > 水面の波の速度のときドップラー波
	int i, ii, j;
	float x, y, r, xx, phase;
	float a0 = 0.1;//円形波の振幅
	float period = 5.0;
	float lambda = 1.0;
	float velocity = lambda / period ;//水面波の速度
	//船の位置(波源）
	float sX = posShip[0]+1.0;
	float sY = -posShip[2];

	if(speedShip == 0.0) return;
	for(j = 0; j <= nMesh; j++)
	{
		y = (float)(-nMesh / 2 + j) * meshY - sY;
		for(i = 0; i <= nMesh; i++)
		{
			x = (float)(-nMesh / 2 + i) * meshX - sX;
			r = sqrt(x * x + y * y);
			phase = 2.0 * M_PI * (elapseTime2 / period - r / lambda);
			if(phase < 0.0) continue;
			xx = speedShip * r / velocity;
			ii = i - floor( xx / meshX + 0.5);
			if(ii < 0 || ii > nMesh) continue;
			if( r < 0.5) r = 0.5;
			data[j * (nMesh+1) + ii] += a0 * sin(phase) / r;//sqrt(r);
		}
	}
}

void drawSun()
{
  float ambient[] = { 1.0f, 0.5f, 0.2f, 1.0f};
  float diffuse[] = { 1.0f, 0.5f, 0.2f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,1);

  glPushMatrix();
  glTranslatef(lightPos[0], lightPos[1] + 10 , lightPos[2] - 15.0);
  drawSphere(10.0, 20, 20);
  glPopMatrix();
}

void drawShip()
{
  float ambient[] = { 0.5f, 0.5f, 0.6f, 1.0f};
  float diffuse[] = { 0.8f, 0.8f, 0.9f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

	for(int i= 0; i < 3; i++) posShip[i] = posShip0[i];
	posShip[0] = posShip0[0] + speedShip * elapseTime2;//ｘ方向の船の速度
  glPushMatrix();
	glTranslatef(posShip[0], posShip[1], posShip[2]);
	glRotatef(90.0, 1.0, 0.0, 0.);
	glScalef(1.0, 0.5, 0.5);
	drawHemiSuper(1.0, 12, 6, 0.8, 0.5);
	drawCube(1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.5);
	drawCylinder1(0.2, 0.2, 1.0, 5, 2);//煙突
	glPopMatrix();
  glPopMatrix();
}

void drawParticle(float dt)
{
  int i;

  glEnable(GL_POINT_SPRITE);//ポイントスプライト有効
	glEnable(GL_TEXTURE_2D);  //テクスチャマッピング有効
  glEnable(GL_ALPHA_TEST);


	for(i = 0; i < num0; i++) p[countP + i].create(elapseTime2);//粒子の生成
	countP += num0;
  if(countP + num0 >= NUM_PARTICLE) countP = 0;

  for(i = 0; i < NUM_PARTICLE; i++)
  {
		//粒子の位置に相当するTerrainの格子座標coordI,coordJを求める
		if(p[i].vPosition.x <= - sizeX / 2.0 || p[i].vPosition.x >= sizeX / 2.0
		|| p[i].vPosition.z <= - sizeY / 2.0 || p[i].vPosition.z >= sizeY / 2.0) 
		{
			p[i].inside = false;//領域外
		}
		else
		{
			p[i].inside = true;
			p[i].heightT = getHeightTerrain(p[i].vPosition.x, p[i].vPosition.z);
		}
		p[i].show();
		p[i].update(elapseTime2, dt);
		p[i].vAccel.x = accelX;//[F4]キーで変更
	}

  glDisable(GL_ALPHA_TEST);
  glDisable(GL_POINT_SPRITE);
  glDisable(GL_TEXTURE_2D);
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
  float lightAmbient[] = {0.5f, 0.3f, 0.2f, 1.0f}; //環境光
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
	  makeSeaData();
  }
  //K2
  if(key == GLUT_KEY_F2) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) K2 -= 1;
	  else K2 += 1;
	  if(K2 <= K1) K2 = K1 + 1;
	  if(K2 >= nMesh/2) K2 = nMesh/2 - 1;
		printf("K2 = %d \n", K2);
	  makeSeaData();
  }
  //L1
  if(key == GLUT_KEY_F3) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L1 -= 1;
	  else L1 += 1;
	  if(L1 <= 1) L1 = 1;
	  if(L1 >= L2) L1 = L2 -1;
		printf("L1 = %d \n", L1);
	  makeSeaData();
  }
  //L2
  if(key == GLUT_KEY_F4) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) L2 -= 1;
	  else L2 += 1;
	  if(L2 <= L1) L2 = L1 + 1;
	  if(L2 >= nMesh/2) L2 = nMesh/2 - 1;
		printf("L2 = %d \n", L2);
	  makeSeaData();
  }
	//beta0
  if(key == GLUT_KEY_F5) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) beta0 -= 0.1;
	  else beta0 += 0.1;
		printf("beta0 = %f \n", beta0);
	  makeSeaData();
  }

  //ampを変える
  if(key == GLUT_KEY_F6) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) amp0 -= 0.01;
	  else amp0 += 0.01;
		if(amp0 < 0.0) amp0 = 0.0;
		printf("amp0 = %f \n", amp0);
	  makeSeaData();
  }
  //水面波位相シフト
  if(key == GLUT_KEY_F7) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) deltaWave -= 0.01;
	  else deltaWave += 0.01;
		printf("deltaWave = %f \n", deltaWave);
	  makeSeaData();
  }
  if(key == GLUT_KEY_F8) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) reflectivity0 -= 0.1;
	  else reflectivity0 += 0.1;
		if(reflectivity0 < 0.0) reflectivity0 = 0.0;
		if(reflectivity0 > 1.0) reflectivity0 = 1.0;
		printf("reflectivity0 = %f \n", reflectivity0);
  }  //x軸方向風力
  if(key == GLUT_KEY_F9) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) accelX -= 0.01;
	  else accelX += 0.01;
		printf("accelX = %f \n", accelX);
  }

  if(key == GLUT_KEY_F10) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) bias -= 0.1;
	  else bias += 0.1;
		if(bias < 0.0) bias = 0.0;
		if(bias > 1.0) bias = 1.0;
		printf("bias = %f \n", bias);
		makeCloudData();
		for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
		countP = 0.0;//numParticle;
  }
  if(key == GLUT_KEY_F11) 
  {
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) seedC -= 1;
	  else seedC += 1;
		printf("seedC = %d \n", seedC);
		makeCloudData();
		for(int k = 0; k < NUM_PARTICLE; k++) p[k] = CParticle();
		countP = 0.0;//numParticle;
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
