
uniform mat4 ViewTranspose;
varying vec3 P, N;
varying vec4 Reflect;
uniform float time;

void main(void)
{
  gl_Vertex.x += 0.1 * sin(2.0 * 3.141592 * gl_Vertex.z + 2.0 * time);
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
  vec3 reflect0 = reflect(incident, N);//反射ベクトル
  Reflect = ViewTranspose * vec4(reflect0, 0.0);
  gl_Position = ftransform();
}
