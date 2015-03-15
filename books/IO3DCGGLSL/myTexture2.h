//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//Texture
//------------------------------------------------------------------------------

void drawTexPlate(float size, int nRepeatS, int nRepeatT)//x-y平面
{
	float sz = 0.5 * size;
	static float p[4][3] = //z:上方向
	{ 
		{ sz,-sz, 0.0}, { sz, sz, 0.0}, 
		{-sz, sz, 0.0}, {-sz,-sz, 0.0}
	};

	float s = (float)nRepeatS;
	float t = (float)nRepeatT;
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0); //z方向の法線
		//ﾃｸｽﾁｬｰ座標と頂点番号との対応付け
		glTexCoord2f(0.0, 0.0); glVertex3fv(p[3]);
		glTexCoord2f( s , 0.0); glVertex3fv(p[0]);
		glTexCoord2f( s ,  t ); glVertex3fv(p[1]);
		glTexCoord2f(0.0,  t ); glVertex3fv(p[2]);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//-----------------------------------------------------------------------
//正面だけﾏｯﾋﾟﾝｸﾞ(z軸が上方向）
void drawTexCube1(float size, int nRepeatS, int nRepeatT)
{
	float sz = 0.5 * size;
	float p[8][3] = 
	{ {sz,sz,sz}, {-sz,sz,sz}, {-sz,-sz,sz}, 
	  {sz,-sz,sz},{sz,sz,-sz}, {-sz,sz,-sz},
	  {-sz,-sz,-sz}, {sz,-sz,-sz}
	};

	float s = (float)nRepeatS;
	float t = (float)nRepeatT;

	glBegin(GL_QUADS);
		glNormal3f(0.0f,0.0f,1.0f); //z方向
		glVertex3fv(p[0]); glVertex3fv(p[1]);
		glVertex3fv(p[2]); glVertex3fv(p[3]);
	glEnd();

	glEnable(GL_TEXTURE_2D);//正面だけマッピング
	glBegin(GL_QUADS);
		glNormal3f(1.0,0.0,0.0); //x方向(正面）
		glTexCoord2f( s ,  t ); glVertex3fv(p[0]);
		glTexCoord2f(0.0,  t ); glVertex3fv(p[3]);
		glTexCoord2f(0.0, 0.0); glVertex3fv(p[7]);
		glTexCoord2f( s , 0.0); glVertex3fv(p[4]);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
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
//-----------------------------------------------------------------------
//６面に同じ模様をﾏｯﾋﾟﾝｸﾞ
void drawTexCube2(float size, int nRepeatS, int nRepeatT)
{
	float sz = 0.5 * size;
	float p[8][3] =
	{ { sz, sz, sz}, {-sz, sz, sz}, {-sz,-sz, sz}, 
	  { sz,-sz, sz}, { sz, sz,-sz}, {-sz, sz,-sz},
	  {-sz,-sz,-sz}, { sz,-sz,-sz}
	};

	float s = (float)nRepeatS;
	float t = (float)nRepeatT;

	glEnable(GL_TEXTURE_2D);
  glBegin(GL_QUADS);
	//top(z軸が鉛直軸のときz方向）
	glNormal3f(0.0f,0.0f,1.0f); 
	glTexCoord2f( s , 0.0); glVertex3fv(p[0]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[1]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[2]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[3]);
	//x方向(正面）
	glNormal3f(1.0f,0.0f,0.0f); 
	glTexCoord2f( s ,  t ); glVertex3fv(p[0]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[3]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[7]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[4]);
	//y方向
	glNormal3f(0.0f,1.0f,0.0f);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[0]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[4]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[5]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[1]);
	//-x方向
	glNormal3f(-1.0f,0.0f,0.0f); 
	glTexCoord2f(0.0,  t ); glVertex3fv(p[6]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[2]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[1]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[5]);
	//-y方向
	glNormal3f(0.0f,-1.0f,0.0f); 
	glTexCoord2f(0.0,  t ); glVertex3fv(p[2]);
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[6]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[7]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[3]);
	//-z方向
	glNormal3f(0.0f,0.0f,-1.0f); 
	glTexCoord2f(0.0, 0.0); glVertex3fv(p[6]);
	glTexCoord2f( s , 0.0); glVertex3fv(p[5]);
	glTexCoord2f( s ,  t ); glVertex3fv(p[4]);
	glTexCoord2f(0.0,  t ); glVertex3fv(p[7]);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

