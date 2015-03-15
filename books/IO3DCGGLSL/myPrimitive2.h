#include <math.h>
#include <stdio.h>
#include "../../spline.h"
#include "../../myMath2.h"

#define M_PI 3.14159265358979323846

//---------------------------------------------------------------------
//法線方向計算ルーチン
void calcNormal(float* p1,float* p2,float* p3,float* nn)
{
	CVector A = CVector(p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]);
	CVector B = CVector(p3[0] - p2[0], p3[1] - p2[1], p3[2] - p2[2]);
	CVector n = A ^ B;//外積
	n.normalize();
	nn[0] = n.x; nn[1] = n.y; nn[2] = n.z;
}

//------------------------------------------------------------------------------
void drawPlate(float s)//x-y平面
{
	static float p[4][3] = 
	{ { s/2.0,-s/2.0, 0.0}, { s/2.0, s/2.0, 0.0}, {-s/2.0, s/2.0, 0.0}, {-s/2.0,-s/2.0, 0.0}};

	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0); //z方向の法線
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
		glNormal3f(0.0f,0.0f,1.0f); //z方向
		glVertex3fv(p[0]); glVertex3fv(p[1]);
		glVertex3fv(p[2]); glVertex3fv(p[3]);

		glNormal3f(1.0f,0.0f,0.0f); //x方向(正面）
		glVertex3fv(p[0]); glVertex3fv(p[3]);
		glVertex3fv(p[7]); glVertex3fv(p[4]);

		glNormal3f(0.0f,1.0f,0.0f); //y方向
		glVertex3fv(p[0]); glVertex3fv(p[4]);
		glVertex3fv(p[5]); glVertex3fv(p[1]);

	 	glNormal3f(-1.0f,0.0f,0.0f); //-x方向
		glVertex3fv(p[1]); glVertex3fv(p[5]);
		glVertex3fv(p[6]); glVertex3fv(p[2]);

		glNormal3f(0.0f,-1.0f,0.0f); //-y方向
		glVertex3fv(p[2]); glVertex3fv(p[6]);
		glVertex3fv(p[7]); glVertex3fv(p[3]);

		glNormal3f(0.0f,0.0f,-1.0f); //-z方向
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
		//j=0は北極点(x=0, y=0, z=radius)
		//これらの天頂角
		th0 = M_PI * (double)j / (double)nStack;
		th1 = M_PI * (double)(j+1) / (double)nStack;
		//x-y平面に投影した半径
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//頂点z座標
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//頂点のxy座標(i=0をobjectからみて右端) 
			p[0][0] =   r0 * sin(phi);//x座標
			p[0][1] = - r0 * cos(phi);//y座標
			p[1][0] =   r1 * sin(phi);//x座標
			p[1][1] = - r1 * cos(phi);//y座標

			glNormal3dv(p[0]);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標

			glNormal3dv(p[1]);//法線ベクトル
			glVertex3dv(p[1]);//頂点座標			
		}
		glEnd();
	}
}

//-----------------------------------------------------------------------
void drawHemiSphere(double radius, int nSlice, int nStack)
{	//上半球
	//radius:半径
	//nSlice:経度(x-y)
	//nStack:緯度(z)
	int i, j;
	double r0, r1, th0, th1, phi;
	double p[2][3];

	for(j = 0; j < nStack; j++)
	{
		//j=0は北極点(x=0, y=0, z=radius)
		//これらの天頂角
		th0 = M_PI * (double)j / (double)nStack;
		th1 = M_PI * (double)(j+1) / (double)nStack;
		//x-y平面に投影した半径
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//頂点z座標
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//頂点のxy座標
			p[0][0] = r0 * cos(phi);//x座標
			p[0][1] = r0 * sin(phi);//y座標
			p[1][0] = r1 * cos(phi);//x座標
			p[1][1] = - r1 * sin(phi);//y座標

			glNormal3dv(p[0]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[0]);//頂点座標

			glNormal3dv(p[1]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}
}

//----------------------------------------------------------------------
void drawCylinder1(double rBottom, double rTop, double height, int nSlice, int nStack)
{
	//円柱(rBottom=rTop))、円錐台、円錐(rTop = 0.0)
	//rBottom:下底半径, rTop:上底半径
	//nSlice--xy断面分割数
	//nStack---ｚ方向分割数
	//物体の中心は下底と上底の中間

	int i, j;
	double x, y, z, z0, z1;
	double theta;
	double theta0 = 2.0*M_PI/(double)nSlice;
	//上底(Top)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, height/2.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		x = (float)(rTop * cos(theta)); //x成分
		y = (float)(rTop * sin(theta)); //ｙ成分
		z = height/2.0;
		glVertex3f(x, y, z);
	}
	glEnd();

	//下底(Bottom)
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, -1.0);
	glVertex3f(0.0, 0.0, -height/2.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)(nSlice - i);
		x = (float)(rBottom * cos(theta)); //x成分
		y = (float)(rBottom * sin(theta)); //ｙ成分
		z = -height/2.0;
		glVertex3f(x, y, z);
	}
	glEnd();

	double s, t0, t1, r0, r1, phi;
	double p[2][3], n[3];

	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);
	//側面
	for(j = 0; j < nStack; j++)
	{
		//j=0は上底(x=0, y=0, z=height/2)
		//2つのt座標
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//底面からの高さ
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//半径
		r0 = rBottom + (rTop - rBottom) * z0 / height;
		r1 = rBottom + (rTop - rBottom) * z1 / height;

		//頂点z座標
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;

		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s座標
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//頂点のxy座標
			p[0][0] = r0 * cos(phi);//x座標
			p[0][1] = r0 * sin(phi);//y座標
			p[1][0] = r1 * cos(phi);//x座標
			p[1][1] = r1 * sin(phi);//y座標
			//法線ベクトル
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] = nz;

			glNormal3dv(n);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}
}

