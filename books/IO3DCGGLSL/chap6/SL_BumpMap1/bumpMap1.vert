
varying vec3 P;//�ʒu�x�N�g��
varying vec3 N;//�@���x�N�g��
varying vec3 T;//�ڐ��x�N�g��
varying vec3 B;//�]�@���x�N�g��
attribute vec3 tangent;//OpenGL����

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  T = normalize(gl_NormalMatrix * tangent);
  B = cross(N, T);//�]�@���x�N�g��
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform() ; 
}
