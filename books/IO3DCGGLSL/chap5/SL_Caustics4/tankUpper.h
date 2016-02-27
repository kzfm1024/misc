//tankUpper.h
//上部水槽（投影マッピングの対象ではない）
void drawTank2(float widthX, float tankH, float widthZ, float waveH);
void drawCeiling(float widthX, float tankH, float widthZ);
void drawWall_nx2(float widthX, float tankH, float widthZ, float waveH);//左側
void drawWall_px2(float widthX, float tankH, float widthZ, float waveH);//右側
void drawWall_nz2(float widthX, float tankH, float widthZ, float waveH);//奥

void drawTank2(float widthX, float tankH, float widthZ, float waveH)
{
	drawWall_px2( widthX, tankH,  widthZ, waveH);//右側
	drawWall_nx2( widthX, tankH,  widthZ, waveH);//左側
	drawCeiling(widthX, tankH,  widthZ);  //天井
	drawWall_nz2( widthX, tankH,  widthZ, waveH); //奥
}

void drawCeiling(float widthX, float tankH, float widthZ)
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
    glVertex3f(-widthX / 2.0, tankH,  -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, tankH,   widthZ / 2.0);
    glVertex3f( widthX / 2.0, tankH,   widthZ / 2.0);
    glVertex3f( widthX / 2.0, tankH,  -widthZ / 2.0);
  glEnd();

  //光源オブジェクト
  float diffuseL[] = { 1.0, 1.0, 1.0, 1.0};
  float ambientL[] = { 1.0, 1.0, 1.0, 1.0};
  float specularL[]= { 1.0, 1.0, 1.0, 1.0};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuseL);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambientL);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specularL);
  glMaterialf(GL_FRONT,GL_SHININESS,100);

  //Light1
  glPushMatrix();
  glTranslatef(0.0, tankH-0.05, 0.0);
  glScalef(2.0, 0.1, 2.0);
  glutSolidCube(1.0);
  glPopMatrix();	
}

void drawWall_nx2(float widthX, float tankH, float widthZ, float waveH)
{
//	setTextureMatrix();
  float diffuse[4] = { 0.5f, 0.6f, 0.9f, 1.0f};
  float ambient[] = { 0.2f, 0.3f, 0.5f, 1.0f};
  float specular[]= { 0.5f, 0.5f, 0.5f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(1.0, 0.0, 0.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, waveH, widthZ / 2.0);
    glVertex3f(-widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, tankH, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, tankH, widthZ / 2.0);
  glEnd();

}

void drawWall_px2(float widthX, float tankH, float widthZ, float waveH)
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
    glVertex3f( widthX / 2.0, waveH, widthZ / 2.0);
    glVertex3f( widthX / 2.0, tankH, widthZ / 2.0);
    glVertex3f( widthX / 2.0, tankH, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, waveH, -widthZ / 2.0);
  glEnd();
}

void drawWall_nz2(float widthX, float tankH, float widthZ, float waveH)
{//奥
  float diffuse[4] = { 0.6f, 0.8f, 0.8f, 1.0f};
  float ambient[] = { 0.3f, 0.4f, 0.4f, 1.0f};
  float specular[]= { 0.1f, 0.1f, 0.1f, 1.0f};

  glMaterialfv(GL_FRONT,GL_DIFFUSE,diffuse);
  glMaterialfv(GL_FRONT,GL_AMBIENT,ambient);
  glMaterialfv(GL_FRONT,GL_SPECULAR,specular);
  glMaterialf(GL_FRONT,GL_SHININESS,10);

  glNormal3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
    glVertex3f(-widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, waveH, -widthZ / 2.0);
    glVertex3f( widthX / 2.0, tankH, -widthZ / 2.0);
    glVertex3f(-widthX / 2.0, tankH, -widthZ / 2.0);
  glEnd();
}
