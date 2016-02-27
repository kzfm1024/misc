/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

キューブマッピングによる
反射環境ﾊﾞﾝﾌﾟﾏｯﾋﾟﾝｸﾞ
OpenGL+GLSL
*/
#include <stdio.h>
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../room2.h"
#include "../../myGlsl.h"
#include "../../imageLoadSave.h"

//関数のプロトタイプ宣言
void init();
void idle();
void display();
void draw();
void resize(int w, int h);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void setLight();
void setCamera();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void setCubeMap();
void makeTexture();
void setNormalMap();
void makeNormalMap(Bitmap *bmp);
//void drawLight();
//バンプマッピング用プリミティブ
void drawBumpCube(float size, int nRepeatS, int nRepeatT);
void drawBumpSphere(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT);
void drawBumpTorus(double radius1, double radius2, int nSide, int nRing, int nRepeatS, int nRepeatT);

//affine変換用変数
float pos[] = { 0.0, 1.5, 0.0 };  //object中心のxyz座標
float scale[] = { 1.0, 1.0, 1.0 };//大きさ(倍率)
float angle[] = { 0.0, 0.0, 0.0 };//回転角度
//初期値
float pos0[] = { 0.0, 1.5, 0.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, 0.0, 0.0 };
//光源
float lightPos[] = {0.0, 5.0, 0.0, 1.0};//光源位置
float lightPos0[] = {0.0, 5.0, 0.0, 1.0};//光源位置(初期値)

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
  0.0, 1.5, 0.0,//cnt 
	1.0, 10.0, 0.0,//dist, theta, phi
	60.0, 0.1, 10.0//fovY,nearZ, farZ
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
//アニメーション
#include "../../myPet.h"
CmyPet pet ;
float curTime, lastTime, elapseTime;
int frameCount = 0;
int fps = 0;
int actionMode = 0;
float reflectivity0 = 0.5;//反射率

//normal map用
#define TEX_WIDTH 256
#define TEX_HEIGHT 256
GLubyte normalMap[TEX_HEIGHT][TEX_WIDTH][3];
float inv = 1.0;//凹凸反転
float fGrad = 5.0;//勾配強調
//cube map用
GLuint texName;//cube map
#define CUBE_WIDTH  64
#define CUBE_HEIGHT 64
//シェーダプログラム名
GLuint shaderProg;

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
    0, CUBE_HEIGHT,
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
    CUBE_WIDTH, CUBE_HEIGHT,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  },
  { 
		//床
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    CUBE_WIDTH,0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
  },
  { //手前
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    2*CUBE_WIDTH, CUBE_HEIGHT,
    0.0, 0.0, 1.0,
    0.0, -1.0, 0.0,
  },
  { //奥
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    2*CUBE_WIDTH, 0,
    0.0, 0.0, -1.0,
    0.0, -1.0, 0.0,
  }
};

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
  glutCreateWindow("反射ﾊﾞﾝﾌﾟﾏｯﾌﾟ(SL_ReflectBump)");
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
  initGlsl(&shaderProg, "reflectBump.vert", "reflectBump.frag");
	
  //イベント処理ループに入る
  glutMainLoop();
	//リソースの解放
	glDeleteTextures(1, &texName);
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

  glGenTextures(1, &texName);//テクスチャオブジェクトの名前付け
	//cube map
  setCubeMap();//テクスチャの設定
  //pet robot アニメーション
  pet.unitTime = 0.06;//１フレーム当たりの時間（最初は仮の値）
  //フレームデータを作成
  pet.numFrame0 = 0;
  frameCount = 0;
  if(actionMode == 0)
  {
		pet.vPos.x = -0.5;
		pet.vPos.z = -0.5;
		pet.vAng.y = 0.0;
		pet.walk(1.0, 0.2, 1.0);
		pet.turn(90.0, 2.0);
		pet.walk(1.0, 0.2, 1.0);
		pet.turn(90.0, 2.0);
		pet.walk(1.0, 0.2, 1.0);
		pet.turn(90.0, 2.0);
		pet.walk(1.0, 0.2, 1.0);
		pet.turn(90.0, 2.0);
  }
  else
  {
		pet.walk(0.8, 0.2, 1.0);
		pet.turn(90.0, 2.0);
		pet.walk(1.0, 0.2, 0.8);
		pet.turn(90.0, 1.0);
		pet.walk(0.5, 0.2, 0.8);
		pet.stand(5.0);
		pet.swingTail(5, 2.0);
		pet.wait(2.0);
		pet.initPose(3.0);
  }
	printf("numFrame0=%d \n", pet.numFrame0);

	makeTexture();
}

