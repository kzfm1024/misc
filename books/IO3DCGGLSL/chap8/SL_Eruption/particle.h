
float getRandom(float fMin, float fMax)
{
  return fMin + (fMax - fMin) * (float)rand() / (float)RAND_MAX;
}

float getNormalRandom(float mean, float sigma)
{//正規乱数
  double ran = 0.0;
  for(int i=0; i < 12; i++) ran += (double)rand() / (double)RAND_MAX;
  ran -= 6.0;
  ran *= sigma;
  return mean + (float)ran;
}

CVector getRandomVector(int type)
{//xz大きさ1で放射状に一様に広がるベクトル
  CVector vVector;
  if(type == 0) vVector.y = getRandom( -1.0f, 1.0f );//球全体
  else vVector.y = getRandom( 0.0f, 1.0f );//上半球

  float radius = (float)sqrt(1.0 - vVector.y * vVector.y);
  float theta = getRandom( -M_PI, M_PI );
  vVector.z = (float)cos(theta) * radius;
  vVector.x = (float)sin(theta) * radius;
  return vVector;
}

//particle.h
#define NUM_PARTICLE 10000
int num0 = 200;//一度に放出する粒子数
int countP = 0;//使用した粒子数
float hCrater = 13.0;//火口の高さ
float g = 9.8 ;//重力加速度
float drag = 1.0;
float accelX = 0.0;

class CParticle {
public:
  CVector vPosition; // 位置
  CVector vVelocity; // 速度
  CVector vAccel;    //加速度
  float createTime;  //particle発生する時刻
	float intervalTime;//particleが発生してから放出されるまでの時間
	float pointSize;
	float heightT;

  CParticle();
  void update(float t, float dt);
  void create(float t);
  void show(float elapseTime);
};

CParticle::CParticle()
{
	pointSize = getNormalRandom(2.0, 1.0);
  vPosition = CVector(0.0, hCrater, 0.0);
  vVelocity = CVector(0.0, 5.0, 0.0);
  vAccel = CVector(0.0, - g, 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle::create(float t)
{
	vPosition = CVector(getNormalRandom(0.0, 0.5), hCrater + getNormalRandom(0.0, 0.2), getNormalRandom(0.0, 0.2));
	vVelocity = getRandomVector(1) * getNormalRandom(1.0, 0.1);
	vVelocity.x *= 4.0;
	vVelocity.z *= 4.0;
	vVelocity.y *= 45.0;
	createTime = t;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;
	CVector accel = vAccel;
	accel -= drag * vVelocity;
	vVelocity += accel * dt;
	if(vPosition.y < heightT + 0.1) 
	{
		vPosition.y = heightT + 0.1;
		vVelocity = CVector(0.0, 0.0, 0.0);
	}
	vPosition += vVelocity * dt;
}

void CParticle::show(float t)
{
float t1 = 0.8, t2 = 1.5, t3 = 2.5;
  float red, green, blue;
	float t0 = t - createTime;

	if(t0 < t1)      {red = 1.0; green = 1.0; blue = 0.2; }
	else if(t0 < t2) {red = 1.0; green = 0.5; blue = 0.1; }
	else             {red = 0.8; green = 0.1; blue = 0.0;}

	glDisable(GL_LIGHTING);
	glColor4f(red, green, blue, 0.5);
	glPointSize(pointSize);
  glBegin(GL_POINTS);
  glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}

