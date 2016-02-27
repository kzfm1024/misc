//tankLower.h
//投影マッピングの対象
void drawTank1(float widthX, float tankH, float widthZ, float waveH);
void drawFloor1(float widthX, float widthZ, int nx, int nz);
void drawWall_nx1(float widthX, float tankH, float widthZ, float waveH);//左側
void drawWall_px1(float widthX, float tankH, float widthZ, float waveH);//右側
void drawWall_nz1(float widthX, float tankH, float widthZ, float waveH);//奥
void drawBlock(float wx, float wz, float h);

void drawTank1(float widthX, float tankH, float widthZ, float waveH)
{
	drawFloor1(widthX, widthZ, 10, 10);
	drawWall_px1( widthX, tankH,  widthZ, waveH);//右側
	drawWall_nx1( widthX, tankH,  widthZ, waveH);//左側
	drawWall_nz1( widthX, tankH,  widthZ, waveH);//奥
//drawBlock(1.0, 2.0, 1.0);
/*
	setTextureMatrix();

	glTranslatef(-widthX/2+1.0, 1.0, -1.0);
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
	//block１
	glPushMatrix();
	glTranslatef(-widthX/2+1.0, 1.0, -1.0);
	drawBlock(1.0, 2.0, 1.0);
	glPopMatrix();

	setTextureMatrix();
	glTranslatef(0.0, 0.0, -widthZ/2 + 2.0);
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
	//block２
	glPushMatrix();
	glTranslatef(0.0, 0.0, -widthZ/2 + 2.0);
	drawBlock(4.0, 1.0, 1.0);
	glPopMatrix();
*/
}

void drawFloor1(float widthX, float widthZ, int nx, int nz)
{
  int i, j;
  //Floor１枚当たりの幅
  float wX = widthX / (float)nx;
  float wZ = widthZ / (float)nz;

  float diffuse[][4] = {
	  { 0.9f, 0.8f, 0.7f, 1.0f}, { 0.4f, 0.4f, 0.5f} };
  float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
  float specular[]= { 0.0f, 0.0f, 0.0f, 1.0f};

  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

	setTextureMatrix();
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);

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

void drawWall_nx1(float widthX, float tankH, float widthZ, float waveH)
{
  float diffuse[4] = { 0.5f, 0.6f, 0.9f, 1.0f};
  float ambient[] = { 0.2f, 0.3f, 0.5f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

	setTextureMatrix();
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
  glNormal3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, 0.0, widthZ / 2.0);
    glVertex3f(-widthX / 2.0, 0.0, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, waveH, widthZ / 2.0);
  glEnd();
}

void drawWall_px1(float widthX, float tankH, float widthZ, float waveH)
{
  float diffuse[4] = { 0.8, 0.5f, 0.4f, 1.0f};
  float ambient[] = { 0.4f, 0.3f, 0.2f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

	setTextureMatrix();
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
  glNormal3f(-1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f( widthX / 2.0, 0, widthZ / 2.0);
    glVertex3f( widthX / 2.0, waveH, widthZ / 2.0);
    glVertex3f( widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, 0, -widthZ / 2.0);
  glEnd();
}

void drawWall_nz1(float widthX, float tankH, float widthZ, float waveH)
{//奥
  float diffuse[4] = { 0.6f, 0.8f, 0.8f, 1.0f};
  float ambient[] = { 0.3f, 0.4f, 0.4f, 1.0f};
  float specular[]= { 0.1f, 0.1f, 0.1f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

	setTextureMatrix();
  //通常のモデルビュー変換に戻す 
  glMatrixMode(GL_MODELVIEW);
  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, 0, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, waveH, -widthZ / 2.0);
  glEnd();
}
void drawBlock(float wx, float wz, float h)
{
  float diffuse[4] = { 0.9f, 0.2f, 0.f, 1.0f};
  float ambient[] = { 0.4f, 0.1f, 0.f, 1.0f};
  float specular[]= { 0.2f, 0.2f, 0.2f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

//	setTextureMatrix();
//	glTranslatef(0.0, 0.0, 0.0);
//  glTranslatef(-2.0, h/2.0, 0.0);
// glScalef(wx, h, wz);
  //通常のモデルビュー変換に戻す 
//  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
//	glTranslatef(0.0, 0.0, 0.0);
//  glTranslatef(-2.0, h/2.0, 0.0);
  glScalef(wx, h, wz);
  drawCube(1.0);
  glPopMatrix();

//  glMatrixMode(GL_TEXTURE);
//  glLoadIdentity();

//	setTextureMatrix();
//  glTranslatef(0.0, -h/2.0, 0.0);
//  glTranslatef(0.0, 0.0, 0.0);
  //通常のモデルビュー変換に戻す 
//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
	
}
