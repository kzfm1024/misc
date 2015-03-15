//lakeデータ

#define nMesh 32//最大分割数（両辺同じ）
#define NUM_WAVE 1//円形波の波源数
float period = 5.0;
float lambda = 0.5;
float amp0 = 0.1;//0.03;
float sizeX = 10;//4.0;
float sizeY = 10;//8.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//Waveの高さデータ
float alphaWave = 0.5;//水の不透明度

void drawLake()
{
  float diffuse[] = { 0.4f, 0.5f, 0.9f, alphaWave};
  float ambient[] = { 0.1f, 0.2f, 0.6f, alphaWave};
  float specular[]= { 0.8f, 0.8f, 0.8f, 1.0};

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
		a = (float)( amp0 * (sin(phase) + 0.5 * sin(3.0*phase))) / (sqrt(r)) ;
  else
    a = 0.0f;

  return( a );
}

void makeLake(float elapseTime)
{
  int i, j, k;
  float x, y, r;
  float sX[10], sY[10];//円形波の中心

  //波源
	int n = 0;
  sX[n] =  0.5; sY[n] = 5.5; n++;
  //sX[n] = -1.0; sY[n] = 6.0; n++;
  //sX[n] =  1.0; sY[n] = 6.5; n++;
//  sX[n] =  0.5; sY[n] = 0.0; n++;
//  sX[n] = -0.5; sY[n] = -0.2; n++;

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
}

