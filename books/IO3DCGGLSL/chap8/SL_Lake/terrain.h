//地形データ
#define nMeshT 128//最大分割数（両辺同じ）
float dataT[(nMeshT+1)*(nMeshT+1)];//高さデータ
double beta = 3.0;//fBmの指数β
double sigma0 = 0.1;//標準偏差初期値
int seed = 1;//乱数のｼｰﾄﾞ
//地形affine変換用変数
float pos[] = { 0.0, 0.0, 0.0 };//中心のxyz座標
float scale[] = { 1.0, 1.0, 1.5 };//大きさ(倍率)
float angle[] = { 0.0, 0.0, 0.0 };//回転角度

void makeTerrainData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 8;
  mp.numMax = nMeshT;
  mp.sigma0 = sigma0;
  mp.beta = beta;

  //初期高度データ
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			mp.h[i][j] = 0.5;
		}

	mp.h[1][1] = 0.9; mp.h[1][2] = 0.3; mp.h[1][3] = 0.2; mp.h[1][4] = 0.2;
	mp.h[1][5] = 0.2; mp.h[1][6] = 1.2; mp.h[1][7] = 1.5; mp.h[1][8] = 0.7;
	mp.h[2][1] = 0.5; mp.h[2][2] = -0.3; mp.h[2][3] = -0.2;
	mp.h[2][4] = -0.3; mp.h[2][5] = -0.2; mp.h[2][6] = -0.2; mp.h[2][7] = 1.4;
	mp.h[3][1] = 0.2; mp.h[3][2] = -0.4; mp.h[3][3] = -0.3; mp.h[3][4] = -0.4;
	mp.h[3][5] = -0.2; mp.h[3][6] = -0.1; mp.h[3][7] = 1.9; //mp.h[3][8] = 1.9; 
	mp.h[4][1] = 0.2; mp.h[4][2] = -0.3; mp.h[4][3] = -0.2; mp.h[4][4] = 0.5;
	mp.h[4][5] = -0.3;  mp.h[4][6] = -0.2; mp.h[4][7] = 0.8; //mp.h[4][8] = 1.2;
	mp.h[5][1] = 0.2;  mp.h[5][2] = -0.3; mp.h[5][3] = -0.1;  mp.h[5][4] = -0.2;
	mp.h[5][5] = -0.3;  mp.h[5][6] = -0.2;mp.h[5][7] = 1.6;  //mp.h[5][8] = 1.7;
	//mp.h[5][6] = 0.0;  mp.h[5][7] = 0.8; mp.h[5][8] = 0.6;
	mp.h[6][1] =  0.7;  mp.h[6][2] = -0.4;mp.h[6][3] = -0.2;  mp.h[6][4] = -0.2;
	mp.h[6][5] = -0.2;  mp.h[6][6] = -0.2; mp.h[6][7] = 0.8;  mp.h[6][8] = 0.9;
	mp.h[7][1] = 0.4;  mp.h[7][2] = 0.8; mp.h[7][3] = 1.3;  mp.h[7][4] = 1.2;
	mp.h[7][5] = 1.9;  mp.h[7][6] = 1.0; mp.h[7][7] = 1.3;

/*
	mp.h[1][1] = 0.3; mp.h[1][3] = 0.4;
	mp.h[1][5] = 0.4; mp.h[1][7] = 0.9;
	mp.h[2][0] = 0.3; mp.h[2][1] = 0.2;
	mp.h[2][2] = 0.1; mp.h[2][7] = 0.8;
	mp.h[3][2] = -0.2; mp.h[3][3] = -0.2;
	mp.h[3][4] = -0.2; mp.h[3][5] = -0.1; mp.h[3][7] = 1.2;  
	mp.h[4][1] = 0.3;  mp.h[4][2] = -0.2;
	mp.h[4][3] = -0.2; mp.h[4][4] = -0.2;
	mp.h[4][5] = 0.0;  mp.h[4][6] = 0.1;
	mp.h[4][7] = 1.3; mp.h[4][8] = 0.5;
	mp.h[5][2] = 0.0;  mp.h[5][3] = -0.2;
	mp.h[5][4] = -0.3;  mp.h[5][5] = -0.2;
	mp.h[5][6] = 0.0;  mp.h[5][7] = 0.8; mp.h[5][8] = 0.6;
	mp.h[6][1] = 0.3;  mp.h[6][3] = 0.4;
	mp.h[6][4] = 0.5;  mp.h[6][5] = 0.7;
	mp.h[6][6] = 0.5;  mp.h[6][7] = 0.9;
	mp.h[7][1] = 0.4;  mp.h[7][2] = 0.8; 
	mp.h[7][3] = 1.3;  mp.h[7][4] = 1.2;
	mp.h[7][5] = 1.0;  mp.h[7][6] = 1.0; mp.h[7][7] = 1.3;
*/
  mp.create(seed, dataT);//第1引数は乱数のseed（値を変えると異なったパターンが作成）

}

void drawTerrain()
{
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f};
  float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[0]-90.0, 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0], scale[1], scale[2]);

//  drawElevation(nMeshT, nMeshT, 8.0, 12.0, 0, dataT);
  drawElevation(nMeshT, nMeshT, 12.0, 12.0, 0, dataT);
  glPopMatrix();
}
