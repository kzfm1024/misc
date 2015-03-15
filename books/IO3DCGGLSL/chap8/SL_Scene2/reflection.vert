
uniform mat4 ViewTranspose;
varying vec3 P;
varying vec3 N;
varying vec3 Reflect;//反射ベクトル

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
  vec3 R = reflect(incident, N);//反射ベクトル
  Reflect = (ViewTranspose * vec4(R, 0.0)).xyz;
  gl_Position = ftransform();
}
