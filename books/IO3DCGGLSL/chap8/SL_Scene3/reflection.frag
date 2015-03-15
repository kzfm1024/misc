
varying vec3 P;
varying vec3 N;
varying vec3 Reflect;//���˃x�N�g��
uniform samplerCube texCube;
uniform float reflectivity;//���˗�

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
	//���`���
	gl_FragColor = mix(ambient + diffuse, textureCube(texCube, Reflect.stp), reflectivity) + specular;
}
