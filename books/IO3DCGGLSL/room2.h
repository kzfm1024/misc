//room2.h

void drawRoom(float widthX, float widthZ, float height);
void drawFloor0(float widthX, float widthZ, int nx, int nz);
void drawCeiling(float widthX, float widthZ, float height);
void drawWall_nx(float widthX, float widthZ, float height);//左側
void drawWall_px(float widthX, float widthZ, float height);//右側
void drawWall_nz(float widthX, float widthZ, float height);//奥
void drawWall_pz(float widthX, float widthZ, float height);//手前
void drawDoor(float w, float h);//ドア
void drawKnob(float radius, float len);//取っ手
void drawWindow(float w, float h, float d);
void drawBlackboard(float w, float h, float d);
void drawTable(float wx, float wz, float h);
void drawChair(float wx, float wz, float h);

void drawRoom(float widthX, float widthZ, float height)
{
	drawWall_nx( widthX, widthZ, height);//左側
	drawWall_px( widthX, widthZ, height);//右側
	drawWall_nz( widthX, widthZ, height);//奥
	drawWall_pz( widthX, widthZ, height);//手前
	drawFloor0(widthX, widthZ, 8, 8);
	drawCeiling(widthX, widthZ, height);
	//テーブル１
	glPushMatrix();
	glTranslatef(-widthX/2+1.0, 0.0, -1.0);
	drawTable(0.9, 1.8, 0.8);
	glPopMatrix();
	//テーブル２
	glPushMatrix();
	glTranslatef(0.0, 0.0, -widthZ/2 + 0.5);
	drawTable(2.0, 1.0, 0.8);
	glPopMatrix();
	//テーブル３
	glPushMatrix();
	glTranslatef(3.0, 0.0, widthZ/2 - 1.5);
	drawTable(1.0, 1.0, 0.8);
	glPopMatrix();
	//椅子１
	glPushMatrix();
	glTranslatef( -widthX/2+1.5, 0.0, -1.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawChair(0.4, 0.4, 0.4);
	glPopMatrix();
	//椅子2
	glPushMatrix();
	glTranslatef( 1.0, 0.0, -widthZ/2 + 0.5);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	drawChair(0.4, 0.4, 0.4);
	glPopMatrix();
}

void drawFloor0(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor１枚当たりの幅
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	  { 0.9f, 0.8f, 0.7f, 1.0f}, { 0.4f, 0.4f, 0.5f} };
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glNormal3f(0.0, 1.0, 0.0);
  glPushMatrix();
  glTranslatef(-widthX / 2.0, 0.0, -widthZ / 2.0);
  glBegin(GL_QUADS);
  for (j = 0; j < nz; j++) {
    float z1 = wZ * j; float z2 = z1 + wZ;
    for (i = 0; i < nx; i++) {
      float x1 = wX * i; float x2 = x1 + wX;

      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse[(i + j) & 1]);
      glVertex3f(x1, 0.0, z1);
      glVertex3f(x1, 0.0, z2);
      glVertex3f(x2, 0.0, z2);
      glVertex3f(x2, 0.0, z1);
    }
  }
  glEnd();
	glPopMatrix();
}

void drawCeiling(float widthX, float widthZ, float height)
{
  float diffuse[4] = { 0.9f, 0.9f, 0.8f, 1.0f};
  float ambient[] = { 0.5f, 0.5f, 0.4f, 1.0f};
  float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  glNormal3f(0.0, -1.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, height,  -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, height,   widthZ / 2.0);
    glVertex3f( widthX / 2.0, height,   widthZ / 2.0);
    glVertex3f( widthX / 2.0, height,  -widthZ / 2.0);
  glEnd();

  //光源オブジェクト
  float diffuseL[] = { 1.0, 1.0, 1.0, 1.0};
  float ambientL[] = { 1.0, 1.0, 1.0, 1.0};
  float specularL[]= { 1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseL);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientL);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularL);
	
  glMaterialf(GL_FRONT,GL_SHININESS,1);

  //Light1
  glPushMatrix();
  glTranslatef(0.0, height-0.05, 0.0);
  glScalef(1.0, 0.1, 1.0);
  glutSolidCube(1.2);
  glPopMatrix();	
  /*	//Light2
  glPushMatrix();
  glTranslatef(0.0, height-0.05, 0.3*widthZ);
  glScalef(5.0, 0.1, 1.0);
  drawCube(0.5);
  glPopMatrix();	
  //Light3
  glPushMatrix();
  glTranslatef(0.0, height-0.05, -0.3*widthZ);
  glScalef(5.0, 0.1, 1.0);
  drawCube(0.5);
  glPopMatrix();	
  */ 
}

