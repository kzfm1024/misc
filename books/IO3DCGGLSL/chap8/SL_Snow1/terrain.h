//�n�`�f�[�^
#define nMeshT 128//�ő啪�����i���ӓ����j
float sizeX = 12.0;
float sizeY = 12.0;
float dataT[(nMeshT+1)*(nMeshT+1)];//�����f�[�^
int coordI, coordJ;
float beta = 2.5;//fBm�̎w����
float sigma0 = 0.25;//�W���΍�
int seed = 1;//�����̼���
//�n�`affine�ϊ��p�ϐ�
float pos[] = { 0.0, 0.0, 0.0 };//���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
float angle[] = { 0.0, 0.0, 0.0 };//��]�p�x
void calcDirection();

void makeTerrainData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 8;
  mp.numMax = nMeshT;
  mp.sigma0 = sigma0;
  mp.beta = beta;

  //�������x�f�[�^
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++) mp.h[i][j] = 0.0;
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
  mp.create(seed, dataT);//��1�����͗�����seed�i�l��ς���ƈقȂ����p�^�[�����쐬�j
/*
	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//���Ȃ��0�Ƃ���
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}*/
//	calcDirection();
}

void drawTerrain()
{
  float diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f};
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float specular[]= { 0.4f, 0.4f, 0.4f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0]-90.0, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0], scale[1], scale[2]);

  drawElevation(nMeshT, nMeshT, sizeX, sizeY, 0, dataT);
  glPopMatrix();
}

float getHeightTerrain(float x, float z)
{
	double  xx, zz;
	int x0, z0;
	xx = (x / sizeX) * (float)nMeshT;
	if(xx - floor(xx) < 0.5) x0 = (int)floor(xx);
	else x0 = (int)ceil(xx);
	coordI = nMeshT / 2 + x0;

	zz = (z / sizeY) * (float)nMeshT;
	if(zz - floor(zz) < 0.5) z0 = (int)floor(zz);
	else z0 = (int)ceil(zz);
	coordJ = nMeshT / 2 - z0;
	return dataT[coordJ*(nMeshT + 1) + coordI];
}
