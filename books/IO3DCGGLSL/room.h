//room.h

void drawRoom(double widthX, double widthZ, double height);
void drawFloor0(double widthX, double widthZ, int nx, int nz);
void drawCeiling(double widthX, double widthZ, double height);
void drawWall_nx(double widthX, double widthZ, double height);//左側
void drawWall_px(double widthX, double widthZ, double height);//右側
void drawWall_nz(double widthX, double widthZ, double height);//奥
void drawWall_pz(double widthX, double widthZ, double height);//手前
void drawDoor(double w, double h);//ドア
void drawKnob(double radius, double len);//取っ手
void drawWindow(double w, double h, double d);
void drawBlackboard(double w, double h, double d);
void drawTable(double wx, double wz, double h);
void drawChair(double wx, double wz, double h);

void drawRoom(double widthX, double widthZ, double height)
{
	drawWall_nx( widthX, widthZ, height);//左側
	drawWall_px( widthX, widthZ, height);//右側
	drawWall_nz( widthX, widthZ, height);//奥
	drawWall_pz( widthX, widthZ, height);//手前
	drawFloor0(widthX, widthZ, 8, 8);
	drawCeiling(widthX, widthZ, height);
	//テーブル１
	glPushMatrix();
	glTranslated(-widthX/2+4.0, 0.0, -1.0);
	drawTable(0.9, 1.8, 0.8);
	glPopMatrix();
	//テーブル２
	glPushMatrix();
	glTranslated(0.0, 0.0, -widthZ/2 + 0.5);
	drawTable(2.0, 1.0, 0.8);
	glPopMatrix();
	//テーブル３
	glPushMatrix();
	glTranslated(3.0, 0.0, widthZ/2 - 1.5);
	drawTable(1.0, 1.0, 0.8);
	glPopMatrix();
	//椅子１
	glPushMatrix();
	glTranslated( -widthX/2+4.5, 0.0, -1.0);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	drawChair(0.4, 0.4, 0.4);
	glPopMatrix();
	//椅子2
	glPushMatrix();
	glTranslated( 1.0, 0.0, -widthZ/2 + 0.5);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	drawChair(0.4, 0.4, 0.4);
	glPopMatrix();
}

void drawFloor0(double widthX, double widthZ, int nx, int nz)
{
	int i, j;
	//Floor１枚当たりの幅
	double wX = widthX / (double)nx;
	double wZ = widthZ / (double)nz;

	static float diffuse[][4] = {
		{ 0.9f, 0.8f, 0.7f, 1.0f}, { 0.4f, 0.4f, 0.5f} };
	static float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	static float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	
//	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,100);

	glNormal3d(0.0, 1.0, 0.0);
	glPushMatrix();
	glTranslated(-widthX / 2.0, 0.0, -widthZ / 2.0);
  glBegin(GL_QUADS);
  for (j = 0; j < nz; j++) {
    double z1 = wZ * j; double z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      double x1 = wX * i; double x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
      glVertex3d(x1, 0.0, z1);
      glVertex3d(x1, 0.0, z2);
      glVertex3d(x2, 0.0, z2);
      glVertex3d(x2, 0.0, z1);
    }
  }
  glEnd();
	glPopMatrix();
}

void drawCeiling(double widthX, double widthZ, double height)
{

	static float diffuse[4] = { 0.9f, 0.9f, 0.8f, 1.0f};
	static float ambient[] = { 0.5f, 0.5f, 0.4f, 1.0f};
	static float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,100);

	glNormal3d(0.0, -1.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3d(-widthX / 2.0, height,  -widthZ / 2.0);
    glVertex3d(-widthX / 2.0, height,   widthZ / 2.0);
    glVertex3d( widthX / 2.0, height,   widthZ / 2.0);
    glVertex3d( widthX / 2.0, height,  -widthZ / 2.0);
  glEnd();

	//見かけのライト
	static float diffuseL[4] = { 1.0f, 0.7f, 0.8f, 1.0f};
	static float ambientL[] = { 0.9f, 0.6f, 0.6f, 1.0f};
	static float specularL[]= { 1.0f, 1.0f, 1.0f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseL);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientL);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularL);
	glMaterialf(GL_FRONT,GL_SHININESS,100);

	//Light1(中央）
	glPushMatrix();
	glTranslated(0.0, height-0.05, 0.0);
	glScaled(5.0, 0.1, 1.0);
	drawCube(0.5);
	glPopMatrix();	
	//Light2(手前）
	glPushMatrix();
	glTranslated(0.0, height-0.05, 0.3*widthZ);
	glScaled(5.0, 0.1, 1.0);
	drawCube(0.5);
	glPopMatrix();	
	//Light3
	glPushMatrix();
	glTranslated(0.0, height-0.05, -0.3*widthZ);
	glScaled(5.0, 0.1, 1.0);
	drawCube(0.5);
	glPopMatrix();	
  
}