void drawWall_nx(float widthX, float widthZ, float height)
{
  float diffuse[4] = { 0.5f, 0.6f, 0.9f, 1.0f};
  float ambient[] = { 0.2f, 0.3f, 0.5f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, 0, widthZ / 2.0);
    glVertex3f(-widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, height, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, height, widthZ / 2.0);
  glEnd();

  //door1
  float w = 1.5;
  float h = 2.8;
  glPushMatrix();
  glTranslatef(-widthX / 2.0 + 0.01, h / 2.0, -widthZ / 2.0 + w / 2.0 + 1.0);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  drawDoor(w, h);
  glPopMatrix();	
  //door2
  glPushMatrix();
  glTranslatef(-widthX / 2.0 + 0.01, h / 2.0, widthZ / 2.0 - w / 2.0 - 0.2);
  glRotatef(90.0, 0.0, 1.0, 0.0);
  drawDoor(w, h);
  glPopMatrix();	

}

void drawWall_px(float widthX, float widthZ, float height)
{

  float diffuse[4] = { 0.8, 0.5f, 0.4f, 1.0f};
  float ambient[] = { 0.4f, 0.3f, 0.2f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(-1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f( widthX / 2.0, 0, widthZ / 2.0);
    glVertex3f( widthX / 2.0, height, widthZ / 2.0);
    glVertex3f( widthX / 2.0, height, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, 0, -widthZ / 2.0);
  glEnd();

  //窓
  glPushMatrix();
  glTranslatef(widthX / 2.0, 0.6 * height, 0.0);
  glRotatef(-90.0, 0.0, 1.0, 0.0);
  drawWindow(5.0, 0.4 * height, 0.02);
  glPopMatrix();
}

void drawWall_nz(float widthX, float widthZ, float height)
{//奥
  float diffuse[4] = { 0.6f, 0.8f, 0.8f, 1.0f};
  float ambient[] = { 0.5f, 0.4f, 0.4f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, height, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, height, -widthZ / 2.0);
  glEnd();

  //door
  float w = 2.0;
  float h = 2.8;
  glPushMatrix();
  glTranslatef(widthX / 2.0 - w/2.0-0.5, h / 2.0, -widthZ / 2.0 );
  //glRotatef(90.0, 0.0, 1.0, 0.0);
  drawDoor(w, h);
  glPopMatrix();	
  //黒板
  glPushMatrix();
  glTranslatef(-2.0, 0.6 * height, -widthZ / 2.0);
  //	glRotatef(0.0, 0.0, 1.0, 0.0);
  drawBlackboard(0.4*widthX, 0.6 * height, 0.02);
  glPopMatrix();
}

void drawWall_pz(float widthX, float widthZ, float height)
{//手前
  float diffuse[4] = { 0.8, 0.8f, 0.6f, 1.0f};
  float ambient[] = { 0.4f, 0.4f, 0.3f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(-1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f( -widthX / 2.0, 0, widthZ / 2.0);
    glVertex3f( -widthX / 2.0, height, widthZ / 2.0);
    glVertex3f( widthX / 2.0, height, widthZ / 2.0);
    glVertex3f( widthX / 2.0, 0, widthZ / 2.0);
  glEnd();

  //黒板
  glPushMatrix();
  glTranslatef(0.0, 0.6 * height, widthZ / 2.0);
  //	glRotatef(0.0, 0.0, 1.0, 0.0);
  drawBlackboard(0.7*widthX, 0.6 * height, 0.02);
  glPopMatrix();
}

void drawDoor(float w, float h)
{
  float diffuseD[] = { 0.6f, 0.4f, 0.3f, 1.0f};
  float ambientD[] = { 0.4f, 0.2f, 0.1f, 1.0f};
  float specularD[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseD);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientD);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularD);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glPushMatrix();
  glScalef( w, h, 0.03);
  drawCube(1.0);
  glPopMatrix();	

  //窓
  glPushMatrix();
  glTranslatef(0.0, 0.2 * h, 0.0);
  drawWindow(0.4 * w, 0.4 * h, 0.04);
  glPopMatrix();
  //取っ手
  glPushMatrix();
  glTranslatef(0.45*w, 0.0, 0.0);
  drawKnob(0.05, 0.15);
  glPopMatrix();
}

void drawKnob(float radius, float len)
{
  float diffuseD[4] = { 0.9f, 0.9f, 0.3f, 1.0f};
  float ambientD[] = { 0.5f, 0.5f, 0.1f, 1.0f};
  float specularD[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseD);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientD);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularD);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  drawCylinder1( radius, radius*1.5, len, 6, 3);
}

