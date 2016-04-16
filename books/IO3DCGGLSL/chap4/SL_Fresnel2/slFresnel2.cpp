/*
Microsoft Visual Studio .NET2005 Standard Edition
プロジェクトの種類：VisualC++プロジェクト
テンプレート：Win32コンソールプロジェクト
アプリケーションの種類：コンソールアプリケーション
追加のオプション：空のプロジェクト

キューブマッピングによる
フレネル効果2（カラーマップを追加)
OpenGL+GLSL
*/
#include <stdio.h>
//#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "../../myPrimitive2.h"
#include "../../myTexture2.h"
#include "../../room2.h"
#include "../../myGlsl.h"
#include "../../imageLoadSave.h"
#include <time.h> // for clock_gettime()

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
void makeTexImage(Bitmap *bm);
void setTexture();
void initFramebuffer();

//affine変換用変数
float pos[] =   { 0.0, 1.0, -1.0 };  //object中心のxyz座標
float scale[] = { 1.0, 1.0, 1.0 };//大きさ(倍率)
float angle[] = { 0.0, 0.0, 0.0 };//回転角度
//初期値
float pos0[] =   { 0.0, 1.0, -1.0 };
float scale0[] = { 1.0, 1.0, 1.0 };
float angle0[] = { 0.0, 0.0, 0.0 };
//光源
float lightPos[] = {5.0, 5.0, 5.0, 1.0}; //光源位置
float lightPos0[] = {5.0, 5.0, 5.0, 1.0};//光源位置(初期値)

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
  0.0, 1.2, 0.0,//cnt 
	3.0, 10.0, 0.0,//dist, theta, phi
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
//アニメーション
#include "../../myTexPet.h"
CmyTexPet pet ;
float curTime, lastTime, elapseTime;
int frameCount = 0;
int fps = 0;
int actionMode = 0;
float transparency0 = 0.3;//透明度
float reflectivity0 = 0.3;//垂直反射率
float nRatio0 = 2.0;

//既存画像のﾃｸｽﾁｬ用
#define TEX_WIDTH2  128
#define TEX_HEIGHT2 128
GLubyte texImage[TEX_HEIGHT2][TEX_WIDTH2][4];
//キューブマップ用
#define TEX_WIDTH  512
#define TEX_HEIGHT 512
//シェーダプログラム名
GLuint shaderProg;
//FBO関連
GLuint	texName[2];//texture object
GLuint	rbName;//renderbuffer
GLuint	fbName;//framebuffer
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
    0, TEX_HEIGHT,
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
    TEX_WIDTH, TEX_HEIGHT,
    0.0, 1.0, 0.0,
    0.0, 0.0, 1.0,
  },
  { 
		//床
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    TEX_WIDTH,0,
    0.0, -1.0, 0.0,
    0.0, 0.0, -1.0,
  },
  { //手前
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    2*TEX_WIDTH, TEX_HEIGHT,
    0.0, 0.0, 1.0,
    0.0, -1.0, 0.0,
  },
  { //奥
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    2*TEX_WIDTH, 0,
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
  glutCreateWindow("フレネル効果2(SL_Fresnel2)");
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
	initFramebuffer();
  initGlsl(&shaderProg, "Fresnel2.vert", "Fresnel2.frag");
	
  //イベント処理ループに入る
  glutMainLoop();
	glDeleteTextures(2, texName);
	glDeleteRenderbuffersEXT(1, &rbName);
	glDeleteFramebuffersEXT(1, &fbName);
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

  //テクスチャの設定
  glGenTextures(2, texName);//テクスチャオブジェクトの名前付け
	//CubeMap
  setCubeMap();
	//ColorMap
  Bitmap* bm0 = new Bitmap();
  loadBitmap(bm0, "../../bmp/check1.bmp");
  makeTexImage(bm0);
  setTexture();
  //pet robot アニメーション
  pet.unitTime = 0.015;//１フレーム当たりの時間（最初は仮の値）
  //フレームデータを作成
  pet.numFrame0 = 0;
  //フレームデータを作成
  pet.numFrame0 = 0;
  frameCount = 0;
  if(actionMode == 0)
  {
	  pet.walk(1.5, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.0, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.5, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
	  pet.walk(1.0, 0.2, 1.0);
	  pet.turn(90.0, 2.0);
  }
  else
  {
	  pet.turn(90.0, 2.0);
	  pet.stand(5.0);
	  pet.swingTail(5, 2.0);
	  pet.wait(3.0);
	  pet.initPose(5.0);
	  pet.turn(-90.0, 2.0);
  }

  printf("numFrame0=%d \n", pet.numFrame0);
}

void setCubeMap()
{
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[0]);
   //テクスチャオブジェクトの作成
  for(int i = 0; i < 6; i++)
	glTexImage2D(target[i].name,0,GL_RGBA,TEX_WIDTH,TEX_HEIGHT,0,GL_RGBA,GL_UNSIGNED_BYTE,0);

  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_REPEAT);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void initFramebuffer( void )
{
	//renderbuffer
	glGenRenderbuffersEXT( 1, &rbName );
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, rbName );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, TEX_WIDTH, TEX_HEIGHT );
  //framebuffer
	glGenFramebuffersEXT( 1, &fbName );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbName );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
								  GL_RENDERBUFFER_EXT, rbName );

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}


