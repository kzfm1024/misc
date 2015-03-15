//particle.h
#include <stdlib.h>
#include <math.h>
#define NUM_PARTICLE 30000
int num0 = 100;

int countP = 0;//放出された粒子数

float g = 0.0;//重力加速度
float drag = 1.0;//空気抵抗
float e = 0.0;//跳ね返り係数
float hCloud = 2.5;//雲の高さ
float accelX = 0.0;

float getRandom(float fMin, float fMax)
{
  return fMin + (fMax - fMin) * (float)rand() / (float)RAND_MAX;
}

//正規乱数
float getNormalRandom(float mean, float sigma)
{
  double ran = 0.0;
  for(int i=0; i < 12; i++) ran += (double)rand() / (double)RAND_MAX;
  ran -= 6.0;
  ran *= sigma;
  return mean + (float)ran;
}

class CParticle {
public:
  bool flagProject;
  CVector vPosition; // 位置
  CVector vVelocity; // 速度
  CVector vAccel;    //加速度
  float pointSize;   //粒子のサイズ
	float createTime;  //発生した時刻
	float intervalTime;//放出するまでの間隔
	float minHeight;   //雲の高さ最低値
	float maxHeight;   //雲の高さ最高値
	float midHeight;   //雲の高さ中間値
	float heightT;//particleの位置（x,z)に対するterrainの高さ
	bool inside;  //terrainの内外判定
  CParticle();
  CParticle(CVector vPosition0, CVector vVelocity0, CVector vAccel0);
  void create(float t);
  void update(float t, float dt);
  void show();
};

CParticle::CParticle()
{
  pointSize = getNormalRandom(5.0, 2.0);
	float ran = getRandom(0.0, 1.0);
	if(ran < 0.3)
	{
		minHeight = hCloud - 1.2;
		maxHeight = hCloud - 0.2;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(-4.0, 0.5), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-2.0, 0.5));
	}
	else if(ran < 0.7)
	{
		minHeight = hCloud - 1.0;
		maxHeight = hCloud - 0.5;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(0.0, 0.3), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-3.0, 0.5));
	}
	else
	{
		minHeight = hCloud - 0.6;
		maxHeight = hCloud + 0.2;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(4.0, 0.5), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-2.0, 0.5));
	}

  vVelocity = CVector(getNormalRandom(0.0, 0.0), 
		                  getNormalRandom(0.0, 0.0),
											getNormalRandom(0.0, 0.0));
  vAccel = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(0.5, 0.05));
	if(vAccel.z < 0.0) vAccel.z = - vAccel.z;
	createTime = 0.0;
	intervalTime = getRandom(0, 3.0);
}

void CParticle::create(float t)
{
	if(t < 10.0) return;
  pointSize = getNormalRandom(5.0, 2.0);
	float ran = getRandom(0.0, 1.0);
	if(ran < 0.3)
	{
		minHeight = hCloud - 0.4;
		maxHeight = hCloud - 0.2;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(-4.0, 0.5), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-2.0, 0.5));
	}
	else if(ran < 0.6)
	{
		minHeight = hCloud - 1.0;
		maxHeight = hCloud - 0.5;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(0.0, 0.3), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-3.0, 0.5));
	}
	else
	{
		minHeight = hCloud - 0.6;
		maxHeight = hCloud + 0.2;
		midHeight = (minHeight + maxHeight) / 2.0;
		vPosition = CVector(getNormalRandom(4.0, 0.5), 
								midHeight + getNormalRandom(0.0, 0.1), 
								getNormalRandom(-2.0, 0.5));
	}

  vVelocity = CVector(getNormalRandom(0.0, 0.0), 
		                  getNormalRandom(0.0, 0.0),
											getNormalRandom(0.0, 0.0));
  vAccel = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(0.1, 0.05));
	if(vAccel.z < 0.0) vAccel.z = - vAccel.z;
	createTime = 0.0;
	intervalTime = getRandom(0, 3.0);
}

void CParticle::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;
  CVector accel = vAccel ;//重力加速度と外力は常に存在
  accel -= drag * vVelocity;//空気抵抗
  vVelocity += accel * dt;//速度の更新
  //位置の更新
	vPosition += vVelocity * dt;
	//高さ制限
	if(vPosition.y > maxHeight) 
	{
		if(vAccel.y > 0.0) vAccel.y = -vAccel.y;
		//vPosition = minHeight;
	} 
	if(vPosition.y < minHeight) 
	{
		if(vAccel.y < 0.0) vAccel.y = -vAccel.y;
	} 

	//大地に到達
	if(inside == true && vPosition.y < heightT  + 0.2)
	{	
		vPosition.y = heightT + 0.2;
	}
}

void CParticle::show()
{
	float light;

	glDisable(GL_LIGHTING);
	if(vPosition.y < minHeight) glColor4f(0.5, 0.5, 0.5, 1.0);
	else if(vPosition.y < maxHeight)
	{
		light = 0.5 + (vPosition.y - minHeight) / (maxHeight - minHeight);
		glColor4f(light, light, light, 1.0);
	}
	else glColor4f(1.0, 1.0, 1.0, 1.0);

//	if(vPosition.y > midHeight) glColor4f(1.0, 1.0, 1.0, 1.0);
//	else glColor4f(0.5, 0.5, 0.5, 1.0);
  glPointSize(pointSize);
  glBegin(GL_POINTS);
	glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}


