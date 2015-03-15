//dog.h
//�l���̉��z����
//root�͓��̂̒��S

#define MAXJOINT 20
#define MAXFRAME 5000//���e�ő�R�}��
#define MAXKEYFRAME 10//�ő�L�[�t���[����
//��L�̒萔�͑��̃I�u�W�F�N�g�N���X�ɂ��g�p

class CDog
{
public:
  CDog(void);
  ~CDog(void) { };
  //�����o�ϐ�
  int numJoint;//�֐ߌ�
  //�ʒu(Root�͓��̂̒��S)
  CVector vPos;
  //�p���p
  CVector vAng;
  //�i�s����
  float dir;
  //�֐ߊp�̔z��
  CVector joint[MAXJOINT];
  //����
  CVector trunk;
  //��
  CVector neck;
  //����
  CVector head ;
  //��
  CVector eye;
  //�@
  CVector nose;
  //��
  CVector ear;
  //��
  CVector mouth;
  //�r�i�O�r�C��r)
  CVector leg1, leg2, foot;
  //��
  CVector tail;
  //�r�����t����ʒu
  CVector legPos;//���̒��S����̑��Έʒu
  //�r�̑S�̂̒���
  float legLen;
  //animation�p
  CVector f_vPos[MAXFRAME];
  CVector f_vAng[MAXFRAME];
  float  f_dir[MAXFRAME];
  CVector k_vPos[MAXKEYFRAME];
  CVector k_vAng[MAXKEYFRAME];
  float  k_dir[MAXKEYFRAME];
  CVector f_joint[MAXFRAME][20];
  CVector k_joint[MAXKEYFRAME][20];
  float actTime[MAXKEYFRAME];//keyFrame�Ԃ̓��쎞��
  float unitTime;//1�t���[��������̕`�掞��
  int numKeyFrame;//���ڰѐ�
  int numFrame0;//�e�I�u�W�F�N�g��1�̃V�[���̑S�\���ڰѐ�

  //�����o�֐�
  void draw();
  void draw(int n);
  void reset();
  void walk(float dist, float step, float tt);
  void turn(float angle, float tt);
  void stand(float tt);
  void initPose(float tt);
  void wait(float tt);
  void swingTail(int n, float tt);
  void upTail(float tt);
  void downTail(float tt);
private:
  void drawTrunk();
  void drawHead();
  void drawLeg();
  void drawLeg1(int jNo);
  void drawLeg2(int jNo);
  void drawTail();
  void initKeyAnimation();
  void makeFrameData();
  void walkRight(float step, float tt);
  void walkLeft(float step, float tt);
};
//----------------------------------------------------------------
CDog::CDog()
{
  int i;
  unitTime = 0.01;//1�t���[��������̕`�掞�ԁi���̒l)
  numJoint = 10;
  vPos = CVector();
  vAng = CVector();
  for(i = 0; i< numJoint; i++) joint[i] = CVector();

  //����
  trunk = CVector( 0.2, 0.18, 0.4);
  //����
  head = CVector( 0.23, 0.2, 0.23);
  //��
  eye = CVector( 0.04, 0.04, 0.04);
  //�@
  nose = CVector( 0.03, 0.03, 0.03);
  //��
  ear = CVector( 0.03, 0.15, 0.06);
  //��
  mouth = CVector(0.14, 0.14, 0.14);
  //��r�i�O�r�C��r)
  leg1 = CVector(0.07, 0.06, 0.07);
  //���r�i�O�r�C��r)
  leg2 = CVector(0.065, 0.06, 0.065);
  //��
  tail = CVector( 0.03, 0.03, 0.20);
  //�r�����t����ʒu
  legPos.x = trunk.x / 2.0 - 0.03;
  legPos.y = trunk.y / 2.0 - 0.03;//���̒��S����̋���
  legPos.z = trunk.z / 2.0 - 0.04;
  //�r�S��
  legLen = leg1.y + leg2.y;
  //Root�̈ʒu�i���̂̒��S)�@
  vPos.y = legLen + legPos.y;
}
//--------------------------------------------------------------------------------------
void CDog:: draw(int n)
{
  int j;
  //n�Ԗڂ̃t���[���ԍ��̎p����`��
  if(n > numFrame0) n = numFrame0;
  vPos = f_vPos[n];
	vAng = f_vAng[n];
	dir = f_dir[n];
  for(j = 0; j < numJoint; j++) joint[j] = f_joint[n][j];
  draw();
}