//-------------------------------------------------------------------------------------
void drawCylinder2(double rBottom, double rTop, double height, double thick, int nSlice, int nStack)
{
	//厚みのある中空円柱
	//円柱(rBottom=rTop))、円錐台、円錐(rTop = 0.0)
	//rBottom:下底半径, rTop:上底半径, thick:厚さ
	//nSlice--xy断面分割数
	//nStack---ｚ方向分割数
	//物体の中心は下底と上底の中間

	int i, j;
	double x, y, z, z0, z1;
	double theta;
	double theta0 = 2.0*M_PI/(double)nSlice;
	//上底(Top)
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, 1.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		z = height/2.0;
		//内側座標
		x = (rTop - thick) * cos(theta); //x成分
		y = (rTop - thick) * sin(theta); //y成分
		glVertex3f(x, y, z);

		//外側座標
		x = rTop * cos(theta); //x成分
		y = rTop * sin(theta); //y成分
		glVertex3f(x, y, z);
	}
	glEnd();

	//下底(Bottom)
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0.0, 0.0, -1.0);
	for(i = 0; i <= nSlice; i++) 
	{ 
		theta = theta0 * (double)i;
		z = -height/2.0;
		//外側座標
		x = rBottom * cos(theta); //x成分
		y = rBottom * sin(theta); //y成分
		glVertex3f(x, y, z);
		//内側座標
		x = (rBottom - thick) * cos(theta); //x成分
		y = (rBottom - thick) * sin(theta); //y成分
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
		//j=0は上底(x=0, y=0, z=height/2)
		//2つのt座標
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//底面からの高さ
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//半径
		r0 = rBottom + (rTop - rBottom) * z0 / height;
		r1 = rBottom + (rTop - rBottom) * z1 / height;

		//頂点z座標
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;
		
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = r0 * cos(phi);//x座標
			p[0][1] = r0 * sin(phi);//y座標
			p[1][0] = r1 * cos(phi);//x座標
			p[1][1] = r1 * sin(phi);//y座標
			//法線ベクトル
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] =  nz;

			glNormal3dv(n);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}

	//内側側面
	for(j = 0; j < nStack; j++)
	{
		//j=0は上底(x=0, y=0, z=height/2)
		//2つのt座標
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//底面からの高さ
		z0 = height * (1.0 - t0);
		z1 = height * (1.0 - t1);
		//半径
		r0 = rBottom - thick + (rTop - rBottom) * z0 / height;
		r1 = rBottom - thick + (rTop - rBottom) * z1 / height;

		//頂点z座標
		p[0][2] = z0 - height / 2.0;
		p[1][2] = z1 - height / 2.0;

		glBegin(GL_QUAD_STRIP);
		for(i = nSlice; i >= 0; i--)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = -r0 * cos(phi);//x座標
			p[0][1] = -r0 * sin(phi);//y座標
			p[1][0] = -r1 * cos(phi);//x座標
			p[1][1] = -r1 * sin(phi);//y座標

			//法線ベクトル
			n[0] = nxy * cos(phi);
			n[1] = nxy * sin(phi);
			n[2] = - nz;
			
			glNormal3dv(n);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}
}

//----------------------------------------------------------------------
void drawPrism1(float rBottom, float rTop, float height, int nSlice, int nStack)
{
	//円柱(rBottom=rTop))、円錐台、円錐(rTop = 0.0)
	//rBottom:下底半径, rTop:上底半径
	//nSlice--xy断面分割数
	//nStack---ｚ方向分割数
	//物体の中心は下底と上底の中間
	float p[31][31][3];
	float n[30][3];
	double theta0, theta, z, r;
	int i, ii, j;

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;
	
	theta0 = 2*M_PI/(double)nSlice;
	for(j = 0; j <= nStack; j++)
	{
		z = height * (1.0 - (double)j/(double)nStack);//底面からの高さ
		r = rBottom + (rTop - rBottom) * z / height;
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[i][j][0] = (float)(r * cos(theta)); //x成分
			p[i][j][1] = (float)(r * sin(theta)); //ｙ成分
			p[i][j][2] = (float)z-0.5f*height;    //ｚ成分(高さ)
		}
	}
	glBegin(GL_TRIANGLES);
	//上底(Top)
	glNormal3f(0.0f,0.0f,1.0f);
	for(i = 0; i < nSlice; i++) 
	{ 
		ii = i + 1;
		if(ii == nSlice) ii = 0;
		glVertex3f(0.0f, 0.0f, height/2.0f);
		glVertex3fv(p[i][0]);
		glVertex3fv(p[ii][0]);
	}
	//下底(Bottom)
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
	//側面
	
	double nz, nxy, rr;//方向余弦
	rr = rBottom - rTop;
	nz = rr / sqrt(rr*rr + height * height);
	nxy = sqrt(1.0 - nz*nz);
	for(i=0; i<nSlice;i++)
	{//一周分だけでよい
		n[i][0] = (float)(nxy * cos(theta0 * (double)i));
		n[i][1] = (float)(nxy * sin(theta0 * (double)i));
		n[i][2] = (float)nz;
	}
	for(i=0; i < nSlice;i++)
	{//隣との和の平均
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
	//円柱(rBottom=rTop))、円錐台、円錐(rTop = 0.0)
	//rBottom:下底半径, rTop:上底半径, thick:厚さ
	//nSlice--xy断面分割数
	//nStack---ｚ方向分割数
	//物体の中心は下底と上底の中間
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
		z = height * ( 1.0 - (double)j/(double)nStack );//底面からの高さ
		r1 = rBottom + (rTop - rBottom) * z / height;//外側半径
		r2 = r1 - thick;                       //内側半径
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[j*nSlice+i][0] = (float)(r1 * cos(theta)); //x成分
			p[j*nSlice+i][1] = (float)(r1 * sin(theta)); //ｙ成分
			p[j*nSlice+i][2] = (float)z-0.5f * height;           //ｚ成分(高さ)
		}
		//内側
		for(i=0;i<nSlice;i++)
		{
			theta = theta0*(double)i;
			p[j*nSlice+i+N][0] = (float)(r2 * cos(theta)); //x成分
			p[j*nSlice+i+N][1] = (float)(r2 * sin(theta)); //ｙ成分
			p[j*nSlice+i+N][2] = (float)z-0.5f*height;     //ｚ成分(高さ)
		}

	}

	//内側側面（内側から描画）
	//方向余弦
	rr = rBottom - rTop;
	nz = rr / sqrt(rr*rr + height*height);
	nxy = sqrt(1.0 - nz * nz);
	for(i=0; i<nSlice;i++)
	{//一周分だけでよい
		n[i][0] = -(float)(nxy * cos(theta0 * (double)i));
		n[i][1] = -(float)(nxy * sin(theta0 * (double)i));
		n[i][2] = -(float)nz;
	}
	for(i=0; i < nSlice;i++)
	{//隣との和の平均
		ii = i+1;
		if(ii == nSlice) ii = 0;
		n[i][0] = (n[i][0] + n[ii][0]) / 2.0f;
		n[i][1] = (n[i][1] + n[ii][1]) / 2.0f;
	}


	//内側側面の描画
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

	//外側側面
	for(i=0; i<nSlice;i++)
	{//内側法線方向の逆方向
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
	//上底
	for(i=0;i<nSlice;i++)
	{
		ii = i+1;
		if(ii == nSlice) ii = 0;
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3fv(p[i]); glVertex3fv(p[ii]);
		glVertex3fv(p[N+ii]); glVertex3fv(p[N+i]);
		glEnd();
	}
	//下底
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
	//radius1:円環断面半径
	//radius2:円環の中心軸半径
	//nSide:円環断面における表面分割点数
	//nRing:円環の分割数
	if(radius1 > radius2) { printf("radius1 < radius2としてください \n "); return;}

	int i, j;
	double rr, zz;
	double phi0, phi1, theta;
	double p[2][3];

	for(i = 0; i < nRing; i++)
	{
		//i=0は基本断面(x=radius2を中心とする円, y=0）
		phi0 = 2.0 * M_PI * (double)i / (double)nRing;
		phi1 = 2.0 * M_PI * (double)(i+1) / (double)nRing;

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			theta = M_PI - 2.0 * M_PI * (double)j / (double)nSide;
			rr = radius2 + radius1 * cos(theta);//z軸からの距離
			zz = radius1 * sin(theta);
			//頂点のxyz座標(j=0を内側xy平面)
			p[0][0] = rr * cos(phi0);//x座標
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x座標
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z      

			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//頂点座標

			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
	}
}

