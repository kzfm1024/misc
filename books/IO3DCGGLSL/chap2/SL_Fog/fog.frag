
varying vec3 P;
varying vec3 N;
uniform int fogMode;

void main(void)
{
  vec3 L = gl_LightSource[0].position.xyz - P;
  float d = length(L);//光源までの距離
  //減衰計数
	float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation +
	                           gl_LightSource[0].linearAttenuation * d +
	                           gl_LightSource[0].quadraticAttenuation * d * d );
  L = normalize(L);
  N = normalize(N);
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	//統合
	gl_FragColor = (ambient + diffuse + specular) * attenuation;
  //フォグ
  float fog;
  float a = gl_Fog.density * gl_FogFragCoord;
  if(fogMode == 0) fog = exp( - a);
  else if(fogMode == 1) fog = exp(- a * a);
  else fog = (gl_Fog.end - gl_FogFragCoord) * gl_Fog.scale ;
  fog = clamp(fog, 0.0, 1.0);
	gl_FragColor = mix(gl_Fog.color, gl_FragColor, fog);
}
