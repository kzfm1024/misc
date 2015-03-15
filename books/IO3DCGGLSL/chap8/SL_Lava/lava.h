
//lava.h
#define NUM_LAVA 50000
int num0L = 50;//一度に放出する粒子数
int countL = 0;//使用した粒子数

class CLava {
public:
  CVector vPosition; // 位置
  CVector vVelocity; // 速度
  CVector vAccel;    //加速度
  float createTime;  //particle発生する時刻
	float intervalTime;//particleが発生してから放出されるまでの時間
	float pointSize;
	float speed ;
	float heightT;

  CLava();
  void update(float t, float dt);
  void create(float t);
  void show(float t);
};

CLava::CLava()
{
	pointSize = getNormalRandom(30.0, 5.0);
  vPosition = CVector(0.0, hCrater, 0.0);
  vVelocity = CVector(0.0, 0.0, 0.0);//5.0, 0.0);
  vAccel = CVector(0.0, - g, 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0.0, 10.0);
	speed = getNormalRandom(0.5, 0.2);
}

void CLava::create(float t)
{
	vPosition = CVector(getRandom(-1.0, 1.0), hCrater, getRandom(-1.0, 1.0));
  vVelocity = CVector(0.0, 0.0, 0.0);//5.0, 0.0);
	vAccel = getRandomVector(1) * getNormalRandom(0.2, 0.05);
	vAccel += CVector(0.0, - g, 0.0); 
	createTime = t;
	intervalTime = getRandom(0.0, 1.0);
}

void CLava::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;
	int dir;

	if(heightT < 0.1)
			dir = (floor)(8.0 * (float)rand() / (float)RAND_MAX);
	else 
		dir = direction[coordJ*(nMeshT+1) + coordI];//terrain.h
	if(dir == 0)      {vVelocity.x = speed;  vVelocity.z = 0.0;}
	else if(dir == 1) {vVelocity.x = speed;  vVelocity.z = -speed;}
	else if(dir == 2) {vVelocity.x = 0.0;    vVelocity.z = -speed;}
	else if(dir == 3) {vVelocity.x = -speed; vVelocity.z = -speed;}
	else if(dir == 4) {vVelocity.x = -speed; vVelocity.z = 0.0;}
	else if(dir == 5) {vVelocity.x = -speed; vVelocity.z = speed;}
	else if(dir == 6) {vVelocity.x = 0.0;    vVelocity.z = speed;}
	else if(dir == 7)	{vVelocity.x = speed;  vVelocity.z = speed;}
	CVector accel = vAccel;
	accel -= drag * vVelocity;
	vVelocity += accel * dt ;
	vPosition += vVelocity * dt;
	vPosition.y = heightT + 0.2;
}

void CLava::show(float t)
{
  float red, green, blue;
	float t0 = t - createTime;
	float t1 = 30.0, t2 = 60.0;//120.0;
	if(t0 < t1) {red = 1.0; green = 1.0; blue = 1.0;}
	else if(t0 < t2) {red = 0.9; green = 0.8; blue = 0.3;}
	else { red = 0.5; green = 0.3; blue = 0.2;}

  glPointSize(pointSize);
	glDisable(GL_LIGHTING);
  glColor4f(red, green, blue, 1.0);//alpha);
  glBegin(GL_POINTS);
  glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}

