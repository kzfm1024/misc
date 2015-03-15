//particle2.h
//水煙

#define NUM_PARTICLE2 5000
int num02 = 100;
int countP2 = 0;//放出された粒子数

class CParticle2 {
 public:
  CVector vPosition; // 位置
  CVector vVelocity; // 速度
  CVector vAccel;    //加速度
  float pointSize;
  float createTime;  //particle発生する時刻
	float intervalTime;//particleが発生してから放出されるまでの時間
  CParticle2();
  void create(float t);
  void update(float t,float dt);
  void show();
};

CParticle2::CParticle2()
{
  pointSize = getNormalRandom(1.5, 0.2);
  vPosition = CVector(0.0, 0.0, 0.0);
  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, 0.0, 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle2::create(float t)
{
	vPosition = CVector(getRandom(-0.6, 0.6),getRandom(0.0, 0.2), getRandom(0.08, 0.12));
	vVelocity = CVector(getNormalRandom(0.0, 0.05), getNormalRandom(0.2, 0.05), getNormalRandom(0.0, 0.05));//1.5, 0.1));
	createTime = t;
	intervalTime = getRandom(0.0, 1.0);
}

void CParticle2::update(float t, float dt)
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

void CParticle2::show()
{
  float diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,diffuse);

  glPointSize(pointSize);
  glBegin(GL_POINTS);
	glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}
