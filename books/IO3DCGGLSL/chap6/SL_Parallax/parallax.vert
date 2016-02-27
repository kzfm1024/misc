
varying vec3 P;
varying vec3 N;
varying vec3 T;
varying vec3 B;
varying vec3 view;
attribute vec3 tangent;
uniform sampler2D normalMap;//�@���}�b�v

void main(void)
{
  P = (gl_ModelViewMatrix * gl_Vertex).xyz;
  N = normalize(gl_NormalMatrix * gl_Normal).xyz;
  T = normalize(gl_NormalMatrix * tangent).xyz;
  B = cross(N, T);
  
  // �ڋ�Ԃɂ����鎋�_�x�N�g�������߂�
  view.x = dot(-P, T);
  view.y = dot(-P, B);
  view.z = dot(-P, N);

  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform() ;
}
