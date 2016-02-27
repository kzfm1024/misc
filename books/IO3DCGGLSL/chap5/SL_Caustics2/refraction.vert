vec3 calcRefract(vec3 I, vec3 N, float nRatio, int flagInverse)
{//���܃x�N�g���̌v�Z
  if(flagInverse == 1){//���_�����ʈȉ�
		N = - N;
		nRatio = 1.0 / nRatio;
  }
  float cosIN = dot(-I, N);
  float a = nRatio * nRatio - 1.0 + cosIN * cosIN;
  if(a < 0.0) return reflect(I, N);//�S����
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
  vec3 incident = normalize(P);//����(����)�x�N�g��
  vec3 T = calcRefract(incident, N, nRatio, flagInverse) ;//���̋��܃x�N�g��
  Refract = ViewTranspose * vec4(T, 0.0) ;
  gl_Position = ftransform();
}
