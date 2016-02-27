
varying vec3 P;
varying vec3 N;
uniform sampler3D smpl3D;

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
	//“‡
/*
	float intensity = abs(texture3D(smpl3D, gl_TexCoord[0].stp).r - 0.5)
	                + abs(texture3D(smpl3D, gl_TexCoord[0].stp).g - 0.5) 
	                + abs(texture3D(smpl3D, gl_TexCoord[0].stp).b - 0.5) 
	                + abs(texture3D(smpl3D, gl_TexCoord[0].stp).a - 0.5);
*/
	float intensity = (texture3D(smpl3D, gl_TexCoord[0].stp).r - 0.5) * 0.5
	                + (texture3D(smpl3D, gl_TexCoord[0].stp).g - 0.5) * 1.0
	                + (texture3D(smpl3D, gl_TexCoord[0].stp).b - 0.5) * 4.0
	                + (texture3D(smpl3D, gl_TexCoord[0].stp).a - 0.5) * 8.0;

	intensity = clamp(0.5 + intensity,0.0, 1.0);
	gl_FragColor = (ambient + diffuse) * intensity+ specular; 
}