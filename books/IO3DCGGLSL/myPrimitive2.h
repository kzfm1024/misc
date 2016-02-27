#include <math.h>
#include <stdio.h>
#include "../../spline.h"
#include "../../myMath2.h"

#define M_PI 3.14159265358979323846

//---------------------------------------------------------------------
//�@�������v�Z���[�`��
void calcNormal(float* p1,float* p2,float* p3,float* nn)
{
	CVector A = CVector(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
	CVector B = CVector(p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]);
	CVector n = A ^ B;//�O��
	n.normalize();
	nn[0] = n.x; nn[1] = n.y; nn[2] = n.z;
}

//------------------------------------------------------------------------------
void drawPlate(float s)//x-y����
{
	static float p[4][3] = 
	{ { s/2.0,-s/2.0, 0.0}, { s/2.0, s/2.0, 0.0}, {-s/2.0, s/2.0, 0.0}, {-s/2.0,-s/2.0, 0.0}};

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0); //z�����̖@��
		glVertex3fv(p[0]);
		glVertex3fv(p[1]);
		glVertex3fv(p[2]);
		glVertex3fv(p[3]);
	glEnd();
}
//-----------------------------------------------------------------------

void drawCube(float s)
{
	float p[8][3] = 
	{ {0.5*s,0.5*s,0.5*s}, {-0.5*s,0.5*s,0.5*s}, {-0.5*s,-0.5*s,0.5*s}, 
	  {0.5*s,-0.5*s,0.5*s},{0.5*s,0.5*s,-0.5*s}, {-0.5*s,0.5*s,-0.5*s},
	  {-0.5*s,-0.5*s,0-0.5*s}, {0.5*s,-0.5*s,-0.5*s}
	};

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f); //z����
		glVertex3fv(p[0]); glVertex3fv(p[1]);
		glVertex3fv(p[2]); glVertex3fv(p[3]);

		glNormal3f(1.0f,0.0f,0.0f); //x����(���ʁj
		glVertex3fv(p[0]); glVertex3fv(p[3]);
		glVertex3fv(p[7]); glVertex3fv(p[4]);

		glNormal3f(0.0f,1.0f,0.0f); //y����
		glVertex3fv(p[0]); glVertex3fv(p[4]);
		glVertex3fv(p[5]); glVertex3fv(p[1]);

	 	glNormal3f(-1.0f,0.0f,0.0f); //-x����
		glVertex3fv(p[1]); glVertex3fv(p[5]);
		glVertex3fv(p[6]); glVertex3fv(p[2]);

		glNormal3f(0.0f,-1.0f,0.0f); //-y����
		glVertex3fv(p[2]); glVertex3fv(p[6]);
		glVertex3fv(p[7]); glVertex3fv(p[3]);

		glNormal3f(0.0f,0.0f,-1.0f); //-z����
		glVertex3fv(p[4]); glVertex3fv(p[7]);
		glVertex3fv(p[6]); glVertex3fv(p[5]);
	glEnd();
}

void drawSphere(double radius, int nSlice, int nStack)
{
	int i, j;
	double r0, r1, th0, th1, phi;
	double p[2][3];

	for(j = 0; j < nStack; j++)
	{
		//j=0�͖k�ɓ_(x=0, y=0, z=radius)
		//�����̓V���p
		th0 = M_PI * (double)j / (double)nStack;
		th1 = M_PI * (double)(j+1) / (double)nStack;
		//x-y���ʂɓ��e�������a
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//���_z���W
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//���_��xy���W(i=0��object����݂ĉE�[) 
			p[0][0] =   r0 * sin(phi);//x���W
			p[0][1] = - r0 * cos(phi);//y���W
			p[1][0] =   r1 * sin(phi);//x���W
			p[1][1] = - r1 * cos(phi);//y���W

			glNormal3dv(p[0]);//�@���x�N�g��
			glVertex3dv(p[0]);//���_���W

			glNormal3dv(p[1]);//�@���x�N�g��
			glVertex3dv(p[1]);//���_���W			
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------
void drawHemiSphere(double radius, int nSlice, int nStack)
{	//�㔼��
	//radius:���a
	//nSlice:�o�x(x-y)
	//nStack:�ܓx(z)
	int i, j;
	double r0, r1, th0, th1, phi;
	double p[2][3];

	for(j = 0; j < nStack; j++)
	{
		//j=0�͖k�ɓ_(x=0, y=0, z=radius)
		//�����̓V���p
		th0 = M_PI * (double)j / (double)nStack;
		th1 = M_PI * (double)(j+1) / (double)nStack;
		//x-y���ʂɓ��e�������a
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//���_z���W
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//���_��xy���W
			p[0][0] = r0 * cos(phi);//x���W
			p[0][1] = r0 * sin(phi);//y���W
			p[1][0] = r1 * cos(phi);//x���W
			p[1][1] = - r1 * sin(phi);//y���W

			glNormal3dv(p[0]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[0]);//���_���W

			glNormal3dv(p[1]);//�@���x�N�g��,���K������Β��_���W�ɓ���
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

//----------------------------------------------------------------------
void drawCylinder1(double rBottom, double rTop, double height, int nSlice, int nStack)
{
	//�~��(rBottom=rTop))�A�~����A�~��(rTop = 0.0)
	//rBottom:���ꔼ�a, rTop:��ꔼ�a
	//nSlice--xy�f�ʕ�����
	//nStack---������������
	//���̂̒��S�͉���Ə��̒���

	int i, j;
	double x, y, z, z0, z1;
	double theta;
	double theta0 = 2.0*M_PI/(double)nSlice;
	//���(Top)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, height/2.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		x = (float)(rTop * cos(theta)); //x����
		y = (float)(rTop * sin(theta)); //������
		z = height/2.0;
		glVertex3f(x, y, z);
	}
	glEnd();

	//����(Bottom)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, -height/2.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)(nSlice - i);
		x = (float)(rBottom * cos(theta)); //x����
		y = (float)(rBottom * sin(theta)); //������
		z = -height/2.0;
		glVertex3f(x, y, z);
	}
	glEnd();

	double s, t0, t1, r0, r1, phi;
	double p[2][3], n[3];

	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);
	//����
	for(j = 0; j < nStack; j++)
	{
		//j=0�͏��(x=0, y=0, z=height/2)
		//2��t���W
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//��ʂ���̍���
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//���a
		r0 = rBottom + (rTop - rBottom) * z0 / height;
		r1 = rBottom + (rTop - rBottom) * z1 / height;

		//���_z���W
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s���W
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//���_��xy���W
			p[0][0] = r0 * cos(phi);//x���W
			p[0][1] = r0 * sin(phi);//y���W
			p[1][0] = r1 * cos(phi);//x���W
			p[1][1] = r1 * sin(phi);//y���W
			//�@���x�N�g��
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] = nz;

			glNormal3dv(n);//�@���x�N�g��
			glVertex3dv(p[0]);//���_���W
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

//-------------------------------------------------------------------------------------
void drawCylinder2(double rBottom, double rTop, double height, double thick, int nSlice, int nStack)
{
	//���݂̂��钆��~��
	//�~��(rBottom=rTop))�A�~����A�~��(rTop = 0.0)
	//rBottom:���ꔼ�a, rTop:��ꔼ�a, thick:����
	//nSlice--xy�f�ʕ�����
	//nStack---������������
	//���̂̒��S�͉���Ə��̒���

	int i, j;
	double x, y, z, z0, z1;
	double theta;
	double theta0 = 2.0*M_PI/(double)nSlice;
	//���(Top)
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, 1.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		z = height/2.0;
		//�������W
		x = (rTop - thick) * cos(theta); //x����
		y = (rTop - thick) * sin(theta); //y����
		glVertex3f(x, y, z);

		//�O�����W
		x = rTop * cos(theta); //x����
		y = rTop * sin(theta); //y����
		glVertex3f(x, y, z);
	}
	glEnd();

	//����(Bottom)
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, -1.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		z = -height/2.0;
		//�O�����W
		x = rBottom * cos(theta); //x����
		y = rBottom * sin(theta); //y����
		glVertex3f(x, y, z);
		//�������W
		x = (rBottom - thick) * cos(theta); //x����
		y = (rBottom - thick) * sin(theta); //y����
		glVertex3f(x, y, z);
	}
	glEnd();

	double t0, t1, r0, r1, phi;
	double p[2][3], n[3];

	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);

	for(j = 0; j < nStack; j++)
	{
		//j=0�͏��(x=0, y=0, z=height/2)
		//2��t���W
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//��ʂ���̍���
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//���a
		r0 = rBottom + (rTop - rBottom) * z0 / height;
		r1 = rBottom + (rTop - rBottom) * z1 / height;

		//���_z���W
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;
		
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//���_��xy���W(i=0��^���)
			p[0][0] = r0 * cos(phi);//x���W
			p[0][1] = r0 * sin(phi);//y���W
			p[1][0] = r1 * cos(phi);//x���W
			p[1][1] = r1 * sin(phi);//y���W
			//�@���x�N�g��
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] =  nz;

			glNormal3dv(n);//�@���x�N�g��
			glVertex3dv(p[0]);//���_���W
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}

	//��������
	for(j = 0; j < nStack; j++)
	{
		//j=0�͏��(x=0, y=0, z=height/2)
		//2��t���W
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//��ʂ���̍���
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//���a
		r0 = rBottom - thick + (rTop - rBottom) * z0 / height;
		r1 = rBottom - thick + (rTop - rBottom) * z1 / height;

		//���_z���W
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;

		glBegin(GL_QUAD_STRIP);
		for(i = nSlice; i >= 0; i--)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//���_��xy���W(i=0��^���)
			p[0][0] = -r0 * cos(phi);//x���W
			p[0][1] = -r0 * sin(phi);//y���W
			p[1][0] = -r1 * cos(phi);//x���W
			p[1][1] = -r1 * sin(phi);//y���W

			//�@���x�N�g��
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] = - nz;
			
			glNormal3dv(n);//�@���x�N�g��
			glVertex3dv(p[0]);//���_���W
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

