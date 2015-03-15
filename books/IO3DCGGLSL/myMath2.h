//myMath.h
//グラフィックス用数学ライブラリ
#define M_PI 3.14159265358979323846
//double pp = M_PI / 180.0;

double sign(double x)
{
	if( x == 0.0) return 0.0;
	else if( x > 0.0) return 1.0;
	else return -1.0;
}

//3次元ベクトル演算
class CVector{

public:
  //メンバ変数
  float x, y, z;
  //コンストラクタ
  CVector();
  CVector(float x0, float y0, float z0);
  //演算子
  void operator+=(CVector a);//加算
  void operator-=(CVector a);//減算
  void operator*=(float s);//スカラ乗算
  void operator/=(float s);//スカラ除算
  friend CVector operator*(CVector a, float s);//スカラ乗算
  friend CVector operator*(float s, CVector a);//スカラ乗算
  friend CVector operator/(CVector a, float s);//スカラ除算
  friend CVector operator+(CVector a, CVector b);//加算
  friend CVector operator-(CVector a, CVector b);//減算
  friend float operator*(CVector a, CVector b);//内積
  friend CVector operator^(CVector a, CVector b);//外積
  friend CVector operator>>(CVector a, CVector b);//aからbへ向かう単位ベクトル
  //メンバ関数
  float magnitude();  //大きさ
  void normalize(void);//大きさ1のベクトルに変換
  void reverse(void);//反転
	CVector reverse2();
	CVector normalize2();//正規化したベクトルを返す
  CVector rotate(CVector v, CVector elr);//オイラー角による回転
  CVector rotateReverse(CVector v, CVector elr);//オイラー角による逆順で負の回転
	void scale(CVector vSize);//ｽｹｰﾘﾝｸﾞ
	void rotX(float angle); //x軸回転
  void rotY(float angle); //y軸回転
  void rotZ(float angle); //z軸回転
  float dist(CVector a, CVector b);//距離
  float getAngle(CVector a, CVector b);//２つのベクトルのなす角度
  CVector getEulerX(CVector a, CVector b);//aからｂへ向かうベクトルのオイラー角
  CVector getEulerZ(CVector a, CVector b);//aからｂへ向かうベクトルのオイラー角
};
//----------------------------------------------------------------------------
//コンストラクタ
CVector::CVector()
{
    x = y = z = 0.0;
}
//----------------------------------------------------------------------------
//コンストラクタ
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
//スカラ乗算
CVector operator*(CVector a, float s)
{
    return CVector(a.x * s, a.y * s, a.z * s);
}
//------------------------------------------------------------------------------
//スカラ乗算
CVector operator*(float s, CVector a)
{
    return CVector(a.x * s, a.y * s, a.z * s);
}
//------------------------------------------------------------------------------
//スカラ除算
CVector operator/(CVector a, float s)
{
    if(s == 0.0)
			printf("スカラ除算の分母が０です！ \n");

    return CVector(a.x / s, a.y / s, a.z / s);
}

