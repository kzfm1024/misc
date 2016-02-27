
varying vec3 P;//�ʒu�x�N�g��
varying vec3 N;//�@���x�N�g��
varying vec3 T;//�ڐ��x�N�g��
varying vec3 B;//�]�@���x�N�g��
varying vec3 view;//�ڋ�Ԏ����x�N�g��
uniform sampler2D normalMap;//�@���}�b�v
uniform float fShift;//�����x�N�g���V�t�g�ʒ����t�@�N�^

void main(void)
{
	view = normalize(view);//�����x�N�g�����K��
	//�����x�N�g���ɂ��V�t�g��
	vec2 shift = view.xy * texture2D(normalMap, gl_TexCoord[0]).b;
	//�C�����ꂽ�e�N�X�`�����W
	vec2 texCoord = gl_TexCoord[0].st + shift * fShift;
	//���x���z���擾
	vec3 grad = (texture2D(normalMap, texCoord) - 0.5) * 2.0;
	//�C�����ꂽ�@���x�N�g��
	N = normalize( N - grad.x * T - grad.y * B) ;
	//�ȉ��͒ʏ�̃��C�e�B���O
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	gl_FragColor = ambient + diffuse + specular;  
}

