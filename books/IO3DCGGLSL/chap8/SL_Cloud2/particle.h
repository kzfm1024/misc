//particle.h
#include <stdlib.h>
#include <math.h>
#define NUM_PARTICLE 10000
int num0 = 1;

int countP = 0;//���o���ꂽ���q��

float g = 0.0;//�d�͉����x
float drag = 1.0;//��C��R
float e = 0.2;//���˕Ԃ�W��
float hCloud = 10.0;//�_�̍���
float accelX = 0.0; //x����������
float r0 = sizeYC / 2.0;//�h�[���̔��a
//float viewX, viewZ;//�����̕���

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
	float heightT;     //particle�̈ʒu�ix,z)�ɑ΂���terrain�̍���
	float maxHeight;   //�_�����ō��l
	float minHeight;   //�_�����Œ�l
	bool inside;       //terrain�̓��O����
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
	float c = dataC[j*(nMeshC + 1) + i] ;//���q�̑��݊m��
	//������Ⴍ����(�h�[���^�j	
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
	else { vPosition.x = 100000.0; }//���E�O��

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
	//������Ⴍ����(�h�[���^�j	
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
	else { vPosition.x = 100000.0; }//���E�O

  vVelocity = CVector(0.0, 0.0, 0.0);
  vAccel = CVector(0.0, getNormalRandom(0.0, 0.005), 0.0);

  intervalTime = getRandom(0.0, 3.0);
	createTime = t;

}

void CParticle::update(float t, float dt)
{
	if(t < createTime + intervalTime) return;
  CVector accel = vAccel ;//�d�͉����x�ƊO�͂͏�ɑ���
  accel -= drag * vVelocity;//��C��R
  vVelocity += accel * dt;//���x�̍X�V
  //�ʒu�̍X�V
	vPosition += vVelocity * dt;
	//��������
	if(vPosition.y > maxHeight) 
	{
		if(vAccel.y > 0.0) vAccel.y = -vAccel.y;
	} 
	if(vPosition.y < minHeight) 
	{
		if(vAccel.y < 0.0) vAccel.y = -vAccel.y;
	} 	//��n�ɓ��B
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