//------------------------------------------------------------------------------
//内積
float operator*(CVector a, CVector b)
{
    return( a.x*b.x + a.y*b.y + a.z*b.z );
}
//------------------------------------------------------------------------------
//外積
CVector operator^(CVector a, CVector b)
{
    float x = a.y * b.z - a.z * b.y;
    float y = a.z * b.x - a.x * b.z;
    float z = a.x * b.y - a.y * b.x;
    return( CVector(x, y, z) );
}
//------------------------------------------------------------------------------
//aからbへ向かう単位ベクトル
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
    //逆順で負の回転
    v.rotZ(-elr.z);
    v.rotY(-elr.y);
    v.rotX(-elr.x);
    return v;
}
//------------------------------------------------------------------------------
//ベクトル間距離
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
    return ang;//rad単位で返す
}
//-----------------------------------------------------------------------------
CVector getEulerX(CVector a, CVector b)
{//基本姿勢で中心軸がｘ軸方向である棒状オブジェクトのオイラー角(deg)
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
{//基本姿勢で中心軸がz軸方向である棒状オブジェクトのオイラー角(deg)
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
//3*3のマトリクス
//------------------------------------------------------------------------------
class CMatrix{

public:
    //メンバ変数
    float e11, e12, e13, e21, e22, e23, e31, e32, e33;
    //コンストラクタ
    CMatrix();
    CMatrix(float a11, float a12, float a13,
            float a21, float a22, float a23,
            float a31, float a32, float a33);
    //演算子
    friend CVector operator*(CMatrix m, CVector v);
    friend CVector operator*(CVector v, CMatrix m);
    //メンバ関数
    float det();
    CMatrix inverse();
};
//----------------------------------------------------------------------------
//コンストラクタ
CMatrix::CMatrix()
{
    e11 = e12 = e13 = 0.0;
    e21 = e22 = e23 = 0.0;
    e31 = e32 = e33 = 0.0;
}
//-----------------------------------------------------------------------------
//コンストラクタ
CMatrix::CMatrix(float a11, float a12, float a13,
                 float a21, float a22, float a23,
                 float a31, float a32, float a33)
{
    e11 = a11; e12 = a12; e13 = a13;
    e21 = a21; e22 = a22; e23 = a23;
    e31 = a31; e32 = a32; e33 = a33;
}
//----------------------------------------------------------------------------
//行列式
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
			 printf("逆行列を求めることができません！ \n");
       d = 1.0;
    }
    return CMatrix(
        (e22*e33-e23*e32)/d, -(e12*e33-e13*e32)/d,  (e12*e23-e13*e22)/d,
       -(e21*e33-e23*e31)/d,  (e11*e33-e13*e31)/d, -(e11*e23-e13*e21)/d,
        (e21*e32-e22*e31)/d, -(e11*e32-e12*e31)/d,  (e11*e22-e12*e21)/d );
}
//----------------------------------------------------------------------------
//ベクトル乗算
CVector operator*(CMatrix m, CVector v)
{
    return CVector(
        m.e11*v.x + m.e12*v.y + m.e13*v.z,    //x成分
        m.e21*v.x + m.e22*v.y + m.e23*v.z,    //y成分
        m.e31*v.x + m.e32*v.y + m.e33*v.z );  //z成分
}
//----------------------------------------------------------------------------
//ベクトル乗算
CVector operator*(CVector v, CMatrix m)
{
    return CVector(
        v.x*m.e11 + v.y*m.e21 + v.z*m.e31,
        v.x*m.e12 + v.y*m.e22 + v.z*m.e32,
        v.x*m.e13 + v.y*m.e23 + v.z*m.e33 );
}


