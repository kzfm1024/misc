
uniform float nRatio;
uniform mat4 ViewTranspose;//ビュー変換だけのモデルビュー行列(転置）
varying vec3 P;
varying vec3 N;
varying vec4 Refract;

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
	vec3 refract0 = refract(incident, N, 1.0/nRatio);//屈折ベクトル
  Refract = ViewTranspose * vec4(refract0, 0.0);//屈折ベクトルによるテクスチャ座標
  gl_Position = ftransform();
}
