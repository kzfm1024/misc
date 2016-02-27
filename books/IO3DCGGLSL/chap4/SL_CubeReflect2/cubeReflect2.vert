
varying vec3 P;
varying vec3 N;
varying vec4 Reflect; 
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��̓]�u�s��

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//���ˎ����x�N�g��
  vec3 reflect0 = reflect(incident, N);//���̔��˃x�N�g��
  Reflect = ViewTranspose * vec4(reflect0, 0.0);//�r���[�s��̓]�u�s����悶��
  gl_Position = ftransform();
}
