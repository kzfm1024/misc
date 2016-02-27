//�Q�����̒��_�ψʖ@
#include <stdlib.h>
#include <math.h>

//���K����
float getNormalRandom( double sigma )
{
  double ran = 0.0;
  for(int i=0; i < 12; i++) ran += (double)rand() / (double)RAND_MAX;
  
  ran -= 6.0;
  ran *= sigma;
  return (float)ran;
}

class CMidpoint2
{
public:
  int numDiv;//����������
  int numMax;//�ő啪����
  double sigma0;//�W���΍������l
  double beta;//fBm�̎w��
  float h[130][130];//�����̏����l
  CMidpoint2();
//  ~CMidpoint2() { };
  void create(int seed, float* data);
};

CMidpoint2::CMidpoint2()
{/*
  numDiv = 1;
  numMax = 128;
  sigma0 = 0.5;
  beta = 2.0;
  h[0][0] = getNormalRandom(sigma0);
  h[1][0] = getNormalRandom(sigma0);
  h[0][1] = getNormalRandom(sigma0);
  h[1][1] = getNormalRandom(sigma0);*/
}

void CMidpoint2::create(int seed, float *data)
{
  int i, j, ii, jj;
  float f[130][130];
  double hurst = (beta-1.0) / 2.0;
	double sigma = sigma0;

  srand(0);srand(seed);
//	srand(1);
//printf("seed=%d \n", seed);
  while(numDiv <= numMax / 2)
  {
		sigma /= pow(2.0, hurst);
		numDiv *= 2;//������
	  	
		for(jj = 0; jj <= numDiv; jj++)
		{
			for(ii = 0 ; ii <= numDiv; ii++)
			{
				j = jj / 2;
				i = ii / 2;
				if(j*2 == jj && i*2 == ii) //jj=even, ii=even
				{
					f[ii][jj] = h[i][j];//�m��l
				}

				else if(j*2 == jj && i*2 != ii)//jj=even, ii=odd
				{
					f[ii][jj] = (h[i][j] + h[i+1][j]) / 2.0 + getNormalRandom(sigma);
				}
				else if(j*2 != jj && i*2 == ii)//jj=od, ii=even
				{
					f[ii][jj] = (h[i][j] + h[i][j+1]) / 2.0 + getNormalRandom(sigma);
				}
				else //jj=odd, ii=odd
				{
					f[ii][jj] = (h[i][j] + h[i+1][j] + h[i][j+1] + h[i+1][j+1]) / 4.0 
						+ getNormalRandom(sigma);
				}
			}
		}
		for(j = 0; j <= numDiv; j++) 
			for(i = 0; i <= numDiv; i++) h[i][j] = f[i][j];
  }
  for(j = 0; j <= numDiv; j++) 
		for(i = 0; i <= numDiv; i++) 
		{
			data[j * (numMax+1) + i] = f[i][j];
		}
}

//---------------------------------------------------------------------------------------

class CMidpoint1
{
public:
  int numDiv;//����������
  int numMax;//�ő啪����
  double sigma0;//�W���΍������l
  double beta;//fBm�̎w��
  float h[1001];//�����l
  CMidpoint1();
  ~CMidpoint1() { };
  void create(int seed, float* data);
};

CMidpoint1::CMidpoint1()
{
  numDiv = 1;
  numMax = 1000;
  sigma0 = 0.5;
  beta = 2.0;
  h[0] = getNormalRandom(sigma0);
  h[1] = getNormalRandom(sigma0);
}

void CMidpoint1::create(int seed, float *data)
{
  int i, ii;
  float f[1001];
  double hurst = (beta-1.0) / 2.0;
	double sigma = sigma0;

  srand(seed);
  while(numDiv <= numMax / 2)
  {
		sigma /= pow(2.0, hurst);
		numDiv *= 2;//������
	  	
		for(ii = 0 ; ii <= numDiv; ii++)
		{
			i = ii / 2;
			if(i*2 == ii) //ii=even
			{
				f[ii] = h[i];//�m��l
			}

			else//ii=odd
			{
				f[ii] = (h[i] + h[i+1]) / 2.0 + getNormalRandom(sigma);
			}
		}
		for(i = 0; i <= numDiv; i++) h[i] = f[i];//���̕����ɑ΂��鏉���l
  }
  for(i = 0; i <= numDiv; i++) data[i] = f[i];
}
