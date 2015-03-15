//myFish2.h
//���z��

class CFish2
{
public:
	CFish2(void);
	~CFish2(void) { };
	//�����o�ϐ�
	float* diffuse ;//�g�U�����˗�
	float* specular;//���ʌ����˗�
	float* ambient; //�������˗�
	float highlight;//����

  float pos[3];//�ʒu 
  float ang[3];//�p���p
	float radius0;//�~�^���̔��a
	float period0;//�����]�̎���
	float periodSwing;//���E�܂��͏㉺�ɗh�炷����
	float angSwing;//�h�炷�p�x
	float maxAngSwing;//���̍ő�p�x

	//����
	float trunk1[3];
	float trunk2[3];

	//�����o�֐�
	void draw();
	void motion1(float t);
private:
	void drawTrunk();
	void drawEyes();
	void drawMouth();
};

//----------------------------------------------------------------
CFish2::CFish2()
{

	static float diffuse0[] = {0.2f, 0.8f, 0.8f, 1.0f};
  static float ambient0[] = {0.1f, 0.4f, 0.4f, 1.0f};
	static float specular0[] = {0.3f, 0.8f, 0.8f, 1.0f};

	//���z�����̃}�e���A������
	diffuse = diffuse0;
  ambient = ambient0;
  specular = specular0;
  highlight = 100.0f;

	pos[0] = -1.0; pos[1] = 1.0; pos[2] = 0.0;
  ang[0] = 0.0; ang[1] = 0.0; ang[2] = 0.0;
	angSwing = 0.0;
	radius0 = 2.0;
	period0 = 10.0;
	periodSwing = 2.0;
	maxAngSwing = 50.0;

  //����
	trunk1[0] = 0.5; trunk1[1] = 0.9; trunk1[2] = 1.0;
	trunk2[0] = 0.3; trunk2[1] = 0.7; trunk2[2] = 0.5;
	//��
//	mouth = CVector(0.1, 0.03, 0.03);
  //��
//  tail = CVector( 0.03, 0.03, 0.2);
}

//------------------------------------------------------------------
void CFish2::draw()
{
	setTextureMatrix();
	//�e�N�X�`�����W�̂��߂̃��f�����O�ϊ�
  glTranslatef(pos[0], pos[1], pos[2]);
  //�̑S�̂̎p��
  glRotatef(ang[1], 0.0, 1.0, 0.0); //y����]
  glRotatef(ang[0], 1.0, 0.0, 0.0); //x����]
  glRotatef(ang[2], 0.0, 0.0, 1.0); //z����]
	//�ʏ�̃��f���r���[�ϊ��ɖ߂� 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
  //���݈ʒu
  glTranslatef(pos[0], pos[1], pos[2]);
  //�̑S�̂̎p��
  glRotatef(ang[1], 0.0, 1.0, 0.0); //y����]
  glRotatef(ang[0], 1.0, 0.0, 0.0); //x����]
  glRotatef(ang[2], 0.0, 0.0, 1.0); //z����]
	if(flagShadow) 
	{
		//�e�̂Ƃ��͌Œ�V�F�[�_�[
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
	}
	else
	{
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100);
	}

  //����
	drawSuper(trunk1, trunk2, 20, 20, 1.8, 1.5, 0.2, angSwing, 6, 2);
	drawEyes();
	drawMouth();
	glPopMatrix();
}

void CFish2::drawEyes()
{
	float eyeDif[] = { 0.3, 0.3, 0.6, 1.0 };
	float eyeAmb[] = { 0.1, 0.1, 0.4, 1.0};
	float eyeSpe[] = { 0.4, 0.4, 0.4, 1.0};
	//����
	setTextureMatrix();
	//�e�N�X�`�����W�̂��߂̃��f�����O�ϊ�
  glTranslatef(pos[0], pos[1], pos[2]);
	//�ʏ�̃��f���r���[�ϊ��ɖ߂� 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(trunk1[0]/5.0, trunk1[1] /5.0, trunk1[2] *4.0/ 5.0);
		if(flagShadow) 
		{
			//�e�̂Ƃ��͌Œ�V�F�[�_�[
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, eyeDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, eyeAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, eyeSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 20.0);
//			transModel();
		}
		drawSphere(0.06, 8, 8);
	glPopMatrix();
	//�E��
	setTextureMatrix();
	//�e�N�X�`�����W�̂��߂̃��f�����O�ϊ�
  glTranslatef(pos[0], pos[1], pos[2]);
	//�ʏ�̃��f���r���[�ϊ��ɖ߂� 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(-trunk1[0]/5.0, trunk1[1] /5.0, trunk1[2] * 4.0/ 5.0);
		if(flagShadow) 
		{
			//�e�̂Ƃ��͌Œ�V�F�[�_�[
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
		  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, eyeDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, eyeAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, eyeSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 20.0);
//			transModel();
		}
		drawSphere(0.06, 8, 8);
	glPopMatrix();
}

void CFish2::drawMouth()
{
	float mouthDif[] = { 0.5, 0.5, 0.4, 1.0};
	float mouthAmb[] = { 0.3, 0.3, 0.2, 1.0};
	float mouthSpe[] = { 0.2, 0.2, 0.2, 1.0};
	setTextureMatrix();
	//�e�N�X�`�����W�̂��߂̃��f�����O�ϊ�
  glTranslatef(pos[0], pos[1], pos[2]);
	//�ʏ�̃��f���r���[�ϊ��ɖ߂� 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.0, 0.0, trunk1[2]* 0.93);
		//glScalef(1.0, 0.6, 1.0);
		if(flagShadow) 
		{
			//�e�̂Ƃ��͌Œ�V�F�[�_�[
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
		  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mouthDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, mouthAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, mouthSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 20.0);
//			transModel();
		}
		drawSphere(0.07, 8, 8);
	glPopMatrix();
}

void CFish2::motion1(float t)
{
	//������]
	double theta = 360.0 * t / period0;
	pos[0] = radius0 * sin( M_PI * theta / 180.0);
	pos[2] = radius0 * cos( M_PI * theta / 180.0);
	//theta = atan2(pos[0], pos[2]) * 180.0 / M_PI;
	ang[1] = theta + 90.0;//����
	//ang[2] = -20.0;
	angSwing = maxAngSwing * sin(2.0 * M_PI * t / periodSwing);
	draw();

}