//----------------------------------------------------------------------
void drawPrism1(float rBottom, float rTop, float height, int nSlice, int nStack)
{
	//�~��(rBottom=rTop))�A�~����A�~��(rTop = 0.0)
	//rBottom:���ꔼ�a, rTop:��ꔼ�a
	//nSlice--xy�f�ʕ�����
	//nStack---������������
	//���̂̒��S�͉���Ə��̒���
	float p[31][31][3];
	float n[30][3];
	double theta0, theta, z, r;
	int i, ii, j;

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;
	
	theta0 = 2*M_PI/(double)nSlice;
	for(j = 0; j <= nStack; j++)
	{
		z = height * (1.0 - (double)j/(double)nStack);//��ʂ���̍���
		r = rBottom + (rTop - rBottom) * z / height;
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[i][j][0] = (float)(r * cos(theta)); //x����
			p[i][j][1] = (float)(r * sin(theta)); //������
			p[i][j][2] = (float)z-0.5f*height;    //������(����)
		}
	}
	glBegin(GL_TRIANGLES);
	//���(Top)
	glNormal3f(0.0f,0.0f,1.0f);
	for(i = 0; i < nSlice; i++) 
	{ 
		ii = i + 1;
		if(ii == nSlice) ii = 0;
		glVertex3f(0.0f, 0.0f, height/2.0f);
		glVertex3fv(p[i][0]);
		glVertex3fv(p[ii][0]);
	}
	//����(Bottom)
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0f,0.0f,-1.0f); 
	for(i = nSlice-1; i >= 0; i--) 
	{
		ii = i - 1;
		if(i == 0) ii = nSlice - 1;
		glVertex3f(0.0f, 0.0f, -height/2.0f);
		glVertex3fv(p[i][nStack]);
		glVertex3fv(p[ii][nStack]);
	}
	glEnd();
	//����
	
	double nz, nxy, rr;//�����]��
	rr = rBottom - rTop;
	nz = rr / sqrt(rr*rr + height * height);
	nxy = sqrt(1.0 - nz*nz);
	for(i=0; i<nSlice;i++)
	{//����������ł悢
		n[i][0] = (float)(nxy * cos(theta0 * (double)i));
		n[i][1] = (float)(nxy * sin(theta0 * (double)i));
		n[i][2] = (float)nz;
	}
	for(i=0; i < nSlice;i++)
	{//�ׂƂ̘a�̕���
		ii = i+1;
		if(ii == nSlice) ii = 0;
		n[i][0] = (n[i][0] + n[ii][0]) / 2.0f;
		n[i][1] = (n[i][1] + n[ii][1]) / 2.0f;
	}

	glBegin(GL_QUADS);
	for(j=0; j<nStack; j++)
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			glNormal3fv(n[i]);
			glVertex3fv(p[i][j]);
			glVertex3fv(p[i][j+1]);
			glVertex3fv(p[ii][j+1]); 
			glVertex3fv(p[ii][j]);
		}
	glEnd();
}

//-------------------------------------------------------------------------------------
void drawPrism2(float rBottom, float rTop, float height, float thick, int nSlice, int nStack)
{
	//�~��(rBottom=rTop))�A�~����A�~��(rTop = 0.0)
	//rBottom:���ꔼ�a, rTop:��ꔼ�a, thick:����
	//nSlice--xy�f�ʕ�����
	//nStack---������������
	//���̂̒��S�͉���Ə��̒���
	float p[961*2][3];
	float n[30][3];
	double theta0,theta, z, r1, r2;
	double nz, nxy, rr;
	int i, ii, j, N;

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;
	
	N = nSlice * (nStack+1);

	theta0 = 2*M_PI/(double)nSlice;
	for(j=0; j<=nStack; j++)
	{
		z = height * ( 1.0 - (double)j/(double)nStack );//��ʂ���̍���
		r1 = rBottom + (rTop - rBottom) * z / height;//�O�����a
		r2 = r1 - thick;                       //�������a
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[j*nSlice+i][0] = (float)(r1 * cos(theta)); //x����
			p[j*nSlice+i][1] = (float)(r1 * sin(theta)); //������
			p[j*nSlice+i][2] = (float)z-0.5f * height;           //������(����)
		}
		//����
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[j*nSlice+i+N][0] = (float)(r2 * cos(theta)); //x����
			p[j*nSlice+i+N][1] = (float)(r2 * sin(theta)); //������
			p[j*nSlice+i+N][2] = (float)z-0.5f*height;     //������(����)
		}

	}

	//�������ʁi��������`��j
	//�����]��
	rr = rBottom - rTop;
	nz = rr / sqrt(rr*rr + height*height);
	nxy = sqrt(1.0 - nz * nz);
	for(i=0; i<nSlice;i++)
	{//����������ł悢
		n[i][0] = -(float)(nxy * cos(theta0 * (double)i));
		n[i][1] = -(float)(nxy * sin(theta0 * (double)i));
		n[i][2] = -(float)nz;
	}
	for(i=0; i < nSlice;i++)
	{//�ׂƂ̘a�̕���
		ii = i+1;
		if(ii == nSlice) ii = 0;
		n[i][0] = (n[i][0] + n[ii][0]) / 2.0f;
		n[i][1] = (n[i][1] + n[ii][1]) / 2.0f;
	}


	//�������ʂ̕`��
	glBegin(GL_QUADS);
	for(j=0; j<nStack; j++)
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			glNormal3fv(n[i]);
			glVertex3fv(p[j*nSlice+i+N]);
			glVertex3fv(p[j*nSlice+ii+N]);
			glVertex3fv(p[j*nSlice+ii+nSlice+N]); 
			glVertex3fv(p[j*nSlice+i+nSlice+N]);
		}
	glEnd();

	//�O������
	for(i=0; i<nSlice;i++)
	{//�����@�������̋t����
		n[i][0] = -n[i][0];
		n[i][1] = -n[i][1];
		n[i][2] = -n[i][2];
	}

	glBegin(GL_QUADS);
	for(j=0; j<nStack; j++)
	{
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			glNormal3fv(n[i]);
			glVertex3fv(p[j*nSlice+i]);
			glVertex3fv(p[j*nSlice+i+nSlice]);
			glVertex3fv(p[j*nSlice+ii+nSlice]); 
			glVertex3fv(p[j*nSlice+ii]);
		}
	}
	glEnd();

	glBegin(GL_QUADS);
	//���
	for(i=0;i<nSlice;i++)
	{
		ii = i+1;
		if(ii == nSlice) ii = 0;
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3fv(p[i]); glVertex3fv(p[ii]);
		glVertex3fv(p[N+ii]); glVertex3fv(p[N+i]);
		glEnd();
	}
	//����
	for(i=0;i<nSlice;i++)
	{
		ii = i+1;
		if(ii == nSlice) ii = 0;
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3fv(p[nSlice*nStack+i]); glVertex3fv(p[nSlice*nStack+N+i]);
		glVertex3fv(p[nSlice*nStack+N+ii]); glVertex3fv(p[nSlice*nStack+ii]);
	}
	glEnd();
}

//-----------------------------------------------------------------------------------------

void drawTorus(double radius1, double radius2, int nSide, int nRing)
{	
	//radius1:�~�f�ʔ��a
	//radius2:�~�̒��S�����a
	//nSide:�~�f�ʂɂ�����\�ʕ����_��
	//nRing:�~�̕�����
	if(radius1 > radius2) { printf("radius1 < radius2�Ƃ��Ă������� \n "); return;}

	int i, j;
	double rr, zz;
	double phi0, phi1, theta;
	double p[2][3];

	for(i = 0; i < nRing; i++)
	{
		//i=0�͊�{�f��(x=radius2�𒆐S�Ƃ���~, y=0�j
		phi0 = 2.0 * M_PI * (double)i / (double)nRing;
		phi1 = 2.0 * M_PI * (double)(i+1) / (double)nRing;

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			theta = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
			rr = radius2 + radius1 * cos(theta);//z������̋���
			zz = radius1 * sin(theta);
			//���_��xyz���W(j=0�����xy����)
			p[0][0] = rr * cos(phi0);//x���W
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x���W
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z      

			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//���_���W

			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//���_���W
		}
		glEnd();
	}
}

//----------------------------------------------------------------
void drawSpring(float radius1, float radius2, float len, int nSide, int nRing, int nPitch)
{
	//radius1:�~�f�ʔ��a
	//radius2:�~�̒��S�����a
	//len;�΂˒�
	//nSide:�~�f�ʂɂ�����\�ʕ����_��
	//nRing:�~�̕�����
	//nPitch:�s�b�`��

	if(radius2 < radius1) { 
		printf("drawSpring�ɂ����� radius2 > radius1 �Ƃ��Ă������� \n");
	}

	int i, j, k;
	double rr, zz0, zz1;
	double phi0, phi1, theta;
	double p[2][3];
	float pitch, dp;

	pitch = len / (float)nPitch;
	if(pitch < 2.0 * radius1) pitch = radius1 * 2.0;
	dp = (float)pitch / (float)nRing;

	for(k = 0; k < nPitch; k++)
	{
		for(i = 0; i < nRing; i++)
		{
			//i=0�͊�{�f��(x=radius2�𒆐S�Ƃ���~, y=0�j
			phi0 = 2.0 * M_PI * (double)i / (double)nRing;
			phi1 = 2.0 * M_PI * (double)(i+1) / (double)nRing;

			glBegin(GL_QUAD_STRIP);
			for(j = 0; j <= nSide; j++)
			{
				theta = - M_PI + 2.0 * M_PI * (double)j / (double)nSide;
				rr = radius2 + radius1 * cos(theta);//z������̋���
				zz0 = radius1 * sin(theta) + dp * (double)i + pitch * (double)k;
				zz1 = radius1 * sin(theta) + dp * (double)(i+1) + pitch * (double)k;
				//���_��xyz���W(j=0�����xy����)
				p[0][0] = rr * cos(phi0);//x���W
				p[0][1] = rr * sin(phi0);//y
				p[0][2] = zz0;            //z
				p[1][0] = rr * cos(phi1);//x���W
				p[1][1] = rr * sin(phi1);//y
				p[1][2] = zz1;            //z      

				glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
				glVertex3dv(p[0]);//���_���W

				glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
				glVertex3dv(p[1]);//���_���W
			}
			glEnd();
		}
	}
	//�n�[
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	for(j = 0; j < nSide; j++)
	{
		theta = - M_PI + 2.0 * M_PI * (double)j / (double)nSide;
		rr = radius2 + radius1 * cos(theta);//z������̋���
		zz0 = radius1 * sin(theta);
		glVertex3f(rr, 0.0, zz0);
	}
	glEnd();
	//�I�[
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	for(j = 0; j < nSide; j++)
	{
		theta = - M_PI - 2.0 * M_PI * (double)j / (double)nSide;
		rr = radius2 + radius1 * cos(theta) ;//z������̋���
		zz0 = radius1 * sin(theta) + pitch * (double)nPitch;
		glVertex3f(rr, 0.0, zz0);
	}
	glEnd();
}

