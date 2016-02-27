
varying vec3 P;
varying vec3 N;
varying vec4 Reflect; 
uniform mat4 ViewTranspose;//ビュー変換だけのモデルビュー行列の転置行列

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//入射視線ベクトル
  vec3 reflect0 = reflect(incident, N);//その反射ベクトル
  Reflect = ViewTranspose * vec4(reflect0, 0.0);//ビュー行列の転置行列を乗じる
  gl_Position = ftransform();
}
