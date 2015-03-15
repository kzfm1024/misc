//noise2.h

#include <stdlib.h>
#include <math.h>
#define NOISE_SIZE 128

//Catmull-Romスプラインによる補間
double CmR(double p0, double p1, double p2, double p3, double t)
{
  double a = (p3 - p0) * 0.5 + (p1 - p2) * 1.5;
  double b =  p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;

  return ((a * t + b)* t + (p2 - p0) * 0.5) * t + p1;
}

class CNoise1
{
private:
  int L;//x方向格子点個数
  int size;//基本区間のテクセル数
  int space;//格子点間テクセル数
  float p[9];//格子点におけるノイズの値
public:
  float f[NOISE_SIZE];//基本区間のノイズ値
  CNoise1();
  double noise0(int x);
};

CNoise1::CNoise1()
{
  L = 9;
  size = NOISE_SIZE;
  space = size / (L-1);
  int i;
  for (i = 0; i < L; i++) p[i] = (double)rand() / (double)RAND_MAX - 0.5;
  p[L-1] = p[0];//始点と終点を同じ値として不連続点のない周期関数とする
	//基本区間の値を求める
  for(i = 0; i < size; i++) f[i] = noise0(i);
}

double CNoise1::noise0(int x)
{
  x = (int)fmod((double)x , (double)size);//基本区間に入るように
  int i = x / space;//格子点番号
  double xx = (double)(x - (i * space));
  double t = xx / (double)space; 

  if (i == 0) {
    return CmR(p[L - 1], p[0], p[1], p[2], t);
  }
  else if (i < L - 2) {
    return CmR(p[i - 1], p[i], p[i + 1], p[i + 2], t);
  }
  else //(i = L - 2)
  {
    return CmR(p[L - 3], p[L - 2], p[L - 1], p[0], t);
  }
}

//----------------------------------------------------------------------
class CNoise2
{
private:
  int M;//y方向格子点個数
  int size;
  int space;
  CNoise1 ns1[9] ;
public:
  float g[NOISE_SIZE][NOISE_SIZE];
  CNoise2();
  double noise0(int x, int y);
};

CNoise2::CNoise2()
{
  M = 9;
  size = NOISE_SIZE;
  space = size / (M-1);
  int i, j;
  for (int i = 0; i < M; i++) ns1[i] = CNoise1();
  ns1[M-1] = ns1[0];
  for(j = 0; j < size; j++)
		for(i = 0; i < size; i++) g[i][j] = noise0(i, j);
}

double CNoise2::noise0(int x, int y)
{
  y = (int)fmod((double)y , (double)size);
  int j = y / space;//格子点番号
  double yy = (double)(y - (j * space));
  double t = yy / (double)space; 

  if (j == 0) {
    return CmR(ns1[M - 1].f[x], ns1[0].f[x], ns1[1].f[x], ns1[2].f[x], t);
  }
  else if (j < M - 2) {
    return CmR(ns1[j - 1].f[x], ns1[j].f[x], ns1[j + 1].f[x], ns1[j + 2].f[x], t);
  }
  else// (j = M - 2)
  {
    return CmR(ns1[M - 3].f[x], ns1[M - 2].f[x], ns1[M - 1].f[x], ns1[0].f[x], t);
  }
}

//---------------------------------------------------------------------------------
class CNoise3
{
private:
  int N;//z方向格子点個数
  int size;
  int space;
  CNoise2 ns2[9] ;
public:
  CNoise3();
	double val[4];
  double noise0(int x, int y, int z);
  void calc(int x, int y, int z);
};

CNoise3::CNoise3()
{
  N = 9;
  size = NOISE_SIZE;
  space = size / (N-1);
  for (int k = 0; k < N; k++) ns2[k] = CNoise2();
  ns2[N-1] = ns2[0];
}


double CNoise3::noise0(int x, int y, int z)
{
  z = (int)fmod((double)z , (double)size);
  int k = z / space;//格子点番号
  double zz = (double)(z - (k * space));
  double t = zz / (double)space; 
  if (k == 0) {
    return CmR(ns2[N - 1].g[x][y], ns2[0].g[x][y], ns2[1].g[x][y], ns2[2].g[x][y], t);
  }
  else if (k < N - 2) {
    return CmR(ns2[k - 1].g[x][y], ns2[k].g[x][y], ns2[k + 1].g[x][y], ns2[k + 2].g[x][y], t);
  }
  else //(k = N - 2) 
  {
    return CmR(ns2[N - 3].g[x][y], ns2[N - 2].g[x][y], ns2[N - 1].g[x][y], ns2[0].g[x][y], t);
  }
}

void CNoise3::calc(int x, int y, int z)
{
  int xx, yy, zz;
  int x2 = x;
  int y2 = y;
  int z2 = z;
  val[0] = noise0(x, y, z);//基本波成分
  double w = 0.5;

  for(int oct = 1; oct < 4; oct++)
  {
		x2 *= 2; y2 *= 2; z2 *= 2;
		xx = (int)fmod((double)x2, (double)size);
		yy = (int)fmod((double)y2, (double)size);
		zz = (int)fmod((double)z2, (double)size);
    val[oct] = noise0(xx, yy, zz) * w;
    w *= 0.5;
  }
}