void makeTexture()
{
	//normal map
  Bitmap* bm0 = new Bitmap();
//  loadBitmap(bm0, "../../bmp256/char256-0.bmp");//disk4-256.bmp");
  loadBitmap(bm0, "../../bmp256/disk4-256.bmp");
  makeNormalMap(bm0);
  setNormalMap();
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
			if(inv == -1.0) 
				normalMap[j][i][2] = 255 - normalMap[j][i][2];
		}
  }
}

void setNormalMap()
{
  glBindTexture(GL_TEXTURE_2D, texName);//テクスチャをバインドする

  glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,TEX_WIDTH,TEX_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,normalMap);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);//バインドを解除
}

void setCubeMap()
{
  for(int i = 0; i < 6; i++)
	  glTexImage2D(target[i].name,0,GL_RGBA,CUBE_WIDTH,CUBE_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}


void display(void)
{
	//step1:環境マップの作成
	//オブジェクト中心から見た６方向のシーンを作成し、テクスチャメモリに転送
  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // 透視変換行列の設定 (texture作成）
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.0, 0.1, 30.0);//視野角を90°
  
  // モデルビュー変換行列の設定 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // オブジェクトから見たシーンをテクスチャに利用 
  for (int i = 0; i < 6; ++i) 
  {
    // ビューポートをテクスチャのサイズに設定する 
    glViewport(target[i].x, target[i].y, CUBE_WIDTH, CUBE_HEIGHT);
		// オブジェクト中心から見えるものをレンダリング 
		glPushMatrix();
		gluLookAt(pos[0], pos[1], pos[2], 
		    pos[0]+target[i].cx, pos[1]+target[i].cy, pos[2]+target[i].cz,
				target[i].ux, target[i].uy, target[i].uz);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		//3D空間の描画
		drawRoom(10.0, 10.0, 5.0);
		glTranslatef(0.0, 1.0, 0.0);
		pet.draw(frameCount);
		glPopMatrix();
	    
		// レンダリングした画像をテクスチャメモリに転送 
		glCopyTexSubImage2D(target[i].name, 0, 0, 0, target[i].x, target[i].y, CUBE_WIDTH, CUBE_HEIGHT);
  }

  //step2:通常の描画ルーチンに戻す
  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resize(width, height); 

  // シェーダプログラムの適用 
  glUseProgram(shaderProg);
  GLint refLoc = glGetUniformLocation(shaderProg, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  //光源設定//'l'を押した後光源位置可変
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

  GLint cubeLoc = glGetUniformLocation(shaderProg, "cubeMap");
  glUniform1i(cubeLoc, 0);//GL_TEXTURE0を適用

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texName);
  GLint normalLoc = glGetUniformLocation(shaderProg, "normalMap");
  glUniform1i(normalLoc, 1);//GL_TEXTURE1を適用

  //描画
  draw(); 
  // シェーダプログラムの適用を解除	
  glUseProgram(0);

  drawRoom(10.0, 10.0, 5.0);//room2.hにある
  glTranslatef(0.0, 1.0, 0.0);
  pet.draw(frameCount);

  curTime = timeGetTime();
  //1コマの所要時間
  float dt = (float)(curTime - lastTime) * 0.001;//sec
  lastTime = curTime;
  elapseTime += dt;
  fps ++;
  if(elapseTime >= 1.0)
  {
  	printf("frame per sec = %d , dt = %f\n", fps, dt);
	  elapseTime = 0.0;
	  fps = 0;
  }
  frameCount++;

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
		printf("[F1]キー：アニメーションモード切替 \n");
		printf("[F2]キー：反射率の増減 \n");
		printf("[F3]キー：凹凸反転 \n");
		printf("[F4]キー：勾配強調 \n");
		flagHelp = false;
  }
  //終了
  glutSwapBuffers();
}