//-----------------------------------------------------------------
//��2���֐�
void drawSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//�㉺�̒��S�����_
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	double ct,theta,phi,z,cc;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	for(j = 0 ;j <= nStack;j++)
	{
		theta = (M_PI/(double)nStack) * ((double)nStack / 2.0 - (double)j);
		                //theta��x-y���ʂ���̕Ίp�ƂȂ��Ă���

		if(theta >= 0.0) //�㔼��
		{
			if(theta == 0.0) z = 0.0;//pow(a,b)��a��a<=0.0�ŃG���[
			else z = pow(sin(fabs(theta)),eps1);//z
		}
		else  //������        
		{
			z = - pow(sin(fabs(theta)), eps1);
		}
		for (i = 0 ;i <= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//object���猩�č���
			k2 = nSlice * j + nSlice - i;//�E��
			phi = 2.0 * M_PI * (double)i/(double)nSlice;
			ct = cos(phi);
			if( ct == 0.0 ) cc = 0.0;
			else if (ct > 0) { cc = pow(ct, eps2);}
			else         { cc = -pow(fabs(ct),eps2); }
			if(j == 0 || j == nStack) 
			{
				pd[k1][0] = 0.0f;
				pd[k1][1] = 0.0f;
			}

			else 
			{
				pd[k1][0] = r * (float)(pow(cos(theta),eps1)*cc);
				if(sin(phi) == 0.0) pd[k1][1] = 0.0f;
				else pd[k1][1] = r * (float)(pow(cos(theta),eps1)*pow(fabs(sin(phi)),eps2));
			}
			if(i == 0) k2 = k1;
			pd[k2][0] = pd[k1][0];
			pd[k2][1] = -pd[k1][1];
			pd[k1][2] = r * (float)z;
			pd[k2][2] = r * (float)z;
		}
	}

	//���ʂ̖@������
	for(i = 0;i < nSlice;i++)
	{
		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//�^��(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		//�^���iBottom)
		a[i][nStack] = 0.0f; b[i][nStack] = 0.0f; c[i][nStack] = -1.0f;

		for(j=1;j<nStack;j++)//�ׂ荇��4�̎O�p�`�̖@���x�N�g���𕽋ω�
		{
			np = j*nSlice+i;//���ړ_
			npL = j*nSlice+im;//����
			npR = j*nSlice+ip;//�E��
			npU = np-nSlice;//��
			npD = np+nSlice;//��
			if(j == 1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;//Top
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			if(j == nStack-1)
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				n3[0]=0.0f; n3[1]=0.0f; n3[2]=-1.0f;//Bottom
				n4[0]=0.0f; n4[1]=0.0f; n4[2]=-1.0f;//Bottom
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//������
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//��
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//��
		}
	}

	glBegin(GL_QUADS);
	for(i = 0;i < nSlice;i++)
	{
		ip = i + 1;
		if(ip == nSlice) ip = 0;
		for(j = 0;j < nStack; j++)
		{
			glNormal3f(a[i][j],b[i][j],c[i][j]);
			glVertex3fv(pd[i+j*nSlice]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(pd[i+(j+1)*nSlice]);
			glNormal3f(a[ip][j+1],b[ip][j+1],c[ip][j+1]);
			glVertex3fv(pd[ip+(j+1)*nSlice]);
			glNormal3f(a[ip][j],b[ip][j],c[ip][j]);
			glVertex3fv(pd[ip +j*nSlice]);
		}
	}
	glEnd();
}

//-----------------------------------------------------------------
//��2���֐�
void drawSuper(float r, int nSlice, int nStack, double eps1, double eps2, double taper)
{
	//�㉺�̒��S�����_
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	double ct,theta,phi,z,cc,fz;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	for(j = 0 ;j <= nStack;j++)
	{
		theta = (M_PI/(double)nStack) * ((double)nStack / 2.0 - (double)j);
		                //theta��x-y���ʂ���̕Ίp�ƂȂ��Ă���

		if(theta >= 0.0) //�㔼��
		{
			if(theta == 0.0) z = 0.0;//pow(a,b)��a��a<=0.0�ŃG���[
			else z = pow(sin(fabs(theta)),eps1);//z
		}
		else  //������        
		{
			z = - pow(sin(fabs(theta)), eps1);
		}
		fz = 0.5f * (taper+1.0f +(taper -1.0f) * z);
		for (i = 0 ;i <= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//object���猩�č���
			k2 = nSlice * j + nSlice - i;//�E��
			phi = 2.0 * M_PI * (double)i/(double)nSlice;
			ct = cos(phi);
			if( ct == 0.0 ) cc = 0.0;
			else if (ct > 0) { cc = pow(ct, eps2);}
			else         { cc = -pow(fabs(ct),eps2); }
			if(j == 0 || j == nStack) 
			{
				pd[k1][0] = 0.0f;
				pd[k1][1] = 0.0f;
			}

			else 
			{
				pd[k1][0] = r * (float)(pow(cos(theta),eps1)*cc*fz);
				if(sin(phi) == 0.0) pd[k1][1] = 0.0f;
				else pd[k1][1] = r * (float)(pow(cos(theta),eps1)*pow(fabs(sin(phi)),eps2)*fz);
			}
			if(i == 0) k2 = k1;
			pd[k2][0] = pd[k1][0];
			pd[k2][1] = -pd[k1][1];
			pd[k1][2] = r * (float)z;
			pd[k2][2] = r * (float)z;
		}
	}

	//���ʂ̖@������
	for(i = 0;i < nSlice;i++)
	{
		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//�^��(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		//�^���iBottom)
		a[i][nStack] = 0.0f; b[i][nStack] = 0.0f; c[i][nStack] = -1.0f;

		for(j=1;j<nStack;j++)//�ׂ荇��4�̎O�p�`�̖@���x�N�g���𕽋ω�
		{
			np = j*nSlice+i;//���ړ_
			npL = j*nSlice+im;//����
			npR = j*nSlice+ip;//�E��
			npU = np-nSlice;//��
			npD = np+nSlice;//��
			if(j == 1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;//Top
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			if(j == nStack-1)
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				n3[0]=0.0f; n3[1]=0.0f; n3[2]=-1.0f;//Bottom
				n4[0]=0.0f; n4[1]=0.0f; n4[2]=-1.0f;//Bottom
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//������
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//��
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//��
		}
	}

	glBegin(GL_QUADS);
	for(i = 0;i < nSlice;i++)
	{
		ip = i + 1;
		if(ip == nSlice) ip = 0;
		for(j = 0;j < nStack; j++)
		{
			glNormal3f(a[i][j],b[i][j],c[i][j]);
			glVertex3fv(pd[i+j*nSlice]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(pd[i+(j+1)*nSlice]);
			glNormal3f(a[ip][j+1],b[ip][j+1],c[ip][j+1]);
			glVertex3fv(pd[ip+(j+1)*nSlice]);
			glNormal3f(a[ip][j],b[ip][j],c[ip][j]);
			glVertex3fv(pd[ip +j*nSlice]);
		}
	}
	glEnd();
}

void drawSuper(float* size1, float* size2, int nSlice, int nStack, double eps1, double eps2, float middle, float angle, int jStart, int type)
{	
	//�㉺�̒��S�����_
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	double ct,phi,theta,z,fz;
	float a[31][31],b[31][31],c[31][31];
	float n1[3],n2[3],n3[3],n4[3];
	double cc, xx, yy, zz, aa, sr, cr;
	float pd[961][3];

	if( nSlice > 30 ) nSlice = 30;
	if( nStack > 30 ) nStack = 30;

  for(j = 0 ;j <= nStack;j++)
	{
    theta = (M_PI/(double)nStack) * ((double)nStack / 2.0 - (double)j);
    if(theta > 0.0) z = (float)(pow(sin(theta),eps1));//z
    else z = - (float)(pow(sin(fabs(theta)), eps1));
    //�`��֐�
    if(z < 0.0) fz = (middle-1.0)*z + middle;
    else fz = (1.0-middle)*z + middle;

    for (i = 0 ;i <= nSlice/2 ;i++)
    {
			k1 = nSlice * j + i;//�������猩�č���(x > 0)
			k2 = nSlice * j + nSlice - i;//�E��(x < 0)
			phi = 2.0 * M_PI * (double)i/(double)nSlice;
			ct = cos(phi);
			if (ct >= 0) { cc = pow(ct, eps2);}
      else         { cc = -pow(fabs(ct),eps2); }
			if(j == 0 || j == nStack) 
			{
				pd[k1][0] = 0.0f;
				pd[k1][1] = 0.0f;
			}
			else 
			{
				if(j <= nStack/2)
				{
					pd[k1][0] = size1[0] * (float)(pow(cos(theta),eps1)*cc*fz);
					pd[k1][1] = size1[1] * (float)(pow(cos(theta),eps1)*pow(fabs(sin(phi)),eps2)*fz);
				}
				else
				{
					pd[k1][0] = size2[0] * (float)(pow(cos(theta),eps1)*cc*fz);
					pd[k1][1] = size2[1] * (float)(pow(cos(theta),eps1)*pow(fabs(sin(phi)),eps2)*fz);
				}
			}
			if(i == 0) k2 = k1;

			pd[k2][0] = pd[k1][0];
			pd[k2][1] = -pd[k1][1];
			if(j <= nStack/2)
			{
				pd[k1][2] = size1[2] * (float)z;
				pd[k2][2] = size1[2] * (float)z;
			}
			else
			{
				pd[k1][2] = size2[2] * (float)z;
				pd[k2][2] = size2[2] * (float)z;
			}
		}
	}

	//�ό`
	if(type == 0)
	{
		//�O���Fz��
		//z����](x>0�Ȃ琳��z����]�Cx<0�Ȃ畉��z����]�j
		for(j = jStart; j < nStack; j++)
		{
			for(i = 0; i <= nSlice/2; i++)
			{
				k1 = nSlice * j + i;//�������猩�č���(x > 0)
				k2 = nSlice * j + nSlice - i;//�E��(x < 0)
				xx = pd[k1][0]; yy = pd[k1][1];
				if(j <= nStack/2)
					aa = M_PI * angle * fabs(xx) / size1[0] / 180.0;
				else
					aa = M_PI * angle * fabs(xx) / size2[0] / 180.0;
				cr = cos(aa);
				sr = sin(aa);
				if(xx > 0.0)
				{
					pd[k1][0] = xx * cr - yy * sr;
					pd[k1][1] = xx * sr + yy * cr;
				}
				else
				{
					pd[k1][0] = xx * cr + yy * sr ;
					pd[k1][1] = -xx * sr + yy * cr;
				}

				xx = pd[k2][0]; yy = pd[k2][1];
				if(j <= nStack/2)
					aa = M_PI * angle * fabs(xx) / size1[0] / 180.0;
				else
					aa = M_PI * angle * fabs(xx) / size2[0] / 180.0;
				
				cr = cos(aa);
				sr = sin(aa);
				if(xx > 0.0)
				{
					pd[k2][0] = (xx * cr - yy * sr) ;
					pd[k2][1] = (xx * sr + yy * cr) ;
				}
				else
				{
					pd[k2][0] = (xx * cr + yy * sr) ;
					pd[k2][1] = (-xx * sr + yy * cr) ;
				}
			}
		}
	}

	else if(type == 1)
	{
		//�O���Fz��
		//�㔼���㉺�ix����]�j
		for(j = jStart; j <= nStack; j++)
		{
			for(i = 0; i < nSlice; i++)
			{
				k1 = nSlice * j + i;
				yy = pd[k1][1]; zz = pd[k1][2];
				aa = M_PI * angle / 180.0 * (float)(j-jStart) / (float)nSlice;
				cr = cos(aa);
				sr = sin(aa);
				pd[k1][1] = yy * cr - zz * sr;
				pd[k1][2] = yy * sr + zz * cr;
				
			}
		}
	}

	else if(type == 2)
	{
		//�O���Fz��
		//�㔼�����E�iy����]�j
		for(j = jStart; j <= nStack; j++)
		{
			for(i = 0; i < nSlice; i++)
			{
				k1 = nSlice * j + i;
				xx = pd[k1][0]; zz = pd[k1][2];
				aa = M_PI * angle / 180.0 * (float)(j-jStart) / (float)nSlice;
				cr = cos(aa);
				sr = sin(aa);
				pd[k1][0] = xx * cr + zz * sr;
				pd[k1][2] = -xx * sr + zz * cr;
				
			}
		}
	}
	
	//���ʂ̖@������
	for(i = 0;i < nSlice;i++)
	{
 		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//�^��(Top)
		a[i][0] = 0.0; b[i][0] = 0.0; c[i][0] = 1.0;
		//�^���iBottom)
		a[i][nStack] = 0.0; b[i][nStack] = 0.0; c[i][nStack] = -1.0;

		for(j=1;j<nStack;j++)//�ׂ荇��4�̎O�p�`�̖@���x�N�g���𕽋ω�
		{
			np = j*nSlice+i;//���ړ_
			npL = j*nSlice+im;//����
			npR = j*nSlice+ip;//�E��
			npU = np-nSlice;//��
			npD = np+nSlice;//��
			if(j == 1) {
				n1[0]=0.0; n1[1]=0.0; n1[2]=1.0;//Top
				n2[0]=0.0; n2[1]=0.0; n2[2]=1.0;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			if(j == nStack-1){
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				n3[0]=0.0; n3[1]=0.0; n3[2]=-1.0;//Bottom
				n4[0]=0.0; n4[1]=0.0; n4[2]=-1.0;//Bottom
			}
			else {
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//������
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//��
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//��
		}
	}
	//�\��
	glBegin(GL_TRIANGLES);
	//���
	for(i = 0;i < nSlice;i++)
	{	ip = i+1;
		if(ip == nSlice) ip = 0;

			glNormal3f(a[i][0],b[i][0],c[i][0]);glVertex3fv(pd[i]);
			glNormal3f(a[i][1],b[i][1],c[i][1]);glVertex3fv(pd[nSlice+i]);
			glNormal3f(a[ip][1],b[ip][1],c[ip][1]);glVertex3fv(pd[nSlice+ip]);
	}
	//����
	j = nStack - 1;
	for(i = 0;i < nSlice;i++)
	{	ip = i+1;
		if(ip == nSlice) ip = 0;
			glNormal3f(a[i][j],b[i][j],c[i][j]);glVertex3fv(pd[j*nSlice+i]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);glVertex3fv(pd[(j+1)*nSlice+i]);
			glNormal3f(a[ip][j],b[ip][j],c[ip][j]);glVertex3fv(pd[j*nSlice+ip]);
	}
	glEnd();
	//����(4�p�`�p�b�`�j
	glBegin(GL_QUADS);
	for(j = 0;j < nStack;j++)
		for(i = 0;i < nSlice;i++)
		{	ip = i+1;
			if(ip == nSlice) ip=0;

				glNormal3f(a[i][j],b[i][j],c[i][j]);glVertex3fv(pd[j*nSlice+i]);
				glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);glVertex3fv(pd[(j+1)*nSlice+i]);
				glNormal3f(a[ip][j+1],b[ip][j+1],c[ip][j+1]);glVertex3fv(pd[(j+1)*nSlice+ip]);
				glNormal3f(a[ip][j],b[ip][j],c[ip][j]);glVertex3fv(pd[j*nSlice+ip]);
		}
	glEnd();
}

//-----------------------------------------------------------------
//�㔼���̒�2���֐�
void drawHemiSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//��̒��S�����_
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	double ct,theta,phi,z,cc;
	double e1, e2;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	//�㔼������
	for(j = 0 ;j <= nStack ;j++)
	{
		theta = (M_PI/(double)nStack) * ((double)nStack  - (double)j)/2.0;
		                          //theta��x-y���ʂ���̕Ίp�ƂȂ��Ă���
		e1 = fabs(sin(theta));
		if( e1 == 0.0) z = 0.0;
		else z = (float)(pow(e1, eps1));//z
		for (i = 0 ;i<= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//�O���猩�ĉE��
			k2 = nSlice * j + nSlice - i;//����
			phi = 2.0*M_PI * (double)i / (double)nSlice;
			ct = cos(phi);
			if( ct == 0.0) cc = 0.0;
			else if (ct > 0) { cc = pow(ct, eps2); }
			else         { cc = -pow(fabs(ct),eps2); }
			if(j == 0) 
			{
				pd[k1][0] = 0.0f;
				pd[k1][1] = 0.0f;
			}
			else
			{
				e1 = fabs(cos(theta));
				if( e1 == 0.0) 
				{
					pd[k1][0] = 0.0f;
					pd[k1][1] = 0.0f;
				}
				else
				{
					pd[k1][0] = r * (float)(pow(e1,eps1) * cc);
					e2 = fabs(sin(phi));
					if( e2 == 0.0) pd[k1][1] = 0.0f;
					else
						pd[k1][1] = r * (float)(pow(e1,eps1) * pow(e2,eps2));
				}
			}
			pd[k2][0] = pd[k1][0];
			pd[k2][1] = -pd[k1][1];
			pd[k1][2] = r * (float)z;
			pd[k2][2] = r * (float)z;
		}
	}

	//���ʂ̖@������
	for(i = 0;i < nSlice; i++)
	{
		ip = i + 1;
		if(ip==nSlice) ip = 0;
		im=i-1;
		if(i==0) im = nSlice - 1;

		//�^��(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		np = 0; npU = 0; npL = 0; npR = 0; npU = 0;

		for(j=1;j<nStack;j++)
		{
			np = j*nSlice+i;//���ړ_
			npL = j*nSlice+im;//����
			npR = j*nSlice+ip;//�E��
			npU = np-nSlice;//��
			npD = np+nSlice;//��
			if(j==1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
				calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
				calcNormal(pd[np],pd[npL],pd[npD],n3);//�O���猩�č���
				calcNormal(pd[np],pd[npD],pd[npR],n4);//�E��
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//������
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//��
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//��
		}
		j = nStack;//��ԉ��̑���(���2�̎O�p�`�̕��ρj
		calcNormal(pd[np],pd[npU],pd[npL],n1);//�O���猩�č���
		calcNormal(pd[np],pd[npR],pd[npU],n2);//�E��
		a[i][j] = (float)((n1[0]+n2[0])/2.0f);
		b[i][j] = (float)((n1[1]+n2[1])/2.0f);
		c[i][j] = (float)((n1[2]+n2[2])/2.0f);
	}

	glBegin(GL_QUADS);
	for(i = 0;i < nSlice;i++)
	{
		ip = i + 1; 
		if(ip == nSlice) ip = 0;
		for(j = 0;j < nStack; j++)
		{
			glNormal3f(a[i][j],b[i][j],c[i][j]);
			glVertex3fv(pd[i+j*nSlice]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(pd[i+(j+1)*nSlice]);
			glNormal3f(a[ip][j+1],b[ip][j+1],c[ip][j+1]);
			glVertex3fv(pd[ip+(j+1)*nSlice]);
			glNormal3f(a[ip][j],b[ip][j],c[ip][j]);
			glVertex3fv(pd[ip +j*nSlice]);
		}
	}
	glEnd();
	
	//��
	j = nStack;
	glBegin(GL_POLYGON);
	glNormal3f(0.0f,0.0f,-1.0f);//-z����
	for(i = nSlice-1;i >= 0;i--) glVertex3fv(pd[i+j*nSlice]);
	glEnd();
}

//---------------------------------------------------------------------------
void drawRevolution1(float cs[][2], int nSide, int nSlice)
{
	//
	//�f�ʍ��Wcs[][]��x-z���ʂŗ^����(����_�j
	//( cs[][0]:x���W�Ccs[][1]:z���W)
	//��]���F����
	//�n�_�ƏI�_�͈قȂ�C������ɂ��邱��
	//nSide:�f�ʕ����_��(x-z���ʁj
	//nSlice:xy�f�ʕ�����
	int i, ii, j, jj, jm, jp;
	double phi;//���ʊp
	float p[31][31][3];
	float n0[30][2];//x-z��{�f�ʂ̊e�Ő��̖@���ix,z�����j
	float n1[30][2];//x-z��{�f�ʂ̊e���_�̖@���ix,z�����j
	float nn[31][31][3];//�e���_�̖@��
	double len, dx, dz;

	if(nSide > 30) { printf("nSide < 30�Ƃ��邱�� \n"); }//exit(1); }
	if(nSlice > 30) nSlice = 30;

	int nSide2;
	if(cs[0][0] == 0.0 && cs[nSide-1][0] == 0.0)//�n�_�ƏI�_��������
			nSide2 = nSide-1;
	else nSide2 = nSide;

	//��{�f�ʂɂ�����e�Ő��̖@���i�Q�����j
	for(j = 0; j < nSide; j++)
	{
		jp = j + 1;
		if(jp == nSide) jp = 0;
		dx = (double)(cs[jp][0] - cs[j][0]);
		dz = (double)(cs[jp][1] - cs[j][1]);
		len = sqrt( dx*dx +  dz*dz);
		n0[j][0] = -dz / len;//x����
		n0[j][1] = dx / len;//y����
	}

	//x-z�f�ʏ�̒��_�̖@���i�Q����)
	if(cs[0][0] == 0.0 && cs[nSide-1][0] == 0.0)//�n�_�ƏI�_��������
	{
		n1[0][0] = 0.0; n1[0][1] = 1.0;//�n�_
		n1[nSide-1][0] = 0.0; n1[nSide-1][1] = -1.0;//�I�_;
		for(j = 1; j < nSide-1; j++)
		{
			n1[j][0] = (n0[j-1][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[j-1][1] + n0[j][1]) / 2.0;
		}
	}
	else//�n�_�ƏI�_�̗����܂��͂ǂ��炩��������ɂȂ��ꍇ�j
	{
		for(j = 0; j < nSide; j++)
		{
			jm = j - 1;
			if(j == 0) jm = nSide - 1;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;
		}
	}

	//���̒f�ʂ̍��W�Ɩ@��(�R����)
	for(i = 0; i < nSlice; i++)
	{
		phi = 2.0*M_PI*(double)i/(double)nSlice;//���ʊp
		for(j = 0; j < nSide; j++)
		{	
			p[i][j][0] = cs[j][0] * (float)cos(phi); //x���W
			p[i][j][1] = cs[j][0] * (float)sin(phi); //y
			p[i][j][2] = cs[j][1];                   //z
			nn[i][j][0] = n1[j][0] * (float)cos(phi);
			nn[i][j][1] = n1[j][0] * (float)sin(phi);
			nn[i][j][2] = n1[j][1] ;
		}
	}

	//���_����`���`��
	glBegin(GL_QUADS);
	for(i = 0; i < nSlice; i++)
	{
		ii = i+1;
		if(ii == nSlice) ii = 0;
		for(j = 0;j < nSide2;j++)
		{
			jj = j+1;

			if(jj == nSide) jj = 0;
			glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]); 
			glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
			glNormal3fv(nn[ii][jj]);glVertex3fv(p[ii][jj]); 
			glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
		}
	}
	glEnd();
}

//------------------------------------------------------------------------------
void drawRevolution2(float cs0[][2], int nCont, int nSlice, int nDiv)
{
	//��]���F����
	//�n�_�ƏI�_�͈�v(nSide�͏I�_���܂ށj
	//nCont:�f�ʕ����_��(����_���j
	//nSlice:���S���ɉ����������_��
	int i, ii, j, jj, jm;
	int nSide;
	double phi; //���ʊp
	float p[31][81][3];
	float cs[81][2];//�f�ʍ��W
	float n0[80][2];//x-z��{�f�ʂ̊e�Ő��̖@���ix,z�����j
	float n1[80][2];//x-z��{�f�ʂ̊e���_�̖@���ix,z�����j
	float nn[31][81][3];//�e���_�̖@��
	double len, dx, dz;

	if(nCont > 20) printf("nCont <= 20�Ƃ��邱�� \n");
	if(nDiv > 4) printf("nDiv <= 4�Ƃ��邱�� \n");
	if(nSlice > 30) nSlice = 30;

	if(cs0[0][0] == 0.0 && cs0[nCont-1][0] == 0.0)//�n�_�ƏI�_��z����
	{
		//Spline���
		spline2(cs0, cs, nCont, nDiv);
		nSide = (nCont -1) * nDiv + 1;//�V���������_��
		//��{�f�ʂɂ�����e�Ő��̖@���i�Q�����j
		for(j = 0; j < nSide-1; j++)
		{
			jj = j + 1;
			dx = (double)(cs[jj][0] - cs[j][0]);
			dz = (double)(cs[jj][1] - cs[j][1]);
			len = sqrt( dx*dx +  dz*dz);
			n0[j][0] = -dz / len;
			n0[j][1] = dx / len;
		}

		//x-z�f�ʏ�̒��_�̖@���i�Q����)
		n1[0][0] = 0.0; n1[0][1] = 1.0;//�n�_��z������
		n1[nSide-1][0] = 0.0; n1[nSide-1][1] = -1.0;//�I�_�́|z������
		for(j = 1; j < nSide-1; j++)
		{
			jm = j - 1;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;//x����
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;//z����
		}

		//���̒f�ʂ̍��W�Ɩ@��(�R����)
		for(i = 0; i < nSlice; i++)
		{
			phi = 2.0*M_PI*(double)i/(double)nSlice;
			for(j = 0; j < nSide; j++)
			{	
				p[i][j][0] = cs[j][0] * (float)cos(phi); //x���W
				p[i][j][1] = cs[j][0] * (float)sin(phi); //y
				p[i][j][2] = cs[j][1];                   //z
				nn[i][j][0] = n1[j][0] * (float)cos(phi);
				nn[i][j][1] = n1[j][0] * (float)sin(phi);
				nn[i][j][2] = n1[j][1] ;
			}
		}

		//���_����`���`��
		glBegin(GL_QUADS);
		for(i = 0; i < nSlice; i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			for(j = 0;j < nSide-1;j++)
			{
				jj = j + 1;
				glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]); 
				glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
				glNormal3fv(nn[ii][jj]);glVertex3fv(p[ii][jj]); 
				glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
			}
		}
		glEnd();
	}

	else
	{
		//�n�_�܂��͏I�_��������ɂȂ���Ύn�_���I�_�ƂȂ�悤��1���₷
		float cs1[21][2];
		for(j = 0; j < nCont; j++)
		{
			cs1[j][0] = cs0[j][0];
			cs1[j][1] = cs0[j][1];
		}
		nCont ++;
		cs1[nCont-1][0] = cs0[0][0];
		cs1[nCont-1][1] = cs0[0][1];
	
		//Spline���
		spline2(cs1, cs, nCont, nDiv);
		nSide = (nCont -1) * nDiv + 1;//�V���������_��
		//��{�f�ʂɂ�����e�Ő��̖@���i�Q�����j
		for(j = 0; j < nSide-1; j++)
		{
			jj = j + 1;
			if(jj == nSide - 1) jj = 0;
			dx = (double)(cs[jj][0] - cs[j][0]);
			dz = (double)(cs[jj][1] - cs[j][1]);
			len = sqrt( dx*dx +  dz*dz);
			n0[j][0] = -dz / len;
			n0[j][1] = dx / len;
		}

		//��{�f�ʂɂ�����e���_�̖@���i�Q�����j
		for(j = 0; j < nSide-1; j++)
		{
			jm = j - 1;
			if(j == 0) jm = nSide-2;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;//x����
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;//z����
		}

		//���̒f�ʂ̍��W�Ɩ@��(�R����)
		for(i = 0; i < nSlice; i++)
		{
			phi = 2.0*M_PI*(double)i/(double)nSlice;
			for(j = 0; j < nSide-1; j++)
			{	
				p[i][j][0] = cs[j][0] * (float)cos(phi); //x���W
				p[i][j][1] = cs[j][0] * (float)sin(phi); //y
				p[i][j][2] = cs[j][1];                   //z
				nn[i][j][0] = n1[j][0] * (float)cos(phi);
				nn[i][j][1] = n1[j][0] * (float)sin(phi);
				nn[i][j][2] = n1[j][1] ;
			}
		}

		//���_����`���`��
		glBegin(GL_QUADS);
		for(i = 0; i < nSlice; i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			for(j = 0;j < nSide-1;j++)
			{
				jj = j + 1;
				if(jj == nSide - 1) jj = 0;
					glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]); 
					glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
					glNormal3fv(nn[ii][jj]);glVertex3fv(p[ii][jj]); 
					glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
			}
		}
		glEnd();
	}
}