void drawWindow(float w, float h, float d)
{
  //x-y平面
  float diffuseW[] = { 0.7f, 0.8f, 1.0f, 1.0f};
  float ambientW[] = { 0.7f, 0.8f, 1.0f, 1.0f};
  float specularW[]= { 0.2f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseW);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientW);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularW);
  glMaterialf(GL_FRONT,GL_SHININESS,100);
  glPushMatrix();
  glScalef(w, h, d);
  drawCube(1.0);
  glPopMatrix();
  //frame
  float diffuseF[] = { 0.2f, 0.4f, 0.4f, 1.0f};
  float ambientF[] = { 0.1f, 0.2f, 0.2f, 1.0f};
  float specularF[]= { 0.2f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseF);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientF);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularF);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  //frame1
  glPushMatrix();
  glTranslatef(0.0, h/2.0, 0.0);
  glScalef(w, 0.05, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame2
  glPushMatrix();
  glTranslatef(0.0, -h/2.0, 0.0);
  glScalef(w, 0.05, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame3
  glPushMatrix();
  glTranslatef(w/2.0, 0.0, 0.0);
  glScalef(0.05, h, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame4
  glPushMatrix();
  glTranslatef(-w/2.0, 0.0, 0.0);
  glScalef(0.05, h, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
}

void drawBlackboard(float w, float h, float d)
{
  //x-y平面
  float diffuseB[] = { 0.2f, 0.5f, 0.6f, 1.0f};
  float ambientB[] = { 0.1f, 0.3f, 0.4f, 1.0f};
  float specularB[]= { 0.2f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseB);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientB);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularB);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  glPushMatrix();
  glScalef(w, h, d);
  drawCube(1.0);
  glPopMatrix();
  //frame
  float diffuseF[] = { 0.6f, 0.4f, 0.2f, 1.0f};
  float ambientF[] = { 0.3f, 0.2f, 0.1f, 1.0f};
  float specularF[]= { 0.2f, 0.2f, 0.2f, 1.0f};
  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseF);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientF);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularF);
  glMaterialf(GL_FRONT,GL_SHININESS,10);
  //frame1
  glPushMatrix();
  glTranslatef(0.0, h/2.0, 0.0);
  glScalef(w, 0.05, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame2
  glPushMatrix();
  glTranslatef(0.0, -h/2.0, 0.0);
  glScalef(w, 0.05, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame3
  glPushMatrix();
  glTranslatef(w/2.0, 0.0, 0.0);
  glScalef(0.05, h, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
  //frame4
  glPushMatrix();
  glTranslatef(-w/2.0, 0.0, 0.0);
  glScalef(0.05, h, d + 0.04);
  drawCube(1.0);
  glPopMatrix();
}

void drawTable(float wx, float wz, float h)
{
  float thick = 0.1;
  float diffuse[4] = { 0.9f, 0.5f, 0.3f, 1.0f};
  float ambient[] = { 0.5f, 0.3f, 0.1f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);
  glPushMatrix();
  glTranslatef(0.0, h, 0.0);
  glScalef(wx, thick, wz);
  drawCube(1.0);
  glPopMatrix();
  //脚1
  glPushMatrix();
  glTranslatef(wx/2.0 - thick, h/2.0, wz/2.0 - thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚2
  glPushMatrix();
  glTranslatef(wx/2.0 - thick, h/2.0, -wz/2.0 + thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚3
  glPushMatrix();
  glTranslatef(-wx/2.0 + thick, h/2.0, wz/2.0 - thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚4
  glPushMatrix();
  glTranslatef(-wx/2.0 + thick, h/2.0, -wz/2.0 + thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
}

void drawChair(float wx, float wz, float h)
{
  float thick = 0.04;
  float diffuse[4] = { 0.9f, 0.3f, 0.3f, 1.0f};
  float ambient[] = { 0.5f, 0.1f, 0.1f, 1.0f};
  float specular[]= { 1.0f, 1.0f, 1.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);
  glPushMatrix();
  glTranslatef(0.0, h, 0.0);
  glScalef(wx, thick, wz);
  drawCube(1.0);
  glPopMatrix();
  //脚1
  glPushMatrix();
  glTranslatef(wx/2.0 - thick, h/2.0, wz/2.0 - thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚2
  glPushMatrix();
  glTranslatef(wx/2.0 - thick, h/2.0, -wz/2.0 + thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚3
  glPushMatrix();
  glTranslatef(-wx/2.0 + thick, h/2.0, wz/2.0 - thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //脚4
  glPushMatrix();
  glTranslatef(-wx/2.0 + thick, h/2.0, -wz/2.0 + thick);
  glScalef(thick, h, thick);
  drawCube(1.0);
  glPopMatrix();
  //背もたれ
  glPushMatrix();
  glTranslatef( 0.0, 1.5 * h, -0.6 * wz);
  glRotatef(-10.0, 1.0, 0.0, 0.0); 
  glScalef( wx, 1.2 * h, thick/2.0);
  drawCube(1.0);
  glPopMatrix();
}