//------------------------------------------------------------------
void CDog::draw()
{
	//���݈ʒu(�S�̂𓷑̂̒��S�܂Ŏ����グ��j
	glTranslatef(vPos.x, vPos.y, vPos.z);

	//�̑S�̂̎p��
	glRotatef(vAng.y, 0.0, 1.0, 0.0); //y����]
	glRotatef(vAng.x, 1.0, 0.0, 0.0); //x����]
	glRotatef(vAng.z, 0.0, 0.0, 1.0); //z����]
	drawTrunk();//����
	drawHead();//����܂߂�����
	drawLeg();
	drawTail();
}
//------------------------------------------------------------------------------
void CDog::drawTrunk(void)
{
	//����
	glPushMatrix();
	  glScalef(trunk.x, trunk.y, trunk.z);
	  glRotatef(90.0, 1.0, 0.0, 0.0);//90�xx����]
	  drawSuper(0.5, 16, 8, 0.5, 0.5);
	glPopMatrix();
}
//-----------------------------------------------------------------------------
void CDog::drawHead()
{
  glPushMatrix();

  //��̈ʒu�ŕ��s�ړ�
	glTranslatef( 0.0, trunk.y / 2.0 - 0.02, trunk.z / 2.4 );
  //���]
  glRotatef(joint[0].y, 0.0, 1.0, 0.0);//y����]
  glRotatef(-90.0 + joint[0].x, 1.0, 0.0, 0.0);//x����]
  glRotatef(joint[0].z, 0.0, 0.0, 1.0);//z����]

  glTranslatef( 0.0, 0.0, neck.z / 2.0 + head.z / 2.0);//��̐�[�܂ŕ��s�ړ�
  //��
	glPushMatrix();
		glScalef(head.x, head.y, head.z);	
		drawSuper(0.5, 8, 8, 0.9, 0.8);
	glPopMatrix();
  //�E��
  glPushMatrix();
    glTranslatef( -head.x / 4.0, - head.y / 2.7, head.z / 3.0 );//
    glScalef(eye.x, eye.y, eye.z);
    drawSphere(0.5, 8, 8);
  glPopMatrix();
  //����
  glPushMatrix();
    glTranslatef( head.x / 4.0, - head.y / 2.7, head.z / 3.0 );//
    glScalef(eye.x, eye.y, eye.z);
    drawSphere(0.5, 8, 8);
  glPopMatrix();
  //�@
  glPushMatrix();
    glTranslatef( 0.0, - head.y / 1.2, head.z / 5.0 );//
    glScalef(nose.x, nose.y, nose.z);
    drawSphere(0.5, 8, 8);
  glPopMatrix();
  //�E��
  glPushMatrix();
    glTranslatef( -head.x / 2.2, 0.0, head.z / 4.0);//
    glRotatef(90.0 + joint[11].x, 1.0, 0.0, 0.0);//x����]
    glScalef(ear.x, ear.y, ear.z);
    glTranslatef(0.0, -0.5, 0.0);
    drawSphere(0.5, 8, 8);
  glPopMatrix();
  //����
  glPushMatrix();
    glTranslatef( head.x / 2.2, 0.0, head.z / 4.0);//
    glRotatef(90.0 + joint[11].x, 1.0, 0.0, 0.0);//x����]
    glScalef(ear.x, ear.y, ear.z);
    glTranslatef(0.0, -0.5, 0.0);
    drawSphere(0.5, 8, 8);
  glPopMatrix();
	//��
	glPushMatrix();
    glTranslatef( 0.0, -head.y / 2.0, 0.0);//
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glScalef(mouth.x, mouth.y, mouth.z);
		drawSuper(0.5, 8, 8, 0.5, 0.7, 0.5);
	glPopMatrix();

  glPopMatrix();
}
//------------------------------------------------------------------
void CDog::drawLeg()
{
  //�O�E�r
  glPushMatrix();
    // �O�E�r�̈ʒu�֕��s�ړ�
    glTranslatef(-legPos.x, -legPos.y, legPos.z);
    drawLeg1(1);
    drawLeg2(5);
	glPopMatrix();
  //�O���r
  glPushMatrix();
    // �O���r�̈ʒu�֕��s�ړ�
    glTranslatef( legPos.x, -legPos.y, legPos.z);
    drawLeg1(2);
    drawLeg2(6);
  glPopMatrix();
  //��E�r
  glPushMatrix();
    // ��E�r�̈ʒu�֕��s�ړ�
    glTranslatef(-legPos.x, -legPos.y, -legPos.z);
    drawLeg1(3);
    drawLeg2(7);
	glPopMatrix();
  //�㍶�r
  glPushMatrix();
    // �㍶�r�̈ʒu�֕��s�ړ�
    glTranslatef( legPos.x, -legPos.y, -legPos.z);
    drawLeg1(4);
    drawLeg2(8);
  glPopMatrix();
}
//----------------------------------------------------------------------------
void CDog::drawLeg1(int jNo)
{
  //�r�t�����̉�]
  glRotatef(joint[jNo].y, 0.0, 1.0, 0.0);//y����]
  glRotatef(joint[jNo].x, 1.0, 0.0, 0.0);//x����]
  glRotatef(joint[jNo].z, 0.0, 0.0, 1.0);//z����]
  glPushMatrix();
    glScalef(leg1.x, leg1.y + 0.01, leg1.z );
    glTranslatef(0.0, -0.5, 0.0);//���_�̈ʒu�։�����
	drawSuper(0.5, 8, 8, 0.5, 0.8);
  glPopMatrix();
}
//----------------------------------------------------------------------------
void CDog::drawLeg2(int jNo)
{
  //���r�̈ʒu�֕��s�ړ�
  glTranslatef(0.0, -leg1.y, 0.0);
  //�G�̉�]
  glRotatef(joint[jNo].y, 0.0, 1.0, 0.0);//y����]
  glRotatef(joint[jNo].x, 1.0, 0.0, 0.0);//x����]
  glRotatef(joint[jNo].z, 0.0, 0.0, 1.0);//z����]
  glPushMatrix();
    glScalef(leg2.x, leg2.y, leg2.z);
    glTranslatef(0.0, -0.5, 0.0);//���_�̈ʒu�։�����
	drawSuper(0.5, 8, 8, 0.5, 0.8);
  glPopMatrix();
}
//----------------------------------------------------------------------------
void CDog::drawTail()
{
  glPushMatrix();
    //���̈ʒu�֕��s�ړ�
    glTranslatef( 0.0, 0.0, -trunk.z / 3.0 );
    //���̉�]
    glRotatef(joint[9].y, 0.0, 1.0, 0.0);//y����]
    glRotatef(30.0 + joint[9].x, 1.0, 0.0, 0.0);//x����]
    glRotatef(joint[9].z, 0.0, 0.0, 1.0);//z����]
    glPushMatrix();
	  glScalef(tail.x, tail.y, tail.z);
	  glTranslatef(0.0, 0.0, -0.5);
	  glRotatef(90.0, 1.0, 0.0, 0.0);//90�xx����]
	  drawCylinder1(0.5, 0.5, 1.0, 10, 3);
    glPopMatrix();
  glPopMatrix();
}
//----------------------------------------------------------------------------
//keyframe���쐬���郁���o�֐��ɂ����čŏ���call����
void CDog::initKeyAnimation()
{
	short k, j;
	//���݂̈ʒu�p���i�ŏ��̎p���ŏ�����)
	for(k = 0; k < numKeyFrame; k++)
	{
		k_vPos[k] = vPos;
    k_vAng[k] = vAng;
    k_dir[k] = dir;

		for(j = 0; j < numJoint; j++) k_joint[k][j] = joint[j];
	}
}
//----------------------------------------------------------------------------
//�ڰ��ް��쐬
//keyframe���쐬���郁���o�֐��ōŌ��call
void CDog::makeFrameData()
{
	short i, j, k, numHokan;
	float ss;

  if(unitTime == 0.0)
  {	printf("unitTime = 0�ƂȂ�܂��� \n");
      return;
  }

	for(k = 1; k < numKeyFrame; k++)
  {
		if(actTime[k] < 0)
		{	printf("actTime<0�ƂȂ�܂��� \n");
			return;//�ǂ����̃��[�`���ŕ��̒l�ɐݒ肵���I�I
		}

		numHokan = (short)(actTime[k] / unitTime);//��ԓ_��
		if(numHokan == 0) continue;
		if(numFrame0 + numHokan > MAXFRAME) 
		{
			printf("�t���[�������������z���邨���ꂪ����܂� \n");
			return;
		}
		for(i = 0; i <= numHokan; i++)
		{
			ss = (float)i / (float)numHokan;
            f_vPos[numFrame0 + i] = k_vPos[k-1] + (k_vPos[k] - k_vPos[k-1]) * ss;
            f_vAng[numFrame0 + i] = k_vAng[k-1] + (k_vAng[k] - k_vAng[k-1]) * ss;

			for(j = 0; j < numJoint; j++)
			{//��ԃf�[�^���쐬(���`���)
                f_joint[numFrame0 + i][j] = k_joint[k-1][j]
				                 + (k_joint[k][j] - k_joint[k- 1][j]) * ss;
			}
		}
		numFrame0 += numHokan;
	}
	reset();
}
//---------------------------------------------------------------------------------
//�c���ڰт��ް����ŏI�l�ɍĐݒ�
//makeFrameData()�̍Ō��call����
void CDog::reset()
{
	short i, j;

	//�R�}���ő�l�̍X�V
	//�ŏI�l�����ݒl��
  if(numFrame0 > 0)
	{
    vPos = f_vPos[numFrame0];
    vAng = f_vAng[numFrame0];

		for(j = 0; j < numJoint; j++) joint[j] = f_joint[numFrame0][j];
	}

	//�c������ׂČ��ݒl
	for(i = numFrame0; i < MAXFRAME; i++)
	{
    f_vPos[i] = vPos;
    f_vAng[i] = vAng;

		for(j = 0; j < numJoint; j++) f_joint[i][j] = joint[j];
	}
}
//----------------------------------------------------------------
//�^����ꂽ��������
void CDog::walk(float dist,  float step, float time)//
{                    //����, ���� , ���������̏��v����,�������ԑ���     �@�@�@�@�@�@
	short i, numStep, numStep2;
	float rest;

	dir = vAng.y;//���݌����Ă��������
	numStep = (short)(dist / step);
	//�E��,����2����P�ʂƂ�������
	numStep2 = numStep / 2;
	if(dist < 2.0 * step) rest = dist;
	else rest = dist - 2.0 * step * (float)numStep2;

	for(i = 0;i < numStep2; i++)
	{
		walkRight(step, time);//�O�E�C�㍶
		walkLeft(step, time); //�O���C��E
	}
	//�c��(1���ȓ��Ȃ�E����)
	if(rest < step) walkRight(rest, time * rest / step);
	else	
	{	walkRight(step, time);
    rest -= step;
		if(rest > 0.0) walkLeft(rest, time * rest / step);
	}
}
//---------------------------------------------------------------------
void CDog::walkRight(float step, float time)
{
  float step2 = step / 2.0;//����
  float pp = M_PI / 180.0;
  //�Ҋ֐ߊp�x
	float stepA = 180.0 * asin(step2 / legLen) / M_PI;
	float cc = step2 * cos(pp * dir) ;//������z����(��{�p���ł�z�������O�j
	float ss = step2 * sin(pp * dir) ;//x����

	numKeyFrame = 3;
 	initKeyAnimation();
	//action1(�E�O�r����O��)
	k_joint[1][1].x = -stepA;//�O�E
	k_joint[1][2].x = stepA; //�O��
	k_joint[1][3].x = stepA;//��E
	k_joint[1][4].x = -stepA; //�㍶
	k_vPos[1].z = k_vPos[0].z + cc;
	k_vPos[1].x = k_vPos[0].x + ss;
  k_vPos[1].y = legLen * cos(pp * k_joint[1][1].x) + legPos.y;//y���W
	actTime[1] = time / 2.0;

	//action2(��{�p���ɖ߂�)
	k_joint[2][1].x = 0.0;//�O�E
	k_joint[2][2].x = 0.0; //�O��
	k_joint[2][3].x = 0.0;//��E
	k_joint[2][4].x = 0.0; //�㍶
	k_vPos[2].z = k_vPos[1].z + cc;
	k_vPos[2].x = k_vPos[1].x + ss;
    k_vPos[2].y = legLen * cos(pp * k_joint[2][1].x) + legPos.y;//z���W
	actTime[2] = time / 2.0;

	makeFrameData();
}
//---------------------------------------------------------------------
void CDog::walkLeft(float step, float time)
{
  float step2 = step / 2.0;
  float pp = M_PI / 180.0;
  //�Ҋ֐ߊp�x
	float stepA = 180.0 * asin(step2 / legLen) / M_PI;
	float cc = step2 * cos(pp * dir);//������z����
	float ss = step2 * sin(pp * dir);//x����

	numKeyFrame = 3;
 	initKeyAnimation();
	//action1(�E�O�r����O��)
	k_joint[1][1].x = stepA;//�O�E
	k_joint[1][2].x = -stepA; //�O��
	k_joint[1][3].x = -stepA;//��E
	k_joint[1][4].x = stepA; //�㍶
	k_vPos[1].z = k_vPos[0].z + cc;
	k_vPos[1].x = k_vPos[0].x + ss;
  k_vPos[1].y = legLen * cos(pp * k_joint[1][1].x) + legPos.y;//z���W
	actTime[1] = time / 2.0;

	//action2(��{�p���ɖ߂�)
	k_joint[2][1].x = 0.0;//�O�E
	k_joint[2][2].x = 0.0; //�O��
	k_joint[2][3].x = 0.0;//��E
	k_joint[2][4].x = 0.0; //�㍶
	k_vPos[2].z = k_vPos[1].z + cc;
	k_vPos[2].x = k_vPos[1].x + ss;
    k_vPos[2].y = legLen * cos(pp * k_joint[2][1].x) + legPos.y;//z���W
	actTime[2] = time / 2.0;

	makeFrameData();
}
//--------------------------------------------------------------------------
//���ʕ����̉�](angle>0�ŏォ�猩�č���])
void CDog::turn(float angle, float time)
{
	numKeyFrame = 2;
	initKeyAnimation();

	//���݂̊e�֐߂̊p�x
	k_vAng[1].y = vAng.y + angle;
	actTime[1] = time;
	makeFrameData();
	dir = vAng.y;
}

