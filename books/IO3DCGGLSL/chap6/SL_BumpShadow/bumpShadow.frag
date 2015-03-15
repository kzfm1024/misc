
varying vec3 P;
varying vec3 N;
varying vec3 T;
varying vec3 B;
uniform sampler2D normalMap;
uniform sampler2D colorMap;
uniform sampler2DShadow shadowMap;

void main(void)
{
	vec3 grad = (texture2D(normalMap, gl_TexCoord[0].st).xyz - 0.5) * 2.0;
	N = normalize( N - grad.x * T - grad.y * B); 

  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
  //テクスチャの色
  vec4 texColor = texture2D(colorMap, gl_TexCoord[0].st);
  ambient *= texColor;
  diffuse *= texColor;
  //シャドウマップ
	float shadow = 0.3;//影濃度
  float s = shadow2DProj(shadowMap, gl_TexCoord[1]).r;
  diffuse *= clamp(s, shadow, 1.0);
	gl_FragColor = ambient + diffuse + specular * s;
}
