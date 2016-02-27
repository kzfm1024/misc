
varying vec3 P;
varying vec3 N;

void main(void)
{
  vec3 L = gl_LightSource[0].position.xyz - P;
  float d = length(L);//åıåπÇ‹Ç≈ÇÃãóó£
  //å∏êäåvêî
	float attenuation = 1.0 / (gl_LightSource[0].constantAttenuation 
	                    + gl_LightSource[0].linearAttenuation * d 
	                    + gl_LightSource[0].quadraticAttenuation * d * d );
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
	//ìùçá
	gl_FragColor = ambient + diffuse + specular;
	//å∏êäó¶ÇèÊÇ∂ÇÈ
  gl_FragColor *= attenuation;
}
