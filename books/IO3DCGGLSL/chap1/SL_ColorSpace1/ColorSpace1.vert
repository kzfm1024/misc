
void main(void)
{
	//���_���W�̏o��
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;  //�t���O�����g�V�F�[�_�[��
}