//------------------------------------------------------------------------
void drawSweepLine1(float cs[][2], int nSide, float sp[][3], int nSpine)
{//���������o��
	//��̒f�ʂ�x-y���ʁi���_���z�������猩�č����)
	//�ʂ̖@���͂S���_�̍��W�Ō��߂�
	//��{�p���ł͒f�ʂ͂��ׂĂ������܂���-z����
	//��̒��S�����_
	//nSide:�f�ʕ����_��
	//nSpine:�o�H�i�w���j�̌��i�n�_�C�I�_���܂ށj
	int i, ii, j, jj;
	float p[31][31][3];
	float n[3];

	if(nSide > 30) printf("nSide < 30�Ƃ��邱�� \n");
	if(nSpine > 30) nSpine = 30;
	//���_���W
	for(j = 0; j < nSpine; j++)//j�͉�������
	{
		for(i = 0; i < nSide; i++)
		{	
			p[i][j][0] = cs[i][0] + sp[j][0];//x���W
			p[i][j][1] = cs[i][1] + sp[j][1]; //y
			p[i][j][2] = sp[j][2] ;                      //z
		}
	}

	//���_����`���`��
	for(j = 0; j < nSpine-1; j++)
	{
		jj = j+1;
		for(i = 0;i < nSide;i++)
		{
			ii = i+1;
			if(ii == nSide) ii = 0;
			calcNormal(p[i][j],p[ii][j],p[ii][jj],n);
			glBegin(GL_QUADS);
			glNormal3fv(n);
			glVertex3fv(p[i][j]); 
			glVertex3fv(p[ii][j]);
			glVertex3fv(p[ii][jj]); 
			glVertex3fv(p[i][jj]);
			glEnd();
		}
	}

	//����
	glBegin(GL_TRIANGLES);
	glNormal3f(0.0, 0.0, -1.0);
	for(i = 0; i < nSide; i++)
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][0]); 
		glVertex3fv(sp[0]);
		glVertex3fv(p[ii][0]); 
	}
	//���
	glNormal3f(0.0, 0.0, 1.0);
	for(i = 0; i < nSide; i++) 
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][nSpine-1]);
		glVertex3fv(p[ii][nSpine-1]);
		glVertex3fv(sp[nSpine-1]);
	}
	glEnd();
}
//------------------------------------------------------------------------
void drawSweep1(float cs[][2], int nSide, float sp[][3], int nSpine)
{
	//��̒f�ʂ�x-y���ʁi���_���z�������猩�č����)
	//cs�F�f�ʍ��W(x-y���ʁj
	//sp�F�o�H�i�w���j���W
	//��̒��S�����_(sp[0][2]�͕K��0�ł��邱�Ɓj
	//nSide:�f�ʕ����_��
	//nSpine:�o�H�i�w���j�̌��i�n�_�C�I�_���܂ށj
	//�f�ʂ͎������ɒ�������悤�ɕϊ�
	int i, ii, j, jj, im, k;
	float p[31][31][3];
	//CVector P[31][31];

	float nn[31][31][3];//�e���_�̖@��
	double len, x, y, z;

	if(nSide > 30) printf("nSide < 30�Ƃ��邱�� \n");
	if(nSpine > 30) nSpine = 30;

	if(sp[0][0] != 0.0 || sp[0][1] != 0.0 || sp[0][2] != 0.0) printf("�o�H�ߓ_�f�[�^���s���ł� \n"); 
	if(sp[1][0] != 0.0 || sp[1][1] != 0.0 || sp[1][2] == 0.0) printf("�o�H�ߓ_�f�[�^���s���ł� \n"); 
	//�������x�N�g��
	double e[30][3];
	for(j = 0; j < nSpine-1; j++)
	{
		for(k = 0; k < 3; k++) e[j][k] = sp[j+1][k] - sp[j][k];
		len = sqrt(e[j][0] * e[j][0] + e[j][1] * e[j][1] + e[j][2] * e[j][2]);
		for(k = 0; k <3; k++) e[j][k] /= len;//�P�ʃx�N�g����
	}
	//�f�ʂ̕����x�N�g��
	double a[30][3];
	for(k = 0; k <3; k++) a[0][k] = e[0][k];//�n�_�̒f��
	for(k = 0; k <3; k++) a[nSpine-1][k] = e[nSpine-2][k];//�I�[�̒f��
	for(j = 1; j < nSpine-1; j++)
	{
		for(k = 0; k <3; k++) a[j][k] = (e[j-1][k] + e[j][k]) / 2.0;
	}

	//�f�ʒ��_�̍��W(�R����)
	//�ŏ��̖ʂ̒��_���W
	CVector E = CVector(0.0, 0.0, 1.0);//�������x�N�g��
	CVector A = CVector(a[0][0], a[0][1], a[0][2]);//�f�ʂ̖@���x�N�g��
	CVector P = CVector();//�O�̒f�ʂ̒��_�ʒu�x�N�g��
	CVector P0 = CVector(sp[0][0], sp[0][1], sp[0][2]);//���ڂ��Ă���f�ʂ̒��S�i�ߓ_)�̈ʒu�x�N�g��
	CVector newP = CVector();//���ڒf�ʂ̐V�������_�ʒu�x�N�g��
	double b ;//= A * E;//�x�N�g���̓���
	double c ;
	for(i = 0; i < nSide; i++)
	{
		p[i][0][0] = cs[i][0];
		p[i][0][1] = cs[i][1];
		p[i][0][2] = 0.0;;
	}

	for(j = 1; j < nSpine; j++)
	{
		for(i = 0; i < nSide; i++)
		{
			P = CVector(p[i][j-1][0], p[i][j-1][1], p[i][j-1][2]);
			P0 = CVector(sp[j][0], sp[j][1], sp[j][2]);
			A = CVector(a[j][0], a[j][1], a[j][2]);
			E = CVector(e[j-1][0], e[j-1][1], e[j-1][2]);
			b = A * E;
			c = A * (P0 - P);
			newP = P + E * (c / b);

			p[i][j][0] = newP.x;
			p[i][j][1] = newP.y;
			p[i][j][2] = newP.z;
		}
	}

	//�f�ʑ��ʏ�̒��_�̖@��
	CVector L = CVector();
	CVector N[30];
	CVector NN[30];
	for(j = 0; j < nSpine; j++)
	{
		//�Ő��̖@��
		for(i = 0; i < nSide; i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			x = p[ii][j][0] - p[i][j][0];
			y = p[ii][j][1] - p[i][j][1];
			z = p[ii][j][2] - p[i][j][2];
			L = CVector(x, y, z);//�Ő��x�N�g��
			A = CVector(a[j][0], a[j][1], a[j][2]);
			N[i] = L ^ A;//�O��
			N[i].normalize();
		}
		//���_�̖@��
		for(i = 0; i < nSide; i++)
		{
			im = i - 1;
			if(i == 0) im = nSide-1;
			NN[i] = (N[i] + N[im]) / 2.0;
			NN[i].normalize();
			nn[i][j][0] = NN[i].x;
			nn[i][j][1] = NN[i].y;
			nn[i][j][2] = NN[i].z;
		}
	}

	//���_����`���`��
	glBegin(GL_QUADS);
	for(j = 0; j < nSpine-1; j++)
	{
		jj = j+1;
		for(i = 0;i < nSide;i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			
			glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]); 
			glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
			glNormal3fv(nn[ii][jj]);glVertex3fv(p[ii][jj]); 
			glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
		}
	}
	glEnd();
	
	//����
	float n[3];
	calcNormal(p[0][0],sp[0],p[1][0],n);
	glBegin(GL_TRIANGLES);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++)
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][0]); 
		glVertex3fv(sp[0]);
		glVertex3fv(p[ii][0]); 
	}
	glEnd();
	//���
	calcNormal(p[0][nSpine-1], p[1][nSpine-1], sp[nSpine-1],n);
	glBegin(GL_TRIANGLES);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++) 
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][nSpine-1]);
		glVertex3fv(p[ii][nSpine-1]);
		glVertex3fv(sp[nSpine-1]);
	}
	glEnd();
}
//------------------------------------------------------------------------
void drawSweep2(float cs0[][2], int nSide0, float sp0[][3], int nSpine0, int nDiv1, int nDiv2)
{
	//�f�ʂƌo�H�ǂ����3���X�v���C�����
	//��̒f�ʂ�x-y���ʁi���_���z�������猩�č����)
	//cs�F�f�ʍ��W(x-y���ʁj
	//sp�F�o�H�i�w���j���W
	//��̒��S�����_(sp[0][2]�͕K��0�ł��邱�Ɓj
	//nSide:�f�ʕ����_��
	//nSpine:�o�H�i�w���j�̌��i�n�_�C�I�_���܂ށj
	//nDiv1�F�f�ʂ̃X�v���C��������
	//nDiv2�F�o�H�̃X�v���C��������
	int i, ii, j, jj, im, k;
	float p[81][81][3];
	float nn[81][81][3];//�e���_�̖@��
	double len, x, y, z;
	float cs[81][2], cs1[21][2];
	float sp[81][3];
	int nSide, nSpine;//spline��Ԍ�̒f�ʕ������C�o�H��

	if(nSide0 > 20) printf("nSide0 <= 20�Ƃ��邱�� \n");
	if(nSpine0 > 20) printf("nSpine0 <= 20�Ƃ��邱�� \n");
	if(nDiv1 > 4) printf("nDiv1 <= 4�Ƃ��邱�� \n");
	if(nDiv2 > 4) printf("nDiv2 <= 4�Ƃ��邱�� \n");
	if(sp0[0][0] != 0.0 || sp0[0][1] != 0.0 || sp0[0][2] != 0.0)
	{
		printf("�o�H�ߓ_�f�[�^���s���ł�(drawSweep2) \n"); 
		return;
	}
	if(sp0[1][0] != 0.0 || sp0[1][1] != 0.0 || sp0[1][2] == 0.0)
	{
		printf("�o�H�ߓ_�f�[�^���s���ł�(drawSweep2) \n"); 
		return;
	}

	//�f�ʂ̃X�v���C�����
	for(i = 0; i < nSide0; i++)
	{
		cs1[i][0] = cs0[i][0];
		cs1[i][1] = cs0[i][1];
	}
	//�������E������spline��Ԃ̂��߂Ɏn�_���I�_�Ƃ���
	nSide0 ++;
	cs1[nSide0-1][0] = cs0[0][0];
	cs1[nSide0-1][1] = cs0[0][1];
	spline2(cs1, cs, nSide0, nDiv1);//2������Spline
	nSide = (nSide0 -1) * nDiv1;//�V���������_��
	//�o�H�̃X�v���C�����
	spline3(sp0, sp, nSpine0, nDiv2);//3������Spline
	nSpine = (nSpine0-1) * nDiv2 + 1;

	//�������x�N�g��
	double e[80][3];
	for(j = 0; j < nSpine-1; j++)
	{
		for(k = 0; k < 3; k++) e[j][k] = sp[j+1][k] - sp[j][k];
		len = sqrt(e[j][0] * e[j][0] + e[j][1] * e[j][1] + e[j][2] * e[j][2]);
		for(k = 0; k <3; k++) e[j][k] /= len;//�P�ʃx�N�g����
	}
	//�f�ʂ̕����x�N�g��
	double a[80][3];
	for(k = 0; k <3; k++) a[0][k] = e[0][k];//�n�_�̒f��
	for(k = 0; k <3; k++) a[nSpine-1][k] = e[nSpine-2][k];//�I�[�̒f��
	for(j = 1; j < nSpine-1; j++)
	{
		for(k = 0; k <3; k++) a[j][k] = (e[j-1][k] + e[j][k]) / 2.0;
	}

	//�f�ʒ��_�̍��W(�R����)
	//�ŏ��̖ʂ̒��_���W
	CVector E = CVector(0.0, 0.0, 1.0);//�������x�N�g��
	CVector A = CVector(a[0][0], a[0][1], a[0][2]);//�f�ʂ̖@���x�N�g��
	CVector P = CVector();//�O�̒f�ʂ̒��_�ʒu�x�N�g��
	CVector P0 = CVector(sp[0][0], sp[0][1], sp[0][2]);//���ڂ��Ă���f�ʂ̒��S�i�ߓ_)�̈ʒu�x�N�g��
	CVector newP = CVector();//���ڒf�ʂ̐V�������_�ʒu�x�N�g��
	double b ;//= A * E;//�x�N�g���̓���
	double c ;
	for(i = 0; i < nSide; i++)
	{
		p[i][0][0] = cs[i][0];
		p[i][0][1] = cs[i][1];
		p[i][0][2] = 0.0;;
	}

	for(j = 1; j < nSpine; j++)
	{
		for(i = 0; i < nSide; i++)
		{
			P = CVector(p[i][j-1][0], p[i][j-1][1], p[i][j-1][2]);
			P0 = CVector(sp[j][0], sp[j][1], sp[j][2]);
			A = CVector(a[j][0], a[j][1], a[j][2]);
			E = CVector(e[j-1][0], e[j-1][1], e[j-1][2]);
			b = A * E;
			c = A * (P0 - P);
			newP = P + E * (c / b);

			p[i][j][0] = newP.x;
			p[i][j][1] = newP.y;
			p[i][j][2] = newP.z;
		}
	}

	//�f�ʑ��ʏ�̒��_�̖@��
	CVector L = CVector();
	CVector N[30];
	CVector NN[30];
	for(j = 0; j < nSpine; j++)
	{
		//�Ő��̖@��
		for(i = 0; i < nSide; i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			x = p[ii][j][0] - p[i][j][0];
			y = p[ii][j][1] - p[i][j][1];
			z = p[ii][j][2] - p[i][j][2];
			L = CVector(x, y, z);//�Ő��x�N�g��
			A = CVector(a[j][0], a[j][1], a[j][2]);
			N[i] = L ^ A;//�O��
			N[i].normalize();
		}
		//���_�̊O�����@������
		for(i = 0; i < nSide; i++)
		{
			im = i - 1;
			if(i == 0) im = nSide-1;
			NN[i] = (N[i] + N[im]) / 2.0;
			NN[i].normalize();
			nn[i][j][0] = NN[i].x;
			nn[i][j][1] = NN[i].y;
			nn[i][j][2] = NN[i].z;
		}
	}

	//���_����`���`��
	glBegin(GL_QUADS);
	for(j = 0; j < nSpine-1; j++)
	{
		jj = j+1;
		for(i = 0;i < nSide;i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			
			glNormal3fv(nn[i][j]);  glVertex3fv(p[i][j]); 
			glNormal3fv(nn[ii][j]); glVertex3fv(p[ii][j]);
			glNormal3fv(nn[ii][jj]);glVertex3fv(p[ii][jj]); 
			glNormal3fv(nn[i][jj]); glVertex3fv(p[i][jj]);
		}
	}
	glEnd();
	
	//����
	float n[3];
	calcNormal(p[0][0],sp[0],p[1][0],n);
	glBegin(GL_TRIANGLES);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++)
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][0]); 
		glVertex3fv(sp[0]);
		glVertex3fv(p[ii][0]); 
	}
	//���
	calcNormal(p[0][nSpine-1], p[1][nSpine-1], sp[nSpine-1],n);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++) 
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		glVertex3fv(p[i][nSpine-1]);
		glVertex3fv(p[ii][nSpine-1]);
		glVertex3fv(sp[nSpine-1]);
	}
	glEnd();
}

