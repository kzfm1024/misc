
//�n�`�f�[�^
#define nMeshT 128//�ő啪�����i���ӓ����j
float sizeX = 12.0;
float sizeY = 8.0;
float dataT[(nMeshT+1)*(nMeshT+1)];//�����f�[�^
int coordI, coordJ;
unsigned char direction[(nMeshT+1)*(nMeshT+1)];//�����f�[�^�i������z�̑傫�������j
float beta = 3.0;//fBm�̎w����
float sigma0 = 0.3;//�W���΍�
int seed = 0;//�����̼���
//�n�`affine�ϊ��p�ϐ�
float pos[] = { 0.0, 0.0, 0.0 };//���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
float angle[] = { 0.0, 0.0, 0.0 };//��]�p�x
float sc = 5.0;//�{��

void getDirection();

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
	mp.h[1][j] = 0.2; mp.h[2][j] = 0.5; mp.h[3][j] = 1.0; mp.h[4][j] = 0.9;
	mp.h[5][j] = 0.2; mp.h[6][j] = 0.5; mp.h[7][j] = 0.4; mp.h[8][j] = 0.2;
	j = 3;
	mp.h[1][j] = 0.5; mp.h[2][j] = 1.0; mp.h[3][j] = 1.5; mp.h[4][j] = 1.8;
	mp.h[5][j] = 1.0; mp.h[6][j] = 0.9; mp.h[7][j] = 0.3; mp.h[8][j] = 0.3;
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

	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//���Ȃ��0�Ƃ���
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}

	getDirection();
}

void drawTerrain()
{
  float ambient[] = { 0.5, 0.5, 0.5, 1.0};
  float diffuse[] = { 1.0, 1.0, 1.0, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  
  glPushMatrix();
  glTranslatef(pos[0], pos[1], pos[2]);
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0] -90.0, 1.0, 0.0, 0.0);//x����]
  glScalef(sc, sc, sc);

  drawElevation(nMeshT, nMeshT, sizeX, sizeY, 0, dataT);
  glPopMatrix();
}

void getDirection()
{
	//������z�̋}�s�ȕ����̃f�[�^���쐬
	//�W�����̌��z�𒲂ׁA�ł��������l�̌��z���������̓_�̕����Ƃ���
	int i, j, k, kMin;
	float grad[8];
	float min;
	float root2 = sqrt(2.0);
	int d = 3;

	for(j = d; j < nMeshT-d; j++)
		for(i = d; i < nMeshT-d; i++)
		{
			grad[0] = (dataT[j*(nMeshT + 1) + i+d] - dataT[j*(nMeshT + 1) + i])* root2;//x����
			grad[1] = dataT[(j+d)*(nMeshT + 1) + i+d] - dataT[j*(nMeshT + 1) + i];			//x,y����
			grad[2] = (dataT[(j+d)*(nMeshT + 1) + i] - dataT[j*(nMeshT + 1) + i])* root2;//y����
			grad[3] = dataT[(j+d)*(nMeshT + 1) + i-d] - dataT[j*(nMeshT + 1) + i];//-x,-y����
			grad[4] = (dataT[j*(nMeshT + 1) + i-d] - dataT[j*(nMeshT + 1) + i])* root2;//-y����
			grad[5] = dataT[(j-d)*(nMeshT + 1) + i-d] - dataT[j*(nMeshT + 1) + i];//-x,-y����
			grad[6] = (dataT[(j-d)*(nMeshT + 1) + i] - dataT[j*(nMeshT + 1) + i])* root2;//-y����
			grad[7] = dataT[(j-d)*(nMeshT + 1) + i+d] - dataT[j*(nMeshT + 1) + i] ;//x,-y����
			min = grad[0];
			kMin = 0;
			for(k = 1; k < 8; k++)
			{
				if(min > grad[k])
				{
					min = grad[k];
					kMin = k;
				}
			}
			direction[j*(nMeshT+1) + i] = kMin;
		}
}

float getHeightTerrain(float x, float z)
{
	double  xx, zz;
	int x0, z0;
	xx = (x / sizeX) * (float)nMeshT / sc;
	if(xx - floor(xx) < 0.5) x0 = (int)floor(xx);
	else x0 = (int)ceil(xx);
	coordI = nMeshT / 2 + x0;

	zz = (z / sizeY) * (float)nMeshT / sc;
	if(zz - floor(zz) < 0.5) z0 = (int)floor(zz);
	else z0 = (int)ceil(zz);
	coordJ = nMeshT / 2 - z0;
	return dataT[coordJ*(nMeshT + 1) + coordI] * sc;
}