//----------------------------------------------------------------------------
//四元数：quaternion
// 定義式 q = s + xi + yj + zk;
//----------------------------------------------------------------------------
class CQuaternion{

public:
    //メンバ変数
    double s;//スカラ値
    CVector v;//ベクトル
    //コンストラクタ
    CQuaternion();
    CQuaternion(double a0, double a1, double a2, double a3);
    //演算子
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
    //メンバ関数
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
//コンストラクタ
CQuaternion::CQuaternion()
{
    s = 0;
    v.x = v.y = v.z = 0.0;
}
//-----------------------------------------------------------------------------
//コンストラクタ
CQuaternion::CQuaternion(double a0, double a1, double a2, double a3)
{
    s = a0;
    v.x = a1;
    v.y = a2;
    v.z = a3;
}
//-----------------------------------------------------------------------------
//共役四元数
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
//四元数どうしの和
CQuaternion operator+(CQuaternion p, CQuaternion q)
{
    return CQuaternion( p.s + q.s,
           p.v.x + q.v.x, p.v.y + q.v.y, p.v.z + q.v.z);
}
//------------------------------------------------------------------------------
//四元数どうしの差
CQuaternion operator-(CQuaternion p, CQuaternion q)
{
    return CQuaternion( p.s - q.s,
           p.v.x - q.v.x, p.v.y - q.v.y, p.v.z - q.v.z);
}
//------------------------------------------------------------------------------
//四元数どうしの乗算
CQuaternion operator*(CQuaternion p, CQuaternion q)
{
    CQuaternion qq;
     qq.s = p.s * q.s - p.v * q.v ; //２項目はベクトル内積
     qq.v = p.s * q.v + q.s * p.v + (p.v^q.v); //３項目はベクトル外積
    return qq;
}
//-----------------------------------------------------------------------------
//スカラ乗算
CQuaternion operator*(CQuaternion p, double a)
{
    return CQuaternion(p.s * a, p.v.x * a, p.v.y * a, p.v.z * a);
}
//-----------------------------------------------------------------------------
//スカラ乗算
CQuaternion operator*(double a, CQuaternion p)
{
    return CQuaternion(p.s * a, p.v.x * a, p.v.y * a, p.v.z * a);
}
//------------------------------------------------------------------------------
//ベクトルと四元数の乗算
CQuaternion operator*(CVector v, CQuaternion q)
{
    CQuaternion p, pp;
    p.s = 0.0; p.v = v; //スカラ部が0のクオータニオン
    pp = p * q;
    return pp; 
}
//------------------------------------------------------------------------------
//ベクトルと四元数の乗算
CQuaternion operator*(CQuaternion p, CVector v)
{
    CQuaternion q, pp;
    q.s = 0.0; q.v = v;
    pp = p * q;
    return pp;
}

//-----------------------------------------------------------------------------
//大きさを返す
double CQuaternion::magnitude()
{
    return sqrt( s * s + v.x * v.x + v.y * v.y + v.z * v.z);
}
//-----------------------------------------------------------------------------
//ベクトル部分を返す
CVector CQuaternion::getVector()
{
    return CVector(v.x, v.y, v.z);
}
//-----------------------------------------------------------------------------
//スカラ部を返す
double CQuaternion::getScalar()
{
    return s;
}
//-----------------------------------------------------------------------------
//ベクトルvを４元数qで回転
CVector qvRotate(CQuaternion q, CVector v)
{
    CQuaternion p = q * v * (~q);
    return CVector(p.v.x, p.v.y, p.v.z);
}
//------------------------------------------------------------------------------
//任意の回転軸から４元数を取得(axisはworld座標）
CQuaternion makeQFromAxis(double angle, CVector axis)
{
    axis.normalize();
    double a2 = (double)(angle * M_PI / 360.0);//rad単位でθ/2
    return CQuaternion( cos(a2) ,
         sin(a2) * axis.x, sin(a2) * axis.y, sin(a2) * axis.z );
}

//-----------------------------------------------------------------------------
//基本ベクトルを現在のEuler角で回転し，
//さらにクオータニオンｑによる回転を行い
//回転後の新しいオイラー角を求める
CVector makeEulerFromEuler(CVector elr0, CQuaternion q)
{   //現在姿勢からq回転
    double eps = 0.9999999;
    CVector elr; //返されるオイラー角（ベクトル形式)
    //物体の基本ベクトル
    CVector v1 = CVector(1.0, 0.0, 0.0);//x軸(TD_FORE)
    CVector v2 = CVector(0.0, 1.0, 0.0);//y軸(TD_LEFT)
    CVector v3 = CVector(0.0, 0.0, 1.0);//z軸(TD_UP)
    //現在姿勢(elr0)による基本ベクトルの回転
    v1 = rotate(v1, elr0);
    v2 = rotate(v2, elr0);
    v3 = rotate(v3, elr0);
    //クオータニオンによる回転
    v1 = qvRotate(q, v1);
    v2 = qvRotate(q, v2);
    v3 = qvRotate(q, v3);
    //回転後の基本ベクトルからオイラー角
    if(fabs(v1.z) > eps)//elr.yが±90度に近いときは注意
    {   //x軸回転を0，y軸を90°または-90°として
        //z軸だけを回転すればよい
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
				 printf("atan2ｴﾗｰ in MakeEulerFromEuler \n");
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

