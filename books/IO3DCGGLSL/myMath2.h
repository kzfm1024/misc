//myMath.h
//�O���t�B�b�N�X�p���w���C�u����
#define M_PI 3.14159265358979323846
//double pp = M_PI / 180.0;

double sign(double x)
{
	if( x == 0.0) return 0.0;
	else if( x > 0.0) return 1.0;
	else return -1.0;
}

//3�����x�N�g�����Z
class CVector{

public:
  //�����o�ϐ�
  float x, y, z;
  //�R���X�g���N�^
  CVector();
  CVector(float x0, float y0, float z0);
  //���Z�q
  void operator+=(CVector a);//���Z
  void operator-=(CVector a);//���Z
  void operator*=(float s);//�X�J����Z
  void operator/=(float s);//�X�J�����Z
  friend CVector operator*(CVector a, float s);//�X�J����Z
  friend CVector operator*(float s, CVector a);//�X�J����Z
  friend CVector operator/(CVector a, float s);//�X�J�����Z
  friend CVector operator+(CVector a, CVector b);//���Z
  friend CVector operator-(CVector a, CVector b);//���Z
  friend float operator*(CVector a, CVector b);//����
  friend CVector operator^(CVector a, CVector b);//�O��
  friend CVector operator>>(CVector a, CVector b);//a����b�֌������P�ʃx�N�g��
  //�����o�֐�
  float magnitude();  //�傫��
  void normalize(void);//�傫��1�̃x�N�g���ɕϊ�
  void reverse(void);//���]
	CVector reverse2();
	CVector normalize2();//���K�������x�N�g����Ԃ�
  CVector rotate(CVector v, CVector elr);//�I�C���[�p�ɂ���]
  CVector rotateReverse(CVector v, CVector elr);//�I�C���[�p�ɂ��t���ŕ��̉�]
	void scale(CVector vSize);//����ݸ�
	void rotX(float angle); //x����]
  void rotY(float angle); //y����]
  void rotZ(float angle); //z����]
  float dist(CVector a, CVector b);//����
  float getAngle(CVector a, CVector b);//�Q�̃x�N�g���̂Ȃ��p�x
  CVector getEulerX(CVector a, CVector b);//a���炂�֌������x�N�g���̃I�C���[�p
  CVector getEulerZ(CVector a, CVector b);//a���炂�֌������x�N�g���̃I�C���[�p
};
//----------------------------------------------------------------------------
//�R���X�g���N�^
CVector::CVector()
{
    x = y = z = 0.0;
}
//----------------------------------------------------------------------------
//�R���X�g���N�^
CVector::CVector(float x0, float y0, float z0)
{
    x = x0;
    y = y0;
    z = z0;
}
//------------------------------------------------------------------------------
void CVector::operator+=(CVector a)
{
    x += a.x;
    y += a.y;
    z += a.z;
}
//------------------------------------------------------------------------------
void CVector::operator-=(CVector a)
{
    x -= a.x;
    y -= a.y;
    z -= a.z;
}
//------------------------------------------------------------------------------
void CVector::operator*=(float s)
{
    x *= s;
    y *= s;
    z *= s;
}
//------------------------------------------------------------------------------
void CVector::operator/=(float s)
{
    x /= s;
    y /= s;
    z /= s;
}
//------------------------------------------------------------------------------
CVector operator+(CVector a, CVector b)
{
    return CVector(a.x+b.x, a.y+b.y, a.z+b.z);
}
//------------------------------------------------------------------------------
CVector operator-(CVector a, CVector b)
{
    return CVector(a.x - b.x, a.y - b.y, a.z - b.z);
}
//------------------------------------------------------------------------------
//�X�J����Z
CVector operator*(CVector a, float s)
{
    return CVector(a.x * s, a.y * s, a.z * s);
}
//------------------------------------------------------------------------------
//�X�J����Z
CVector operator*(float s, CVector a)
{
    return CVector(a.x * s, a.y * s, a.z * s);
}
//------------------------------------------------------------------------------
//�X�J�����Z
CVector operator/(CVector a, float s)
{
    if(s == 0.0)
			printf("�X�J�����Z�̕��ꂪ�O�ł��I \n");

    return CVector(a.x / s, a.y / s, a.z / s);
}