//----------------------------------------------------------------
void drawSpring(float radius1, float radius2, float len, int nSide, int nRing, int nPitch)
{
	//radius1:円環断面半径
	//radius2:円環の中心軸半径
	//len;ばね長
	//nSide:円環断面における表面分割点数
	//nRing:円環の分割数
	//nPitch:ピッチ数

	if(radius2 < radius1) { 
		printf("drawSpringにおいて radius2 > radius1 としてください \n");
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
			//i=0は基本断面(x=radius2を中心とする円, y=0）
			phi0 = 2.0 * M_PI * (double)i / (double)nRing;
			phi1 = 2.0 * M_PI * (double)(i+1) / (double)nRing;

			glBegin(GL_QUAD_STRIP);
			for(j = 0; j <= nSide; j++)
			{
				theta = - M_PI + 2.0 * M_PI * (double)j / (double)nSide;
				rr = radius2 + radius1 * cos(theta);//z軸からの距離
				zz0 = radius1 * sin(theta) + dp * (double)i + pitch * (double)k;
				zz1 = radius1 * sin(theta) + dp * (double)(i+1) + pitch * (double)k;
				//頂点のxyz座標(j=0を内側xy平面)
				p[0][0] = rr * cos(phi0);//x座標
				p[0][1] = rr * sin(phi0);//y
				p[0][2] = zz0;            //z
				p[1][0] = rr * cos(phi1);//x座標
				p[1][1] = rr * sin(phi1);//y
				p[1][2] = zz1;            //z      

				glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
				glVertex3dv(p[0]);//頂点座標

				glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
				glVertex3dv(p[1]);//頂点座標
			}
			glEnd();
		}
	}
	//始端
	glBegin(GL_POLYGON);
	glNormal3f(0.0, -1.0, 0.0);
	for(j = 0; j < nSide; j++)
	{
		theta = - M_PI + 2.0 * M_PI * (double)j / (double)nSide;
		rr = radius2 + radius1 * cos(theta);//z軸からの距離
		zz0 = radius1 * sin(theta);
		glVertex3f(rr, 0.0, zz0);
	}
	glEnd();
	//終端
	glBegin(GL_POLYGON);
	glNormal3f(0.0, 1.0, 0.0);
	for(j = 0; j < nSide; j++)
	{
		theta = - M_PI - 2.0 * M_PI * (double)j / (double)nSide;
		rr = radius2 + radius1 * cos(theta) ;//z軸からの距離
		zz0 = radius1 * sin(theta) + pitch * (double)nPitch;
		glVertex3f(rr, 0.0, zz0);
	}
	glEnd();
}

