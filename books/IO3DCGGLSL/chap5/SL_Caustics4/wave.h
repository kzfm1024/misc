//waveƒf[ƒ^
#define nMesh 128//Å‘å•ªŠ„”i—¼•Ó“¯‚¶,128‚ªÅ‘åj
int numWave = 1;//‰~Œ`”g‚Ì”gŒ¹”iÅ‘å3j
int kindWave = 0;//‰~Œ`”g
float waveH = 5.0;//”g‚ÌˆÊ’u‚Ì‚‚³
float period = 1.0;
float lambda = 0.6;
float sizeX = 10.0;
float sizeY = 10.0;
float meshX = sizeX / (float)nMesh;
float meshY = sizeY / (float)nMesh;
float data[(nMesh+1)*(nMesh+1)];//Wave‚Ì‚‚³ƒf[ƒ^

void drawWave()
{
	float diffuse[] = { 0.2f, 0.3f, 0.4f, 0.02f};
	float ambient[] = { 0.1f, 0.1f, 0.2f, 0.02f};
	float specular[]= { 0.2f, 0.2f, 0.3f, 0.02f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glPushMatrix();
  glTranslatef(0.0, waveH, 0.0);
  glRotatef(-90.0, 1.0, 0.0, 0.0);//x²‰ñ“](y²‚ğ‰”’¼²‚É‚·‚é‚½‚ß)
	glScalef(1.0, 1.0, 0.03);
  drawElevation(nMesh, nMesh, sizeX, sizeY, 0, data);
  glPopMatrix();
}

float getWave(float r, float t, int k)
{
    float phase;
    float a;

    if(r < 0.05) r = 0.05;
    phase = 2.0 * M_PI * ( t / period - r / lambda);

    if(phase >= 0.0) 
	   a = (float)(sin(phase) + 0.6*sin(2.0*phase) + 0.5 * sin(5.0*phase)) / (sqrt(r)) ;
    else
      a = 0.0f;
 
    return( a );
}

void makeWaveCircle(float amp, float t)
{
  int i, j, k;
  float x, y, r;
  float sX[3], sY[3];//‰~Œ`”g‚Ì’†S

  //”gŒ¹
  sX[0] =  0.0; sY[0] = 0.0;
  sX[1] = -1.0; sY[1] = 1.2;
  sX[2] =  1.0; sY[2] = 1.4;

  //Clear Wave
  for(j = 0; j <= nMesh; j++)
      for(i = 0; i <= nMesh; i++)
          data[j * (nMesh + 1) + i] = 0.0;

  for(k = 0; k < numWave; k++)
  {
      for(j = 0; j <= nMesh; j++)
      {
          y = (float)( - nMesh / 2 + j) * meshY - sY[k];
          for(i = 0; i <= nMesh; i++)
          {
              x = (float)( - nMesh / 2 + i) * meshX - sX[k];
              r = sqrt( x * x + y * y);
              data[j * (nMesh + 1) + i] += amp * getWave(r, t, k);
          }
      }
  }
}

void makeWavePlane(float amp, float t)
{
  int i, j;
  float x, y;
	double pp = 2.0 * M_PI;

	float lambdaX = lambda;
	float lambdaY = lambda * 5.0;

  for(j = 0; j <= nMesh; j++)
  {
      y = (float)( - nMesh / 2 + j) * meshY;
      for(i = 0; i <= nMesh; i++)
      {
          x = (float)( - nMesh / 2 + i) * meshX ;
          
					data[j * (nMesh + 1) + i] = amp * (
						 sin(pp * (t/period - x/lambdaX - y/lambdaY ) + 1.0*sin(pp * x))
						 + sin(pp * (t/period + x/lambdaY - y/lambdaX) + 0.5*sin(pp * y))
						 + 0.5 * sin(pp * (2.0*t/period - x/lambdaX - y/lambdaY))
						 + 0.1 * sin(pp * (3.0*t/period - x/lambdaX - y/lambdaY))
						 );
      }
  }
}

void makeWaveMix(float amp, float t)
{
  int i, j, k;
  float x, y, r;
  float sX[3], sY[3];//‰~Œ`”g‚Ì’†S
	double pp = 2.0 * M_PI;

	float lambdaX = lambda;
	float lambdaY = lambda * 5.0;

  //”gŒ¹
  sX[0] =  0.0; sY[0] = 0.0;
  sX[1] = -1.0; sY[1] = 1.2;
  sX[2] =  1.0; sY[2] = 1.4;

  //Clear Wave
  for(j = 0; j <= nMesh; j++)
      for(i = 0; i <= nMesh; i++)
          data[j * (nMesh + 1) + i] = 0.0;

	for(k = 0; k < numWave; k++)
  {
      for(j = 0; j <= nMesh; j++)
      {
          y = (float)( - nMesh / 2 + j) * meshY - sY[k];
          for(i = 0; i <= nMesh; i++)
          {
              x = (float)( - nMesh / 2 + i) * meshX - sX[k];
              r = sqrt( x * x + y * y);
              data[j * (nMesh + 1) + i] += amp * getWave(r, t, k);
          }
      }
  }

	//•½–Ê”g‚ğ’Ç‰Á
		for(j = 0; j <= nMesh; j++)
		{
      y = (float)( - nMesh / 2 + j) * meshY;
      for(i = 0; i <= nMesh; i++)
      {
          x = (float)( - nMesh / 2 + i) * meshX ;
          
					data[j * (nMesh + 1) + i] += amp * (
						 sin(pp * (t/period - x/lambdaX - y/lambdaY ) + 1.0*sin(pp * x))
						 );
      }
		}


}