//------------------------------------------------------------------------
void drawSkin(double radius, double ratio, double weight, double angle[][3], double len[], int nBone, int nSide, int nDiv)
{	//root�̒�ʒ��S�����_

	//radius:���a
	//ratio:�����䗦
	//angle�F�֐߂̉�]�p
	//nBone�F�{�[����
	//nSide�F�f�ʕ����_��
	//nDiv�F�X�v���C��������
	//�f�ʂ͎������ɒ�������悤�ɕϊ�
	int i, ii, j, jj, k;
	CVector p[21][130];
	CVector nn[21][130];//�e���_�̖@��
	float sp0[41][3];
	float sp[161][3];
	CVector e[161];
	CVector a[161];
	CVector cs[21];
	CVector joint[11];

	if(nBone > 10) printf("nBone <= 10�Ƃ��邱�� \n");
	if(nSide > 20) printf("nSide <= 20�Ƃ��邱�� \n");
	if(nDiv > 4) printf("nDiv1 <= 4�Ƃ��邱�� \n");
	if(angle[0][0] != 0.0 || angle[0][1] != 0.0 || angle[0][2] != 0.0) 
	{
		printf("�o�H�ߓ_�f�[�^���s���ł�(drawSkin) \n"); 
		return;
	}

	//�f�ʌ`��
	double phi;
	for(i = 0; i < nSide; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSide;
		cs[i].x = radius * cos(phi);
		cs[i].y = radius * sin(phi);
		cs[i].z = 0.0;
	}
	//�����p���̊֐߂͂��ׂĂ�����ɂ���
	for(j = 0; j <= nBone; j++) joint[j] = CVector();//�n�_�͌��_
	for(j = 1; j <= nBone; j++) joint[j].z = joint[j-1].z + len[j-1];
	//�֐߈ʒu�����߂�
	for(j = nBone-1; j > 0; j--)//�ŉ��ʑw���狁�߂�
	{
		for(jj = j + 1; jj <= nBone; jj++)
		{
			joint[jj] = joint[j] + rotate(joint[jj] - joint[j], angle[j]);
		}
	}
	double s1, s2, s3;//�ʒu�p�����[�^
	s1 = ratio;
	s2 = 0.5;
	s3 = 1.0 - ratio;
	//�o�H�ߓ_���W
	int no = 0;//�o�H�ߓ_�ԍ�
	//�n�_
	CVector P = CVector();
	for(j = 0; j < nBone; j++)
	{
		sp0[no][0] = joint[j].x;
		sp0[no][1] = joint[j].y;
		sp0[no][2] = joint[j].z;
		no ++;
		P = joint[j] + s1 * (joint[j+1] - joint[j]);
		sp0[no][0] = P.x; sp0[no][1] = P.y; sp0[no][2] = P.z;
		no ++;
		P = joint[j] + s2 * (joint[j+1] - joint[j]);
		sp0[no][0] = P.x; sp0[no][1] = P.y; sp0[no][2] = P.z;
		no ++;
		P = joint[j] + s3 * (joint[j+1] - joint[j]);
		sp0[no][0] = P.x; sp0[no][1] = P.y; sp0[no][2] = P.z;
		no ++;
	}
	//�I�_
	sp0[no][0] = joint[nBone].x;
	sp0[no][1] = joint[nBone].y;
	sp0[no][2] = joint[nBone].z;
	int nSpine = no + 1;//�n�_�C�I�_���܂ތo�H�ߓ_��
	//�֐߈ʒu�̒���
	int m;
	for(m = 1; m < nBone; m++)
	{
		j = 4 * m;
		for(k = 0; k < 3; k++) 
			sp0[j][k] = weight * sp0[j][k] + (1 - weight) * (sp0[j-1][k] + sp0[j+1][k])/2.0;
	}

	
	//�o�H�̃X�v���C�����
	spline3(sp0, sp, nSpine, nDiv);//3������Spline
	nSpine = (nSpine-1) * nDiv + 1;

	//�������x�N�g��
	for(j = 0; j < nSpine-1; j++)
	{
		e[j].x = sp[j+1][0] - sp[j][0];
		e[j].y = sp[j+1][1] - sp[j][1];
		e[j].z = sp[j+1][2] - sp[j][2];
		e[j].normalize();
	}
	//�f�ʂ̕����x�N�g��
	a[0] = e[0];//�n�_�̒f��
	a[nSpine-1] = e[nSpine-2];//�I�[�̒f��
	for(j = 1; j < nSpine-1; j++)
	{
		a[j] = (e[j-1] + e[j]) / 2.0;
	}

	//�f�ʒ��_�̍��W(�R����)
	//�ŏ��̖ʂ̒��_���W
	CVector P0;// = CVector(sp[0][0], sp[0][1], sp[0][2]);//���ڂ��Ă���f�ʂ̒��S�i�ߓ_)�̈ʒu�x�N�g��
	double b ;
	double c ;


	//�ŏ��̒f��
	for(i = 0; i < nSide; i++) p[i][0] = cs[i];

	for(j = 1; j < nSpine; j++)
	{
		for(i = 0; i < nSide; i++)
		{
			P0 = CVector(sp[j][0], sp[j][1], sp[j][2]);
			b = a[j] * e[j-1];
			c = a[j] * (P0 - p[i][j-1]);
			p[i][j] = p[i][j-1] + e[j-1] * (c / b);
		}
	}

	//�f�ʑ��ʏ�̒��_�̖@��
	//�~�`�f�ʂȂ̂Œ��_���W�Ɛߓ_���W�̍��ŋ��߂�
	for(j = 0; j < nSpine; j++)
	{
		for(i = 0; i < nSide; i++)
		{
			P0 = CVector(sp[j][0], sp[j][1], sp[j][2]);
			
			nn[i][j] = P0 >> p[i][j] ;//P0����p[i][j]�֌������P�ʃx�N�g��
		}
	}

	//���_����`���`��
	glBegin(GL_QUADS);
	for(j = 0; j < nSpine-1; j++)
	{
		jj = j+1;
		for(i = 0;i < nSide;i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			
			glNormal3f(nn[i][j].x, nn[i][j].y, nn[i][j].z);  
			glVertex3f(p[i][j].x, p[i][j].y, p[i][j].z); 
			
			glNormal3f(nn[ii][j].x, nn[ii][j].y, nn[ii][j].z);  
			glVertex3f(p[ii][j].x, p[ii][j].y, p[ii][j].z); 
			
			glNormal3f(nn[ii][jj].x, nn[ii][jj].y, nn[ii][jj].z);  
			glVertex3f(p[ii][jj].x, p[ii][jj].y, p[ii][jj].z); 
			
			glNormal3f(nn[i][jj].x, nn[i][jj].y, nn[i][jj].z);  
			glVertex3f(p[i][jj].x, p[i][jj].y, p[i][jj].z); 
		}
	}
	glEnd();

	//����
	float n[3];
	float p1[3], p2[3];
	p1[0] = p[0][0].x; p1[1] = p[0][0].y; p1[2] = p[0][0].z;
	p2[0] = p[1][0].x; p2[1] = p[1][0].y; p2[2] = p[1][0].z;
	calcNormal(p1,sp[0],p2,n);
	glBegin(GL_TRIANGLES);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++)
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		p1[0] = p[i][0].x; p1[1] = p[i][0].y; p1[2] = p[i][0].z;
		p2[0] = p[ii][0].x; p2[1] = p[ii][0].y; p2[2] = p[ii][0].z;
		glVertex3fv(p1); 
		glVertex3fv(sp[0]);
		glVertex3fv(p2); 
	}
	//���
	p1[0] = p[0][nSpine-1].x; p1[1] = p[0][nSpine-1].y; p1[2] = p[0][nSpine-1].z;
	p2[0] = p[1][nSpine-1].x; p2[1] = p[1][nSpine-1].y; p2[2] = p[1][nSpine-1].z;
	calcNormal(p1, p2, sp[nSpine-1],n);
	glNormal3fv(n);
	for(i = 0; i < nSide; i++) 
	{
		ii = i+1;
		if(ii == nSide) ii = 0;
		p1[0] = p[i][nSpine-1].x; p1[1] = p[i][nSpine-1].y; p1[2] = p[i][nSpine-1].z;
		p2[0] = p[ii][nSpine-1].x; p2[1] = p[ii][nSpine-1].y; p2[2] = p[ii][nSpine-1].z;
		glVertex3fv(p1);
		glVertex3fv(p2);
		glVertex3fv(sp[nSpine-1]);
	}
	glEnd();
}
//-----------------------------------------------------------------------------
//�l�p�`�̃��b�V���ix-y����,���S�����_�j
//���������C���������̕����Œ�
void drawElevation(int Nx, int Ny, float sizeX, float sizeY, int sideFlag, float* data)
{
	//�S�̂̕�,����sizeX, sizeY
	//sideFlag = 0:���ʕ\������
	//sideFlag = 1:���ʕ\������

	const int NMAX = 130;
	int i, j;
	float p[NMAX][NMAX][3]; //���_���W
	float a[NMAX][NMAX], b[NMAX][NMAX], c[NMAX][NMAX];//���_�̖@��
	float pitchX, pitchY;
	float n1[3], n2[3], n3[3], n4[3];

	if(Nx > NMAX) printf("Nx��NMAX�𒴂��Ă��܂�(drawElevation1) \n");
	if(Ny > NMAX) printf("Ny��NMAX�𒴂��Ă��܂�(drawElevation1) \n");

	//�Z���̃T�C�Y
	pitchX = sizeX / (float)Nx;
	pitchY = sizeY / (float)Ny;

	//�e���_�̍��W
	for(j = 0; j <= Ny; j++){
		for(i = 0; i <= Nx; i++){
			p[i][j][0] = (float)(i - Nx / 2) * pitchX;
			p[i][j][1] = (float)(j - Ny / 2) * pitchY;
			p[i][j][2] = data[j * (Nx+1) + i];
		}
	}

			
	//�@������
	for(i = 0;i <= Nx;i++)
		for(j = 0;j <= Ny;j++)
		{			
			if(j == 0 )
			{
				if(i == 0) 
				{
					calcNormal(p[0][0],p[1][0],p[0][1],n1);
					a[i][j] = n1[0]; b[i][j] = n1[1]; c[i][j] = n1[2]; }
				else if(i == Nx) 
				{
					calcNormal(p[Nx-1][0],p[Nx][0],p[Nx][1],n1);
					a[i][j] = n1[0]; b[i][j] = n1[1]; c[i][j] = n1[2]; }
				else 
				{
					calcNormal(p[i][0],p[i][1],p[i-1][0],n1);//����
					calcNormal(p[i][0],p[i+1][0],p[i][1],n2);//�E��
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
			}
			else if(j == Ny)
			{
				if(i == 0) 
				{
					calcNormal(p[0][Ny],p[0][Ny-1],p[1][Ny],n1);
					a[i][j] = n1[0]; b[i][j] = n1[1]; c[i][j] = n1[2]; }
				else if(i == Nx) 
				{
					calcNormal(p[Nx][Ny],p[Nx-1][Ny],p[Nx][Ny-1],n1);
					a[i][j] = n1[0]; b[i][j] = n1[1]; c[i][j] = n1[2]; }
				else 
				{
					calcNormal(p[i][Ny],p[i-1][Ny],p[i][Ny-1],n1);//����
					calcNormal(p[i][Ny],p[i][Ny-1],p[i+1][Ny],n2);//�E��
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
			}
			else
			{
				if(i == 0) 
				{
					calcNormal(p[0][j],p[1][j],p[0][j+1],n1);//��
					calcNormal(p[0][j],p[0][j-1],p[0][1],n2);//��
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
				else if(i == Nx) 
				{
					calcNormal(p[Nx][j],p[Nx][j+1],p[Nx-1][j],n1);//��
					calcNormal(p[Nx][j],p[Nx-1][j],p[Nx][j-1],n2);//��
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
				else 
				{//�㉺���E�S�̎O�p�`�̕���
					calcNormal(p[i][j],p[i][j+1],p[i-1][j],n1);//����
					calcNormal(p[i][j],p[i+1][j],p[i][j+1],n2);//�E��
					calcNormal(p[i][j],p[i-1][j],p[i][j-1],n3);//����
					calcNormal(p[i][j],p[i][j-1],p[i+1][j],n4);//�E��
					a[i][j] = (n1[0]+n2[0]+n3[0]+n4[0])/4.0f;
					b[i][j] = (n1[1]+n2[1]+n3[1]+n4[1])/4.0f;
					c[i][j] = (n1[2]+n2[2]+n3[2]+n4[2])/4.0f; }
			}
		}

//	int nC;
	//�O�p�`�Ŗʂ��`
	glBegin(GL_TRIANGLES);
	for(j = 0;j < Ny;j++)
		for(i = 0;i < Nx;i++)
		{
			//�����̎O�p�`
			//�e���_�̖@������,ø�������W,���_���W��^����B
			glNormal3f(a[i][j],b[i][j],c[i][j]);//�@������
			glVertex3fv(p[i][j]);//��غ�݂̒��_���W�i�ȉ��������J��Ԃ��j
			glNormal3f(a[i+1][j],b[i+1][j],c[i+1][j]);
			glVertex3fv(p[i+1][j]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(p[i][j+1]);
			//�E��̎O�p�`
			glNormal3f(a[i+1][j],b[i+1][j],c[i+1][j]);
			glVertex3fv(p[i+1][j]);
			glNormal3f(a[i+1][j+1],b[i+1][j+1],c[i+1][j+1]);
			glVertex3fv(p[i+1][j+1]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(p[i][j+1]);
		}
	glEnd();

	if(sideFlag == 1)//���ʕ`��
	{
		glBegin(GL_QUADS);
		//+x�����ii=Nx)
		glNormal3f(1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[Nx][j][0], p[Nx][j][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], p[Nx][j+1][2]);
			glVertex3f(p[Nx][j][0], p[Nx][j][1], p[Nx][j][2]);
		}
		//-x�����ii=0)
		glNormal3f(-1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[0][j][0], p[0][j][1], 0.0f);
			glVertex3f(p[0][j][0], p[0][j][1], p[0][j][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], p[0][j+1][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], 0.0f);
		}
		//+y�����ij=Ny)
		glNormal3f(0.0, 1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][Ny][0], p[i][Ny][1], 0.0f);
			glVertex3f(p[i][Ny][0], p[i][Ny][1], p[i][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], p[i+1][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], 0.0f);
		}
		//-y�����ij=0)
		glNormal3f(0.0, -1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][0][0], p[i][0][1], 0.0f);
			glVertex3f(p[i+1][0][0], p[i+1][0][1], 0.0f);
			glVertex3f(p[i+1][0][0], p[i+1][0][1], p[i+1][0][2]);
			glVertex3f(p[i][0][0], p[i][0][1], p[i][0][2]);
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------------
//�l�p�`�̃��b�V���ix-y����,���S�����_�j
//���������C���������̕����Œ�
void drawGrid(int Nx, int Ny, float sizeX, float sizeY, int sideFlag)
{
	//�S�̂̕�,����sizeX, sizeY
	//sideFlag = 0:���ʕ\������
	//sideFlag = 1:���ʕ\������
	const int NMAX = 130;
	int i, j;
	float p[NMAX][NMAX][3]; //���_���W
//	float a[NMAX][NMAX], b[NMAX][NMAX], c[NMAX][NMAX];//���_�̖@��
	float pitchX, pitchY;
//	float n1[3], n2[3], n3[3], n4[3];

	if(Nx > NMAX) printf("Nx��NMAX�𒴂��Ă��܂�(drawElevation1) \n");
	if(Ny > NMAX) printf("Ny��NMAX�𒴂��Ă��܂�(drawElevation1) \n");

	//�Z���̃T�C�Y
	pitchX = sizeX / (float)Nx;
	pitchY = sizeY / (float)Ny;

	//�e���_�̍��W
	for(j = 0; j <= Ny; j++){
		for(i = 0; i <= Nx; i++){
			p[i][j][0] = (float)(i - Nx / 2) * pitchX;
			p[i][j][1] = (float)(j - Ny / 2) * pitchY;
			p[i][j][2] = 0.0;//data[j * (Nx+1) + i];
		}
	}
		
//	int nC;
	//�O�p�`�Ŗʂ��`
	glNormal3f(0.0, 0.0, 1.0);//���ׂ�z����
	glBegin(GL_TRIANGLES);
	for(j = 0;j < Ny;j++)
		for(i = 0;i < Nx;i++)
		{
			//�����̎O�p�`
			//�e���_�̖@������,ø�������W,���_���W��^����B
			glVertex3fv(p[i][j]);//��غ�݂̒��_���W�i�ȉ��������J��Ԃ��j
			glVertex3fv(p[i+1][j]);
			glVertex3fv(p[i][j+1]);
			//�E��̎O�p�`
			glVertex3fv(p[i+1][j]);
			glVertex3fv(p[i+1][j+1]);
			glVertex3fv(p[i][j+1]);
		}
	glEnd();

	if(sideFlag == 1)//���ʕ`��
	{
		glBegin(GL_QUADS);
		//+x�����ii=Nx)
		glNormal3f(1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[Nx][j][0], p[Nx][j][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], p[Nx][j+1][2]);
			glVertex3f(p[Nx][j][0], p[Nx][j][1], p[Nx][j][2]);
		}
		//-x�����ii=0)
		glNormal3f(-1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[0][j][0], p[0][j][1], 0.0f);
			glVertex3f(p[0][j][0], p[0][j][1], p[0][j][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], p[0][j+1][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], 0.0f);
		}
		//+y�����ij=Ny)
		glNormal3f(0.0, 1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][Ny][0], p[i][Ny][1], 0.0f);
			glVertex3f(p[i][Ny][0], p[i][Ny][1], p[i][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], p[i+1][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], 0.0f);
		}
		//-y�����ij=0)
		glNormal3f(0.0, -1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][0][0], p[i][0][1], 0.0f);
			glVertex3f(p[i+1][0][0], p[i+1][0][1], 0.0f);
			glVertex3f(p[i+1][0][0], p[i+1][0][1], p[i+1][0][2]);
			glVertex3f(p[i][0][0], p[i][0][1], p[i][0][2]);
		}
		glEnd();
	}
}

