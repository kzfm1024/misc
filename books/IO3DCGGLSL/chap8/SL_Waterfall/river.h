//riverデータ

#define nMesh 32//最大分割数（両辺同じ）
#define NUM_WAVE 3//円形波の波源数
float period = 1.0;
float lambda = 1.0;
float amp0 = 0.02;
float sizeX = 2.0;
float sizeY = 12.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//Waveの高さデータ
float alphaW = 0.7;//水の不透明度

void drawRiver()
{
  float diffuse[] = { 0.4f, 0.5f, 0.7f, alphaW};
  float ambient[] = { 0.1f, 0.2f, 0.3f, alphaW};
  float specular[]= { 0.5f, 0.5f, 0.5f, alphaW};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glPushMatrix();
  glTranslatef(0.0, 0.0, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);//x軸回転
  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

float getWave(float r, float elapseTime, int k)
{
  float phase;
  float a;

  if(r < 0.01) r = 0.01;
  phase = 2.0 * M_PI * ( elapseTime /period - r / lambda);

  if(phase >= 0.0) 
		a = (float)( amp0 * (sin(phase)));// + 0.6*sin(2.0*phase) + 0.5 * sin(5.0*phase)) ) / (sqrt(r)) ;
  else
    a = 0.0f;

  return( a );
}

void makeRiver(float elapseTime)
{
  int i, j, k;
  float x, y, r;
  float sX[10], sY[10];//円形波の中心

  //波源
	int n = 0;
  sX[n] =  0.0; sY[n] = 5.5; n++;
  //sX[n] = -1.0; sY[n] = 6.0; n++;
  //sX[n] =  1.0; sY[n] = 6.5; n++;
  sX[n] =  0.5; sY[n] = 0.0; n++;
  sX[n] = -0.5; sY[n] = -0.2; n++;

  //Clear Wave
  for(j = 0; j <= nMesh; j++)
      for(i = 0; i <= nMesh; i++)
          data[j * (nMesh + 1) + i] = -0.05;

  for(k = 0; k < NUM_WAVE; k++)
  {
    for(j = 0; j <= nMesh; j++)
    {
      y = (float)( - nMesh / 2 + j) * meshY - sY[k];
			if( y < 0)
			{
				for(i = 0; i <= nMesh; i++)
				{
						x = (float)( - nMesh / 2 + i) * meshX - sX[k];
						r = sqrt( x * x + y * y);
						data[j * (nMesh + 1) + i] += getWave(r, elapseTime, k);
				}
			}
    }
  }
	//地形に合わせて持ち上げる
  for(j = 0; j <= nMesh; j++)
  {
    for(i = 0; i <= nMesh; i++)
    {
			if(j == nMesh / 2.0+1) data[j * (nMesh + 1) + i] += 1.0;
			if(j == nMesh / 2.0+2) data[j * (nMesh + 1) + i] += 1.9;
			if(j >= nMesh / 2.0+3) data[j * (nMesh + 1) + i] += 2.0;
    }
  }
}

