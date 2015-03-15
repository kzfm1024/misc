
varying vec3 P;
varying vec3 N;
uniform sampler2D tex;

void main(void)
{
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  vec3 L1 = normalize(gl_LightSource[1].position.xyz - P);
  N = normalize(N);
  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  vec4 ambient1 = gl_FrontLightProduct[1].ambient;
  float dotNL = dot(N, L);
  float dotNL1 = dot(N, L1);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
  vec4 diffuse1 = gl_FrontLightProduct[1].diffuse * max(0.0, dotNL1);
  
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	vec3 H1 = normalize(L1 + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	float powNH1 = pow(max(dot(N, H1), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	if(dotNL1 <= 0.0) powNH1 = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	vec4 specular1 = gl_FrontLightProduct[1].specular * powNH1;
  //テクスチャの色
  vec4 texColor = texture2D(tex, gl_TexCoord[0].st);
	gl_FragColor = (ambient + diffuse) * texColor + specular
	             + 	(ambient1 + diffuse1) * texColor + specular1;
}
