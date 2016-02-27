
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��̓]�u�s��
varying vec3 P, N;
varying vec4 Reflect;//���˃x�N�g��

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//����(����)�x�N�g��
  vec3 reflect0 = reflect(incident, N);//���˃x�N�g��
  Reflect = ViewTranspose * vec4(reflect0, 0.0);
  gl_Position = ftransform();
}