//-------------------------------------------------------------------
void drawBezierLine(float ctrPoint[][3], int nCtr, int nDiv, bool flagControl)
{
	int i;

	glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, nCtr, ctrPoint[0]);
	glEnable( GL_MAP1_VERTEX_3);
	//�Ȑ��̕`��
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for(i = 0; i <= nDiv; i++) glEvalCoord1f((float)i / (float)nDiv);
	glEnd();
	if(flagControl)
	{
		//����_�̕`��
		glPointSize(5.0f);
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_POINTS);
		for(i = 0; i < 4; i++) glVertex3f(ctrPoint[i][0], ctrPoint[i][1], ctrPoint[i][2]);
		glEnd();
		//����_�Ԑ����`��
		glLineWidth(1.0f);
		glColor3f(0.0, 0.8, 1.0);
		glBegin(GL_LINE_STRIP);
		for(i = 0; i < 4; i++) glVertex3f(ctrPoint[i][0], ctrPoint[i][1], ctrPoint[i][2]);
		glEnd();
	}
	glLineWidth(1.0f);
}

void drawBezierWire(float ctrPoints[][4][3], int nDiv, int nMesh,  bool flagControl)
{
	int i, j;

	glEnable( GL_AUTO_NORMAL);
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 4, 0.0f, 1.0f, 12, 4, ctrPoints[0][0]);
	glEnable( GL_MAP2_VERTEX_3);
	for(j = 0; j <= nMesh; j++)
	{
	  glBegin(GL_LINE_STRIP);
	  for(i = 0; i <= nDiv; i++) glEvalCoord2f((float)i/nDiv, (float)j/nMesh);
	  glEnd();
	  glBegin(GL_LINE_STRIP);
	  for(i = 0; i <= nDiv; i++) glEvalCoord2f((float)j/nMesh, (float)i/nDiv);
	  glEnd();
	}	  

	if(flagControl)
	{
		//����_�̕`��
		glPointSize(5.0f);
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_POINTS);
		for(j = 0; j < 4; j++)
			for(i = 0; i < 4; i++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);
		//����_�Ԑ����`��
		//			glColor3d(0.0, 1.0, 0.0);
		glLineWidth(1.0f);
		glColor3f(0.0, 0.8, 1.0);
		for(i = 0; i < 4; i++)
		{
			glBegin(GL_LINE_STRIP);
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[j][i]);
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[i][j]);
			glEnd();
		}
	}
	glLineWidth(1.0f);
}
//------------------------------------------------------------------

