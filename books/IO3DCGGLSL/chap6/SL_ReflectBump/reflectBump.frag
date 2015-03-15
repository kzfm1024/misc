
varying vec3 P;//位置ベクトル
varying vec3 N;//法線ベクトル
varying vec3 T;//接線ベクトル
varying vec3 B;//従法線ベクトル
uniform mat4 ViewTranspose;//ビュー変換だけのモデルビュー行列
uniform float reflectivity;//反射率
uniform samplerCube cubeMap;//環境マップ
uniform sampler2D normalMap;//法線マップ

void main(void)
{
	vec3 grad = (texture2D(normalMap, gl_TexCoord[0].st).xyz - 0.5) * 2.0;
	N = normalize( N - grad.x * T - grad.y * B); 
	//反射ベクトルの計算
  vec3 incident = normalize(P);//入射視線ベクトル
  vec3 reflect0 = reflect(incident, N);//その反射ベクトル
  vec4 Reflect = ViewTranspose * vec4(reflect0, 0.0);//ビュー行列の転置行列を乗じる

  //以下は反射環境マッピング
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	//オブジェクトの色と環境マップを線形補間
	gl_FragColor = mix(ambient + diffuse, textureCube(cubeMap, Reflect.stp), reflectivity) + specular;
}