
varying vec3 P;
varying vec3 N;
varying vec4 Refract;
uniform float nRatio;
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��(�]�u�j

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//����(����)�x�N�g��
	vec3 refract0 = refract(incident, N, 1.0/nRatio);
  Refract = ViewTranspose * vec4(refract0, 0.0);
  gl_Position = ftransform();
}
