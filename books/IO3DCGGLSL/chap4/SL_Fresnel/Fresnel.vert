
uniform float nRatio;
uniform float reflectivity;//垂直反射率
uniform mat4 ViewTranspose;//ビュー変換だけのモデルビュー行列の転置行列
varying vec3 P;
varying vec3 N;
varying vec4 Refract;//屈折ベクトル
varying vec4 Reflect;//反射ベクトル
varying float factorReflect;//反射率

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 incident = normalize(P);//視線(入射)ベクトル
	vec3 refract0 = refract(incident, N, 1.0/nRatio);
  Refract = ViewTranspose * vec4(refract0, 0.0);
  vec3 reflect0 = reflect(incident, N);//反射ベクトル
  Reflect = ViewTranspose * vec4(reflect0, 0.0);
  factorReflect = reflectivity + (1.0 - reflectivity) * pow(1.0 + dot(incident, N), 5.0);
  gl_Position = ftransform();
}