//------------------------------------------------------------------------------
//����
float operator*(CVector a, CVector b)
{
    return( a.x*b.x + a.y*b.y + a.z*b.z );
}
//------------------------------------------------------------------------------
//�O��
CVector operator^(CVector a, CVector b)
{
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    return( CVector(x, y, z) );
}
//------------------------------------------------------------------------------
//a����b�֌������P�ʃx�N�g��
CVector operator>>(CVector a, CVector b)
{
    CVector c = b - a;
    c.normalize();
    return c ;
}

//-----------------------------------------------------------------------------
float CVector::magnitude()
{
    return( sqrt(x * x + y * y + z * z) );
}
//-----------------------------------------------------------------------------
void CVector::normalize()
{
    double eps = 0.000001;
    double mag = sqrt(x * x + y * y + z * z) ;
	if(mag <= eps) mag = 1.0;
    x /= mag;
    y /= mag;
    z /= mag;
    if(fabs(x) < eps) x = 0.0;
    if(fabs(y) < eps) y = 0.0;
    if(fabs(z) < eps) z = 0.0;
}
//-----------------------------------------------------------------------------
CVector CVector::normalize2()
{
    double eps = 0.000001;
    double xx, yy, zz;
    double mag = sqrt(x * x + y * y + z * z) ;
    if(mag < eps) mag = 1.0;
    xx = x / mag;
    yy = y / mag;
    zz = z / mag;
    if(fabs(x) < eps) xx = 0.0;
    if(fabs(y) < eps) yy = 0.0;
    if(fabs(z) < eps) zz = 0.0;
    return(CVector(xx, yy, zz));
}

//-----------------------------------------------------------------------------
void CVector::reverse()
{
    x = -x;
    y = -y;
    z = -z;
}
//-----------------------------------------------------------------------------
CVector CVector:: reverse2()
{
	CVector v = CVector();
	v.x = -x;
	v.y = -y;
	v.z = -z;
	return(v);
}

//-----------------------------------------------------------------------------
void CVector::scale(CVector vSize)
{
	double xx, yy, zz;

	xx = x; yy = y; zz = z;
	x = xx * vSize.x;
	y = yy * vSize.y;
	z = zz * vSize.z;
}

//------------------------------------------------------------------------------------
void CVector::rotX(float angle)
{
	float xx, yy, zz;
	double pp = M_PI / 180.0;

	xx = x; yy = y; zz = z;
	x = xx;
	y = yy * cos(pp * angle) - zz * sin(pp * angle);
	z = yy * sin(pp * angle) + zz * cos(pp * angle);
}
//------------------------------------------------------------------------------------
void CVector::rotY(float angle)
{
	float xx, yy, zz;
	double pp = M_PI / 180.0;

	xx = x; yy = y; zz = z;
	x = xx * cos(pp * angle) + zz * sin(pp * angle);
	y = yy;
	z = - xx * sin(pp * angle) + zz * cos(pp * angle);
}
//------------------------------------------------------------------------------------
void CVector::rotZ(float angle)
{
	float xx, yy, zz;
	double pp = M_PI / 180.0;

	xx = x; yy = y; zz = z;
	x = xx * cos(pp * angle) - yy * sin(pp * angle);
	y = xx * sin(pp * angle) + yy * cos(pp * angle);
	z = zz;
}
//-----------------------------------------------------------------------------
CVector rotate(CVector v, CVector elr)
{
    v.rotX(elr.x);
    v.rotY(elr.y);
    v.rotZ(elr.z);
    return v;
}

CVector rotate(CVector v, double angle[3])
{
    v.rotX(angle[0]);
    v.rotY(angle[1]);
    v.rotZ(angle[2]);
    return v;
}

CVector rotateReverse(CVector v, CVector elr)
{
    //�t���ŕ��̉�]
    v.rotZ(-elr.z);
    v.rotY(-elr.y);
    v.rotX(-elr.x);
    return v;
}
//------------------------------------------------------------------------------
//�x�N�g���ԋ���
float dist(CVector a, CVector b)
{
    CVector c = b - a;
    return( c.magnitude() );
}

