//�n�`�f�[�^
#define nMeshT 128//�ő啪�����i���ӓ����j
float dataT[(nMeshT+1)*(nMeshT+1)];//�����f�[�^
double beta = 2.5;//fBm�̎w����
double sigma0 = 0.1;//�W���΍������l
int seed = 1;//�����̼���
//�n�`affine�ϊ��p�ϐ�
float pos[] = { 0.0, 0.0, 0.0 };//���S��xyz���W
float scale[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
float angle[] = { 0.0, 0.0, 0.0 };//��]�p�x

void makeTerrainData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 4;
  mp.numMax = nMeshT;
  mp.sigma0 = sigma0;
  mp.beta = beta;

  //�������x�f�[�^
/*
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			if(i == 0 || j == 0 || i == 4 || j == 4) mp.h[i][j] = 0.2;
			else if(i == 1) mp.h[i][j] = 0.1;
			else if(i == 2) mp.h[i][j] = 1.0;
			else if(i == 3) mp.h[i][j] = 0.3;
			else mp.h[i][j] = 0.2;//0.9;
		}

 */
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			if(j == 0) mp.h[i][j] = 0.0;
			else if(i == 0 && j == 1) mp.h[i][j] = 0.1;
			else if(i == 1 && j == 1) mp.h[i][j] = 0.6;
			else if(i == 2 && j == 1) mp.h[i][j] = 0.9;
			else if(i == 3 && j == 1) mp.h[i][j] = 0.5;
			else if(i == 4 && j == 1) mp.h[i][j] = 0.1;
			else if(i == 0 && j == 2) mp.h[i][j] = 0.2;
			else if(i == 1 && j == 2) mp.h[i][j] = 0.8;
			else if(i == 2 && j == 2) mp.h[i][j] = 1.5;
			else if(i == 3 && j == 2) mp.h[i][j] = 0.8;
			else if(i == 4 && j == 2) mp.h[i][j] = 0.2;
			else if(i == 0 && j == 3) mp.h[i][j] = -0.1;
			else if(i == 1 && j == 3) mp.h[i][j] = 0.8;
			else if(i == 2 && j == 3) mp.h[i][j] = 0.7;
			else if(i == 3 && j == 3) mp.h[i][j] = 0.7;
			else if(i == 4 && j == 3) mp.h[i][j] = 0.1;
			else if(i == 0 && j == 4) mp.h[i][j] = -0.1;
			else if(i == 1 && j == 4) mp.h[i][j] = -0.1;
			else if(i == 2 && j == 4) mp.h[i][j] = 0.1;
			else if(i == 3 && j == 4) mp.h[i][j] = 0.1;
			else if(i == 4 && j == 4) mp.h[i][j] = 0.0;
		}
/*
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			if(i == 0 && j == 0) mp.h[i][j] = 0.3;
			else if(i == 1 && j == 0) mp.h[i][j] = 0.2;
			else if(i == 2 && j == 0) mp.h[i][j] = 0.3;
			else if(i == 3 && j == 0) mp.h[i][j] = 0.2;
			else if(i == 4 && j == 0) mp.h[i][j] = 0.1;
			else if(i == 0 && j == 1) mp.h[i][j] = 0.4;
			else if(i == 1 && j == 1) mp.h[i][j] = 0.9;
			else if(i == 2 && j == 1) mp.h[i][j] = 0.5;
			else if(i == 3 && j == 1) mp.h[i][j] = 0.9;
			else if(i == 4 && j == 1) mp.h[i][j] = 0.3;
			else if(i == 0 && j == 2) mp.h[i][j] = 0.6;
			else if(i == 1 && j == 2) mp.h[i][j] = 1.2;
			else if(i == 2 && j == 2) mp.h[i][j] = 1.5;
			else if(i == 3 && j == 2) mp.h[i][j] = 0.9;
			else if(i == 4 && j == 2) mp.h[i][j] = 0.7;
			else if(i == 0 && j == 3) mp.h[i][j] = 0.8;
			else if(i == 1 && j == 3) mp.h[i][j] = 1.1;
			else if(i == 2 && j == 3) mp.h[i][j] = 0.9;
			else if(i == 3 && j == 3) mp.h[i][j] = 1.2;
			else if(i == 4 && j == 3) mp.h[i][j] = 0.6;
			else if(i == 0 && j == 4) mp.h[i][j] = 0.6;
			else if(i == 1 && j == 4) mp.h[i][j] = 1.1;
			else if(i == 2 && j == 4) mp.h[i][j] = 1.0;
			else if(i == 3 && j == 4) mp.h[i][j] = 1.2;
			else if(i == 4 && j == 4) mp.h[i][j] = 0.9;
		}

/*
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++)
		{
			if(i == 0 && j == 0) mp.h[i][j] = -0.2;
			else if(i == 1 && j == 0) mp.h[i][j] = 0.0;
			else if(i == 2 && j == 0) mp.h[i][j] = 0.1;
			else if(i == 3 && j == 0) mp.h[i][j] = 0.2;
			else if(i == 4 && j == 0) mp.h[i][j] = 0.1;
			else if(i == 0 && j == 1) mp.h[i][j] = 0.1;
			else if(i == 1 && j == 1) mp.h[i][j] = 0.4;
			else if(i == 2 && j == 1) mp.h[i][j] = 0.6;
			else if(i == 3 && j == 1) mp.h[i][j] = 0.5;
			else if(i == 4 && j == 1) mp.h[i][j] = 0.3;
			else if(i == 0 && j == 2) mp.h[i][j] = 0.6;
			else if(i == 1 && j == 2) mp.h[i][j] = 1.3;
			else if(i == 2 && j == 2) mp.h[i][j] = 1.2;
			else if(i == 3 && j == 2) mp.h[i][j] = 1.6;
			else if(i == 4 && j == 2) mp.h[i][j] = 0.7;
			else if(i == 0 && j == 3) mp.h[i][j] = 0.8;
			else if(i == 1 && j == 3) mp.h[i][j] = 1.1;
			else if(i == 2 && j == 3) mp.h[i][j] = 0.9;
			else if(i == 3 && j == 3) mp.h[i][j] = 1.2;
			else if(i == 4 && j == 3) mp.h[i][j] = 0.6;
			else if(i == 0 && j == 4) mp.h[i][j] = 0.6;
			else if(i == 1 && j == 4) mp.h[i][j] = 1.1;
			else if(i == 2 && j == 4) mp.h[i][j] = 1.0;
			else if(i == 3 && j == 4) mp.h[i][j] = 1.2;
			else if(i == 4 && j == 4) mp.h[i][j] = 0.9;
		}
*/
  mp.create(seed, dataT);//��1�����͗�����seed�i�l��ς���ƈقȂ����p�^�[�����쐬�j
/*
	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//���Ȃ��0�Ƃ���
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}*/
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
  glRotatef(angle[2], 0.0, 0.0, 1.0);//z����]
  glRotatef(angle[1], 0.0, 1.0, 0.0);//y����]
  glRotatef(angle[0]-90.0, 1.0, 0.0, 0.0);//x����]
  glScalef(scale[0], scale[1], scale[2]);

  drawElevation(nMeshT, nMeshT, 4.0, 4.0, 0, dataT);
  glPopMatrix();
}