void drawWall_nx(double widthX, double widthZ, double height)
{

	static float diffuse[4] = { 0.5f, 0.6f, 0.9f, 1.0f};
	static float ambient[] = { 0.2f, 0.3f, 0.5f, 1.0f};
	static float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,10);

	glNormal3d(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3d(-widthX / 2.0, 0, widthZ / 2.0);
    glVertex3d(-widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3d(-widthX / 2.0, height, -widthZ / 2.0);
    glVertex3d(-widthX / 2.0, height, widthZ / 2.0);
  glEnd();

	//door1
	double w = 1.5;
	double h = 2.8;
	glPushMatrix();
	glTranslated(-widthX / 2.0 + 0.01, h / 2.0, -widthZ / 2.0 + w / 2.0 + 1.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	drawDoor(w, h);
	glPopMatrix();	
	//door2
	glPushMatrix();
	glTranslated(-widthX / 2.0 + 0.01, h / 2.0, widthZ / 2.0 - w / 2.0 - 0.2);
	glRotated(90.0, 0.0, 1.0, 0.0);
	drawDoor(w, h);
	glPopMatrix();	

}

void drawWall_px(double widthX, double widthZ, double height)
{

	static float diffuse[4] = { 0.8, 0.5f, 0.4f, 1.0f};
	static float ambient[] = { 0.4f, 0.3f, 0.2f, 1.0f};
	static float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,10);

	glNormal3d(-1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3d( widthX / 2.0, 0, widthZ / 2.0);
    glVertex3d( widthX / 2.0, height, widthZ / 2.0);
    glVertex3d( widthX / 2.0, height, -widthZ / 2.0);
    glVertex3d( widthX / 2.0, 0, -widthZ / 2.0);
  glEnd();

	//窓
	glPushMatrix();
	glTranslated(widthX / 2.0, 0.6 * height, 0.0);
	glRotated(-90.0, 0.0, 1.0, 0.0);
	drawWindow(5.0, 0.4 * height, 0.02);
	glPopMatrix();

}

void drawWall_nz(double widthX, double widthZ, double height)
{//奥

	static float diffuse[4] = { 0.6f, 0.8f, 0.8f, 1.0f};
	static float ambient[] = { 0.5f, 0.4f, 0.4f, 1.0f};
	static float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,10);

	glNormal3d(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3d(-widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3d( widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3d( widthX / 2.0, height, -widthZ / 2.0);
    glVertex3d(-widthX / 2.0, height, -widthZ / 2.0);
  glEnd();

	//door
	double w = 2.0;
	double h = 2.8;
	glPushMatrix();
	glTranslated(widthX / 2.0 - w/2.0-0.5, h / 2.0, -widthZ / 2.0 );
	//glRotated(90.0, 0.0, 1.0, 0.0);
	drawDoor(w, h);
	glPopMatrix();	
	//黒板
	glPushMatrix();
	glTranslated(-2.0, 0.6 * height, -widthZ / 2.0);
//	glRotated(0.0, 0.0, 1.0, 0.0);
	drawBlackboard(0.4*widthX, 0.6 * height, 0.02);
	glPopMatrix();


}

void drawWall_pz(double widthX, double widthZ, double height)
{//手前

	static float diffuse[4] = { 0.8, 0.5f, 0.6f, 1.0f};
	static float ambient[] = { 0.4f, 0.3f, 0.2f, 1.0f};
	static float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,10);

	glNormal3d(-1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3d( -widthX / 2.0, 0, widthZ / 2.0);
    glVertex3d( -widthX / 2.0, height, widthZ / 2.0);
    glVertex3d( widthX / 2.0, height, widthZ / 2.0);
    glVertex3d( widthX / 2.0, 0, widthZ / 2.0);
  glEnd();

	//黒板
	glPushMatrix();
	glTranslated(0.0, 0.6 * height, widthZ / 2.0);
//	glRotated(0.0, 0.0, 1.0, 0.0);
	drawBlackboard(0.7*widthX, 0.6 * height, 0.02);
	glPopMatrix();

}

void drawDoor(double w, double h)
{
	static float diffuseD[] = { 0.6f, 0.4f, 0.3f, 1.0f};
	static float ambientD[] = { 0.4f, 0.2f, 0.1f, 1.0f};
	static float specularD[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseD);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientD);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularD);
	glMaterialf(GL_FRONT,GL_SHININESS,10);

	glPushMatrix();
	glScaled( w, h, 0.03);
	drawCube(1.0);
	glPopMatrix();	

	//窓
	glPushMatrix();
	glTranslated(0.0, 0.2 * h, 0.0);
	drawWindow(0.4 * w, 0.4 * h, 0.04);
	glPopMatrix();
	//取っ手
	glPushMatrix();
	glTranslated(0.45*w, 0.0, 0.0);
	drawKnob(0.05, 0.15);
	glPopMatrix();

}

void drawKnob(double radius, double len)
{
	static float diffuseD[4] = { 0.9f, 0.9f, 0.3f, 1.0f};
	static float ambientD[] = { 0.5f, 0.5f, 0.1f, 1.0f};
	static float specularD[]= { 0.5f, 0.5f, 0.5f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseD);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientD);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularD);
	glMaterialf(GL_FRONT,GL_SHININESS,100);

	drawCylinder1( radius, radius*1.5, len, 6, 3);
}

