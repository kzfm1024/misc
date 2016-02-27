
varying vec3 P;
varying vec3 N;
varying vec4 Refract;//���܃x�N�g��
varying vec4 Reflect;//���˃x�N�g��
varying float factorReflect;//���˗�
uniform sampler2D texSampler;
uniform samplerCube envSampler;
uniform float transparency;
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

	vec4 colorReflect = textureCube(envSampler, Reflect.stp);//���ˊ��̐F
	vec4 colorRefract = textureCube(envSampler, Refract.stp);//���܊��̐F
	vec4 col = (ambient + diffuse) * texture2D(texSampler, gl_TexCoord[0].st);//ColorMap����Z
	vec4 colorT = mix(col, colorRefract, transparency);
	
	gl_FragColor = mix(colorT, colorReflect, factorReflect) + specular;
}
