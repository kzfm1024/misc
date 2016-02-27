
uniform mat4 ViewTranspose;
varying vec3 P;
varying vec3 N;
varying vec3 Reflect;//���˃x�N�g��

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//����(����)�x�N�g��
  vec3 R = reflect(incident, N);//���˃x�N�g��
  Reflect = (ViewTranspose * vec4(R, 0.0)).xyz;
  gl_Position = ftransform();
}