void draw(void)
{
  float ambient[] = { 0.3, 0.3, 0.3, 1.0};
  float diffuse[] = { 0.7, 0.7, 0.7, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[0]-90.0, 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0], scale[1], scale[2]);
//drawBumpCube(0.4, 2, 2);  //オブジェクト形状
//	drawBumpSphere(0.3, 20, 20, 8, 4);
drawBumpTorus(0.1, 0.2, 20, 20, 16, 4);
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

  if(cos(M_PI * view.theta /180.0) >= 0.0)//カメラ仰角90度でﾋﾞｭｰｱｯﾌﾟﾍﾞｸﾄﾙ切替
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, 1.0, 0.0);
  else
	  gluLookAt(view.pos[0], view.pos[1], view.pos[2], view.cnt[0], view.cnt[1], view.cnt[2], 0.0, -1.0, 0.0);
 
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
		for(int i = 0; i < 3; i++)
		{
			pos[i] = pos0[i]; scale[i] = scale0[i]; angle[i] = angle0[i];
			lightPos[i] = lightPos0[i];
		}
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
	actionMode += 1; 
	if(actionMode > 1) actionMode = 0;
	printf("actionMode = %d \n", actionMode);
	init();
  }	
  if(key == GLUT_KEY_F2) 
  {
	if(glutGetModifiers() == 0)reflectivity0 += 0.1;
	  if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	reflectivity0 -= 0.1;  
	  if(reflectivity0 > 1.0) reflectivity0 = 1.0;
	  if(reflectivity0 < 0.0) reflectivity0 = 0.0;
	  printf("反射率= %f \n", reflectivity0);
  }
  if(key == GLUT_KEY_F3) 
  {
		inv *= -1.0;
		makeTexture();
  }	
  if(key == GLUT_KEY_F4) 
  {
		if(glutGetModifiers() == 0) fGrad += 1.0;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT) fGrad -= 1.0;
		if(fGrad < 0.0) fGrad = 0.0;
		printf("fGrad = %f \n", fGrad);
		makeTexture();
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
		  view.dist -= 0.1; //近づく
	  }
  }
  else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
	  if(x > width/4 && x < 3*width/4 && y > height/4 && y < 3*height/4)//dolly
	  {
		  view.dist += 0.1; //遠ざかる
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

//------------------------------------------------------------------------------
//ﾊﾞﾝﾌﾟﾏｯﾋﾟﾝｸﾞ用基本立体
void drawBumpCube(float size, int nRepeatS, int nRepeatT)
{
	float sz, s, t, tnt[3];
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
	tnt[0] = 0.0;tnt[1] = 1.0;tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,1.0f); 
	glTexCoord2f( s , 0.0); glVertex3fv(p[0]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[1]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[2]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[3]);
	//x方向(正面）
	tnt[0] = 0.0; tnt[1] = 1.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(1.0f,0.0f,0.0f); 
	glTexCoord2f( s ,  t ); glVertex3fv(p[0]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[3]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[7]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[4]);
	//y方向
	tnt[0] = -1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[0]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[4]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[5]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[1]);
	//-x方向
	tnt[0] = 0.0; tnt[1] = 1.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(-1.0f,0.0f,0.0f); 
	glTexCoord2f(0.0,  t ); glVertex3fv(p[6]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[2]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[1]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[5]);
	//-y方向
	tnt[0] = 1.0; tnt[1] = 0.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,-1.0f,0.0f); 
	glTexCoord2f(0.0,  t ); glVertex3fv(p[2]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[6]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[7]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[3]);
	//-z方向
	tnt[0] = 0.0; tnt[1] = 1.0; tnt[2] = 0.0;
	glVertexAttrib3fv(tangentLoc, tnt);
	glNormal3f(0.0f,0.0f,-1.0f); 
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[6]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[5]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[4]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[7]);
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

		//南極点を0,北極点を1とするt座標
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

