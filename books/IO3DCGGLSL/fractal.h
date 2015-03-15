//Fractal.h
//テクスチャパターンの指定
#define MANDEL1 0
#define MANDEL2 1
#define JULIA1 2 
#define JULIA2 4

struct Color
{
  unsigned char r, g, b;
};

class CFractal
{
public:
  int numRepeat;//最大繰り返し回数
  int widthX, widthY;
  double posX, posY;
  double scale;
  double initX, initY;//初期値（Mandelbrot)
  double constA, constB;//定数（Julia) 
  Color col[255];
  CFractal();
  void mandel(GLubyte image[][3]);
  void julia(GLubyte image[][3]);
  void mandelTerrain(float data[]);
  void juliaTerrain(float data[]);
};

CFractal::CFractal()
{
  numRepeat = 100;
  widthX = widthY = 32;
  posX = posY = 0.0;
  initX = initY = 0.0;
  constA = 0.0;
  constB = 0.0;
  scale = 1.5;
  //配色
  for(int i = 0; i <= 255; i++)
  {
		if(i < 5){ col[i].r = 0; col[i].g = 0; col[i].b = 255;}
		else if(i < 50) {col[i].r = 0; col[i].g = 5*i; col[i].b = 255-5*i;}
		else if(i < 100) {col[i].r = 5*(i-50); col[i].g = 0; col[i].b = 255-5*(i-50);}
		else if(i < 150) {col[i].r = 255-5*(i-100); col[i].g = 5*(i-100); col[i].b = 0;}
		else if(i < 200) {col[i].r = 5*(i-150); col[i].g = 0; col[i].b = 255-5*(i-150);}
		else if(i < 255) {col[i].r = 220-4*(i-200); col[i].g = 4*(i-200); col[i].b = 0;}
		else {col[i].r = 255; col[i].g = 255; col[i].b =255;}
  }
}

void CFractal::mandel(GLubyte image[][3])
{
  int i, j, n;
  int cc;
  double x, y, u, v, a, b, sum;

  for (j = -widthY; j < widthY; j++)
  {
    b = scale * (double)j / (double)widthY + posY;
    for (i = -widthX; i < widthX; i++)
    {
	  a = scale * (double)i / (double)widthX + posX;
      x = initX ;
      y = initY ;
      sum = 0.0;
      for (n = 1; n <= numRepeat; n++)
      {
          u = x * x - y * y + a;//実部
          v = 2.0 * x * y + b;//虚部
          x = u; y = v;
          sum = x * x + y * y;
          if (sum >= 10.0) break;//終了条件
      }
      cc = (int)(255.0 * (double)n / (double)numRepeat);
      if (cc > 255) cc = 255;
      image[(widthY+j)*(2*widthX) + widthX+i][0] = col[cc].r;
      image[(widthY+j)*(2*widthX) + widthX+i][1] = col[cc].g;
      image[(widthY+j)*(2*widthX) + widthX+i][2] = col[cc].b;
    }
  }
}

void CFractal::julia(GLubyte image[][3])
{
  int i, j, n;
  int cc;
  double x, y, xx, yy, u, v, sum;

  for (j = -widthY; j < widthY; j++)
  {
    yy = scale * (double)j / (double)widthY + posY;
    for (i = -widthX; i < widthX; i++)
    {
	  xx = scale * (double)i / (double)widthX + posX;
      x = xx ;
      y = yy ;
      sum = 0.0;
      for (n = 1; n <= numRepeat; n++)
      {
          u = x * x - y * y + constA;//実部
          v = 2.0 * x * y + constB;//虚部
          x = u; y = v;
          sum = x * x + y * y;
          if (sum >= 10.0) break;//終了条件
      }
      cc = (int)(255.0 * (double)n / (double)numRepeat);
      if (cc > 255) cc = 255;
      image[(widthY+j)*(2*widthX) + widthX+i][0] = col[cc].r;
      image[(widthY+j)*(2*widthX) + widthX+i][1] = col[cc].g;
      image[(widthY+j)*(2*widthX) + widthX+i][2] = col[cc].b;
    }
  }
}

void CFractal::mandelTerrain(float* data)
{
  int i, j, n;
  double x, y, u, v, a, b, sum;

  for (j = -widthY; j <= widthY; j++)
  {
    b = scale * (double)j / (double)widthY + posY;
    for (i = -widthX; i <= widthX; i++)
    {
	  a = scale * (double)i / (double)widthX + posX;
      x = initX ;
      y = initY ;
      sum = 0.0;
      for (n = 1; n <= numRepeat; n++)
      {
          u = x * x - y * y + a;//実部
          v = 2.0 * x * y + b;//虚部
          x = u; y = v;
          sum = x * x + y * y;
          if (sum >= 10.0) break;//終了条件
      }
      data[(widthY+j)*(2*widthX+1) + widthX+i]  = (float)n / (float)numRepeat;
    }
  }
}

void CFractal::juliaTerrain(float* data)
{
  int i, j, n;
  double x, y, xx, yy, u, v, sum;

  for (j = -widthY; j < widthY; j++)
  {
    yy = scale * (double)j / (double)widthY + posY;
    for (i = -widthX; i < widthX; i++)
    {
	  xx = scale * (double)i / (double)widthX + posX;
      x = xx ;
      y = yy ;
      sum = 0.0;
      for (n = 1; n <= numRepeat; n++)
      {
          u = x * x - y * y + constA;//実部
          v = 2.0 * x * y + constB;//虚部
          x = u; y = v;
          sum = x * x + y * y;
          if (sum >= 10.0) break;//終了条件
      }
      data[(widthY+j)*(2*widthX+1) + widthX+i]  = (float)n / (float)numRepeat;
    }
  }
}
