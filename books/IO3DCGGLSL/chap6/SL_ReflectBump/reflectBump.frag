
varying vec3 P;//�ʒu�x�N�g��
varying vec3 N;//�@���x�N�g��
varying vec3 T;//�ڐ��x�N�g��
varying vec3 B;//�]�@���x�N�g��
uniform mat4 ViewTranspose;//�r���[�ϊ������̃��f���r���[�s��
uniform float reflectivity;//���˗�
uniform samplerCube cubeMap;//���}�b�v
uniform sampler2D normalMap;//�@���}�b�v

void main(void)
{
	vec3 grad = (texture2D(normalMap, gl_TexCoord[0].st).xyz - 0.5) * 2.0;
	N = normalize( N - grad.x * T - grad.y * B); 
	//���˃x�N�g���̌v�Z
  vec3 incident = normalize(P);//���ˎ����x�N�g��
  vec3 reflect0 = reflect(incident, N);//���̔��˃x�N�g��
  vec4 Reflect = ViewTranspose * vec4(reflect0, 0.0);//�r���[�s��̓]�u�s����悶��

  //�ȉ��͔��ˊ��}�b�s���O
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);  
  vec4 ambient = gl_FrontLightProduct[0].ambient;
  float dotNL = dot(N, L);
  vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(0.0, dotNL);
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	if(dotNL <= 0.0) powNH = 0.0;
	vec4 specular = gl_FrontLightProduct[0].specular * powNH;
	//�I�u�W�F�N�g�̐F�Ɗ��}�b�v����`���
	gl_FragColor = mix(ambient + diffuse, textureCube(cubeMap, Reflect.stp), reflectivity) + specular;
}