//--------------------------------------------------------------------------
//正面側(x>0)に平行投影
void drawTexSphere1(double radius, int nSlice, int nStack)
{
	int i, j;
	double r0, r1, th0, th1, phi;
	double p[2][3];

	//前面にだけ平行投影
	glEnable(GL_TEXTURE_2D);
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
		for(i = 0; i <= nSlice/2; i++)
		{
			phi = 2.0 * M_PI * (double)i / (double)nSlice;
			//頂点のxy座標(i=0をobjectからみて右端) 
			p[0][0] =   r0 * sin(phi);//x座標
			p[0][1] = - r0 * cos(phi);//y座標
			p[1][0] =   r1 * sin(phi);//x座標
			p[1][1] = - r1 * cos(phi);//y座標

			glTexCoord2d(0.5 + p[0][1], 0.5 + p[0][2]);//テクスチャ座標
			glNormal3dv(p[0]);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標

			glTexCoord2d(0.5 + p[1][1], 0.5 + p[1][2]);//テクスチャ座標
			glNormal3dv(p[1]);//法線ベクトル
			glVertex3dv(p[1]);//頂点座標			
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	//後面
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
		for(i = nSlice/2; i <= nSlice; i++)
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
//--------------------------------------------------------------------------
//球投影
void drawTexSphere2(double radius, int nSlice, int nStack, int nRepeatS, int nRepeatT)
{
	int i, j;
	double s, t0, t1, r0, r1, th0, th1, phi;
	double p[2][3];

	for(j = 0; j < nStack; j++)
	{
		//j=0は北極点(x=0, y=0, z=radius)
		//2つのt座標
		t0 = (double)j / (double)nStack;
		t1 = (double)(j+1) / (double)nStack;
		//これらの天頂角
		th0 = M_PI * t0;
		th1 = M_PI * t1;
		//x-y平面に投影した半径
		r0 = radius * sin(th0);
		r1 = radius * sin(th1);
		//頂点z座標
		p[0][2] = radius * cos(th0);
		p[1][2] = radius * cos(th1);

		//北極点を1とするt座標
		t0 = (1.0 - t0) * nRepeatT;
		t1 = (1.0 - t1) * nRepeatT;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s座標
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = - r0 * cos(phi);//x座標
			p[0][1] = - r0 * sin(phi);//y座標
			p[1][0] = - r1 * cos(phi);//x座標
			p[1][1] = - r1 * sin(phi);//y座標

			s *= nRepeatS;

			glTexCoord2d(s, t0);//テクスチャ座標
			glNormal3dv(p[0]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[0]);//頂点座標

			glTexCoord2d(s, t1);//テクスチャ座標
			glNormal3dv(p[1]);//法線ベクトル,正規化すれば頂点座標に同じ
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}

//----------------------------------------------------------------------
void drawTexCylinder1(double rBottom, double rTop, double height, 
							int nSlice, int nStack, int nRepeatS, int nRepeatT)
{

	//側面全体に円筒投影
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
		x = rTop * cos(theta); //x成分
		y = rTop * sin(theta); //ｙ成分
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
		x = rBottom * cos(theta); //x成分
		y = rBottom * sin(theta); //ｙ成分
		z = - height/2.0;
		glVertex3f(x, y, z);
	}
	glEnd();

	double s, t0, t1, r0, r1, phi;
	double p[2][3], n[3];
	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);

	//側面にテクスチャ
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

		//Topを1とするt座標
		t0 = (1.0 - t0) * nRepeatT;
		t1 = (1.0 - t1) * nRepeatT;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s座標
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = - r0 * cos(phi);//x座標
			p[0][1] = - r0 * sin(phi);//y座標
			p[1][0] = - r1 * cos(phi);//x座標
			p[1][1] = - r1 * sin(phi);//y座標
			//法線ベクトル
			n[0] = -nxy * cos(phi);
			n[1] = -nxy * sin(phi);
			n[2] =  nz;

			s *= nRepeatS;

			glTexCoord2d(s, t0);//テクスチャ座標
			glNormal3dv(n);     //法線ベクトル
			glVertex3dv(p[0]);  //頂点座標

			glTexCoord2d(s, t1);//テクスチャ座標
			glVertex3dv(p[1]);  //頂点座標
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

}

//-------------------------------------------------------------------------------------
void drawTexCylinder2(double rBottom, double rTop, double height, double thick, 
											int nSlice, int nStack, int nRepeatS, int nRepeatT)
{
	//厚みのある中空円柱(外側側面に円筒投影)
	//円柱(rBottom=rTop))、円錐台、円錐(rTop = 0.0)
	//rBottom:下底半径, rTop:上底半径, thick:厚さ
	//nSlice--xy断面分割数
	//nStack---ｚ方向分割数
	//物体の中心は下底と上底の中間

	int i, j;
	float x, y, z, z0, z1;
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
		x = (float)(rBottom * cos(theta)); //x成分
		y = (float)(rBottom * sin(theta)); //y成分
		glVertex3f(x, y, z);
		//内側座標
		x = (float)((rBottom - thick) * cos(theta)); //x成分
		y = (float)((rBottom - thick) * sin(theta)); //y成分
		glVertex3f(x, y, z);
	}
	glEnd();

	double s, t0, t1, r0, r1, phi;
	double p[2][3], n[3];

	double rr = rBottom - rTop;
	double nz = rr / sqrt(rr*rr + height*height);
  double nxy = sqrt(1.0 - nz * nz);

	//外側側面にだけテクスチャ
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
		
		//Topを1とするt座標
		t0 = (1.0 - t0) * nRepeatT;
		t1 = (1.0 - t1) * nRepeatT;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(i = 0; i <= nSlice; i++)
		{
			//s座標
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = - r0 * cos(phi);//x座標
			p[0][1] = - r0 * sin(phi);//y座標
			p[1][0] = - r1 * cos(phi);//x座標
			p[1][1] = - r1 * sin(phi);//y座標
			//法線ベクトル
			n[0] = -nxy * cos(phi);
			n[1] = -nxy * sin(phi);
			n[2] =  nz;

			s *= nRepeatS;

			glTexCoord2d(s, t0);//テクスチャ座標
			glNormal3dv(n);//法線ベクトル
			glVertex3dv(p[0]);//頂点座標

			glTexCoord2d(s, t1);//テクスチャ座標
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
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
			//s座標
			s = (double)i / (double)nSlice;
			phi = 2.0 * M_PI * s;
			//頂点のxy座標(i=0を真後ろ)
			p[0][0] = - r0 * cos(phi);//x座標
			p[0][1] = - r0 * sin(phi);//y座標
			p[1][0] = - r1 * cos(phi);//x座標
			p[1][1] = - r1 * sin(phi);//y座標

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
void drawTexPrism1(float rBottom, float rTop, float height, int nSlice, int nStack)
{
	//側面全体に円筒投影
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

		//上底(Top)
	glBegin(GL_TRIANGLES);
		glNormal3f(0.0f,0.0f,1.0f);
		for(i = 0; i < nSlice; i++) 
		{ 
			ii = i + 1;
			if(ii == nSlice) ii = 0;
			glVertex3f(0.0f, 0.0f, height/2.0f);
			glVertex3fv(p[i][0]);
			glVertex3fv(p[ii][0]);
		}
	glEnd();
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
	//方向余弦
	double nz, nxy, rr;
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

	//円筒投影
	//側面一周にﾏｯﾋﾟﾝｸﾞ
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	float s1, t1, s2, t2;//ﾃｸｽﾁｬ座標
	for(j=0; j<nStack; j++)
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			s1 = (float)i / (float)nSlice; s2 = (float)(i+1) / (float)nSlice;
			t1 = 1.0f -(float)j / (float)nStack; t2 = 1.0f -(float)(j+1) / (float)nStack;
			glNormal3fv(n[i]);
			glTexCoord2f(s1, t1); glVertex3fv(p[i][j]);
			glTexCoord2f(s1, t2); glVertex3fv(p[i][j+1]);
			glTexCoord2f(s2, t2); glVertex3fv(p[ii][j+1]); 
			glTexCoord2f(s2, t1); glVertex3fv(p[ii][j]);
		}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//-------------------------------------------------------------------------------------
void drawTexPrism2(float rBottom, float rTop, float height, float thick, int nSlice, int nStack)
{
	//厚みのある中空円柱(外側側面に円筒投影)
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
	//方向余弦
	for(i=0; i<nSlice;i++)
	{//内側法線方向の逆方向
		n[i][0] = -n[i][0];
		n[i][1] = -n[i][1];
		n[i][2] = -n[i][2];
	}
	//円筒投影
	//側面一周にﾏｯﾋﾟﾝｸﾞ
	float s1, t1, s2, t2;
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for(j=0; j<nStack; j++)
	{
		for(i=0;i<nSlice;i++)
		{
			ii = i+1;
			if(ii == nSlice) ii = 0;
			s1 = (float)i / (float)nSlice; s2 = (float)(i+1) / (float)nSlice;
			t1 = 1.0f -(float)j / (float)nStack; t2 = 1.0f -(float)(j+1) / (float)nStack;
			glNormal3fv(n[i]);
			glTexCoord2f(s1, t1); glVertex3fv(p[j*nSlice+i]);
			glTexCoord2f(s1, t2); glVertex3fv(p[j*nSlice+i+nSlice]);
			glTexCoord2f(s2, t2); glVertex3fv(p[j*nSlice+ii+nSlice]); 
			glTexCoord2f(s2, t1); glVertex3fv(p[j*nSlice+ii]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);
	//上底
	for(i=0;i<nSlice;i++)
	{
		ii = i+1;
		if(ii == nSlice) ii = 0;
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3fv(p[i]); glVertex3fv(p[ii]);
		glVertex3fv(p[N+ii]); glVertex3fv(p[N+i]);
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

void drawTexTorus(double radius1, double radius2, 
									int nSide, int nRing, int nRepeatS, int nRepeatT)
{	
	//radius1:円筒断面半径
	//radius2:円筒の中心軸半径
	//nSide:円筒断面における表面分割点数
	//nRing:円筒中心軸に沿った分割点数

	int i, j;
	double rr, zz;
	double s0, s1, t, phi0, phi1, theta;
	double p[2][3];

	for(i = 0; i < nRing; i++)
	{
		//i=0は基本断面(x=radius2を中心とする円, y=0）
		//2つのｓ座標
		s0 = (double)i / (double)nRing;
		s1 = (double)(i+1) / (double)nRing;
		phi0 = 2.0 * M_PI * s0;
		phi1 = 2.0 * M_PI * s1;

		//s座標
		s0 = s0 * (double)nRepeatS;
		s1 = s1 * (double)nRepeatS;

		glEnable(GL_TEXTURE_2D);
		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nSide; j++)
		{
			//t座標
			t = (double)j / (double)nSide;
			theta = -M_PI + 2.0 * M_PI * t;
			rr = radius2 + radius1 * cos(theta);//z軸からの距離
			zz = radius1 * sin(theta);
			//頂点のxyz座標(j=0を内側xy平面)
			p[0][0] = rr * cos(phi0);//x座標
			p[0][1] = rr * sin(phi0);//y
			p[0][2] = zz;            //z
			p[1][0] = rr * cos(phi1);//x座標
			p[1][1] = rr * sin(phi1);//y
			p[1][2] = zz;            //z      

		  t = (1.0 - t) * (double)nRepeatT;

			glTexCoord2d(s0, t);//テクスチャ座標
			glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
			glVertex3dv(p[0]);//頂点座標

			glTexCoord2d(s1, t);//テクスチャ座標
			glNormal3d(cos(theta)*cos(phi1),cos(theta)*sin(phi1),sin(theta));
			glVertex3dv(p[1]);//頂点座標
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
}
//---------------------------------------------------------------------------------------
void drawTexSpring(float radius1, float radius2, float len, 
					int nSide, int nRing, int nPitch, int nRepeatS, int nRepeatT)
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
	double s0, s1, t, phi0, phi1, theta;
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
			//2つのｓ座標
			s0 = (double)i / (double)nRing;
			s1 = (double)(i+1) / (double)nRing;
			phi0 = 2.0 * M_PI * s0;
			phi1 = 2.0 * M_PI * s1;

			//s座標
			s0 = s0 * (double)nRepeatS;
			s1 = s1 * (double)nRepeatS;

			phi0 = 2.0 * M_PI * (double)i / (double)nRing;
			phi1 = 2.0 * M_PI * (double)(i+1) / (double)nRing;

			glBegin(GL_QUAD_STRIP);
			for(j = 0; j <= nSide; j++)
			{
				//t座標
				t = (double)j / (double)nSide;
				theta = -M_PI + 2.0 * M_PI * t;
				rr = radius2 + radius1 * cos(theta);//z軸からの距離
				zz0 = radius1 * sin(theta) + dp * (double)i + pitch * (double)k;
				zz1 = radius1 * sin(theta) + dp * (double)(i+1) + pitch * (double)k;
				//頂点のxyz座標(j=0を内側xy平面)
				p[0][0] = rr * cos(phi0);//x座標
				p[0][1] = rr * sin(phi0);//y
				p[0][2] = zz0;           //z
				p[1][0] = rr * cos(phi1);//x座標
				p[1][1] = rr * sin(phi1);//y
				p[1][2] = zz1;           //z      

				t = (1.0 - t) * (double)nRepeatT;
				glTexCoord2d(s0, t);//テクスチャ座標
				glNormal3d(cos(theta)*cos(phi0),cos(theta)*sin(phi0),sin(theta));
				glVertex3dv(p[0]);//頂点座標

				glTexCoord2d(s1, t);//テクスチャ座標
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
void drawTexSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//上下の中心が原点
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	float s1,t1,s2,t2;
	double ct,phi,theta,z,cc;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	for(j = 0 ;j <= nStack;j++)
	{
		phi = (M_PI/(double)nStack) * ((double)nStack / 2.0 - (double)j);
		if(phi >= 0.0) //上半分
		{
			if(phi == 0.0) z = 0.0;//pow(a,b)のaがa<=0.0でエラー
			else z = pow(sin(fabs(phi)),eps1);//z
		}
		else  //下半分        
		{
			z = - pow(sin(fabs(phi)), eps1);
		}
		for (i = 0 ;i <= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//自分から見て左側
			k2 = nSlice * j + nSlice - i;//右側
			theta = 2.0 * M_PI * (double)i/(double)nSlice;
			ct = cos(theta);
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
				pd[k1][0] = r * (float)(pow(cos(phi),eps1)*cc);
				if(sin(theta) == 0.0) pd[k1][1] = 0.0f;
				else pd[k1][1] = r * (float)(pow(cos(phi),eps1)*pow(fabs(sin(theta)),eps2));
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
	//全体にﾏｯﾋﾟﾝｸﾞ
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for(i = 0;i < nSlice;i++)
	{
		ip = i + 1;
		if(ip == nSlice) ip = 0;
		s1 = (float)i / (float)nSlice;//2πで正規化した角度
		s2 = (float)(i+1) / (float)nSlice;
		//if(i == nSlice-1) s2 = 1.0f;
		for(j = 0;j < nStack; j++)
		{
			t1 = 1.0f - (float)j / (float)nStack;//j=0で1となるように
			t2 = 1.0f - (float)(j+1) / (float)nStack;

			//左端(p1[1]=-0.5)でﾃｸｽﾁｬｰ座標が0となるようにする
			glNormal3d(a[i][j],b[i][j],c[i][j]);
			glTexCoord2f(s1, t1);glVertex3fv(pd[i+j*nSlice]);
			glNormal3d(a[i][j+1],b[i][j+1],c[i][j+1]);
			glTexCoord2f(s1, t2);glVertex3fv(pd[i+(j+1)*nSlice]);
			glNormal3d(a[ip][j+1],b[ip][j+1],c[ip][j+1]);
			glTexCoord2f(s2, t2);glVertex3fv(pd[ip+(j+1)*nSlice]);
			glNormal3d(a[ip][j],b[ip][j],c[ip][j]);
			glTexCoord2f(s2, t1);glVertex3fv(pd[ip +j*nSlice]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
//上半分の超2次関数
void drawTexHemiSuper(float r, int nSlice, int nStack, double eps1, double eps2)
{
	//底の中心が原点
	int i,j,ip,im,np,npL,npR,npU,npD,k1,k2;
	float s1, t1, s2, t2;
	double ct,phi,theta,z,cc;
	double e1, e2;
	float a[31][31], b[31][31], c[31][31];
	float n1[3], n2[3], n3[3], n4[3];
	float pd[31*31][3];

	if(nSlice > 30) nSlice = 30;
	if(nStack > 30) nStack = 30;

	//上半分だけ
	for(j = 0 ;j <= nStack ;j++)
	{
		phi = (M_PI/(double)nStack) * ((double)nStack  - (double)j)/2.0;
		e1 = fabs(sin(phi));
		if( e1 == 0.0) z = 0.0;
		else z = (float)(pow(e1, eps1));//z
		for (i = 0 ;i<= nSlice / 2;i++)
		{
			k1 = nSlice * j + i;//外から見て右側
			k2 = nSlice * j + nSlice - i;//左側
			theta = 2.0*M_PI * (double)i / (double)nSlice;
			ct = cos(theta);
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
				e1 = fabs(cos(phi));
				if( e1 == 0.0) 
				{
					pd[k1][0] = 0.0f;
					pd[k1][1] = 0.0f;
				}
				else
				{
					pd[k1][0] = r * (float)(pow(e1,eps1) * cc);
					e2 = fabs(sin(theta));
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

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	for(i = 0;i < nSlice;i++)
	{
		ip = i + 1; 
		if(ip == nSlice) ip = 0;
		s1 = (float)i / (float)nSlice;//ﾃｸｽﾁｬ座標
		s2 = (float)(i+1) / (float)nSlice;
		//if(i == nSlice-1) s2 = 1.0f;
		for(j = 0;j < nStack; j++)
		{
			t1 = 1.0f - (float)j / (float)nStack;//j=0で1となるように
			t2 = 1.0f - (float)(j+1) / (float)nStack;
			//左端(p1[1]=-0.5)でﾃｸｽﾁｬｰ座標が0となるようにする
			glNormal3d(a[i][j],b[i][j],c[i][j]);
			glTexCoord2f(s1, t1);glVertex3fv(pd[i+j*nSlice]);
			glNormal3d(a[i][j+1],b[i][j+1],c[i][j+1]);
			glTexCoord2f(s1, t2);glVertex3fv(pd[i+(j+1)*nSlice]);
			glNormal3d(a[ip][j+1],b[ip][j+1],c[ip][j+1]);
			glTexCoord2f(s2, t2);glVertex3fv(pd[ip+(j+1)*nSlice]);
			glNormal3d(a[ip][j],b[ip][j],c[ip][j]);
			glTexCoord2f(s2, t1);glVertex3fv(pd[ip +j*nSlice]);
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//底
	j = nStack;
	glBegin(GL_POLYGON);
	glNormal3f(0.0f,0.0f,-1.0f);//-z方向
	for(i = nSlice-1;i >= 0;i--) glVertex3fv(pd[i+j*nSlice]);
	glEnd();
}

