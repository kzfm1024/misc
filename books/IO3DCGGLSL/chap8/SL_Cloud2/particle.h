//particle.h
#include <stdlib.h>
#include <math.h>
#define NUM_PARTICLE 10000
int num0 = 1;

int countP = 0;//放出された粒子数

float g = 0.0;//重力加速度
float drag = 1.0;//空気抵抗
float e = 0.2;//跳ね返り係数
float hCloud = 10.0;//雲の高さ
float accelX = 0.0; //x軸方向風力
float r0 = sizeYC / 2.0;//ドームの半径
//float viewX, viewZ;//視線の方向

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
	float heightT;     //particleの位置（x,z)に対するterrainの高さ
	float maxHeight;   //雲高さ最高値
	float minHeight;   //雲高さ最低値
	bool inside;       //terrainの内外判定
  CParticle();
  CParticle(CVector vPosition0, CVector vVelocity0, CVector vAccel0);
  void create(float t);
  void update(float t, float dt);
  void show();
};

CParticle::CParticle()
{
	int i, j;
  vPosition = CVector(getRandom(-sizeXC/2, sizeXC/2), hCloud , getRandom(-sizeYC/2, sizeYC/2));
 	i = (int)((float)(vPosition.x + sizeXC/2) * (float)nMeshC / (float)sizeXC);
	j = (int)((float)(vPosition.z + sizeYC/2) * (float)nMeshC / (float)sizeYC);
	float c = dataC[j*(nMeshC + 1) + i] ;//粒子の存在確率
	//遠方を低くする(ドーム型）	
	float x = vPosition.x;
	float z = vPosition.z;
	float r = sqrt(r0 * r0 - x * x - z * z);
	minHeight = hCloud * r / r0 -2.0 + getNormalRandom(0.0, 1.0); 
	maxHeight = minHeight + 4.0 + getNormalRandom(0.0, 1.0);
	if(c > 0.0)
	{
		pointSize = 50.0 + c * 200.0;
		vPosition.y = getRandom(minHeight, maxHeight);
	}
	else { vPosition.x = 100000.0; }//視界外へ

  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, getNormalRandom(0.0, 0.005), 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0, 3.0);
}

void CParticle::create(float t)
{

	int i, j;
  pointSize = getRandom(1.0, 100.0);
  vPosition = CVector(getRandom(-sizeXC/2, sizeXC/2), hCloud , getRandom(-sizeYC/2, sizeYC/2));
 	i = (int)((float)(vPosition.x + sizeXC/2) * (float)nMeshC / (float)sizeXC);
	j = (int)((float)(vPosition.z + sizeYC/2) * (float)nMeshC / (float)sizeYC);
	float c = dataC[j*(nMeshC + 1) + i] ;
	//遠方を低くする(ドーム型）	
	float x = vPosition.x;
	float z = vPosition.z;
	float r = sqrt(r0 * r0 - x * x - z * z);
	minHeight = hCloud * r / r0 -2.0; 
	maxHeight = minHeight + 4.0;
	if(c > 0.0)
	{
		pointSize = 30.0 + c * 100.0;
		vPosition.y = getRandom(minHeight, maxHeight);
	}
	else { vPosition.x = 100000.0; }//視界外

  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, getNormalRandom(0.0, 0.005), 0.0);

  intervalTime = getRandom(0.0, 3.0);
	createTime = t;

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
	} 
	if(vPosition.y < minHeight) 
	{
		if(vAccel.y < 0.0) vAccel.y = -vAccel.y;
	} 	//大地に到達
	if(inside == true && vPosition.y < heightT  + 0.1)
	{	
		vPosition.y = heightT + 0.1;
		vVelocity.y = - e * vVelocity.y;
	}
}

void CParticle::show()
{
	float light;
	glDisable(GL_LIGHTING);
	if(vPosition.y < minHeight) light = 0.7;
	else if(vPosition.y < maxHeight)
	{
		light = 0.7 + (vPosition.y - minHeight) / (maxHeight - minHeight);
	}
	else light = 1.0;
  glColor4f(light, 0.4*light, 0.1*light, 0.999);
  glPointSize(pointSize);
  glBegin(GL_POINTS);
	glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
	glEnable(GL_LIGHTING);
}


