
void main(void)
{
  vec3 P = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 N = normalize(gl_NormalMatrix * gl_Normal);
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	//“‡
	gl_FrontColor = ambient + diffuse + specular;
 
  gl_Position = ftransform();//ƒNƒŠƒbƒv‹óŠÔ‚Ö
}