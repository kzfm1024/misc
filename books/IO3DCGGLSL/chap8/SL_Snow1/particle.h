//particle.h
#include <stdlib.h>
#include <math.h>
#define NUM_PARTICLE 50000
int num0 = 500;

int countP = 0;//���o���ꂽ���q��

float g = 9.8;//�d�͉����x
float drag = 5.0;//��C��R
float e = 0.2;//���˕Ԃ�W��
float hSnow = 5.0;//�~��̍���
float accelX = 0.0;

float getRandom(float fMin, float fMax)
{
  return fMin + (fMax - fMin) * (float)rand() / (float)RAND_MAX;
}

//���K����
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
  CVector vPosition; // �ʒu
  CVector vVelocity; // ���x
  CVector vAccel;    //�����x
  float pointSize;   //���q�̃T�C�Y
	float createTime;  //������������
	float intervalTime;//���o����܂ł̊Ԋu
	float heightT;//particle�̈ʒu�ix,z)�ɑ΂���terrain�̍���
	bool inside;  //terrain�̓��O����
  CParticle();
  CParticle(CVector vPosition0, CVector vVelocity0, CVector vAccel0);
  void create(float t);
  void update(float t, float dt);
  void show();
};

CParticle::CParticle()
{
  pointSize = getNormalRandom(50.0, 5.0);
  vPosition = CVector(getRandom(-10.0, 10.0), hSnow+ getRandom(-0.1, 0.1), getRandom(-10.0, 10.0));
  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, - g, 0.0);
	createTime = 0.0;
	intervalTime = getRandom(0, 3.0);
}

void CParticle::create(float t)
{
  vPosition = CVector(getNormalRandom(0.0, sizeX / 4.0),
		hSnow+getNormalRandom(0.0, 0.1), getNormalRandom(0.0, sizeY / 2.4));
  vVelocity = CVector(getNormalRandom(0.0, 0.5), 
		getNormalRandom(0.0, 0.3), getNormalRandom(0.0, 0.5));
  vAccel = CVector(accelX+getNormalRandom(0.0, 0.8),
	        -g + getNormalRandom(0.0, 0.8), getNormalRandom(0.0, 0.8)); 
  intervalTime = getRandom(0.0, 1.0);
	createTime = t;
}

void CParticle::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;
  CVector accel = vAccel ;//�d�͉����x�ƊO�͂͏�ɑ���
  accel -= drag * vVelocity;//��C��R
  vVelocity += accel * dt;//���x�̍X�V

  //�n��ɗ�������͌Œ�
	if(inside == true && vPosition.y < heightT + 0.1)
  {
		vVelocity *= 0.0;
  }
  //�ʒu�̍X�V
	vPosition += vVelocity * dt;
	//��n�ɓ��B
	if(inside == true && vPosition.y < heightT  + 0.1)
	{	
		vPosition.y = heightT + 0.1;
		vVelocity.y = - e * vVelocity.y;
	}
}

void CParticle::show()
{
	glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 1.0);
  glPointSize(pointSize);
  glBegin(GL_POINTS);
	glVertex3f(vPosition.x, vPosition.y, vPosition.z);
  glEnd();
}


