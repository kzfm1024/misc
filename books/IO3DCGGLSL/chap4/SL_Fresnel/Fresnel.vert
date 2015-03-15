
uniform float nRatio;
uniform float reflectivity;//�������˗�
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��̓]�u�s��
varying vec3 P;
varying vec3 N;
varying vec4 Refract;//���܃x�N�g��
varying vec4 Reflect;//���˃x�N�g��
varying float factorReflect;//���˗�

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//����(����)�x�N�g��
	vec3 refract0 = refract(incident, N, 1.0/nRatio);
  Refract = ViewTranspose * vec4(refract0, 0.0);
  vec3 reflect0 = reflect(incident, N);//���˃x�N�g��
  Reflect = ViewTranspose * vec4(reflect0, 0.0);
  factorReflect = reflectivity + (1.0 - reflectivity) * pow(1.0 + dot(incident, N), 5.0);
  gl_Position = ftransform();
}
