//�n�`�f�[�^
#define nMeshC 64//�ő啪�����i���ӓ����j
float sizeXC = 100.0;
float sizeYC = 100.0;
float dataC[(nMeshC+1)*(nMeshC+1)];//�����f�[�^
float betaC = 2.0;//fBm�̎w����
float sigma0C = 0.25;//�W���΍�
int seedC = 0;//�����̼���
float bias = 0.3;//�傫������Ɛ���Ԃ������Ȃ�
//�n�`affine�ϊ��p�ϐ�
//float posC[] = { 0.0, 0.0, 0.0 };//���S��xyz���W
//float scaleC[] = { 1.0, 1.0, 1.0 };//�傫��(�{��)
//float angleC[] = { 0.0, 0.0, 0.0 };//��]�p�x

void makeCloudData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 4;
  mp.numMax = nMeshC;
  mp.sigma0 = sigma0C;
  mp.beta = betaC;

  //�������x�f�[�^
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++) mp.h[i][j] = - bias + (float)rand() / (float)RAND_MAX;;
  mp.create(seedC, dataC);//��1�����͗�����seed�i�l��ς���ƈقȂ����p�^�[�����쐬�j
/*
	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//���Ȃ��0�Ƃ���
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}*/
//	calcDirection();
}

