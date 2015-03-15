
varying vec3 P;
varying vec3 N;
uniform sampler2D sampler0;
uniform sampler2D sampler1;
uniform float mixFactor;//������

void main(void)
{
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  N = normalize(N);
  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);//max(0.0, dot(N, L));
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
  //�e�N�X�`���̐F
  vec4 texColor0 = texture2D(sampler0, gl_TexCoord[0].st);
  vec4 texColor1 = texture2D(sampler1, gl_TexCoord[1].st);
  vec4 texColor = mix(texColor0, texColor1, mixFactor);
//vec4 texColor = clamp(texColor0 + texColor1, vec4(0.0), vec4(1.0));
  
	//GL_MODULATE���[�h
	gl_FragColor = (ambient + diffuse) * texColor + specular;
	//�C�ӂ̍�����
//	gl_FragColor = mix(ambient + diffuse, texColor, 0.5) + specular;	
}