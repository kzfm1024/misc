
varying vec3 P;
varying vec3 N;
varying vec3 T;//接線ベクトル
varying vec3 B;//従法線ベクトル
attribute vec3 tangent;//OpenGLから

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  T = normalize(gl_NormalMatrix * tangent);
  B = cross(N, T);//従法線ベクトル
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform() ;
}
