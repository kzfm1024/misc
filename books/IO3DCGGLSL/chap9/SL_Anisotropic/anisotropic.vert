attribute vec3 tangent;
uniform int changeTangent;
varying vec3 P, N, T; 

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);//���_���W
  N = normalize(gl_NormalMatrix * gl_Normal);//�@���x�N�g��
  T = normalize(gl_NormalMatrix * tangent);//�ڐ��x�N�g��
  if(changeTangent == 1) T = cross(T, N);//�ڐ��x�N�g���̐؂�ւ�
  gl_Position = ftransform();
}
