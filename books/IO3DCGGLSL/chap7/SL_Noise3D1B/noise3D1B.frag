
varying vec3 P;
varying vec3 N;
uniform float threshold;
uniform sampler3D smpl3D;
vec4 col1 = vec4(0.2, 0.1, 0.9, 1.0);
vec4 col2 = vec4(0.9, 0.5, 0.2, 1.0);

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
  float intensity = texture3D(smpl3D, gl_TexCoord[0].stp).r;
  if(intensity > threshold) gl_FragColor = (ambient + diffuse) * col1 * intensity + specular;
  else gl_FragColor = (ambient + diffuse) * col2 * intensity * 2.0 + specular;
}