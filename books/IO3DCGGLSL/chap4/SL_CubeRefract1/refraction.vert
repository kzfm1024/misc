
uniform float nRatio;
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��(�]�u�j
varying vec3 P;
varying vec3 N;
varying vec4 Refract;

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//����(����)�x�N�g��
	vec3 refract0 = refract(incident, N, 1.0/nRatio);//���܃x�N�g��
  Refract = ViewTranspose * vec4(refract0, 0.0);//���܃x�N�g���ɂ��e�N�X�`�����W
  gl_Position = ftransform();
}