void drawBezier(float ctrPoints[][4][3], int nMesh,  bool flagControl)
{
	int i, j;
	float diffuseP[] = {1.0f,0.0f,0.0f,1.0f};//����_�̊g�U��
	float diffuseL[] = {0.0f,0.0f,1.0f,1.0f};//����_�Ԃ̊g�U��

	glEnable( GL_AUTO_NORMAL);//�������K��
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 4, 0.0f, 1.0f, 12, 4, ctrPoints[0][0]);
	glEnable( GL_MAP2_VERTEX_3);
	glMapGrid2d(nMesh, 1.0, 0.0, nMesh, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, nMesh, 0, nMesh);

	if(flagControl)
	{
		//����_�̕`��
		glPointSize(5.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseP);

		glBegin(GL_POINTS);
		for(j = 0; j < 4; j++)
			for(i = 0; i < 4; i++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);
		//����_�Ԑ����`��
		glLineWidth(1.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseL);
		glBegin(GL_LINE_STRIP);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[j][i]);
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[i][j]);
		}
		glEnd();
	}
	glLineWidth(1.0f);
}

//---------------------------------------------------------------------
void drawNurbs(float ctrPoints[][4][3], bool flagControl)
{
	int i, j;
	float diffuseP[] = {0.9f,0.0f,0.0f,1.0f};//����_�̊g�U��
	float diffuseL[] = {0.0f,0.0f,1.0f,1.0f};//����_�Ԃ̊g�U��
	GLUnurbsObj* nurb;//NURBS�ւ̃|�C���^
	float knots[] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	glEnable( GL_AUTO_NORMAL);
	nurb = gluNewNurbsRenderer();
	gluNurbsProperty(nurb, GLU_SAMPLING_TOLERANCE, 50.0);
	gluNurbsProperty(nurb, GLU_DISPLAY_MODE, GLU_FILL);

	gluBeginSurface(nurb);
	gluNurbsSurface(nurb, 8, knots, 8, knots, 3, 12, ctrPoints[0][0], 4, 4, GL_MAP2_VERTEX_3);
	gluEndSurface(nurb);

	if(flagControl)
	{
		//����_�̕`��
		glPointSize(5.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseP);

		glBegin(GL_POINTS);
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);

		//����_�Ԑ����`��
		glLineWidth(1.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseL);
		glBegin(GL_LINE_STRIP);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[j][i]);
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[i][j]);
		}
		glEnd();
	}
	glPointSize(1.0f);
}

//-------------------------------------------------------------------
void drawFloor(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor�P��������̕�
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	{ 0.7, 0.7, 0.6, 1.0}, { 0.3f, 0.4, 0.4, 1.0} };
  float ambient[] = { 0.2, 0.2, 0.2, 1.0};
  float specular[]= { 0.5, 0.5, 0.5, 1.0};
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glNormal3f(0.0, 1.0, 0.0);
  glPushMatrix();
  for (j = 0; j < nz; j++) 
  {
    float z1 = -widthZ / 2.0 + wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = -widthX / 2.0 + wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
			glBegin(GL_QUADS);
      glVertex3f(x1, 0.0, z1);
      glVertex3f(x1, 0.0, z2);
      glVertex3f(x2, 0.0, z2);
      glVertex3f(x2, 0.0, z1);
			glEnd();
    }
  }
  glPopMatrix();
}