//-----------------------------------------------------------------
//超2次関数
void drawSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//上下の中心が原点
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
		                //thetaはx-y平面からの偏角となっている

		if(theta >= 0.0) //上半分
		{
			if(theta == 0.0) z = 0.0;//pow(a,b)のaがa<=0.0でエラー
			else z = pow(sin(fabs(theta)),eps1);//z
		}
		else  //下半分        
		{
			z = - pow(sin(fabs(theta)), eps1);
		}
		for (i = 0 ;i <= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//objectから見て左側
			k2 = nSlice * j + nSlice - i;//右側
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

	//側面の法線成分
	for(i = 0;i < nSlice;i++)
	{
		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//真上(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		//真下（Bottom)
		a[i][nStack] = 0.0f; b[i][nStack] = 0.0f; c[i][nStack] = -1.0f;

		for(j=1;j<nStack;j++)//隣り合う4個の三角形の法線ベクトルを平均化
		{
			np = j*nSlice+i;//注目点
			npL = j*nSlice+im;//左側
			npR = j*nSlice+ip;//右側
			npU = np-nSlice;//上
			npD = np+nSlice;//下
			if(j == 1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;//Top
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			if(j == nStack-1)
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				n3[0]=0.0f; n3[1]=0.0f; n3[2]=-1.0f;//Bottom
				n4[0]=0.0f; n4[1]=0.0f; n4[2]=-1.0f;//Bottom
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//ｘ方向
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//ｙ
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//ｚ
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
//超2次関数
void drawSuper(float r, int nSlice, int nStack, double eps1, double eps2, double taper)
{
	//上下の中心が原点
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
		                //thetaはx-y平面からの偏角となっている

		if(theta >= 0.0) //上半分
		{
			if(theta == 0.0) z = 0.0;//pow(a,b)のaがa<=0.0でエラー
			else z = pow(sin(fabs(theta)),eps1);//z
		}
		else  //下半分        
		{
			z = - pow(sin(fabs(theta)), eps1);
		}
		fz = 0.5f * (taper+1.0f +(taper -1.0f) * z);
		for (i = 0 ;i <= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//objectから見て左側
			k2 = nSlice * j + nSlice - i;//右側
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

	//側面の法線成分
	for(i = 0;i < nSlice;i++)
	{
		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//真上(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		//真下（Bottom)
		a[i][nStack] = 0.0f; b[i][nStack] = 0.0f; c[i][nStack] = -1.0f;

		for(j=1;j<nStack;j++)//隣り合う4個の三角形の法線ベクトルを平均化
		{
			np = j*nSlice+i;//注目点
			npL = j*nSlice+im;//左側
			npR = j*nSlice+ip;//右側
			npU = np-nSlice;//上
			npD = np+nSlice;//下
			if(j == 1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;//Top
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			if(j == nStack-1)
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				n3[0]=0.0f; n3[1]=0.0f; n3[2]=-1.0f;//Bottom
				n4[0]=0.0f; n4[1]=0.0f; n4[2]=-1.0f;//Bottom
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//ｘ方向
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//ｙ
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//ｚ
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
	//上下の中心が原点
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
    //形状関数
    if(z < 0.0) fz = (middle-1.0)*z + middle;
    else fz = (1.0-middle)*z + middle;

    for (i = 0 ;i <= nSlice/2 ;i++)
    {
			k1 = nSlice * j + i;//自分から見て左側(x > 0)
			k2 = nSlice * j + nSlice - i;//右側(x < 0)
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

	//変形
	if(type == 0)
	{
		//前方：z軸
		//z軸回転(x>0なら正のz軸回転，x<0なら負のz軸回転）
		for(j = jStart; j < nStack; j++)
		{
			for(i = 0; i <= nSlice/2; i++)
			{
				k1 = nSlice * j + i;//自分から見て左側(x > 0)
				k2 = nSlice * j + nSlice - i;//右側(x < 0)
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
		//前方：z軸
		//後半を上下（x軸回転）
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
		//前方：z軸
		//後半を左右（y軸回転）
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
	
	//側面の法線成分
	for(i = 0;i < nSlice;i++)
	{
 		ip = i+1;
		if(ip == nSlice) ip = 0;
		im = i-1;
		if(i == 0) im = nSlice-1;

		//真上(Top)
		a[i][0] = 0.0; b[i][0] = 0.0; c[i][0] = 1.0;
		//真下（Bottom)
		a[i][nStack] = 0.0; b[i][nStack] = 0.0; c[i][nStack] = -1.0;

		for(j=1;j<nStack;j++)//隣り合う4個の三角形の法線ベクトルを平均化
		{
			np = j*nSlice+i;//注目点
			npL = j*nSlice+im;//左側
			npR = j*nSlice+ip;//右側
			npU = np-nSlice;//上
			npD = np+nSlice;//下
			if(j == 1) {
				n1[0]=0.0; n1[1]=0.0; n1[2]=1.0;//Top
				n2[0]=0.0; n2[1]=0.0; n2[2]=1.0;//Top
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			if(j == nStack-1){
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				n3[0]=0.0; n3[1]=0.0; n3[2]=-1.0;//Bottom
				n4[0]=0.0; n4[1]=0.0; n4[2]=-1.0;//Bottom
			}
			else {
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//ｘ方向
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//ｙ
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//ｚ
		}
	}
	//表示
	glBegin(GL_TRIANGLES);
	//上底
	for(i = 0;i < nSlice;i++)
	{	ip = i+1;
		if(ip == nSlice) ip = 0;

			glNormal3f(a[i][0],b[i][0],c[i][0]);glVertex3fv(pd[i]);
			glNormal3f(a[i][1],b[i][1],c[i][1]);glVertex3fv(pd[nSlice+i]);
			glNormal3f(a[ip][1],b[ip][1],c[ip][1]);glVertex3fv(pd[nSlice+ip]);
	}
	//下底
	j = nStack - 1;
	for(i = 0;i < nSlice;i++)
	{	ip = i+1;
		if(ip == nSlice) ip = 0;
			glNormal3f(a[i][j],b[i][j],c[i][j]);glVertex3fv(pd[j*nSlice+i]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);glVertex3fv(pd[(j+1)*nSlice+i]);
			glNormal3f(a[ip][j],b[ip][j],c[ip][j]);glVertex3fv(pd[j*nSlice+ip]);
	}
	glEnd();
	//側面(4角形パッチ）
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
//上半分の超2次関数
void drawHemiSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//底の中心が原点
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	double ct,theta,phi,z,cc;
	double e1, e2;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	//上半分だけ
	for(j = 0 ;j <= nStack ;j++)
	{
		theta = (M_PI/(double)nStack) * ((double)nStack  - (double)j)/2.0;
		                          //thetaはx-y平面からの偏角となっている
		e1 = fabs(sin(theta));
		if( e1 == 0.0) z = 0.0;
		else z = (float)(pow(e1, eps1));//z
		for (i = 0 ;i<= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//外から見て右側
			k2 = nSlice * j + nSlice - i;//左側
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

	//側面の法線成分
	for(i = 0;i < nSlice; i++)
	{
		ip = i + 1;
		if(ip==nSlice) ip = 0;
		im=i-1;
		if(i==0) im = nSlice - 1;

		//真上(Top)
		a[i][0] = 0.0f; b[i][0] = 0.0f; c[i][0] = 1.0f;
		np = 0; npU = 0; npL = 0; npR = 0; npU = 0;

		for(j=1;j<nStack;j++)
		{
			np = j*nSlice+i;//注目点
			npL = j*nSlice+im;//左側
			npR = j*nSlice+ip;//右側
			npU = np-nSlice;//上
			npD = np+nSlice;//下
			if(j==1) 
			{
				n1[0]=0.0f; n1[1]=0.0f; n1[2]=1.0f;
				n2[0]=0.0f; n2[1]=0.0f; n2[2]=1.0f;
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			else 
			{
				calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
				calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
				calcNormal(pd[np],pd[npL],pd[npD],n3);//外から見て左下
				calcNormal(pd[np],pd[npD],pd[npR],n4);//右下
			}
			a[i][j] = (float)((n1[0]+n2[0]+n3[0]+n4[0])/4.0f);//ｘ方向
			b[i][j] = (float)((n1[1]+n2[1]+n3[1]+n4[1])/4.0f);//ｙ
			c[i][j] = (float)((n1[2]+n2[2]+n3[2]+n4[2])/4.0f);//ｚ
		}
		j = nStack;//一番下の側面(上の2個の三角形の平均）
		calcNormal(pd[np],pd[npU],pd[npL],n1);//外から見て左上
		calcNormal(pd[np],pd[npR],pd[npU],n2);//右上
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
	
	//底
	j = nStack;
	glBegin(GL_POLYGON);
	glNormal3f(0.0f,0.0f,-1.0f);//-z方向
	for(i = nSlice-1;i >= 0;i--) glVertex3fv(pd[i+j*nSlice]);
	glEnd();
}

//---------------------------------------------------------------------------
void drawRevolution1(float cs[][2], int nSide, int nSlice)
{
	//
	//断面座標cs[][]はx-z平面で与える(制御点）
	//( cs[][0]:x座標，cs[][1]:z座標)
	//回転軸：ｚ軸
	//始点と終点は異なり，ｚ軸上にあること
	//nSide:断面分割点数(x-z平面）
	//nSlice:xy断面分割数
	int i, ii, j, jj, jm, jp;
	double phi;//方位角
	float p[31][31][3];
	float n0[30][2];//x-z基本断面の各稜線の法線（x,z成分）
	float n1[30][2];//x-z基本断面の各頂点の法線（x,z成分）
	float nn[31][31][3];//各頂点の法線
	double len, dx, dz;

	if(nSide > 30) { printf("nSide < 30とすること \n"); }//exit(1); }
	if(nSlice > 30) nSlice = 30;

	int nSide2;
	if(cs[0][0] == 0.0 && cs[nSide-1][0] == 0.0)//始点と終点がｚ軸上
			nSide2 = nSide-1;
	else nSide2 = nSide;

	//基本断面における各稜線の法線（２次元）
	for(j = 0; j < nSide; j++)
	{
		jp = j + 1;
		if(jp == nSide) jp = 0;
		dx = (double)(cs[jp][0] - cs[j][0]);
		dz = (double)(cs[jp][1] - cs[j][1]);
		len = sqrt( dx*dx +  dz*dz);
		n0[j][0] = -dz / len;//x成分
		n0[j][1] = dx / len;//y成分
	}

	//x-z断面上の頂点の法線（２次元)
	if(cs[0][0] == 0.0 && cs[nSide-1][0] == 0.0)//始点と終点がｚ軸上
	{
		n1[0][0] = 0.0; n1[0][1] = 1.0;//始点
		n1[nSide-1][0] = 0.0; n1[nSide-1][1] = -1.0;//終点;
		for(j = 1; j < nSide-1; j++)
		{
			n1[j][0] = (n0[j-1][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[j-1][1] + n0[j][1]) / 2.0;
		}
	}
	else//始点と終点の両方またはどちらかがｚ軸上にない場合）
	{
		for(j = 0; j < nSide; j++)
		{
			jm = j - 1;
			if(j == 0) jm = nSide - 1;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;
		}
	}

	//他の断面の座標と法線(３次元)
	for(i = 0; i < nSlice; i++)
	{
		phi = 2.0*M_PI*(double)i/(double)nSlice;//方位角
		for(j = 0; j < nSide; j++)
		{	
			p[i][j][0] = cs[j][0] * (float)cos(phi); //x座標
			p[i][j][1] = cs[j][0] * (float)sin(phi); //y
			p[i][j][2] = cs[j][1];                   //z
			nn[i][j][0] = n1[j][0] * (float)cos(phi);
			nn[i][j][1] = n1[j][0] * (float)sin(phi);
			nn[i][j][2] = n1[j][1] ;
		}
	}

	//頂点列を定義し描画
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
	//回転軸：ｚ軸
	//始点と終点は一致(nSideは終点を含む）
	//nCont:断面分割点数(制御点数）
	//nSlice:中心軸に沿った分割点数
	int i, ii, j, jj, jm;
	int nSide;
	double phi; //方位角
	float p[31][81][3];
	float cs[81][2];//断面座標
	float n0[80][2];//x-z基本断面の各稜線の法線（x,z成分）
	float n1[80][2];//x-z基本断面の各頂点の法線（x,z成分）
	float nn[31][81][3];//各頂点の法線
	double len, dx, dz;

	if(nCont > 20) printf("nCont <= 20とすること \n");
	if(nDiv > 4) printf("nDiv <= 4とすること \n");
	if(nSlice > 30) nSlice = 30;

	if(cs0[0][0] == 0.0 && cs0[nCont-1][0] == 0.0)//始点と終点がz軸上
	{
		//Spline補間
		spline2(cs0, cs, nCont, nDiv);
		nSide = (nCont -1) * nDiv + 1;//新しい分割点数
		//基本断面における各稜線の法線（２次元）
		for(j = 0; j < nSide-1; j++)
		{
			jj = j + 1;
			dx = (double)(cs[jj][0] - cs[j][0]);
			dz = (double)(cs[jj][1] - cs[j][1]);
			len = sqrt( dx*dx +  dz*dz);
			n0[j][0] = -dz / len;
			n0[j][1] = dx / len;
		}

		//x-z断面上の頂点の法線（２次元)
		n1[0][0] = 0.0; n1[0][1] = 1.0;//始点はz軸方向
		n1[nSide-1][0] = 0.0; n1[nSide-1][1] = -1.0;//終点は－z軸方向
		for(j = 1; j < nSide-1; j++)
		{
			jm = j - 1;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;//x成分
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;//z成分
		}

		//他の断面の座標と法線(３次元)
		for(i = 0; i < nSlice; i++)
		{
			phi = 2.0*M_PI*(double)i/(double)nSlice;
			for(j = 0; j < nSide; j++)
			{	
				p[i][j][0] = cs[j][0] * (float)cos(phi); //x座標
				p[i][j][1] = cs[j][0] * (float)sin(phi); //y
				p[i][j][2] = cs[j][1];                   //z
				nn[i][j][0] = n1[j][0] * (float)cos(phi);
				nn[i][j][1] = n1[j][0] * (float)sin(phi);
				nn[i][j][2] = n1[j][1] ;
			}
		}

		//頂点列を定義し描画
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
		//始点または終点がｚ軸上になければ始点＝終点となるように1個増やす
		float cs1[21][2];
		for(j = 0; j < nCont; j++)
		{
			cs1[j][0] = cs0[j][0];
			cs1[j][1] = cs0[j][1];
		}
		nCont ++;
		cs1[nCont-1][0] = cs0[0][0];
		cs1[nCont-1][1] = cs0[0][1];
	
		//Spline補間
		spline2(cs1, cs, nCont, nDiv);
		nSide = (nCont -1) * nDiv + 1;//新しい分割点数
		//基本断面における各稜線の法線（２次元）
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

		//基本断面における各頂点の法線（２次元）
		for(j = 0; j < nSide-1; j++)
		{
			jm = j - 1;
			if(j == 0) jm = nSide-2;
			n1[j][0] = (n0[jm][0] + n0[j][0]) / 2.0;//x成分
			n1[j][1] = (n0[jm][1] + n0[j][1]) / 2.0;//z成分
		}

		//他の断面の座標と法線(３次元)
		for(i = 0; i < nSlice; i++)
		{
			phi = 2.0*M_PI*(double)i/(double)nSlice;
			for(j = 0; j < nSide-1; j++)
			{	
				p[i][j][0] = cs[j][0] * (float)cos(phi); //x座標
				p[i][j][1] = cs[j][0] * (float)sin(phi); //y
				p[i][j][2] = cs[j][1];                   //z
				nn[i][j][0] = n1[j][0] * (float)cos(phi);
				nn[i][j][1] = n1[j][0] * (float)sin(phi);
				nn[i][j][2] = n1[j][1] ;
			}
		}

		//頂点列を定義し描画
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
{//直線押し出し
	//底の断面はx-y平面（頂点列はz方向から見て左回り)
	//面の法線は４頂点の座標で決める
	//基本姿勢では断面はすべてｚ方向または-z方向
	//底の中心が原点
	//nSide:断面分割点数
	//nSpine:経路（背骨）の個数（始点，終点を含む）
	int i, ii, j, jj;
	float p[31][31][3];
	float n[3];

	if(nSide > 30) printf("nSide < 30とすること \n");
	if(nSpine > 30) nSpine = 30;
	//頂点座標
	for(j = 0; j < nSpine; j++)//jは下から上へ
	{
		for(i = 0; i < nSide; i++)
		{	
			p[i][j][0] = cs[i][0] + sp[j][0];//x座標
			p[i][j][1] = cs[i][1] + sp[j][1]; //y
			p[i][j][2] = sp[j][2] ;                      //z
		}
	}

	//頂点列を定義し描画
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

	//下底
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
	//上底
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
	//底の断面はx-y平面（頂点列はz方向から見て左回り)
	//cs：断面座標(x-y平面）
	//sp：経路（背骨）座標
	//底の中心が原点(sp[0][2]は必ず0であること）
	//nSide:断面分割点数
	//nSpine:経路（背骨）の個数（始点，終点を含む）
	//断面は軸方向に直交するように変換
	int i, ii, j, jj, im, k;
	float p[31][31][3];
	//CVector P[31][31];

	float nn[31][31][3];//各頂点の法線
	double len, x, y, z;

	if(nSide > 30) printf("nSide < 30とすること \n");
	if(nSpine > 30) nSpine = 30;

	if(sp[0][0] != 0.0 || sp[0][1] != 0.0 || sp[0][2] != 0.0) printf("経路節点データが不正です \n"); 
	if(sp[1][0] != 0.0 || sp[1][1] != 0.0 || sp[1][2] == 0.0) printf("経路節点データが不正です \n"); 
	//軸方向ベクトル
	double e[30][3];
	for(j = 0; j < nSpine-1; j++)
	{
		for(k = 0; k < 3; k++) e[j][k] = sp[j+1][k] - sp[j][k];
		len = sqrt(e[j][0] * e[j][0] + e[j][1] * e[j][1] + e[j][2] * e[j][2]);
		for(k = 0; k <3; k++) e[j][k] /= len;//単位ベクトル化
	}
	//断面の方向ベクトル
	double a[30][3];
	for(k = 0; k <3; k++) a[0][k] = e[0][k];//始点の断面
	for(k = 0; k <3; k++) a[nSpine-1][k] = e[nSpine-2][k];//終端の断面
	for(j = 1; j < nSpine-1; j++)
	{
		for(k = 0; k <3; k++) a[j][k] = (e[j-1][k] + e[j][k]) / 2.0;
	}

	//断面頂点の座標(３次元)
	//最初の面の頂点座標
	CVector E = CVector(0.0, 0.0, 1.0);//軸方向ベクトル
	CVector A = CVector(a[0][0], a[0][1], a[0][2]);//断面の法線ベクトル
	CVector P = CVector();//前の断面の頂点位置ベクトル
	CVector P0 = CVector(sp[0][0], sp[0][1], sp[0][2]);//注目している断面の中心（節点)の位置ベクトル
	CVector newP = CVector();//注目断面の新しい頂点位置ベクトル
	double b ;//= A * E;//ベクトルの内積
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

	//断面側面上の頂点の法線
	CVector L = CVector();
	CVector N[30];
	CVector NN[30];
	for(j = 0; j < nSpine; j++)
	{
		//稜線の法線
		for(i = 0; i < nSide; i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			x = p[ii][j][0] - p[i][j][0];
			y = p[ii][j][1] - p[i][j][1];
			z = p[ii][j][2] - p[i][j][2];
			L = CVector(x, y, z);//稜線ベクトル
			A = CVector(a[j][0], a[j][1], a[j][2]);
			N[i] = L ^ A;//外積
			N[i].normalize();
		}
		//頂点の法線
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

	//頂点列を定義し描画
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
	
	//下底
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
	//上底
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
	//断面と経路どちらも3次スプライン補間
	//底の断面はx-y平面（頂点列はz方向から見て左回り)
	//cs：断面座標(x-y平面）
	//sp：経路（背骨）座標
	//底の中心が原点(sp[0][2]は必ず0であること）
	//nSide:断面分割点数
	//nSpine:経路（背骨）の個数（始点，終点を含む）
	//nDiv1：断面のスプライン分割数
	//nDiv2：経路のスプライン分割数
	int i, ii, j, jj, im, k;
	float p[81][81][3];
	float nn[81][81][3];//各頂点の法線
	double len, x, y, z;
	float cs[81][2], cs1[21][2];
	float sp[81][3];
	int nSide, nSpine;//spline補間後の断面分割数，経路数

	if(nSide0 > 20) printf("nSide0 <= 20とすること \n");
	if(nSpine0 > 20) printf("nSpine0 <= 20とすること \n");
	if(nDiv1 > 4) printf("nDiv1 <= 4とすること \n");
	if(nDiv2 > 4) printf("nDiv2 <= 4とすること \n");
	if(sp0[0][0] != 0.0 || sp0[0][1] != 0.0 || sp0[0][2] != 0.0)
	{
		printf("経路節点データが不正です(drawSweep2) \n"); 
		return;
	}
	if(sp0[1][0] != 0.0 || sp0[1][1] != 0.0 || sp0[1][2] == 0.0)
	{
		printf("経路節点データが不正です(drawSweep2) \n"); 
		return;
	}

	//断面のスプライン補間
	for(i = 0; i < nSide0; i++)
	{
		cs1[i][0] = cs0[i][0];
		cs1[i][1] = cs0[i][1];
	}
	//周期境界条件のspline補間のために始点＝終点とする
	nSide0 ++;
	cs1[nSide0-1][0] = cs0[0][0];
	cs1[nSide0-1][1] = cs0[0][1];
	spline2(cs1, cs, nSide0, nDiv1);//2次元のSpline
	nSide = (nSide0 -1) * nDiv1;//新しい分割点数
	//経路のスプライン補間
	spline3(sp0, sp, nSpine0, nDiv2);//3次元のSpline
	nSpine = (nSpine0-1) * nDiv2 + 1;

	//軸方向ベクトル
	double e[80][3];
	for(j = 0; j < nSpine-1; j++)
	{
		for(k = 0; k < 3; k++) e[j][k] = sp[j+1][k] - sp[j][k];
		len = sqrt(e[j][0] * e[j][0] + e[j][1] * e[j][1] + e[j][2] * e[j][2]);
		for(k = 0; k <3; k++) e[j][k] /= len;//単位ベクトル化
	}
	//断面の方向ベクトル
	double a[80][3];
	for(k = 0; k <3; k++) a[0][k] = e[0][k];//始点の断面
	for(k = 0; k <3; k++) a[nSpine-1][k] = e[nSpine-2][k];//終端の断面
	for(j = 1; j < nSpine-1; j++)
	{
		for(k = 0; k <3; k++) a[j][k] = (e[j-1][k] + e[j][k]) / 2.0;
	}

	//断面頂点の座標(３次元)
	//最初の面の頂点座標
	CVector E = CVector(0.0, 0.0, 1.0);//軸方向ベクトル
	CVector A = CVector(a[0][0], a[0][1], a[0][2]);//断面の法線ベクトル
	CVector P = CVector();//前の断面の頂点位置ベクトル
	CVector P0 = CVector(sp[0][0], sp[0][1], sp[0][2]);//注目している断面の中心（節点)の位置ベクトル
	CVector newP = CVector();//注目断面の新しい頂点位置ベクトル
	double b ;//= A * E;//ベクトルの内積
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

	//断面側面上の頂点の法線
	CVector L = CVector();
	CVector N[30];
	CVector NN[30];
	for(j = 0; j < nSpine; j++)
	{
		//稜線の法線
		for(i = 0; i < nSide; i++)
		{
			ii = i + 1;
			if(ii == nSide) ii = 0;
			x = p[ii][j][0] - p[i][j][0];
			y = p[ii][j][1] - p[i][j][1];
			z = p[ii][j][2] - p[i][j][2];
			L = CVector(x, y, z);//稜線ベクトル
			A = CVector(a[j][0], a[j][1], a[j][2]);
			N[i] = L ^ A;//外積
			N[i].normalize();
		}
		//頂点の外向き法線方向
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

	//頂点列を定義し描画
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
	
	//下底
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
	//上底
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
{	//rootの底面中心が原点

	//radius:半径
	//ratio:長さ比率
	//angle：関節の回転角
	//nBone：ボーン個数
	//nSide：断面分割点数
	//nDiv：スプライン分割数
	//断面は軸方向に直交するように変換
	int i, ii, j, jj, k;
	CVector p[21][130];
	CVector nn[21][130];//各頂点の法線
	float sp0[41][3];
	float sp[161][3];
	CVector e[161];
	CVector a[161];
	CVector cs[21];
	CVector joint[11];

	if(nBone > 10) printf("nBone <= 10とすること \n");
	if(nSide > 20) printf("nSide <= 20とすること \n");
	if(nDiv > 4) printf("nDiv1 <= 4とすること \n");
	if(angle[0][0] != 0.0 || angle[0][1] != 0.0 || angle[0][2] != 0.0) 
	{
		printf("経路節点データが不正です(drawSkin) \n"); 
		return;
	}

	//断面形状
	double phi;
	for(i = 0; i < nSide; i++)
	{
		phi = 2.0 * M_PI * (double)i / (double)nSide;
		cs[i].x = radius * cos(phi);
		cs[i].y = radius * sin(phi);
		cs[i].z = 0.0;
	}
	//初期姿勢の関節はすべてｚ軸上にある
	for(j = 0; j <= nBone; j++) joint[j] = CVector();//始点は原点
	for(j = 1; j <= nBone; j++) joint[j].z = joint[j-1].z + len[j-1];
	//関節位置を求める
	for(j = nBone-1; j > 0; j--)//最下位層から求める
	{
		for(jj = j + 1; jj <= nBone; jj++)
		{
			joint[jj] = joint[j] + rotate(joint[jj] - joint[j], angle[j]);
		}
	}
	double s1, s2, s3;//位置パラメータ
	s1 = ratio;
	s2 = 0.5;
	s3 = 1.0 - ratio;
	//経路節点座標
	int no = 0;//経路節点番号
	//始点
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
	//終点
	sp0[no][0] = joint[nBone].x;
	sp0[no][1] = joint[nBone].y;
	sp0[no][2] = joint[nBone].z;
	int nSpine = no + 1;//始点，終点を含む経路節点数
	//関節位置の調整
	int m;
	for(m = 1; m < nBone; m++)
	{
		j = 4 * m;
		for(k = 0; k < 3; k++) 
			sp0[j][k] = weight * sp0[j][k] + (1 - weight) * (sp0[j-1][k] + sp0[j+1][k])/2.0;
	}

	
	//経路のスプライン補間
	spline3(sp0, sp, nSpine, nDiv);//3次元のSpline
	nSpine = (nSpine-1) * nDiv + 1;

	//軸方向ベクトル
	for(j = 0; j < nSpine-1; j++)
	{
		e[j].x = sp[j+1][0] - sp[j][0];
		e[j].y = sp[j+1][1] - sp[j][1];
		e[j].z = sp[j+1][2] - sp[j][2];
		e[j].normalize();
	}
	//断面の方向ベクトル
	a[0] = e[0];//始点の断面
	a[nSpine-1] = e[nSpine-2];//終端の断面
	for(j = 1; j < nSpine-1; j++)
	{
		a[j] = (e[j-1] + e[j]) / 2.0;
	}

	//断面頂点の座標(３次元)
	//最初の面の頂点座標
	CVector P0;// = CVector(sp[0][0], sp[0][1], sp[0][2]);//注目している断面の中心（節点)の位置ベクトル
	double b ;
	double c ;


	//最初の断面
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

	//断面側面上の頂点の法線
	//円形断面なので頂点座標と節点座標の差で求める
	for(j = 0; j < nSpine; j++)
	{
		for(i = 0; i < nSide; i++)
		{
			P0 = CVector(sp[j][0], sp[j][1], sp[j][2]);
			
			nn[i][j] = P0 >> p[i][j] ;//P0からp[i][j]へ向かう単位ベクトル
		}
	}

	//頂点列を定義し描画
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

	//下底
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
	//上底
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
//四角形のメッシュ（x-y平面,中心が原点）
//ｘ軸方向，ｙ軸方向の幅を固定
void drawElevation(int Nx, int Ny, float sizeX, float sizeY, int sideFlag, float* data)
{
	//全体の幅,長さsizeX, sizeY
	//sideFlag = 0:側面表示せず
	//sideFlag = 1:側面表示する

	const int NMAX = 130;
	int i, j;
	float p[NMAX][NMAX][3]; //頂点座標
	float a[NMAX][NMAX], b[NMAX][NMAX], c[NMAX][NMAX];//頂点の法線
	float pitchX, pitchY;
	float n1[3], n2[3], n3[3], n4[3];

	if(Nx > NMAX) printf("NxがNMAXを超えています(drawElevation1) \n");
	if(Ny > NMAX) printf("NyがNMAXを超えています(drawElevation1) \n");

	//セルのサイズ
	pitchX = sizeX / (float)Nx;
	pitchY = sizeY / (float)Ny;

	//各頂点の座標
	for(j = 0; j <= Ny; j++){
		for(i = 0; i <= Nx; i++){
			p[i][j][0] = (float)(i - Nx / 2) * pitchX;
			p[i][j][1] = (float)(j - Ny / 2) * pitchY;
			p[i][j][2] = data[j * (Nx+1) + i];
		}
	}

			
	//法線成分
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
					calcNormal(p[i][0],p[i][1],p[i-1][0],n1);//左側
					calcNormal(p[i][0],p[i+1][0],p[i][1],n2);//右側
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
					calcNormal(p[i][Ny],p[i-1][Ny],p[i][Ny-1],n1);//左側
					calcNormal(p[i][Ny],p[i][Ny-1],p[i+1][Ny],n2);//右側
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
			}
			else
			{
				if(i == 0) 
				{
					calcNormal(p[0][j],p[1][j],p[0][j+1],n1);//上
					calcNormal(p[0][j],p[0][j-1],p[0][1],n2);//下
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
				else if(i == Nx) 
				{
					calcNormal(p[Nx][j],p[Nx][j+1],p[Nx-1][j],n1);//上
					calcNormal(p[Nx][j],p[Nx-1][j],p[Nx][j-1],n2);//下
					a[i][j] = (n1[0]+n2[0])/2.0f;
					b[i][j] = (n1[1]+n2[1])/2.0f;
					c[i][j] = (n1[2]+n2[2])/2.0f; }
				else 
				{//上下左右４個の三角形の平均
					calcNormal(p[i][j],p[i][j+1],p[i-1][j],n1);//左上
					calcNormal(p[i][j],p[i+1][j],p[i][j+1],n2);//右上
					calcNormal(p[i][j],p[i-1][j],p[i][j-1],n3);//左下
					calcNormal(p[i][j],p[i][j-1],p[i+1][j],n4);//右下
					a[i][j] = (n1[0]+n2[0]+n3[0]+n4[0])/4.0f;
					b[i][j] = (n1[1]+n2[1]+n3[1]+n4[1])/4.0f;
					c[i][j] = (n1[2]+n2[2]+n3[2]+n4[2])/4.0f; }
			}
		}

//	int nC;
	//三角形で面を定義
	glBegin(GL_TRIANGLES);
	for(j = 0;j < Ny;j++)
		for(i = 0;i < Nx;i++)
		{
			//左下の三角形
			//各頂点の法線方向,ﾃｸｽﾁｬｰ座標,頂点座標を与える。
			glNormal3f(a[i][j],b[i][j],c[i][j]);//法線方向
			glVertex3fv(p[i][j]);//ﾎﾟﾘｺﾞﾝの頂点座標（以下これらを繰り返す）
			glNormal3f(a[i+1][j],b[i+1][j],c[i+1][j]);
			glVertex3fv(p[i+1][j]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(p[i][j+1]);
			//右上の三角形
			glNormal3f(a[i+1][j],b[i+1][j],c[i+1][j]);
			glVertex3fv(p[i+1][j]);
			glNormal3f(a[i+1][j+1],b[i+1][j+1],c[i+1][j+1]);
			glVertex3fv(p[i+1][j+1]);
			glNormal3f(a[i][j+1],b[i][j+1],c[i][j+1]);
			glVertex3fv(p[i][j+1]);
		}
	glEnd();

	if(sideFlag == 1)//側面描画
	{
		glBegin(GL_QUADS);
		//+x方向（i=Nx)
		glNormal3f(1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[Nx][j][0], p[Nx][j][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], p[Nx][j+1][2]);
			glVertex3f(p[Nx][j][0], p[Nx][j][1], p[Nx][j][2]);
		}
		//-x方向（i=0)
		glNormal3f(-1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[0][j][0], p[0][j][1], 0.0f);
			glVertex3f(p[0][j][0], p[0][j][1], p[0][j][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], p[0][j+1][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], 0.0f);
		}
		//+y方向（j=Ny)
		glNormal3f(0.0, 1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][Ny][0], p[i][Ny][1], 0.0f);
			glVertex3f(p[i][Ny][0], p[i][Ny][1], p[i][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], p[i+1][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], 0.0f);
		}
		//-y方向（j=0)
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
//四角形のメッシュ（x-y平面,中心が原点）
//ｘ軸方向，ｙ軸方向の幅を固定
void drawGrid(int Nx, int Ny, float sizeX, float sizeY, int sideFlag)
{
	//全体の幅,長さsizeX, sizeY
	//sideFlag = 0:側面表示せず
	//sideFlag = 1:側面表示する
	const int NMAX = 130;
	int i, j;
	float p[NMAX][NMAX][3]; //頂点座標
//	float a[NMAX][NMAX], b[NMAX][NMAX], c[NMAX][NMAX];//頂点の法線
	float pitchX, pitchY;
//	float n1[3], n2[3], n3[3], n4[3];

	if(Nx > NMAX) printf("NxがNMAXを超えています(drawElevation1) \n");
	if(Ny > NMAX) printf("NyがNMAXを超えています(drawElevation1) \n");

	//セルのサイズ
	pitchX = sizeX / (float)Nx;
	pitchY = sizeY / (float)Ny;

	//各頂点の座標
	for(j = 0; j <= Ny; j++){
		for(i = 0; i <= Nx; i++){
			p[i][j][0] = (float)(i - Nx / 2) * pitchX;
			p[i][j][1] = (float)(j - Ny / 2) * pitchY;
			p[i][j][2] = 0.0;//data[j * (Nx+1) + i];
		}
	}
		
//	int nC;
	//三角形で面を定義
	glNormal3f(0.0, 0.0, 1.0);//すべてz方向
	glBegin(GL_TRIANGLES);
	for(j = 0;j < Ny;j++)
		for(i = 0;i < Nx;i++)
		{
			//左下の三角形
			//各頂点の法線方向,ﾃｸｽﾁｬｰ座標,頂点座標を与える。
			glVertex3fv(p[i][j]);//ﾎﾟﾘｺﾞﾝの頂点座標（以下これらを繰り返す）
			glVertex3fv(p[i+1][j]);
			glVertex3fv(p[i][j+1]);
			//右上の三角形
			glVertex3fv(p[i+1][j]);
			glVertex3fv(p[i+1][j+1]);
			glVertex3fv(p[i][j+1]);
		}
	glEnd();

	if(sideFlag == 1)//側面描画
	{
		glBegin(GL_QUADS);
		//+x方向（i=Nx)
		glNormal3f(1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[Nx][j][0], p[Nx][j][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], 0.0f);
			glVertex3f(p[Nx][j+1][0], p[Nx][j+1][1], p[Nx][j+1][2]);
			glVertex3f(p[Nx][j][0], p[Nx][j][1], p[Nx][j][2]);
		}
		//-x方向（i=0)
		glNormal3f(-1.0, 0.0, 0.0);
		for(j = 0; j < Ny; j++)
		{
			glVertex3f(p[0][j][0], p[0][j][1], 0.0f);
			glVertex3f(p[0][j][0], p[0][j][1], p[0][j][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], p[0][j+1][2]);
			glVertex3f(p[0][j+1][0], p[0][j+1][1], 0.0f);
		}
		//+y方向（j=Ny)
		glNormal3f(0.0, 1.0, 0.0);
		for(i = 0; i < Nx; i++)
		{
			glVertex3f(p[i][Ny][0], p[i][Ny][1], 0.0f);
			glVertex3f(p[i][Ny][0], p[i][Ny][1], p[i][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], p[i+1][Ny][2]);
			glVertex3f(p[i+1][Ny][0], p[i+1][Ny][1], 0.0f);
		}
		//-y方向（j=0)
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
	//曲線の描画
	glLineWidth(2.0f);
	glBegin(GL_LINE_STRIP);
	for(i = 0; i <= nDiv; i++) glEvalCoord1f((float)i / (float)nDiv);
	glEnd();
	if(flagControl)
	{
		//制御点の描画
		glPointSize(5.0f);
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_POINTS);
		for(i = 0; i < 4; i++) glVertex3f(ctrPoint[i][0], ctrPoint[i][1], ctrPoint[i][2]);
		glEnd();
		//制御点間線分描画
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
		//制御点の描画
		glPointSize(5.0f);
		glColor3f(1.0, 0.0, 0.0);

		glBegin(GL_POINTS);
		for(j = 0; j < 4; j++)
			for(i = 0; i < 4; i++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);
		//制御点間線分描画
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
	float diffuseP[] = {1.0f,0.0f,0.0f,1.0f};//制御点の拡散光
	float diffuseL[] = {0.0f,0.0f,1.0f,1.0f};//制御点間の拡散光

	glEnable( GL_AUTO_NORMAL);//自動正規化
	glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 4, 0.0f, 1.0f, 12, 4, ctrPoints[0][0]);
	glEnable( GL_MAP2_VERTEX_3);
	glMapGrid2d(nMesh, 1.0, 0.0, nMesh, 0.0, 1.0);
	glEvalMesh2(GL_FILL, 0, nMesh, 0, nMesh);

	if(flagControl)
	{
		//制御点の描画
		glPointSize(5.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseP);

		glBegin(GL_POINTS);
		for(j = 0; j < 4; j++)
			for(i = 0; i < 4; i++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);
		//制御点間線分描画
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
	float diffuseP[] = {0.9f,0.0f,0.0f,1.0f};//制御点の拡散光
	float diffuseL[] = {0.0f,0.0f,1.0f,1.0f};//制御点間の拡散光
	GLUnurbsObj* nurb;//NURBSへのポインタ
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
		//制御点の描画
		glPointSize(5.0f);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,diffuseP);

		glBegin(GL_POINTS);
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++) glVertex3fv(ctrPoints[j][i]);
		glEnd();
		glPointSize(1.0f);

		//制御点間線分描画
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
  //Floor１枚当たりの幅
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