//���r�ŗ����オ��
void CDog::stand(float time)
{
	numKeyFrame = 2;
	initKeyAnimation();

	k_joint[1][3].x = 90.0;//��E
	k_joint[1][4].x = 90.0; //�㍶
  k_vPos[1].y = legLen + trunk.z / 2.0;//y���W
	k_vAng[1].x = -90.0;
	k_joint[1][0].x = 90.0;
	actTime[1] = time ;
	makeFrameData();
}

//��{�p���ɖ߂�
void CDog::initPose(float time)
{
	short j;
	
	numKeyFrame = 2;
	initKeyAnimation();

	for(j = 0; j < numJoint; j++) k_joint[1][j] = CVector();
	k_vAng[1].x = 0.0;
	k_vAng[1].z = 0.0;
	k_vPos[1].y = legLen + legPos.y;//root�̈ʒu
	actTime[1] = time;
	makeFrameData();
}

void CDog::wait(float time)
{
	numKeyFrame = 2;
	initKeyAnimation();
	actTime[1] = time;
	makeFrameData();
}

void CDog::swingTail(int n, float time)
{
	short i;
	
	for(i = 0; i < n; i++) 
	{
		upTail(time/2.0);
		downTail(time/2.0);
	}
}

void CDog::upTail(float time)
{
	numKeyFrame = 2;
	initKeyAnimation();
	k_joint[1][9].x += 60.0;
	actTime[1] = time;
	makeFrameData();
}

void CDog::downTail(float time)
{
	numKeyFrame = 2;
	initKeyAnimation();
	k_joint[1][9].x -= 60.0;
	actTime[1] = time;
	makeFrameData();
}

 