//画像ファイルからイメージデータを作成
void makeTexImage(Bitmap *bm)
{
  int i, j, ii, jj;

  if(bm->bi.Width < TEX_WIDTH2) printf("画像ファイルのｻｲｽﾞ>=TEX_WIDTHとすること\n");

  for(j = 0; j < TEX_HEIGHT2; j++)
  {
		jj = (int)((float)j * (float)bm->bi.Height / (float)TEX_HEIGHT2);
		for(i = 0; i < TEX_WIDTH2; i++)
    {
      ii = (int)((float)i * (float)bm->bi.Width / (float)TEX_WIDTH2) ;
			if(bm->bi.BitCount <= 24)
			{
				texImage[j][i][0] = (bm->pixel[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixel[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixel[ii+jj*bm->bi.Width]).b;
				texImage[j][i][3] = 255;
			}
			else
			{
				texImage[j][i][0] = (bm->pixelA[ii+jj*bm->bi.Width]).r;
				texImage[j][i][1] = (bm->pixelA[ii+jj*bm->bi.Width]).g;
				texImage[j][i][2] = (bm->pixelA[ii+jj*bm->bi.Width]).b;
				texImage[j][i][3] = (bm->pixelA[ii+jj*bm->bi.Width]).a;
			}
    }
  }
	if(bm->bi.BitCount <= 24) free(bm->pixel);
	else free(bm->pixelA);
}

void setTexture()
{
   //テクスチャオブジェクトの作成
  glBindTexture(GL_TEXTURE_2D, texName[1]);
  //テクスチャの指定 
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,TEX_WIDTH2,TEX_HEIGHT2,0,GL_RGBA,GL_UNSIGNED_BYTE,texImage);
  //テクスチャの繰り返し方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);//GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);//GL_CLAMP);
  //テクスチャを拡大・縮小する方法の指定 
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//NEAREST);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);//NEAREST);
  glBindTexture(GL_TEXTURE_2D, 0);
}

static double timeGetTime()
{
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	double msec = (double)tp.tv_sec * 1000.0 + (double)tp.tv_nsec / 1000.0;
	printf("%s: msec %f\n", __FUNCTION__, msec);
	return msec;
}

void display(void)
{
	//step1:環境マップの作成
	//オブジェクト中心から見た６方向のシーンを作成し、テクスチャメモリに転送

	//framebuffer object を有効化
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbName );

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
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
                              target[i].name, texName[0], 0);
    glViewport(0, 0, TEX_WIDTH, TEX_HEIGHT);
		//カラーバッファ,デプスバッファのクリア
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// オブジェクト中心から見えるものをレンダリング 
		glPushMatrix();
		gluLookAt(pos[0], pos[1], pos[2], 
		    pos[0]+target[i].cx, pos[1]+target[i].cy, pos[2]+target[i].cz,
				target[i].ux, target[i].uy, target[i].uz);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		//3D空間の描画
		drawRoom(10.0, 10.0, 5.0);
		glPopMatrix();
  }
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

  //step2:通常の描画ルーチンに戻す
  //カラーバッファ,デプスバッファのクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  resize(width, height);  
  // シェーダプログラムの適用 
  glUseProgram(shaderProg);
  GLint tranLoc = glGetUniformLocation(shaderProg, "transparency");
  glUniform1f(tranLoc, transparency0);
  GLint refLoc = glGetUniformLocation(shaderProg, "reflectivity");
  glUniform1f(refLoc, reflectivity0);
  GLint ratioLoc = glGetUniformLocation(shaderProg, "nRatio");
  glUniform1f(ratioLoc, nRatio0);
  float m[16] ;
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  GLint ViewTransLoc = glGetUniformLocation(shaderProg, "ViewTranspose");
  glUniformMatrix4fv(ViewTransLoc, 1, GL_TRUE, m);

  //光源設定//'l'を押した後光源位置可変
  glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	//texture unit と texture object の関連付け
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texName[0]);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texName[1]);
	//テクスチャユニットをシェーダ側のサンプラに関連付ける
  GLint samplerLoc0 = glGetUniformLocation(shaderProg, "envSampler");
  glUniform1i(samplerLoc0, 0);//GL_TEXTURE0を適用
  GLint samplerLoc1 = glGetUniformLocation(shaderProg, "texSampler");
  glUniform1i(samplerLoc1, 1);//GL_TEXTURE1を適用
  //描画
  draw(); 
  // シェーダプログラムの適用を解除	
  glUseProgram(0);

  drawRoom(10.0, 10.0, 5.0);//room.hにある

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
		printf("[F1]キー：アニメーションスタート \n");
		printf("[F2]キー：透過率の増減 \n");
		printf("[F3]キー：屈折率の増減 \n");
		printf("[F4]キー：垂直反射率の増減 \n");
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
  glRotatef(angle[0], 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0], scale[1], scale[2]);
//  pet.draw(frameCount);
  drawTexSphere2(1.0, 30, 30, 1, 1);
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
	  init();//アニメーションスタート
	}	
	if(key == GLUT_KEY_F2) 
	{
	  if(glutGetModifiers() == 0) transparency0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	transparency0 -= 0.1;  
		if(transparency0 < 0.0) transparency0 = 0.0;
		if(transparency0 > 1.0) transparency0 = 1.0;
		printf("透明度= %f \n", transparency0);
	}	
	if(key == GLUT_KEY_F3) 
	{
		if(glutGetModifiers() == 0) nRatio0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	nRatio0 -= 0.1;
		printf("比屈折率= %f \n", nRatio0);		
	}	
	if(key == GLUT_KEY_F4) 
	{
	  if(glutGetModifiers() == 0) reflectivity0 += 0.1;
		if(glutGetModifiers() == GLUT_ACTIVE_SHIFT)	reflectivity0 -= 0.1;  
		if(reflectivity0 < 0.0) reflectivity0 = 0.0;
		if(reflectivity0 > 1.0) reflectivity0 = 1.0;
		printf("垂直反射率= %f \n", reflectivity0);
	}	}

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

