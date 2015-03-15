vec3 calcRefract(vec3 I, vec3 N, float nRatio, int flagInverse)
{//屈折ベクトルの計算
  if(flagInverse == 1){//視点が水面以下
		N = - N;
		nRatio = 1.0 / nRatio;
  }
  float cosIN = dot(-I, N);
  float a = nRatio * nRatio - 1.0 + cosIN * cosIN;
  if(a < 0.0) return reflect(I, N);//全反射
  else return (I - N *(sqrt(a) - cosIN)) / nRatio;
}

varying vec3 P;
varying vec3 N;
varying vec4 Refract;
uniform mat4 ViewTranspose;
uniform float nRatio;
uniform int flagInverse;

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
  vec3 T = calcRefract(incident, N, nRatio, flagInverse) ;//その屈折ベクトル
  Refract = ViewTranspose * vec4(T, 0.0) ;
  gl_Position = ftransform();
}