//-----------------------------------------------------------------------------
float getAngle(CVector a, CVector b)
{
    float ang = (a.x*b.x+a.y*b.y+a.z*b.z)/(a.magnitude()*b.magnitude());
    if(ang >= 1.0) ang = 0.0;
    else if (ang <= -1.0) ang = M_PI;
    else ang = acos(ang);
    return ang;//rad�P�ʂŕԂ�
}
//-----------------------------------------------------------------------------
CVector getEulerX(CVector a, CVector b)
{//��{�p���Œ��S�������������ł���_��I�u�W�F�N�g�̃I�C���[�p(deg)
    float cx, cy, cz, len;
    CVector e;
    cx = b.x - a.x;
    cy = b.y - a.y;
    cz = b.z - a.z;
    len = dist(a, b);
    e.x = 0.0;
    if(cz >= len) e.y = -90.0;
    else if(cz <= -len) e.y = 90.0;
    else e.y = - asin(cz / len) * 180.0 / M_PI;
    if(fabs(cx) <= 0.0001 && fabs(cy) <= 0.0001) e.z = 0.0;
    else e.z = atan2(cy, cx) * 180.0 / M_PI;
    return e;
}
//------------------------------------------------------------------------------
CVector getEulerZ(CVector a, CVector b)
{//��{�p���Œ��S����z�������ł���_��I�u�W�F�N�g�̃I�C���[�p(deg)
    float cx, cy, cz, len;
    CVector e;
    cx = b.x - a.x;
    cy = b.y - a.y;
    cz = b.z - a.z;
    len = dist(a, b);
    e.x = 0.0;
    if(cz >= len) e.y = 0.0;
    else if(cz <= -len) e.y = 180.0;
    else e.y = acos(cz / len) * 180.0 / M_PI;
    if(fabs(cx) <= 0.0001 && fabs(cy) <= 0.0001) e.z = 0.0;
    else e.z = atan2(cy, cx) * 180.0 / M_PI ;
    return e;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//3*3�̃}�g���N�X
//------------------------------------------------------------------------------
class CMatrix{

public:
    //�����o�ϐ�
    float e11, e12, e13, e21, e22, e23, e31, e32, e33;
    //�R���X�g���N�^
    CMatrix();
    CMatrix(float a11, float a12, float a13,
            float a21, float a22, float a23,
            float a31, float a32, float a33);
    //���Z�q
    friend CVector operator*(CMatrix m, CVector v);
    friend CVector operator*(CVector v, CMatrix m);
    //�����o�֐�
    float det();
    CMatrix inverse();
};
//----------------------------------------------------------------------------
//�R���X�g���N�^
CMatrix::CMatrix()
{
    e11 = e12 = e13 = 0.0;
    e21 = e22 = e23 = 0.0;
    e31 = e32 = e33 = 0.0;
}
//-----------------------------------------------------------------------------
//�R���X�g���N�^
CMatrix::CMatrix(float a11, float a12, float a13,
                 float a21, float a22, float a23,
                 float a31, float a32, float a33)
{
    e11 = a11; e12 = a12; e13 = a13;
    e21 = a21; e22 = a22; e23 = a23;
    e31 = a31; e32 = a32; e33 = a33;
}
//----------------------------------------------------------------------------
//�s��
float CMatrix::det()
{
    return( e11*e22*e33 + e12*e23*e31 + e13*e32*e21
          - e11*e32*e23 - e12*e21*e33 - e31*e22*e13 );
}
//----------------------------------------------------------------------------
CMatrix CMatrix::inverse()
{
    float d = e11*e22*e33 + e12*e23*e31 + e13*e32*e21
             - e11*e32*e23 - e12*e21*e33 - e31*e22*e13 ;
    if( d == 0.0 ) {
			 printf("�t�s������߂邱�Ƃ��ł��܂���I \n");
       d = 1.0;
    }
    return CMatrix(
        (e22*e33-e23*e32)/d, -(e12*e33-e13*e32)/d,  (e12*e23-e13*e22)/d,
       -(e21*e33-e23*e31)/d,  (e11*e33-e13*e31)/d, -(e11*e23-e13*e21)/d,
        (e21*e32-e22*e31)/d, -(e11*e32-e12*e31)/d,  (e11*e22-e12*e21)/d );
}
//----------------------------------------------------------------------------
//�x�N�g����Z
CVector operator*(CMatrix m, CVector v)
{
    return CVector(
        m.e11*v.x + m.e12*v.y + m.e13*v.z,    //x����
        m.e21*v.x + m.e22*v.y + m.e23*v.z,    //y����
        m.e31*v.x + m.e32*v.y + m.e33*v.z );  //z����
}
//----------------------------------------------------------------------------
//�x�N�g����Z
CVector operator*(CVector v, CMatrix m)
{
    return CVector(
        v.x*m.e11 + v.y*m.e21 + v.z*m.e31,
        v.x*m.e12 + v.y*m.e22 + v.z*m.e32,
        v.x*m.e13 + v.y*m.e23 + v.z*m.e33 );
}


//----------------------------------------------------------------------------
//�l�����Fquaternion
// ��`�� q = s + xi + yj + zk;
//----------------------------------------------------------------------------
class CQuaternion{

public:
    //�����o�ϐ�
    double s;//�X�J���l
    CVector v;//�x�N�g��
    //�R���X�g���N�^
    CQuaternion();
    CQuaternion(double a0, double a1, double a2, double a3);
    //���Z�q
    CQuaternion operator~();
    void operator+=(CQuaternion q);
    void operator-=(CQuaternion q);
    void operator*=(double a);
    void operator/=(double a);
    friend CQuaternion operator+(CQuaternion p, CQuaternion q);
    friend CQuaternion operator-(CQuaternion p, CQuaternion q);
    friend CQuaternion operator*(CQuaternion p, CQuaternion q);
    friend CQuaternion operator*(CQuaternion p, double s);
    friend CQuaternion operator*(double s, CQuaternion p);
    friend CQuaternion operator*(CVector v, CQuaternion q);
    friend CQuaternion operator*(CQuaternion q, CVector v);
    //�����o�֐�
    double magnitude();
    CVector getVector();
    double getScalar();
    CQuaternion qvRotate(CVector v, CQuaternion q);
    CVector qvRotate(CQuaternion q, CVector v);
//    CQuaternion makeQFromEuler(CVector elr);
//    CVector makeEulerFromQ(CQuaternion q);
    CQuaternion makeQFromAxis(double angle, CVector axis);
    CVector makeEulerFromEuler(CVector v, CQuaternion);
};
//-----------------------------------------------------------------------------
//�R���X�g���N�^
CQuaternion::CQuaternion()
{
    s = 0;
    v.x = v.y = v.z = 0.0;
}
//-----------------------------------------------------------------------------
//�R���X�g���N�^
CQuaternion::CQuaternion(double a0, double a1, double a2, double a3)
{
    s = a0;
    v.x = a1;
    v.y = a2;
    v.z = a3;
}
//-----------------------------------------------------------------------------
//�����l����
CQuaternion CQuaternion::operator~()
{
    return CQuaternion( s, -v.x, -v.y, -v.z);
}
//-----------------------------------------------------------------------------
void CQuaternion::operator+=(CQuaternion q)
{
    s += q.s;
    v.x += q.v.x;
    v.y += q.v.y;
    v.z += q.v.z;
}
//-----------------------------------------------------------------------------
void CQuaternion::operator-=(CQuaternion q)
{
    s -= q.s;
    v.x -= q.v.x;
    v.y -= q.v.y;
    v.z -= q.v.z;
}
//-----------------------------------------------------------------------------
void CQuaternion::operator*=(double a)
{
    s *= a;
    v.x *= a;
    v.y *= a;
    v.z *= a;
}
//-----------------------------------------------------------------------------
void CQuaternion::operator/=(double a)
{
    s /= a;
    v.x /= a;
    v.y /= a;
    v.z /= a;
}

//------------------------------------------------------------------------------
//�l�����ǂ����̘a
CQuaternion operator+(CQuaternion p, CQuaternion q)
{
    return CQuaternion( p.s + q.s,
           p.v.x + q.v.x, p.v.y + q.v.y, p.v.z + q.v.z);
}
//------------------------------------------------------------------------------
//�l�����ǂ����̍�
CQuaternion operator-(CQuaternion p, CQuaternion q)
{
    return CQuaternion( p.s - q.s,
           p.v.x - q.v.x, p.v.y - q.v.y, p.v.z - q.v.z);
}
//------------------------------------------------------------------------------
//�l�����ǂ����̏�Z
CQuaternion operator*(CQuaternion p, CQuaternion q)
{
    CQuaternion qq;
     qq.s = p.s * q.s - p.v * q.v ; //�Q���ڂ̓x�N�g������
     qq.v = p.s * q.v + q.s * p.v + (p.v^q.v); //�R���ڂ̓x�N�g���O��
    return qq;
}
//-----------------------------------------------------------------------------
//�X�J����Z
CQuaternion operator*(CQuaternion p, double a)
{
    return CQuaternion(p.s * a, p.v.x * a, p.v.y * a, p.v.z * a);
}
//-----------------------------------------------------------------------------
//�X�J����Z
CQuaternion operator*(double a, CQuaternion p)
{
    return CQuaternion(p.s * a, p.v.x * a, p.v.y * a, p.v.z * a);
}
//------------------------------------------------------------------------------
//�x�N�g���Ǝl�����̏�Z
CQuaternion operator*(CVector v, CQuaternion q)
{
    CQuaternion p, pp;
    p.s = 0.0; p.v = v; //�X�J������0�̃N�I�[�^�j�I��
    pp = p * q;
    return pp; 
}
//------------------------------------------------------------------------------
//�x�N�g���Ǝl�����̏�Z
CQuaternion operator*(CQuaternion p, CVector v)
{
    CQuaternion q, pp;
    q.s = 0.0; q.v = v;
    pp = p * q;
    return pp;
}

//-----------------------------------------------------------------------------
//�傫����Ԃ�
double CQuaternion::magnitude()
{
    return sqrt( s * s + v.x * v.x + v.y * v.y + v.z * v.z);
}
//-----------------------------------------------------------------------------
//�x�N�g��������Ԃ�
CVector CQuaternion::getVector()
{
    return CVector(v.x, v.y, v.z);
}
//-----------------------------------------------------------------------------
//�X�J������Ԃ�
double CQuaternion::getScalar()
{
    return s;
}
//-----------------------------------------------------------------------------
//�x�N�g��v���S����q�ŉ�]
CVector qvRotate(CQuaternion q, CVector v)
{
    CQuaternion p = q * v * (~q);
    return CVector(p.v.x, p.v.y, p.v.z);
}
//------------------------------------------------------------------------------
//�C�ӂ̉�]������S�������擾(axis��world���W�j
CQuaternion makeQFromAxis(double angle, CVector axis)
{
    axis.normalize();
    double a2 = (double)(angle * M_PI / 360.0);//rad�P�ʂŃ�/2
    return CQuaternion( cos(a2) ,
         sin(a2) * axis.x, sin(a2) * axis.y, sin(a2) * axis.z );
}

//-----------------------------------------------------------------------------
//��{�x�N�g�������݂�Euler�p�ŉ�]���C
//����ɃN�I�[�^�j�I�����ɂ���]���s��
//��]��̐V�����I�C���[�p�����߂�
CVector makeEulerFromEuler(CVector elr0, CQuaternion q)
{   //���ݎp������q��]
    double eps = 0.9999999;
    CVector elr; //�Ԃ����I�C���[�p�i�x�N�g���`��)
    //���̂̊�{�x�N�g��
    CVector v1 = CVector(1.0, 0.0, 0.0);//x��(TD_FORE)
    CVector v2 = CVector(0.0, 1.0, 0.0);//y��(TD_LEFT)
    CVector v3 = CVector(0.0, 0.0, 1.0);//z��(TD_UP)
    //���ݎp��(elr0)�ɂ���{�x�N�g���̉�]
    v1 = rotate(v1, elr0);
    v2 = rotate(v2, elr0);
    v3 = rotate(v3, elr0);
    //�N�I�[�^�j�I���ɂ���]
    v1 = qvRotate(q, v1);
    v2 = qvRotate(q, v2);
    v3 = qvRotate(q, v3);
    //��]��̊�{�x�N�g������I�C���[�p
    if(fabs(v1.z) > eps)//elr.y���}90�x�ɋ߂��Ƃ��͒���
    {   //x����]��0�Cy����90���܂���-90���Ƃ���
        //z����������]����΂悢
        elr.x = 0.0;
        elr.y = - (v1.z / fabs(v1.z)) * 90.0f;
        if(v2.x > eps) elr.z = -90.0;
        else if( v2.x < -eps) elr.z = 90.0;
        else{
            if(v2.y >= 0.0) elr.z = - asin(v2.x) * 180.0 / M_PI;
            else  elr.z = 180.0 + asin(v2.x) * 180.0 / M_PI;
        }
    }
    else
    {
        //alpha
       if(v2.z == 0.0 && v3.z == 0.0) {
				 printf("atan2�װ in MakeEulerFromEuler \n");
            elr.x = 0.0;
        }
        else elr.x = atan2(v2.z , v3.z) * 180.0 / M_PI;
        //beta
        if( v1.z > eps) elr.y = -90.0;
        else if( v1.z < -eps) elr.y = 90.0;
        else elr.y = - asin(v1.z) * 180.0 / M_PI;
        //gamma
        elr.z = atan2(v1.y , v1.x) * 180.0 / M_PI;
    }
    return elr;
}
//------------------------------------------------------------------------------

