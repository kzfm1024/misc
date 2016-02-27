//particle.h
#include <stdlib.h>
#include <math.h>

#define NUM_PARTICLE 7000
int num0 = 30;
int countP = 0;//放出された粒子数

float g = 9.8;//重力加速度
float drag = 0.1;//空気抵抗
float e = 0.3;//跳ね返り係数
float radius = 0.001;
int frameCount;
float accelX = 0.0;//x方向の風

//一様乱数
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
  CVector vPosition; // 位置
  CVector vVelocity; // 速度
  CVector vAccel;    //加速度
  float pointSize;   //粒子のサイズ
  float createTime;  //particle発生する時刻
	float intervalTime;//particleが発生してから放出されるまでの時間
  CParticle();
  void create(float t, int nSource);
  void update(float t,float dt);
  void show();
};

CParticle::CParticle()
{
  pointSize = getNormalRandom(3.0, 0.5);
  vPosition = CVector(0.0, 0.0, 0.0);
  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, - g, 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle::create(float t, int nSource)
{
	if(nSource == 0)
	{
		vPosition = CVector(getRandom(-0.6, 0.6),getRandom(1.8, 2.0), getRandom(-2.0, -1.0));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), 0.0, getNormalRandom(1.5, 0.2));//1.5, 0.1));
	}
	if(nSource == 1)
	{
		vPosition = CVector(getNormalRandom(0.0, 0.02),getNormalRandom(1.7, 0.1), getNormalRandom(-0.65, 0.02));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(0.9, 0.1));//1.5, 0.1));
	}
	if(nSource == 2)
	{
		vPosition = CVector(getNormalRandom(-0.3, 0.02),getNormalRandom(1.6, 0.1), getNormalRandom(-0.7, 0.02));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(1.0, 0.1));//1.5, 0.1));
	}
	if(nSource == 3)
	{
		vPosition = CVector(getNormalRandom(0.3, 0.02),getNormalRandom(1.75, 0.1), getNormalRandom(-0.65, 0.02));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(1.2, 0.1));//1.5, 0.1));
	}
	if(nSource == 4)
	{
		vPosition = CVector(getNormalRandom(-0.45, 0.02),getNormalRandom(1.65, 0.1), getNormalRandom(-0.65, 0.02));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(1.2, 0.1));//1.5, 0.1));
	}
	if(nSource == 5)
	{
		vPosition = CVector(getNormalRandom(0.45, 0.02),getNormalRandom(1.6, 0.1), getNormalRandom(-0.65, 0.02));
		vVelocity = CVector(getNormalRandom(0.0, 0.1), getNormalRandom(0.0, 0.1), getNormalRandom(1.2, 0.1));//1.5, 0.1));
	}
	createTime = t;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;

  CVector accel;
  if(vPosition.z < -1.0) accel = CVector(0.0, 0.0, 0.0);
  else 
  {
		accel = vAccel;
		accel -= drag * vVelocity;
		vVelocity += accel * dt;
  }
  vPosition += vVelocity * dt;
  if(vPosition.y < radius)
  {
		vPosition.y = + radius;
		vVelocity.y = - e * vVelocity.y;
  }
}

void CParticle::show()
{
  float diffuse[] = { 0.9f, 0.9f, 1.0f, 1.0f};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,diffuse);
  glPointSize(pointSize);
  glBegin(GL_POINTS);
	glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}
