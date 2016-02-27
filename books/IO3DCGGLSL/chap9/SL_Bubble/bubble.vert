
uniform mat4 ViewTranspose;//ビュー変換だけのモデルビュー行列の転置行列
varying vec3 P, N;
varying vec4 Reflect;//反射ベクトル

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
  vec3 reflect0 = reflect(incident, N);//反射ベクトル
  Reflect = ViewTranspose * vec4(reflect0, 0.0);
  gl_Position = ftransform();
}
