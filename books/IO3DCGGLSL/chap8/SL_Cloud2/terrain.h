
//地形データ
#define nMeshT 64//最大分割数（両辺同じ）
float sizeXT = 30.0;//12.0;
float sizeYT = 30.0;//12.0;
float dataT[(nMeshT+1)*(nMeshT+1)];//高さデータ
int coordI, coordJ;
float beta = 2.5;//fBmの指数β
float sigma0 = 0.25;//標準偏差
int seed = 1;//乱数のｼｰﾄﾞ

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
		for(i = 0; i <= mp.numDiv; i++) mp.h[i][j] = -0.2;
	j = 2;
	mp.h[1][j] = 0.2; mp.h[2][j] = 0.5; mp.h[3][j] = 1.0; mp.h[4][j] = 0.6;
	mp.h[5][j] = 1.0; mp.h[6][j] = 0.6; mp.h[7][j] = 0.4; mp.h[8][j] = 0.2;
	j = 3;
	mp.h[1][j] = 0.5; mp.h[2][j] = 1.0; mp.h[3][j] = 1.5; mp.h[4][j] = 1.8;
	mp.h[5][j] = 1.8; mp.h[6][j] = 0.9; mp.h[7][j] = 0.3; mp.h[8][j] = 0.3;
	j = 4;
	mp.h[1][j] = 0.6; mp.h[2][j] = 1.8; mp.h[3][j] = 3.0; mp.h[4][j] = 2.5;
	mp.h[5][j] = 3.6; mp.h[6][j] = 2.0; mp.h[7][j] = 0.5; mp.h[8][j] = 0.5;
	j = 5;
	mp.h[1][j] = 0.3; mp.h[2][j] = 1.0; mp.h[3][j] = 1.5; mp.h[4][j] = 1.7;
	mp.h[5][j] = 1.8; mp.h[6][j] = 0.9; mp.h[7][j] = 0.3; mp.h[8][j] = 0.1;
	j = 6;
	mp.h[1][j] = 0.2; mp.h[2][j] = 0.4; mp.h[3][j] = 0.9; mp.h[4][j] = 0.7;
	mp.h[5][j] = 1.0; mp.h[6][j] = 0.6; mp.h[7][j] = 0.5; mp.h[8][j] = 0.2;
  mp.create(seed, dataT);//第1引数は乱数のseed（値を変えると異なったパターンが作成）

	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{
			if( dataT[j * (mp.numMax+1) + i] <= 0.2) dataT[j * (mp.numMax+1) + i] = -0.3;
		}
}

void drawTerrain()
{
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float diffuse[] = { 0.7f, 0.7f, 0.7f, 1.0f};
  float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  
  glPushMatrix();
	glTranslatef(0.0, 0.0, -5.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);//x軸回転
	glScalef(1.0, 1.0, 1.6);
  drawElevation(nMeshT, nMeshT, sizeXT, sizeYT, 0, dataT);
  glPopMatrix();
}

float getHeightTerrain(float x, float z)
{
	double  xx, zz;
	int x0, z0;
	xx = (x / sizeXT) * (float)nMeshT;
	if(xx - floor(xx) < 0.5) x0 = (int)floor(xx);
	else x0 = (int)ceil(xx);
	coordI = nMeshT / 2 + x0;

	zz = (z / sizeYT) * (float)nMeshT;
	if(zz - floor(zz) < 0.5) z0 = (int)floor(zz);
	else z0 = (int)ceil(zz);
	coordJ = nMeshT / 2 - z0;
	return dataT[coordJ*(nMeshT + 1) + coordI];
}
