
varying vec3 P;
varying vec3 N;
uniform sampler2DShadow shadowMap;

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
	float shadow = 0.3;//‰e”Z“x
  float s = shadow2DProj(shadowMap, gl_TexCoord[0]).r;
  diffuse *= clamp(s, shadow, 1.0);
	gl_FragColor = ambient + diffuse + specular * s;
}
