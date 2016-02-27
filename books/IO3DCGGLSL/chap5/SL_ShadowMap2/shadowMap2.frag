
varying vec3 P;
varying vec3 N;
uniform sampler2DShadow shadowMap;
uniform float dis;//ƒTƒ“ƒvƒŠƒ“ƒO•ÏˆÊ
float shadow = 0.3;

float lookup(float x, float y)
{
	return shadow2DProj(shadowMap, gl_TexCoord[0] + vec4(x, y, 0, 0)).r;
}

void main(void)
{
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  N = normalize(N);
  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	float s = 2.0* lookup(0.0, 0.0);
	s += lookup(-dis, -dis);
	s += lookup( dis, -dis);
	s += lookup(-dis,  dis);
	s += lookup( dis,  dis);
	s /= 6.0;
  diffuse *= clamp(s, shadow, 1.0);
	gl_FragColor = ambient + diffuse + specular * s;
}