void drawWindow(double w, double h, double d)
{
	//x-y平面
	static float diffuseW[] = { 0.7f, 0.8f, 1.0f, 1.0f};
	static float ambientW[] = { 0.7f, 0.8f, 1.0f, 1.0f};
	static float specularW[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseW);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientW);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularW);
	glMaterialf(GL_FRONT,GL_SHININESS,100);
	glPushMatrix();
	glScaled(w, h, d);
	drawCube(1.0);
	glPopMatrix();
	//frame
	static float diffuseF[] = { 0.2f, 0.4f, 0.4f, 1.0f};
	static float ambientF[] = { 0.1f, 0.2f, 0.2f, 1.0f};
	static float specularF[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseF);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientF);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularF);
	glMaterialf(GL_FRONT,GL_SHININESS,10);
	//frame1
	glPushMatrix();
	glTranslated(0.0, h/2.0, 0.0);
	glScaled(w, 0.05, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame2
	glPushMatrix();
	glTranslated(0.0, -h/2.0, 0.0);
	glScaled(w, 0.05, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame3
	glPushMatrix();
	glTranslated(w/2.0, 0.0, 0.0);
	glScaled(0.05, h, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame4
	glPushMatrix();
	glTranslated(-w/2.0, 0.0, 0.0);
	glScaled(0.05, h, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
}

void drawBlackboard(double w, double h, double d)
{
	//x-y平面
	static float diffuseB[] = { 0.2f, 0.5f, 0.6f, 1.0f};
	static float ambientB[] = { 0.1f, 0.3f, 0.4f, 1.0f};
	static float specularB[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseB);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientB);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularB);
	glMaterialf(GL_FRONT,GL_SHININESS,10);
	glPushMatrix();
	glScaled(w, h, d);
	drawCube(1.0);
	glPopMatrix();
	//frame
	static float diffuseF[] = { 0.6f, 0.4f, 0.2f, 1.0f};
	static float ambientF[] = { 0.3f, 0.2f, 0.1f, 1.0f};
	static float specularF[]= { 0.2f, 0.2f, 0.2f, 1.0f};
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseF);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambientF);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularF);
	glMaterialf(GL_FRONT,GL_SHININESS,10);
	//frame1
	glPushMatrix();
	glTranslated(0.0, h/2.0, 0.0);
	glScaled(w, 0.05, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame2
	glPushMatrix();
	glTranslated(0.0, -h/2.0, 0.0);
	glScaled(w, 0.05, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame3
	glPushMatrix();
	glTranslated(w/2.0, 0.0, 0.0);
	glScaled(0.05, h, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
	//frame4
	glPushMatrix();
	glTranslated(-w/2.0, 0.0, 0.0);
	glScaled(0.05, h, d + 0.04);
	drawCube(1.0);
	glPopMatrix();
}

void drawTable(double wx, double wz, double h)
{
	double thick = 0.1;
	static float diffuse[4] = { 0.9f, 0.5f, 0.3f, 1.0f};
	static float ambient[] = { 0.5f, 0.3f, 0.1f, 1.0f};
	static float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,100);
	glPushMatrix();
	glTranslated(0.0, h, 0.0);
	glScaled(wx, thick, wz);
	drawCube(1.0);
	glPopMatrix();
	//脚1
	glPushMatrix();
	glTranslated(wx/2.0 - thick, h/2.0, wz/2.0 - thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚2
	glPushMatrix();
	glTranslated(wx/2.0 - thick, h/2.0, -wz/2.0 + thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚3
	glPushMatrix();
	glTranslated(-wx/2.0 + thick, h/2.0, wz/2.0 - thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚4
	glPushMatrix();
	glTranslated(-wx/2.0 + thick, h/2.0, -wz/2.0 + thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
}

void drawChair(double wx, double wz, double h)
{
	double thick = 0.04;
	static float diffuse[4] = { 0.9f, 0.3f, 0.3f, 1.0f};
	static float ambient[] = { 0.5f, 0.1f, 0.1f, 1.0f};
	static float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};
	
	glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
	glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
	glMaterialf(GL_FRONT,GL_SHININESS,100);
	glPushMatrix();
	glTranslated(0.0, h, 0.0);
	glScaled(wx, thick, wz);
	drawCube(1.0);
	glPopMatrix();
	//脚1
	glPushMatrix();
	glTranslated(wx/2.0 - thick, h/2.0, wz/2.0 - thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚2
	glPushMatrix();
	glTranslated(wx/2.0 - thick, h/2.0, -wz/2.0 + thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚3
	glPushMatrix();
	glTranslated(-wx/2.0 + thick, h/2.0, wz/2.0 - thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//脚4
	glPushMatrix();
	glTranslated(-wx/2.0 + thick, h/2.0, -wz/2.0 + thick);
	glScaled(thick, h, thick);
	drawCube(1.0);
	glPopMatrix();
	//背もたれ
	glPushMatrix();
	glTranslated( 0.0, 1.5 * h, -0.6 * wz);
	glRotated(-10.0, 1.0, 0.0, 0.0); 
	glScaled( wx, 1.2 * h, thick/2.0);
	drawCube(1.0);
	glPopMatrix();

}
