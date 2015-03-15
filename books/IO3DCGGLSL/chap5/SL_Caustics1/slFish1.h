//myFish1.h
//仮想魚

class CFish1
{
public:
	CFish1(void);
	~CFish1(void) { };
	//メンバ変数
	float* diffuse ;//拡散光反射率
	float* specular;//鏡面光反射率
	float* ambient; //環境光反射率
	float highlight;//光沢

	//位置
  float pos[3];
  //姿勢角
  float ang[3];
	float radius0;//円運動の半径
	float period0;//一周回転の周期
	float angSwing;//胸鰭の振動角度（左右対称）
	float maxAngSwing;//その最大角度
	float periodSwing;//胸鰭の振動周期

	//胴体
	float trunk1[3];
	float trunk2[3];
	//メンバ関数
	void draw();
	void motion1(float t);
private:
	void drawTrunk();
	void drawTaile();
	void drawEyes();
	void drawMouth();
};

//----------------------------------------------------------------
CFish1::CFish1()
{

	static float diffuse0[] = {0.9f, 0.8f, 0.2f, 1.0f};
  static float ambient0[] = {0.5f, 0.4f, 0.1f, 1.0f};
	static float specular0[] = {0.4f, 0.4f, 0.9f, 1.0f};

	//仮想動物のマテリアル特性
	diffuse = diffuse0;
  ambient = ambient0;
  specular = specular0;
  highlight = 100.0f;

	pos[0] = -1.0; pos[1] = 1.0; pos[2] = 0.0;
  ang[0] = 0.0; ang[1] = 0.0; ang[2] = 0.0;
	//胸鰭の角度（左右対称）
	angSwing = 0.0;
	radius0 = 2.0;
	period0 = 10.0;//一周回転の周期
	periodSwing = 2.0;//胸鰭の周期
	maxAngSwing = 20.0;//胸鰭の最大角度

  //胴体
	trunk1[0] = 0.5; trunk1[1] = 0.1; trunk1[2] = 0.5;
	trunk2[0] = 0.5; trunk2[1] = 0.1; trunk2[2] = 0.6;
}

//------------------------------------------------------------------
void CFish1::draw()
{
	setTextureMatrix();
	//テクスチャ座標のためのモデリング変換
  glTranslatef(pos[0], pos[1], pos[2]);
  //体全体の姿勢
  glRotatef(ang[1], 0.0, 1.0, 0.0); //y軸回転
  glRotatef(ang[0], 1.0, 0.0, 0.0); //x軸回転
  glRotatef(ang[2], 0.0, 0.0, 1.0); //z軸回転
	//通常のモデルビュー変換に戻す 
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
  //現在位置
  glTranslatef(pos[0], pos[1], pos[2]);
  //体全体の姿勢
  glRotatef(ang[1], 0.0, 1.0, 0.0); //y軸回転
  glRotatef(ang[0], 1.0, 0.0, 0.0); //x軸回転
  glRotatef(ang[2], 0.0, 0.0, 1.0); //z軸回転
	if(flagShadow) 
	{
		//影のときは固定シェーダー
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
	}
	else
	{
	  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	  glMaterialf(GL_FRONT,GL_SHININESS,100.0);
	}

  //胴体
	drawSuper(trunk1, trunk2, 20, 20, 1.5, 1.5, 0.8, angSwing, 5, 0);
	
  drawTaile();
	drawEyes();
	drawMouth();
	glPopMatrix();
}

void CFish1::drawEyes()
{
	float eyeDif[] = { 0.3, 0.3, 0.6, 1.0 };
	float eyeAmb[] = { 0.1, 0.1, 0.4, 1.0};
	float eyeSpe[] = { 0.1, 0.1, 0.4, 1.0};
	//左目
	setTextureMatrix();
	//テクスチャ座標のためのモデリング変換
  glTranslatef(pos[0], pos[1], pos[2]);
	//通常のモデルビュー変換に戻す 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(trunk1[0]/3.0, trunk1[1] /2.0, trunk1[2] * 0.8);//4.0/ 5.0);
		if(flagShadow) 
		{
			//影のときは固定シェーダー
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, eyeDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, eyeAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, eyeSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 50.0);
		}
		drawSphere(0.06, 8, 8);
	glPopMatrix();
	//右目
	setTextureMatrix();
	//テクスチャ座標のためのモデリング変換
  glTranslatef(pos[0], pos[1], pos[2]);
	//通常のモデルビュー変換に戻す 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(-trunk1[0]/3.0, trunk1[1] /2.0, trunk1[2] * 4.0/ 5.0);
		if(flagShadow) 
		{
			//影のときは固定シェーダー
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, eyeDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, eyeAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, eyeSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 50.0);
		}
		drawSphere(0.06, 8, 8);
	glPopMatrix();
}

void CFish1::drawMouth()
{
	float mouthDif[] = { 0.5, 0.5, 0.2, 1.0};
	float mouthAmb[] = { 0.3, 0.3, 0.1, 1.0};
	float mouthSpe[] = { 0.3, 0.3, 0.3, 1.0};
	setTextureMatrix();
	//テクスチャ座標のためのモデリング変換
  glTranslatef(pos[0], pos[1], pos[2]);
	//通常のモデルビュー変換に戻す 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.0, -trunk1[1] * 3.0/5.0, trunk1[2]* 3.0/ 5.0);
		glScalef(2.0, 0.2, 1.0);
		if(flagShadow) 
		{
			//影のときは固定シェーダー
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, mouthDif);
			glMaterialfv(GL_FRONT,GL_AMBIENT, mouthAmb);
			glMaterialfv(GL_FRONT,GL_SPECULAR, mouthSpe);
			glMaterialf(GL_FRONT,GL_SHININESS, 20.0);
		}
		drawSphere(0.1, 8, 8);
	glPopMatrix();
}

void CFish1::drawTaile()
{
	setTextureMatrix();
	//テクスチャ座標のためのモデリング変換
  glTranslatef(pos[0], pos[1], pos[2]);
	//通常のモデルビュー変換に戻す 
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.0, 0.0, -trunk2[2] * 3.2/ 2.0);
		if(flagShadow) 
		{
			//影のときは固定シェーダー
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,shadowDiffuse);
			glMaterialfv(GL_FRONT,GL_SPECULAR,shadowSpecular);
		}
		else
		{
			glMaterialfv(GL_FRONT,GL_DIFFUSE, diffuse);
			glMaterialfv(GL_FRONT,GL_AMBIENT, ambient);
			glMaterialfv(GL_FRONT,GL_SPECULAR, specular);
			glMaterialf(GL_FRONT,GL_SHININESS, 20.0);
		}
		drawCylinder1(0.01, 0.05, 1.0, 8, 8);
	glPopMatrix();
}

void CFish1::motion1(float t)
{
	//左回り回転
	double theta = 360.0 * t / period0 + 180.0;
	pos[0] = radius0 * sin( M_PI * theta / 180.0);
	pos[2] = radius0 * cos( M_PI * theta / 180.0);
	ang[1] = theta + 90.0;//方向
	ang[2] = -20.0;
	angSwing = maxAngSwing * sin(2.0 * M_PI * t / periodSwing);
	draw();
}