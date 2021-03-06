/*
//一様乱数
float getRandom(float fMin, float fMax)
{
  return fMin + (fMax - fMin) * (float)rand() / (float)RAND_MAX;
}*/

//地形データ
#define nMeshT 128//最大分割数（両辺同じ）
float dataT[(nMeshT+1)*(nMeshT+1)];//高さデータ
float beta = 2.5;//fBmの指数β
float sigma0 = 0.1;//標準偏差
int seed = 1;//乱数のｼｰﾄﾞ
//地形affine変換用変数
float pos[] = { 0.0, 0.0, 0.0 };//中心のxyz座標
float scale[] = { 1.0, 1.0, 1.0 };//大きさ(倍率)
float angle[] = { 0.0, 0.0, 0.0 };//回転角度

void makeTerrainData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 16;
  mp.numMax = nMeshT;
  mp.sigma0 = sigma0;
  mp.beta = beta;

  //初期高度データ
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			if(j <= 8) 
			{
				mp.h[i][j] = 0.2;
				if(i >= 7 && i <= 9) mp.h[i][j] = -0.2;
			}
			else 
			{
				mp.h[i][j] = 2.2;
				if(i >= 7 && i <= 9) mp.h[i][j] = 1.75;
			}
			if(i <= 4 && j >= 10) mp.h[i][j] = 2.8;
			if(i >= 12 && j >= 10) mp.h[i][j] = 2.8;

		}
  mp.create(seed, dataT);//第1引数は乱数のseed（値を変えると異なったパターンが作成）
/*
	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//負ならば0とする
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}
*/
}

void drawTerrain()
{
  float diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f};
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float specular[]= { 0.0f, 0.0f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);
  
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z軸回転
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y軸回転
  glRotatef(angle[0]-90.0, 1.0, 0.0, 0.0);//x軸回転
  glScalef(scale[0], scale[1], scale[2]);

  drawElevation(nMeshT, nMeshT, 8.0, 12.0, 0, dataT);
  glPopMatrix();
}
